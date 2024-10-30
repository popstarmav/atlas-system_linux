BITS 64

global asm_strstr                ; Export the asm_strstr function
section .text

; char *asm_strstr(const char *haystack, const char *needle);
asm_strstr:
    ; Prologue
    push    rbp
    mov     rbp, rsp

    ; Parameters: rdi = haystack, rsi = needle
    ; Check if needle is empty
    movzx   rcx, byte [rsi]      ; Load first byte of needle into rcx
    test    rcx, rcx             ; Check if needle is empty
    je      .return_haystack     ; If empty, return haystack

    ; Outer loop: iterate through haystack
.loop_haystack:
    mov     rax, rdi             ; Set current position in haystack to rax
    mov     rbx, rsi             ; Reset needle pointer to start

    ; Inner loop: compare characters between haystack and needle
.compare_chars:
    movzx   rcx, byte [rax]      ; Load haystack character into rcx
    movzx   rdx, byte [rbx]      ; Load needle character into rdx
    cmp     cl, dl               ; Compare characters in haystack and needle
    jne     .next_char           ; If mismatch, go to next char in haystack
    test    dl, dl               ; Check if at end of needle
    je      .match_found         ; If yes, needle found at this position
    inc     rax                  ; Move to next haystack char
    inc     rbx                  ; Move to next needle char
    jmp     .compare_chars       ; Continue inner comparison loop

    ; Advance haystack pointer to next character and repeat outer loop
.next_char:
    movzx   rcx, byte [rdi]      ; Check if end of haystack
    test    cl, cl
    je      .no_match            ; If end of haystack, return NULL
    inc     rdi                  ; Move to next haystack char
    jmp     .loop_haystack       ; Restart outer loop

.return_haystack:
    mov     rax, rdi             ; Return haystack as needle is empty
    jmp     .exit

.match_found:
    mov     rax, rdi             ; Needle found, return starting position
    jmp     .exit

.no_match:
    xor     rax, rax             ; No match, return NULL

.exit:
    ; Epilogue
    pop     rbp
    ret

