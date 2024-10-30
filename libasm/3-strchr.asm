BITS 64

global asm_strchr       ; Export the asm_strchr function
section .text

; char *asm_strchr(const char *s, int c);
asm_strchr:
    mov     rax, rdi           ; Copy s (pointer to string) to rax as return address if found
    mov     dl, sil            ; Move character c into dl for comparison

.loop:
    movzx   rcx, byte [rax]    ; Load the current character from s into rcx
    test    rcx, rcx           ; Check if it's the null terminator ('\0')
    je      .not_found         ; If null terminator, character was not found, return NULL
    cmp     dl, cl             ; Compare the character at [rax] with c
    je      .found             ; If equal, character is found, return rax
    inc     rax                ; Move to the next character
    jmp     .loop              ; Repeat the loop

.found:
    ret                        ; rax already contains address of found character, return it

.not_found:
    xor     rax, rax           ; Set rax to NULL (0) if character not found
    ret

