BITS 64

global asm_strcmp
section .text

asm_strcmp:
.compare_loop:
    mov al, byte [rdi]         ; Load byte from s1
    mov bl, byte [rsi]         ; Load byte from s2

    test al, al                ; Check if *s1 is null
    jz .end_of_strings         
    test bl, bl                ; Check if *s2 is null
    jz .end_of_strings         

    cmp al, bl                 
    jne .not_equal             

    inc rdi                    
    inc rsi                    
    jmp .compare_loop          

.end_of_strings:
    cmp al, bl                 
    je .return_equal           

.not_equal:
    mov eax, 1                 ; Set default return to 1 or -1
    cmp al, bl                 
    jl .less_than              

    ret                        ; Return 1 if `al > bl`

.less_than:
    mov eax, -1                ; Return -1 if `al < bl`
    ret                       

.return_equal:
    xor eax, eax               ; Set eax to 0 for equality
    ret

