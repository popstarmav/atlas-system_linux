BITS 64

global asm_memcpy            ; Export the asm_memcpy function
section .text

; void *asm_memcpy(void *dest, const void *src, size_t n);
asm_memcpy:
    ; Prologue
    push    rbp
    mov     rbp, rsp

    ; Move parameters to stack
    mov     rdi, rdi            ; dest pointer in rdi
    mov     rsi, rsi            ; src pointer in rsi
    mov     rdx, rdx            ; n in rdx

    ; Check if dest or src is NULL
    test    rdi, rdi            ; Check if dest is NULL
    jz      .return_dest        ; If NULL, return dest
    test    rsi, rsi            ; Check if src is NULL
    jz      .return_dest        ; If NULL, return dest

    ; Initialize index i
    xor     rax, rax            ; i = 0

.loop_copy:
    cmp     rax, rdx            ; Compare i with n
    jge     .return_dest        ; If i >= n, exit loop

    ; Copy byte from src to dest
    movzx   rcx, byte [rsi + rax]  ; Load byte from src
    mov     byte [rdi + rax], cl     ; Store byte in dest

    inc     rax                  ; i++
    jmp     .loop_copy          ; Repeat loop

.return_dest:
    mov     rax, rdi            ; Return dest
    pop     rbp                 ; Epilogue
    ret

