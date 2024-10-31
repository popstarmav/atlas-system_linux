BITS 64

global asm_strcasecmp            ; export `asm_strcasecmp` function

section .text

	;; int asm_strcasecmp(const char *s1, const char *s2)
	;; {
	;;     char c1, c2;
	;;     int diff = 0;
	;;
	;;     while ((*s1 || *s2) && diff == 0)
	;;     {
	;;         c1 = (*s1 >= 'A' && *s1 <= 'Z') ? *s1 + ' ' : *s1;
	;;         c2 = (*s2 >= 'A' && *s2 <= 'Z') ? *s2 + ' ' : *s2;
	;;         diff = c1 - c2;
	;;         s1++;
	;;         s2++;
	;;     }
	;;     return (diff);
	;; }

asm_strcasecmp:
    push    rbp                     ; prologue
    mov     rbp, rsp                ; set up stack frame
    sub     rsp, 32                 ; reserve 32 bytes on stack for locals
    
    mov     QWORD [rbp - 24], rdi   ; store s1 in [rbp - 24]
    mov     QWORD [rbp - 32], rsi   ; store s2 in [rbp - 32]
    mov     DWORD [rbp - 4], 0      ; initialize diff to 0 in [rbp - 4]

    jmp     .s1_or_s2_test          ; initial check of loop condition

.c1_ternary:                        ; ternary for c1 = (*s1 >= 'A' && *s1 <= 'Z') ? *s1 + ' ' : *s1
    mov     rax, QWORD [rbp - 24]   ; load s1
    movzx   eax, BYTE [rax]         ; dereference *s1
    cmp     al, 'A'                 ; check if *s1 >= 'A'
    jl      .c1_ternary_false       ; if not, go to .c1_ternary_false
    cmp     al, 'Z'                 ; check if *s1 <= 'Z'
    jg      .c1_ternary_false       ; if not, go to .c1_ternary_false
    add     al, 32                  ; c1 = *s1 + ' ' (convert to lowercase)
    jmp     .c1_ternary_true

.c1_ternary_false:
    mov     rax, QWORD [rbp - 24]
    movzx   eax, BYTE [rax]         ; c1 = *s1

.c1_ternary_true:
    mov     BYTE [rbp - 5], al      ; store c1 in [rbp - 5]

    ; Ternary for c2 = (*s2 >= 'A' && *s2 <= 'Z') ? *s2 + ' ' : *s2
    mov     rax, QWORD [rbp - 32]   ; load s2
    movzx   eax, BYTE [rax]         ; dereference *s2
    cmp     al, 'A'                 ; check if *s2 >= 'A'
    jl      .c2_ternary_false       ; if not, go to .c2_ternary_false
    cmp     al, 'Z'                 ; check if *s2 <= 'Z'
    jg      .c2_ternary_false       ; if not, go to .c2_ternary_false
    add     al, 32                  ; c2 = *s2 + ' ' (convert to lowercase)
    jmp     .find_diff

.c2_ternary_false:
    mov     rax, QWORD [rbp - 32]
    movzx   eax, BYTE [rax]         ; c2 = *s2

.find_diff:                         ; calculate difference between c1 and c2
    mov     BYTE [rbp - 6], al      ; store c2 in [rbp - 6]
    movsx   edx, BYTE [rbp - 5]     ; sign-extend c1 to edx
    movsx   eax, BYTE [rbp - 6]     ; sign-extend c2 to eax
    sub     edx, eax                ; diff = c1 - c2
    mov     DWORD [rbp - 4], edx    ; store diff in [rbp - 4]

    add     QWORD [rbp - 24], 1     ; increment s1
    add     QWORD [rbp - 32], 1     ; increment s2

.s1_or_s2_test:                     ; check loop condition (*s1 || *s2)
    mov     rax, QWORD [rbp - 24]
    movzx   eax, BYTE [rax]
    test    al, al
    jne     .diff_test              ; if *s1 != 0, check diff
    mov     rax, QWORD [rbp - 32]
    movzx   eax, BYTE [rax]
    test    al, al
    je      .return_diff            ; if *s2 == 0 and *s1 == 0, return diff

.diff_test:
    cmp     DWORD [rbp - 4], 0      ; check if diff == 0
    je      .c1_ternary             ; if diff is 0, continue loop

.return_diff:                       ; return diff
    mov     eax, DWORD [rbp - 4]    ; move diff to return register
    leave                           ; epilogue
    ret                             ; return

