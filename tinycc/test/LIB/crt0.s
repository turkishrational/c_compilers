.global main
.text

/* Kod doðrudan 0. bayttan baþlar, kayma riski sýfýrdýr! */
_start_entry:
    popl %eax                       /* argc deðerini %eax'e çek */
    movl %esp, %ebx                 /* %esp (argv pointer) -> %ebx */
    pushl %ebx                      /* Ýkinci parametre: argv */
    pushl %eax                      /* Birinci parametre: argc */
    
    call main                       /* TCC 'main' sembolünü baðlayacak */
    
    addl $8, %esp                   /* Stack temizliði */
    
    movl %eax, %ebx                 /* main()'den dönen exit code -> %ebx */
    movl $1, %eax                   /* %eax = 1 (sys_exit) */
    int $0x40                       /* TRDOS Kernel Resmi Kesmesi */
