section .text
    global asm_strncasecmp
asm_strncasecmp:
    ; Function parameters:
    ; rdi = s1 (first string)
    ; rsi = s2 (second string)
    ; rdx = n  (max number of characters to compare)

    ; Check if n == 0 (nothing to compare)
    test rdx, rdx
    je .return_zero

.loop:
    ; Load bytes from s1 and s2
    movzx eax, byte [rdi]     ; Load byte from s1 into eax, zero-extend to 32 bits
    movzx ecx, byte [rsi]     ; Load byte from s2 into ecx, zero-extend to 32 bits

    ; Check for end of either string
    test eax, eax
    jz .end_of_strings
    test ecx, ecx
    jz .end_of_strings

    ; Convert characters to lowercase if they are uppercase
    ; To convert, we OR with 0x20 if the character is uppercase (A-Z range)
    ; A-Z ASCII range: 0x41 ('A') to 0x5A ('Z')
    cmp eax, 'A'
    jb .no_conversion_s1       ; If eax < 'A', skip conversion
    cmp eax, 'Z'
    ja .no_conversion_s1       ; If eax > 'Z', skip conversion
    or eax, 0x20               ; Convert to lowercase by setting bit 5

.no_conversion_s1:
    cmp ecx, 'A'
    jb .no_conversion_s2       ; If ecx < 'A', skip conversion
    cmp ecx, 'Z'
    ja .no_conversion_s2       ; If ecx > 'Z', skip conversion
    or ecx, 0x20               ; Convert to lowercase by setting bit 5

.no_conversion_s2:
    ; Compare characters
    cmp eax, ecx
    jne .return_difference     ; If characters are different, return difference

    ; Move to the next characters
    inc rdi
    inc rsi
    dec rdx
    jnz .loop                  ; Continue if n > 0

.return_zero:
    xor eax, eax               ; Return 0 (strings are equal up to n characters)
    ret

.end_of_strings:
    ; If both strings ended simultaneously, return 0
    test eax, eax
    jne .return_difference     ; If only s1 ended, return difference
    test ecx, ecx
    jne .return_difference     ; If only s2 ended, return difference
    jmp .return_zero           ; Otherwise, strings are equal

.return_difference:
    sub eax, ecx               ; Return difference (s1 - s2)
<<<<<<< HEAD
    ret
=======
    ret
>>>>>>> d79af5e41f4d6d076642b3f8ae180245e95a107c
