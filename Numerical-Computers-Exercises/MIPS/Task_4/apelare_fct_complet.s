.data
str1:  .asciiz "Str1: "
str2:   .asciiz "Str2: "
str3:   .asciiz "Str3: "


.text
.globl main

main:

    li $v0, 4
    la $a0, str1
    syscall

    jal 
power:
li $t1,1
li $t0,0

bucla:
mul $t1, $t1, $a0
add $t0, $t0, 1
blt $t0, $a1, bucla
jr $ra
.end power
