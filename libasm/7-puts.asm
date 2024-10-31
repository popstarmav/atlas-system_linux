BITS 64

extern asm_strlen            ; import `asm_strlen` function
global asm_puts               ; export `asm_puts` function

section .text

	;; size_t asm_puts(const char *str)
	;; {
	;;     size_t len;
	;;
	;;     len = asm_strlen(str);
	;;
	;;     return (write(1, str, len));
	;; }

asm_puts:
    push    rbp                     ; prologue - save base pointer
    mov     rbp, rsp                ; setup stack frame
    sub     rsp, 32                 ; reserve 32 bytes on stack for local variables
    
    ; Store the `str` parameter at [rbp - 24]
    mov     QWORD [rbp - 24], rdi   ; allocate space for char *str

    ; Begin: len = asm_strlen(str);
    mov     rax, QWORD [rbp - 24]   ; load `str` into rax
    mov     rdi, rax                ; set up `str` as argument for asm_strlen
    mov     eax, 0                  ; zero out eax before calling
    call    asm_strlen              ; call asm_strlen, length returned in eax
    mov     DWORD [rbp - 4], eax    ; store result as `len` at [rbp - 4]
    ; End: len = asm_strlen(str);

    ; Setup parameters for write syscall
    mov     edx, DWORD [rbp - 4]    ; 3rd param: length from asm_strlen result
    mov     rax, QWORD [rbp - 24]   ; reload `str` into rax
    mov     rsi, rax                ; 2nd param: buffer address (str)
    mov     edi, 1                  ; 1st param: file descriptor (1 = stdout)
    mov     eax, 1                  ; syscall code for write (1)
    syscall                         ; invoke write syscall

    ; Cast write result to size_t and return
    cdqe                            ; cast EAX (ssize_t) to RAX (size_t)
    leave                           ; epilogue - restore stack and base pointer
    ret                             ; return to caller

