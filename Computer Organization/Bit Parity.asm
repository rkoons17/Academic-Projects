#Ryan Koons
# 4-4-2020
#Spring 2020
# Project-3-Koons-Ryan: Design a communication oriented program, 
# capable of computing Bit Parity. Program outputs all 'Balanced'
# bit streams that arise from a fixed 7-bit (integer style) array. 
#
#****Part B: Print out indexes for every possible sub-array of max ParityBalance length
# i.e. if maxLength is 4, print out every balanced sub-array of length 4
#
# /-------Registers:-----/  
#
# $a0-- register used to pass addresses between input and output (labels, printing strings, etc.)
# $v0, register used to pass integers to perform sycalls in MARS
#
# $t1-- stores 1st digit from input array
# $t2-- stores 2nd digit from input array
# $t3-- stores 3rd digit from input array
# $t4-- stores 4th digit from input array
# $t5-- stores 5th digit from input array
# $t6-- stores 6th digit from input array
# $t7-- stores 7th digit from input array
# $t8-- register used to store current max condition (2,4,6 bits long sub-array)
#
# $s1-- stores User's entire 7-digit array in one register
# $s2-- initially stores 1+2---4 digit streams: stores sum of 1,2,3,4--- 6 digit streams: stores sum 1,2,3,4,5,6
# $s3-- initially stores 2+3---4 digit streams: stores sum of 2,3,4,5--- 6 cigit streams: stores sum 2,3,4,5,6,7
# $s4-- initially stores 3+4---4 digit streams: stores sum of 3,4,5,6
# $s5-- initially stores 4+5---4 digit streams: stores sum of 4,5,6,7
# $s6-- initially stores 5+6
# $s7-- initially stores 6+7
#
#
# /-------Labels:-----/
#
#
#
# x: address of fixed-7-digit array
# newline: address of newline for print formatting
# string: address of string for proper print formatting (following specifications)
# to: address of 'to' string for proper formatting
# period: address of period char for proper formatting
# left: address of left paranthesis for printing
# right: address of right paranthesis for printing
#
# PART A
# twoCheck1: move onto digits 2,3
# twoCheck2: move onto digits 3,4
# twoCheck3: move onto digits 4,5
# twoCheck4: move onto digits 5,6
# twoCheck5: move onto digits 6,7
# twoCheckDone: move onto fourDigitCheck
#
# fourCheck1: move onto digits 2,3,4,5
# fourCheck2: move onto digits 3,4,5,6
# fourCheck3: move onto digits 4,5,6,7
# fourCheckDone: done with fourCheck, move onto sixCheck
#
# sixCheck1: go to next six digits: 2,3,4,5,6,7
# sixCheckDone: go to part B
#
# PART B
#
# twoCheck1b: move onto digits 2,3
# twoCheck2b: move onto digits 3,4
# twoCheck3b: move onto digits 4,5
# twoCheck4b: move onto digits 5,6
# twoCheck5b: move onto digits 6,7
# twoCheckDoneb: end Program
#
# fourCheck1b: move onto digits 2,3,4,5
# fourCheck2b: move onto digits 3,4,5,6
# fourCheck3b: move onto digits 4,5,6,7
# fourCheckDoneb: done with fourCheck, move onto twoCheck
#
# sixCheck1b: go to next six digits: 2,3,4,5,6,7
# sixCheckEndb: six Check is done, go onto the 4 check

#   Digit Structure: 1234567
#  0($s1)--Digit 1
#  4($s1)--Digit 2
#  8($s1)--Digit 3
# 12($s1)--Digit 4
# 16($s1)--Digit 5
# 20($s1)--Digit 6
# 24($s1)--Digit 7



.data   #.data portion of program
x: .word 1,1,0,0,1,1,0 #an array x of fixed length i.e. 7 elements
newline: .asciiz "\n" #newline
string: .asciiz "\nThe maximum sub-sequence is from " #string for print formatting
to: .asciiz " to " #prints out to to screen 
period: .asciiz "." #print period to screen
left: .asciiz "(" #string for printing out left paranthesis
right: .asciiz ")" #string for printing out right parenthesis

.text   #denotes beginning of .text portion of program

la $s1, x  #load array into $s1

