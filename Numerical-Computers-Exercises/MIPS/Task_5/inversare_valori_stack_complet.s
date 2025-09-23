.data
    array:  .word   1 2 3 4 5 6 7 8 9 10
    len:    .word   10
    prompt1:    .asciiz "Original:\n"
    prompt2:    .asciiz "\nInversat:\n"
    space:  .asciiz " "
    nl:     .asciiz "\n"

.text
.globl main
main:

    li $v0, 4
    la $a0, prompt1
    syscall


    la $t0, array
    li $t1, 0
    lw $t2, len

citire1:
    lw $a0, ($t0)
    li $v0, 1
    syscall

    li $v0, 4
    la $a0, space
    syscall

    add $t1, $t1, 1
    add $t0, $t0, 4        
    blt $t1, $t2, citire1   


    la $t0, array
    li $t1, 0

push:
    lw $t4, ($t0)
    subu $sp, $sp, 4    
    sw $t4, ($sp)

    add $t1, $t1, 1
    add $t0, $t0, 4
    blt $t1, $t2, push


    la $t0, array
    li $t1, 0

pop:
    lw $t4, ($sp)
    addu $sp, $sp, 4
    sw $t4, ($t0)       

    add $t1, $t1, 1
    add $t0, $t0, 4
    blt $t1, $t2, pop

    # Print prompt2
    li $v0, 4
    la $a0, prompt2
    syscall

    # Print reversed array
    la $t0, array
    li $t1, 0

citire2:
    lw $a0, ($t0)
    li $v0, 1
    syscall

    li $v0, 4
    la $a0, space
    syscall

    add $t1, $t1, 1
    add $t0, $t0, 4         
    blt $t1, $t2, citire2

    li $v0, 10
    syscall
.end main