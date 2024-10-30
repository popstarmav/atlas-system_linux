BITS 64

global asm_strstr                ; Export the asm_strstr function
section .text

; char *asm_strstr(const char *haystack, const char *needle);
asm_strstr:
    ; Prologue
    push    rbp
    mov     rbp, rsp

    ; Move parameters to registers
    mov     rdi, rdi            ; haystack in rdi
    mov     rsi, rsi            ; needle in rsi

    ; Check if needle is empty
    mov     rax, rsi
    movzx   rcx, byte [rsi]
    test    rcx, rcx
    je      .found_empty_needle ; If empty, return haystack

    ; Loop through haystack
.loop_haystack:
    mov     rax, rdi            ; Current position in haystack
    mov     rbx, rsi            ; Reset needle pointer

    ; Inner loop to compare haystack and needle characters
    .compare_chars:
        movzx   rcx, byte [rax] ; Load haystack character
        movzx   rdx, byte [rbx] ; Load needle character
        cmp     cl, dl          ; Compare characters
        jne     .next_char      ; If different, check next char in haystack
        test    cl, cl          ; Check if at end of needle
        je      .found_needle   ; If needle ends, match is found
        inc     rax             ; Move to next character in haystack
        inc     rbx             ; Move to next character in needle
        jmp     .compare_chars  ; Continue comparing

    ; Move to the next character in haystack and loop
.next_char:
    movzx   rcx, byte [rdi]
    test    cl, cl              ; Check if end of haystack
    je      .not_found          ; If end, needle not found
    inc     rdi                 ; Move to next char in haystack
    jmp     .loop_haystack      ; Restart outer loop

.found_empty_needle:
    mov     rax, rdi            ; Return haystack if needle is empty
    jmp     .end

.found_needle:
    mov     rax, rdi            ; Match found, set return to haystack position
    jmp     .end

.not_found:
    xor     rax, rax            ; Return NULL if needle not found

.end:
    ; Epilogue
    pop     rbp
    ret