#load digits into t-registers
	lw $t1  0($s1) #load digit 1 into $t1
	lw $t2  4($s1) #load digit 2 into $t2
	lw $t3  8($s1) #load digit 3 into $t3
	lw $t4 12($s1) #load digit 4 into $t4
	lw $t5 16($s1) #load digit 5 into $t5
	lw $t6 20($s1) #load digit 6 into $t6
	lw $t7 24($s1) #load digit 7 into $t7

#store sums in s-registers
	add $s2, $t1, $t2 #$s2 = digits 1+2
	add $s3, $t2, $t3 #$s2 = digits 2+3
	add $s4, $t3, $t4 #$s2 = digits 3+4
	add $s5, $t4, $t5 #$s2 = digits 4+5
	add $s6, $t5, $t6 #$s2 = digits 5+6
	add $s7, $t6, $t7 #$s2 = digits 6+7

#######/-------2 digit bit-streams:-----/################################################################################################################

beq $t1, $t2, twoCheck1 #if 1=2, then go to next (unbalanced)
#if balanced, print result
	li $t8, 1  #load 1 into $t8, signifies current max length is 2 bits
	la $a0, left #load left address into $a0
	li $v0, 4 #prepare to print string
	syscall #print ( to screen
	move $a0, $t1 #move $t1 to $a0 for printing to screen
	li $v0, 1  #load 1 into $v0, signifies printing an integer to screen
	syscall #print integer to screen
	move $a0, $t2 #move $t2 to $a0 for printing to screen
	li $v0, 1 #load 1 into $v0, signifies printing an integer to screen
	syscall #print integer to screen
	la $a0, right #load right address into $a0
	li $v0, 4 #prepare to print to screen
	syscall # print ) to screen
	la $a0, newline #load adress of newline string into $a0
	li $v0, 4  #prepare to print string to screen
	syscall #print newline

twoCheck1: #finished checking 1,2

beq $t2, $t3, twoCheck2 #if 2=3, then go to next (unbalanced)
#if balanced, print result
	la $a0, left #load left address into $a0
	li $v0, 4 #prepare to print string
	syscall #print ( to screen
	li $t8, 1  #load 1 into $t8, signifies current max length is 2 bits
	move $a0, $t2 #move $t2 to $a0 for printing to screen
	li $v0, 1  #load 1 into $v0, signifies printing an integer to screen
	syscall #print integer to screen
	move $a0, $t3 #move $t3 to $a0 for printing to screen
	li $v0, 1 #load 1 into $v0, signifies printing an integer to screen
	syscall #print integer to screen
	la $a0, right #load right address into $a0
	li $v0, 4 #prepare to print to screen
	syscall # print ) to screen
	la $a0, newline #load adress of newline string into $a0
	li $v0, 4  #prepare to print string to screen
	syscall #print newline

twoCheck2: #finished checking 2,3

beq $t3, $t4, twoCheck3 #if 3=4, then go to next (unbalanced)
#if balanced, print result
	la $a0, left #load left address into $a0
	li $v0, 4 #prepare to print string
	syscall #print ( to screen
	li $t8, 1  #load 1 into $t8, signifies current max length is 2 bits
	move $a0, $t3 #move $t3 to $a0 for printing to screen
	li $v0, 1  #load 1 into $v0, signifies printing an integer to screen
	syscall #print integer to screen
	move $a0, $t4 #move $t4 to $a0 for printing to screen
	li $v0, 1 #load 1 into $v0, signifies printing an integer to screen
	syscall #print integer to screen
	la $a0, right #load right address into $a0
	li $v0, 4 #prepare to print to screen
	syscall # print ) to screen
	la $a0, newline #load adress of newline string into $a0
	li $v0, 4  #prepare to print string to screen
	syscall #print newline

twoCheck3: #finished checking 3,4

beq $t4, $t5, twoCheck4 #if 4=5, then go to next (unbalanced)
#if balanced, print result
	la $a0, left #load left address into $a0
	li $v0, 4 #prepare to print string
	syscall #print ( to screen
	li $t8, 1  #load 1 into $t8, signifies current max length is 2 bits
	move $a0, $t4 #move $t4 to $a0 for printing to screen
	li $v0, 1  #load 1 into $v0, signifies printing an integer to screen
	syscall #print integer to screen
	move $a0, $t5 #move $t5 to $a0 for printing to screen
	li $v0, 1 #load 1 into $v0, signifies printing an integer to screen
	syscall #print integer to screen
	la $a0, right #load right address into $a0
	li $v0, 4 #prepare to print to screen
	syscall # print ) to screen
	la $a0, newline #load adress of newline string into $a0
	li $v0, 4  #prepare to print string to screen
	syscall #print newline

