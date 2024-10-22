section .text
    global asm_strlen       ; Expose function to be used in C

asm_strlen:
    xor rcx, rcx            ; Set RCX (counter) to 0
.loop:
    mov al, [rdi]           ; Load byte at address RDI (char *str) into AL
    test al, al             ; Check if it's null (AL == 0?)
    je .done                ; If it's null, we're done
    inc rdi                 ; Increment the pointer to the next character
    inc rcx                 ; Increment the counter (string length)
    jmp .loop               ; Repeat until null terminator is found

.done:
    mov rax, rcx            ; Move the counter to RAX (return value)
    ret                     ; Return from the function
