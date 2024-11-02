; asm_strspn function in x86-64 assembly
; size_t asm_strspn(const char *s, const char *accept)

section .text
global asm_strspn

asm_strspn:
    ; Function Prologue
    push    rbp
    mov     rbp, rsp
    mov     rdi, rdi        ; rdi = s (pointer to str1)
    mov     rsi, rsi        ; rsi = accept (pointer to str2)

    xor     rcx, rcx        ; rcx will store the count of matching characters

.next_char_s:
    mov     al, byte [rdi]  ; Load the next character from s into al
    test    al, al          ; Check if we've reached the end of s (null terminator)
    jz      .done           ; If null terminator, we're done

    ; Reset rdx to scan accept for the current character in s
    mov     rdx, rsi

.search_accept:
    mov     bl, byte [rdx]  ; Load the next character from accept into bl
    test    bl, bl          ; Check if we've reached the end of accept (null terminator)
    jz      .stop           ; If we reach null, the character is not in accept

    cmp     al, bl          ; Compare the current character from s with current from accept
    je      .found          ; If equal, we found a match

    inc     rdx             ; Move to the next character in accept
    jmp     .search_accept  ; Repeat the search for the current character from s

.stop:
    jmp     .done           ; No match found in accept, stop

.found:
    inc     rcx             ; Increment match count
    inc     rdi             ; Move to the next character in s
    jmp     .next_char_s    ; Continue with the next character in s

.done:
    mov     rax, rcx        ; Move the count to rax for the return value
    pop     rbp
    ret                     ; Return the length of matching segment