twoCheck4: #finished checking 4,5

beq $t5, $t6, twoCheck5 #if 5=6, then go to next (unbalanced)
#if balanced, print result
	la $a0, left #load left address into $a0
	li $v0, 4 #prepare to print string
	syscall #print ( to screen
	li $t8, 1  #load 1 into $t8, signifies current max length is 2 bits
	move $a0, $t5 #move $t5 to $a0 for printing to screen
	li $v0, 1  #load 1 into $v0, signifies printing an integer to screen
	syscall #print integer to screen
	move $a0, $t6 #move $t6 to $a0 for printing to screen
	li $v0, 1 #load 1 into $v0, signifies printing an integer to screen
	syscall #print integer to screen
	la $a0, right #load right address into $a0
	li $v0, 4 #prepare to print to screen
	syscall # print ) to screen
	la $a0, newline #load adress of newline string into $a0
	li $v0, 4  #prepare to print string to screen
	syscall #print newline

twoCheck5: #finished checking 5,6

beq $t6, $t7, twoCheckDone#if 6=7, twoDigitCheck is done (unbalanced)
#if balanced, print result
	la $a0, left #load left address into $a0
	li $v0, 4 #prepare to print string
	syscall #print ( to screen
	li $t8, 1  #load 1 into $t8, signifies current max length is 2 bits
	move $a0, $t6 #move $t6 to $a0 for printing to screen
	li $v0, 1  #load 1 into $v0, signifies printing an integer to screen
	syscall #print integer to screen
	move $a0, $t7 #move $t7 to $a0 for printing to screen
	li $v0, 1 #load 1 into $v0, signifies printing an integer to screen
	syscall #print integer to screen
	la $a0, right #load right address into $a0
	li $v0, 4 #prepare to print to screen
	syscall # print ) to screen
	la $a0, newline #load adress of newline string into $a0
	li $v0, 4  #prepare to print string to screen
	syscall #print newline

twoCheckDone: #finsihed checkig all 2 digit combos


#######/-------4 digit bit-streams:-----/################################################################################################################


add $s2, $s4, $s2 #store sum of 1,2,3,4 in $s2
bne  $s2, 2, fourCheck1  #if sum !=2, (unbalanced) goto next 4 digits
li $t8, 2  #current maxLength is 4 digits
	la $a0, left #load left address into $a0
	li $v0, 4 #prepare to print string
	syscall #print ( to screen
	move $a0, $t1 #move $t1 to $a0 for printing to screen
	li $v0, 1  #load 1 into $v0, signifies printing an integer to screen
	syscall #print integer to screen
	move $a0, $t2 #move $t2 to $a0 for printing to screen
	li $v0, 1 #load 1 into $v0, signifies printing an integer to screen
	syscall #print integer to screen
	move $a0, $t3 #move $t3 to $a0 for printing to screen
	li $v0, 1  #load 1 into $v0, signifies printing an integer to screen
	syscall #print integer to screen
	move $a0, $t4 #move $t4 to $a0 for printing to screen
	li $v0, 1 #load 1 into $v0, signifies printing an integer to screen
	syscall #print integer to screen
	la $a0, right #load right address into $a0
	li $v0, 4 #prepare to print to screen
	syscall # print ) to screen
	la $a0, newline #load adress of newline string into $a0
	li $v0, 4  #prepare to print string to screen
	syscall #print newline

fourCheck1: #finished checking 1,2,3,4

add $s3, $s5, $s3 #store sum of 2,3,4,5 in $s3
bne  $s3, 2, fourCheck2 #if sum !=2, (unbalanced) goto next 4 digits
li $t8, 2  #current maxLength is 4 digits
	la $a0, left #load left address into $a0
	li $v0, 4 #prepare to print string
	syscall #print ( to screen
	move $a0, $t2 #move $t2 to $a0 for printing to screen
	li $v0, 1  #load 1 into $v0, signifies printing an integer to screen
	syscall #print integer to screen
	move $a0, $t3 #move $t3 to $a0 for printing to screen
	li $v0, 1 #load 1 into $v0, signifies printing an integer to screen
	syscall #print integer to screen
	move $a0, $t4 #move $t4 to $a0 for printing to screen
	li $v0, 1  #load 1 into $v0, signifies printing an integer to screen
	syscall #print integer to screen
	move $a0, $t5 #move $t5 to $a0 for printing to screen
	li $v0, 1 #load 1 into $v0, signifies printing an integer to screen
	syscall #print integer to screen
	la $a0, right #load right address into $a0
	li $v0, 4 #prepare to print to screen
	syscall # print ) to screen
	la $a0, newline #load adress of newline string into $a0
	li $v0, 4  #prepare to print string to screen
	syscall #print newline

