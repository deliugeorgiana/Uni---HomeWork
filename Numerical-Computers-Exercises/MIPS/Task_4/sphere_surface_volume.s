.data
pi: .float 3.14159
fourPtZero: .float 4.0
threePtZero:   .float 3.0
radius: .float 5.0

msgSurface: .asciiz "Suprafata este:"
newline:    .asciiz "\n"
msgVolume: .asciiz "Volumul este:"

.text
.globl main

main:

    lwc1 $f0, radius
    lwc1 $f1, pi
    lwc1 $f2, fourPtZero
    lwc1 $f3, threePtZero
    
#Calc suprafata
    mul.s $f4, $f0, $f0    
    mul.s $f4, $f4, $f1     
    mul.s $f4, $f4, $f2 

#Afisare suprafata
    li $v0, 4             
    la $a0, msgSurface
    syscall
    
    li $v0, 2               
    mov.s $f12, $f4       
    syscall

    li $v0, 4             
    la $a0, newline
    syscall


#Calc volum
    mul.s $f5, $f0, $f0    
    mul.s $f5, $f5, $f0    
    mul.s $f5, $f5, $f1    
    mul.s $f5, $f5, $f2   
    div.s $f5, $f5, $f3  

#Afis volum
    li $v0, 4            
    la $a0, msgVolume
    syscall
    
    li $v0, 2            
    mov.s $f12, $f5         
    syscall

#Iesire
    li $v0, 10
    syscall

.end main