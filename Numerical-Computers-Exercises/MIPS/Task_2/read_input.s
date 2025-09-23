.text
.globl main

main:
    li $v0,4
    la $a0, str1
    syscall

    li $v0, 5
    syscall
    move $t0, $v0 

    li $v0, 4
    la $a0, str2
    syscall

    li $v0, 5
    syscall
    move $t1, $v0

    add $t0, $t0, $t1

    li $v0, 4
    la $a0, str3
    syscall

    li $v0,1
    move $a0, $t0
    syscall

    li $v0, 10
    syscall

.end main

.data
str1: .asciiz "Valoarea A: "
str2: .asciiz "Valoarea B: "
str3: .asciiz "A + B = "
newline: .asciiz "\n"