fourCheck2: #finished checking 2,3,4,5

add $s4, $s4, $s6 #store sum of 3,4,5,6 in $s4
bne  $s4, 2, fourCheck3 #if sum !=2, (unbalanced) goto next 4 digits
li $t8, 2  #current maxLength is 4 digits
	la $a0, left #load left address into $a0
	li $v0, 4 #prepare to print string
	syscall #print ( to screen
	move $a0, $t3 #move $t3 to $a0 for printing to screen
	li $v0, 1  #load 1 into $v0, signifies printing an integer to screen
	syscall #print integer to screen
	move $a0, $t4 #move $t4 to $a0 for printing to screen
	li $v0, 1 #load 1 into $v0, signifies printing an integer to screen
	syscall #print integer to screen
	move $a0, $t5 #move $t5 to $a0 for printing to screen
	li $v0, 1  #load 1 into $v0, signifies printing an integer to screen
	syscall #print integer to screen
	move $a0, $t6 #move $t6 to $a0 for printing to screen
	li $v0, 1 #load 1 into $v0, signifies printing an integer to screen
	syscall #print integer to screen
	la $a0, right #load right address into $a0
	li $v0, 4 #prepare to print to screen
	syscall # print ) to screen
	la $a0, newline #load adress of newline string into $a0
	li $v0, 4  #prepare to print string to screen
	syscall #print newline


fourCheck3: #finished checking 3,4,5,6

add $s5, $s5, $s7 #store sum of 4,5,6,7 in $s5
bne  $s5, 2, fourCheckDone #if sum !=2, (unbalanced) exit fourCheck
li $t8, 2  #current maxLength is 4 digits
	la $a0, left #load left address into $a0
	li $v0, 4 #prepare to print string
	syscall #print ( to screen
	move $a0, $t4 #move $t4 to $a0 for printing to screen
	li $v0, 1  #load 1 into $v0, signifies printing an integer to screen
	syscall #print integer to screen
	move $a0, $t5 #move $t5 to $a0 for printing to screen
	li $v0, 1 #load 1 into $v0, signifies printing an integer to screen
	syscall #print integer to screen
	move $a0, $t6 #move $t6 to $a0 for printing to screen
	li $v0, 1  #load 1 into $v0, signifies printing an integer to screen
	syscall #print integer to screen
	move $a0, $t7 #move $t7 to $a0 for printing to screen
	li $v0, 1 #load 1 into $v0, signifies printing an integer to screen
	syscall #print integer to screen
	la $a0, right #load right address into $a0
	li $v0, 4 #prepare to print to screen
	syscall # print ) to screen
	la $a0, newline #load adress of newline string into $a0
	li $v0, 4  #prepare to print string to screen
	syscall #print newline
	
fourCheckDone: #finsihed checking all 4 digit  combos

#######/-------6 digit bit-streams:-----/################################################################################################################


