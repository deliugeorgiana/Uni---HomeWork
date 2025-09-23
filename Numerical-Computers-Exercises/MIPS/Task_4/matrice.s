.data
arr:    .word   2 4 5 6
        .word   1 6 5 4 
        .word   4 9 6 3 
        .word   1 2 3 4

size:   .word   4
sumadiag:   .word   0
msg:    .asciiz "Suma e: "

.text
.globl main

main:

li $t0,0

lw $t1, size

li $t2, 0

la $t3, arr

bucla:

    mul $t4, $t1, $t0
    add $t4, $t4, $t0
    sll $t4, $t4, 4

    lw $t5, arr(t4)
    add $t2, $t2, $t5

    add $t0, $t0, 1

    blt $t0, $t1 bucla
    
    .end bucla

    sw $t0, sumadiag

    li $v0, 4
    la $a0, msg
    syscall
    
    li $v0, 1
    move $a0, $t0
    syscall
    
    li $v0, 10
    syscall

.end main