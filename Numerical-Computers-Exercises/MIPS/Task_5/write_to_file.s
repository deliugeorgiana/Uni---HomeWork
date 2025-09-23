.data

fout:   .asciiz "C:\Users\deliu\Desktop\AN II\CN\Laborator 12\fout.s"
buffer: .asciiz "Lalalallalallal"

    .text
    .globl main

main:
    li $v0, 13
    la $a0, fout
    li $a1, 1

    syscall
    move $s6, $v0

    li $v0,15
    move $a0, $s6
    la $a1, buffer
    li $a2, 44
    syscall

    li $v0, 16
    move $a0, $s6
    syscall

    li $v0, 16
    move $a0, $s6
    syscall

    li $v0,10
    syscall

.end main