add $s2, $s2, $s6 #store sum of 1,2,3,4,5,6 in $s2
bne $s2, 3, sixCheck1 #if sum !=3,  (unbalanced) goto next 6 digits
li $t8, 3 #load 3 into maxLength, maxlength=6 digits currently
	la $a0, left #load left address into $a0
	li $v0, 4 #prepare to print string
	syscall #print ( to screen
	move $a0, $t1 #move $t1 to $a0 for printing to screen
	li $v0, 1  #load 1 into $v0, signifies printing an integer to screen
	syscall #print integer to screen
	move $a0, $t2 #move $t2 to $a0 for printing to screen
	li $v0, 1 #load 1 into $v0, signifies printing an integer to screen
	syscall #print integer to screen
	move $a0, $t3 #move $t3 to $a0 for printing to screen
	li $v0, 1  #load 1 into $v0, signifies printing an integer to screen
	syscall #print integer to screen
	move $a0, $t4 #move $t4 to $a0 for printing to screen
	li $v0, 1 #load 1 into $v0, signifies printing an integer to screen
	syscall #print integer to screen
	move $a0, $t5 #move $t5 to $a0 for printing to screen
	li $v0, 1  #load 1 into $v0, signifies printing an integer to screen
	syscall #print integer to screen
	move $a0, $t6 #move $t6 to $a0 for printing to screen
	li $v0, 1 #load 1 into $v0, signifies printing an integer to screen
	syscall #print integer to screen
	la $a0, right #load right address into $a0
	li $v0, 4 #prepare to print to screen
	syscall # print ) to screen
	la $a0, newline #load adress of newline string into $a0
	li $v0, 4  #prepare to print string to screen
	syscall #print newline

sixCheck1: #finished checking first 6 digits

add $s3, $s3, $s7 #store sum of 2,3,4,5,6,7 n $s3
bne $s3, 3, sixCheckDone #if sum !=3,  (unbalanced) exit sixCheck
li $t8, 3 #load 3 into maxLength, maxlength=6 digits currently
	la $a0, left #load left address into $a0
	li $v0, 4 #prepare to print string
	syscall #print ( to screen
	move $a0, $t2 #move $t2 to $a0 for printing to screen
	li $v0, 1  #load 1 into $v0, signifies printing an integer to screen
	syscall #print integer to screen
	move $a0, $t3 #move $t3 to $a0 for printing to screen
	li $v0, 1 #load 1 into $v0, signifies printing an integer to screen
	syscall #print integer to screen
	move $a0, $t4 #move $t4 to $a0 for printing to screen
	li $v0, 1  #load 1 into $v0, signifies printing an integer to screen
	syscall #print integer to screen
	move $a0, $t5 #move $t5 to $a0 for printing to screen
	li $v0, 1 #load 1 into $v0, signifies printing an integer to screen
	syscall #print integer to screen
	move $a0, $t6 #move $t6 to $a0 for printing to screen
	li $v0, 1  #load 1 into $v0, signifies printing an integer to screen
	syscall #print integer to screen
	move $a0, $t7 #move $t7 to $a0 for printing to screen
	li $v0, 1 #load 1 into $v0, signifies printing an integer to screen
	syscall #print integer to screen
	la $a0, right #load right address into $a0
	li $v0, 4 #prepare to print to screen
	syscall # print ) to screen
	la $a0, newline #load adress of newline string into $a0
	li $v0, 4  #prepare to print string to screen
	syscall #print newline

sixCheckDone: ##Part A is done, execute part B

 #re-initialize all previously affected sums
	add $s2, $t1, $t2 #$s2 = digits 1+2
	add $s3, $t2, $t3 #$s3 = digits 2+3
	add $s4, $t3, $t4 #$s4 = digits 3+4
	add $s5, $t4, $t5 #$s5 = digits 4+5

#****************************************************************************************************************************************************************
#****************************************************************************************************************************************************************
#******/-------Part B: 6 digit bit-streams:-----/****************************************************************************************************************
#****************************************************************************************************************************************************************
#****************************************************************************************************************************************************************


#initialize sum for 6 digit checks
add $s2, $s2, $s6 #store sum of 1,2,5,6 in $s2
add $s2, $s2, $s4 #store sum of 1,2,3,4,5,6
bne $t8, 3, sixCheckEndb #if max is not 6 bits long, chec 4 bits next
bne $s2, 3, sixCheck1b #if sum !=3,  (unbalanced) goto next 6 digits
###print first 6 check indexes
	la $a0, string #load format string
	li $v0, 4 #prepare to print string
	syscall #print string
	li $a0,  0 #load first index
	li $v0, 1 #prepare to print in
	syscall #print first index
	la $a0, to #load 'to' string
	li $v0, 4 #prepare to print string
	syscall #print string
	li $a0, 5 #prepare toprint second index
	li $v0, 1 #load for int
	syscall #print second index
	la $a0, period #prepare .
	li $v0, 4 #prepare to print string
	syscall # print . to screen

sixCheck1b: #finished checking first 6 digits

