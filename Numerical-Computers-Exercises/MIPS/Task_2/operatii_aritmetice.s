.text
.globl main

main:
    li $t2, 25         
    lw $t3, val       
    add $t4, $t2, $t3 

    li $v0, 1          
    move $a0, $t4      
    syscall            

    li $v0, 4      
    la $a0, newline   
    syscall

    li $v0, 10
    syscall

.end main

.data
val: .word 30
newline: .asciiz "\n"
