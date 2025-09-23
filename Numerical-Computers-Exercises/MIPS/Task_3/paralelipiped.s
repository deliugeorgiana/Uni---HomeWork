.text
.globl main

main:

    lw $t0, laturaA     
    lw $t1, laturaB    
    lw $t2, laturaC
    
    mul $t3, $t0, $t1   
    mul $t3, $t3, $t2  
    move $s0, $t4

    #Afisare volum
    li $v0, 4
    la $a0, m1
    syscall

    li $v0, 1
    move $a0, $t3
    syscall

    mul $t3, $t0, $t1  
    mul $t4, $t0, $t2  
    mul $t5, $t1, $t2  

    add $t6, $t3 $t4   
    add $t6, $t6 $t5
    add $t6, $t6 $t6


    #Afisare suprafata
    li $v0, 4
    la $a0, m2
    syscall
    
    li $v0, 1
    move $a0, $t6
    syscall

    #converteste intregii in float
    mtc1.d $s0, $f0
    cvt.s.w $f0, $f0  #convert 

    mtc1.d $s1, $f1
    cvt.s.w $f1. $f1
    
    #afis raportul 
    div.s $f2, $f0, $f1
    li $v0, 2
    mov.s $f12, $f2
    syscall


    li $v0, 10
    syscall

.end main 

.data

laturaA: .word 2
laturaB: .word 3
laturaC: .word 4

m1: .asciiz "volumul este:"
m2: .asciiz "suprafata este:"
m3: .asciiz "raportul volum suprafata este: "
n1: .asciiz "\n"

