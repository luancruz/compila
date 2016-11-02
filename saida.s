.data
_x: .word 0
_y: .word 0
_newline: .asciiz "\n"

.text
.globl main

main:

li $a0, 7
sw $a0, 0($sp)
addiu $sp, $sp, -4
li $a0, 5
lw $t1, 4($sp)
add $a0, $a0, $t1
addiu $sp, $sp, 4

sw $a0, _x

sw $a0, 0($sp)
addiu $sp, $sp, -4
li $a0, 1
lw $t1, 4($sp)
sub $a0, $t1, $a0
addiu $sp, $sp, 4

sw $a0, _y

lw $a0, _y

li $v0, 1
syscall

li $v0, 4
la $a0, _newline
syscall

lw $a0, _x

li $v0, 1
syscall

li $v0, 4
la $a0, _newline
syscall


li $v0, 10
syscall
