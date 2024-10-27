section .text
global asm_strcmp

asm_strcmp:
    ; Function parameters:
    ; s1 is in rdi
    ; s2 is in rsi

compare_loop:
    ; Load the bytes from each string into al and bl
    mov al, byte [rdi]
    mov bl, byte [rsi]

    ; Check if the bytes are equal
    cmp al, bl
    jne not_equal  ; If not equal, jump to not_equal

    ; Check if we reached the end of either string
    test al, al
    je strings_equal  ; If al is 0 (end of string), the strings are equal

    ; Move to the next byte in each string
    inc rdi
    inc rsi
    jmp compare_loop  ; Continue the loop

not_equal:
    ; Calculate the signed difference between al and bl
    movsx eax, al     ; Sign-extend al to eax
    movsx ebx, bl     ; Sign-extend bl to ebx
    sub eax, ebx      ; eax = al - bl (signed difference)
    ret

strings_equal:
    xor eax, eax  ; Set eax to 0 (indicating equality)
    ret

