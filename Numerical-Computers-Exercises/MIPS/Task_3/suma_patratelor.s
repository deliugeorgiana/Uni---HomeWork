.text
.globl main

main:

    li $v0, 5
    syscall
    move $t0, $v0

    li $t1, 1
    li $t2, 0


loop:

    mul $t3, $t1, $t1
    add $t2, $t2, $t3  
    add $t1, $t1, 1 
    ble $t1, $t0, loop 

    move $a0, $t2
    li $v0, 1
    syscall

    li $v0, 4         
    la $a0, msg    
    syscall

    li $v0, 1
    move $a0, $t2
    syscall

    li $v0,10
    syscall
.end main

.data
    msg: .asciiz "Suma patratelor este: "
    n:  .word 10