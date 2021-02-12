extern main
section .start
_start:
    int 29h
    ret

global readb_syscall
readb_syscall:
    push eax
    mov ecx, eax
    int 29h
    pop eax
    ret
section .bss
align 4
app_stack_bottom: equ $
	resb 16384 ; 16 KB
app_stack_top: