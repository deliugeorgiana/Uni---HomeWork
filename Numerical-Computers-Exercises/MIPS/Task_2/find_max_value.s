.text
.globl main

main:
    li $v0,4
    la $a0, prompt
    syscall

    li $t0,0
    lw $s0, max
    lw $s1, n

    m1: bge $t0, $s1, m3

    mul $t1, $t0,4
    lw $t2, a($t1)
    ble $t2, $s0, m2
    move $s0, $t2

    m2: addi $t0, $t0,1
    b m1

    m3: move $a0, $s0

    li $v0, 1  
    syscall

    li $v0, 10
    syscall

.end main

.data
a: .word 69 34 2 85 7 52 48 17 22
n: .word 9
max: .word 0
prompt: .asciiz "maximum: "
