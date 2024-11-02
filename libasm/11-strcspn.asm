section .text
global asm_strcspn

asm_strcspn:
    ; Function prototype: size_t asm_strcspn(const char *s, const char *accept);
    
    ; Input:
    ;   rdi - pointer to the string s (string1)
    ;   rsi - pointer to the string accept (string2)

    xor     rax, rax            ; Clear rax, this will hold the index/count

.loop:
    mov     al, [rdi + rax]     ; Load the next character from s into al
    test    al, al              ; Check if it's null (end of string)
    jz      .done               ; If it is, we are done

    ; Check if the character is in accept
    mov     rbx, rsi            ; Point rbx to the start of accept

.check_accept:
    mov     bl, [rbx]           ; Load character from accept
    test    bl, bl              ; Check if it's null (end of accept)
    jz      .not_found          ; If it is, we haven't found a match in accept

    cmp     al, bl              ; Compare the character from s with the character from accept
    je      .found              ; If they match, we found a character from accept

    inc     rbx                 ; Move to the next character in accept
    jmp     .check_accept       ; Check the next character in accept

.found:
    ; Return the index of the first occurrence found
    ret                         ; Return with index in rax

.not_found:
    inc     rax                 ; Increment the length count (not found yet)
    jmp     .loop               ; Continue to check the next character in s

.done:
    ret                         ; If we reached here, return the length (no match found)