add $s3, $s3, $s7 # store initial sums in $s3
add $s3, $s3, $s5 #store finals sum 2,3,4,5,6,7 in $s3
bne $s3, 3, sixCheckEndb#if sum !=3,  (unbalanced) exit sixCheck

	la $a0, string #load format string
	li $v0, 4 #prepare to print string
	syscall #print string
	li $a0,  1 #load first index
	li $v0, 1 #prepare to print in
	syscall #print first index
	la $a0, to #load 'to' string
	li $v0, 4 #prepare to print string
	syscall #print string
	li $a0, 6 #prepare toprint second index
	li $v0, 1 #load for int
	syscall #print second index
	la $a0, period #prepare .
	li $v0, 4 #prepare to print string
	syscall # print . to screen
		

sixCheckEndb:##done with sixCheck
beq $t8, 3, twoCheckDoneb ##if maxLength is 6, end program

#######/-------Part B: 4 digit bit-streams:-----/################################################################################################################
bne $t8, 2, fourCheckDoneb #if max is not 4 bits, jump to next
# reinitialize all sums previously affected
add $s2, $t1, $t2 #$s2 = digits 1+2
add $s3, $t2, $t3 #$s3 = digits 2+3
add $s2, $s4, $s2 #store sum of 1,2,3,4 in $s2
bne  $s2, 2, fourCheck1b  #if sum !=2, (unbalanced) goto next 4 digits
	la $a0, string #load format string
	li $v0, 4 #prepare to print string
	syscall #print string
	li $a0,  0 #load first index
	li $v0, 1 #prepare to print in
	syscall #print first index
	la $a0, to #load 'to' string
	li $v0, 4 #prepare to print string
	syscall #print string
	li $a0, 3 #prepare toprint second index
	li $v0, 1 #load for int
	syscall #print second index
	la $a0, period #prepare .
	li $v0, 4 #prepare to print string
	syscall # print . to screen

fourCheck1b: #finished checking 1,2,3,4

add $s3, $s5, $s3 #store sum of 2,3,4,5 in $s3
bne  $s3, 2, fourCheck2b #if sum !=2, (unbalanced) goto next 4 digits
	la $a0, string #load format string
	li $v0, 4 #prepare to print string
	syscall #print string
	li $a0,  1 #load first index
	li $v0, 1 #prepare to print in
	syscall #print first index
	la $a0, to #load 'to' string
	li $v0, 4 #prepare to print string
	syscall #print string
	li $a0, 4 #prepare toprint second index
	li $v0, 1 #load for int
	syscall #print second index
	la $a0, period #prepare .
	li $v0, 4 #prepare to print string
	syscall # print . to screen

fourCheck2b: #finished checking 2,3,4,5

add $s4, $s4, $s6 #store sum of 3,4,5,6 in $s4
bne  $s4, 2, fourCheck3b #if sum !=2, (unbalanced) goto next 4 digits
	la $a0, string #load format string
	li $v0, 4 #prepare to print string
	syscall #print string
	li $a0,  2 #load first index
	li $v0, 1 #prepare to print in
	syscall #print first index
	la $a0, to #load 'to' string
	li $v0, 4 #prepare to print string
	syscall #print string
	li $a0, 5 #prepare toprint second index
	li $v0, 1 #load for int
	syscall #print second index
	la $a0, period #prepare .
	li $v0, 4 #prepare to print string
	syscall # print . to screen


fourCheck3b: #finished checking 3,4,5,6

add $s5, $s5, $s7 #store sum of 4,5,6,7 in $s5
bne  $s5, 2, fourCheckDoneb#if sum !=2, (unbalanced) exit fourCheck
	la $a0, string #load format string
	li $v0, 4 #prepare to print string
	syscall #print string
	li $a0,  3 #load first index
	li $v0, 1 #prepare to print in
	syscall #print first index
	la $a0, to #load 'to' string
	li $v0, 4 #prepare to print string
	syscall #print string
	li $a0, 6 #prepare toprint second index
	li $v0, 1 #load for int
	syscall #print second index
	la $a0, period #prepare .
	li $v0, 4 #prepare to print string
	syscall # print . to screen
	
fourCheckDoneb: #finsihed checking all 4 digit  combos
beq $t8, 2, twoCheckDoneb#done computing max length, end program

