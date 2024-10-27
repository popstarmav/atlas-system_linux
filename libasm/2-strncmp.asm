BITS 64

global asm_strncmp
section .text

asm_strncmp:
    ; Parameters:
    ; rdi = s1 (first string)
    ; rsi = s2 (second string)
    ; rdx = n (number of characters to compare)

    ; Return 0 if n is 0 (no comparison needed)
    test rdx, rdx
    jz .return_equal

.compare_loop:
    ; Check if we've reached the end of either string or used all characters (n = 0)
    mov al, byte [rdi]          ; Load byte from s1 into al
    mov bl, byte [rsi]          ; Load byte from s2 into bl

    ; If either byte is null (end of string), or n == 0, end comparison
    test al, al                 ; Check if s1 has reached null terminator
    jz .end_of_strings
    test bl, bl                 ; Check if s2 has reached null terminator
    jz .end_of_strings

    ; Compare bytes from s1 and s2
    cmp al, bl
    jne .not_equal              ; If they differ, return comparison result

    ; Move to the next characters in s1 and s2
    inc rdi                     ; Increment pointer s1
    inc rsi                     ; Increment pointer s2
    dec rdx                     ; Decrement n
    jnz .compare_loop           ; Repeat loop if n > 0

.return_equal:
    xor eax, eax                ; If strings are equal up to n characters, return 0
    ret

.not_equal:
    ; Set return value based on the difference between al and bl
    movzx eax, al               ; Zero-extend *s1 (al) into eax
    movzx ebx, bl               ; Zero-extend *s2 (bl) into ebx
    sub eax, ebx                ; eax = *s1 - *s2
    ret

.end_of_strings:
    ; If we reached the end of both strings, treat as equal
    cmp al, bl
    je .return_equal            ; If both null terminators, return 0
    jmp .not_equal              ; Otherwise, return difference

