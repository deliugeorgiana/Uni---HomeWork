.data

fin:    .asciiz "C:/Users/deliu/Desktop/AN II/CN/Laborator 12/fout.txt"
fileWords:  .space 1024

.text
.globl main

main:

    li $v0, 13
    la $a0, fin        
    li $a1, 0         
    li $a2, 0           
    syscall
    move $s6, $v0   

    li $v0, 14        
    move $a0, $s6       
    la $a1, fileWords
    li $a2, 100          
    syscall
    move $s1, $v0 

    li $v0, 10
    syscall
.end main