#######/-------Part B: 2 digit bit-streams:-----/################################################################################################################

 #re-initialize all previously affected sums
	add $s2, $t1, $t2 #$s2 = digits 1+2
	add $s3, $t2, $t3 #$s3 = digits 2+3
	add $s4, $t3, $t4 #$s4 = digits 3+4
	add $s5, $t4, $t5 #$s5 = digits 4+5
beq $t1, $t2, twoCheck1b #if 1=2, then go to next (unbalanced)
#if balanced, print result
	la $a0, string #load format string
	li $v0, 4 #prepare to print string
	syscall #print string
	li $a0,  0 #load first index
	li $v0, 1 #prepare to print in
	syscall #print first index
	la $a0, to #load 'to' string
	li $v0, 4 #prepare to print string
	syscall #print string
	li $a0, 1 #prepare toprint second index
	li $v0, 1 #load for int
	syscall #print second index
	la $a0, period #prepare .
	li $v0, 4 #prepare to print string
	syscall # print . to screen
	

twoCheck1b: #finished checking 1,2

beq $t2, $t3, twoCheck2b #if 2=3, then go to next (unbalanced)
#if balanced, print result
	la $a0, string #load format string
	li $v0, 4 #prepare to print string
	syscall #print string
	li $a0,  1 #load first index
	li $v0, 1 #prepare to print in
	syscall #print first index
	la $a0, to #load 'to' string
	li $v0, 4 #prepare to print string
	syscall #print string
	li $a0, 2 #prepare toprint second index
	li $v0, 1 #load for int
	syscall #print second index
	la $a0, period #prepare .
	li $v0, 4 #prepare to print string
	syscall # print . to screen
	

twoCheck2b: #finished checking 2,3

beq $t3, $t4, twoCheck3b #if 3=4, then go to next (unbalanced)
#if balanced, print result
	la $a0, string #load format string
	li $v0, 4 #prepare to print string
	syscall #print string
	li $a0,  2 #load first index
	li $v0, 1 #prepare to print in
	syscall #print first index
	la $a0, to #load 'to' string
	li $v0, 4 #prepare to print string
	syscall #print string
	li $a0, 3 #prepare toprint second index
	li $v0, 1 #load for int
	syscall #print second index
	la $a0, period #prepare .
	li $v0, 4 #prepare to print string
	syscall # print . to screen
	
twoCheck3b: #finished checking 3,4

beq $t4, $t5, twoCheck4b #if 4=5, then go to next (unbalanced)
#if balanced, print result
	la $a0, string #load format string
	li $v0, 4 #prepare to print string
	syscall #print string
	li $a0,  3 #load first index
	li $v0, 1 #prepare to print in
	syscall #print first index
	la $a0, to #load 'to' string
	li $v0, 4 #prepare to print string
	syscall #print string
	li $a0, 4 #prepare toprint second index
	li $v0, 1 #load for int
	syscall #print second index
	la $a0, period #prepare .
	li $v0, 4 #prepare to print string
	syscall # print . to screen
	
	
twoCheck4b: #finished checking 4,5

beq $t5, $t6, twoCheck5b #if 5=6, then go to next (unbalanced)
#if balanced, print result
	la $a0, string #load format string
	li $v0, 4 #prepare to print string
	syscall #print string
	li $a0,  4 #load first index
	li $v0, 1 #prepare to print in
	syscall #print first index
	la $a0, to #load 'to' string
	li $v0, 4 #prepare to print string
	syscall #print string
	li $a0, 5 #prepare toprint second index
	li $v0, 1 #load for int
	syscall #print second index
	la $a0, period #prepare .
	li $v0, 4 #prepare to print string
	syscall # print . to screen

twoCheck5b: #finished checking 5,6

beq $t6, $t7, twoCheckDoneb #if 6=7, twoDigitCheck is done (unbalanced)
#if balanced, print result
	la $a0, string #load format string
	li $v0, 4 #prepare to print string
	syscall #print string
	li $a0,  5 #load first index
	li $v0, 1 #prepare to print in
	syscall #print first index
	la $a0, to #load 'to' string
	li $v0, 4 #prepare to print string
	syscall #print string
	li $a0, 6 #prepare toprint second index
	li $v0, 1 #load for int
	syscall #print second index
	la $a0, period #prepare .
	li $v0, 4 #prepare to print string
	syscall # print . to screen
twoCheckDoneb: #finsihed checking all 2 digit combos




li $v0,10  #prepare to end program
syscall #end program



