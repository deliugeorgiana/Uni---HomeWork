.data
   prompt1: .asciiz "Suma initiala\n"
   prompt2: .asciiz "Suma dupa functie"
   newline: .asciiz"\n"

.text
.globl main

main:

    li $t0, 10
    li $t1, 20
 
    add $t2, $t0, $t1 

    li $v0, 4
    la $a0, prompt1
    syscall
    
    li $v0, 1
    move $a0, $t2
    syscall

    subu $sp, $sp, 4
    sw $ra, 0($sp)

    move $a0, $t2

    jal modify_sum

    move $t3, $v0

    lw $ra, 0($sp)
    addu $sp, $sp, 4
 

    li $v0, 4
    la $a0, prompt2
    syscall
    
    li $v0, 1
    move $a0, $t3
    syscall


    li $v0, 10
    syscall
.end main

modify_sum:
    add $v0, 2    
    jr $ra
