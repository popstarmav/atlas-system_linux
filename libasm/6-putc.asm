BITS 64

global asm_putc               ; Export the asm_putc function
section .text

; size_t asm_putc(int c);
asm_putc:
    push    rbp              ; Save the base pointer
    mov     rbp, rsp         ; Set up the stack frame
    
    mov     rax, 1           ; syscall number for sys_write
    mov     rdi, 1           ; file descriptor 1 (stdout)
    mov     rsi, rsp         ; Use stack as the buffer
    mov     byte [rsi], al   ; Store the character (in `al`) at the top of the stack
    mov     rdx, 1           ; Number of bytes to write

    syscall                   ; Invoke the syscall

    mov     rax, 1           ; Return value: number of bytes written (1)
    
    pop     rbp              ; Restore the base pointer
    ret                       ; Return from the function

