# Ryan Koons
# Project1_2.asm--A program for reading string entered by user and regarding the number of
# instances of a particular character: "c,h,a,r,g,e,o,n (accounting for the case of
# each letter. The program then prints out a histogram demonstrating the amount of each
# letter. The program will convert the entire string to lower case to minimze the number
# of necessary checks, and then print out the occurrence of each letter.
#
#
# Registers used: 
# 
#
# $t2: first bytes pointed to by the address (character)
# $t7: first bytes pointed to by the address for computing count
# $t8: Used as counter during Histogram printing
# characters in the user's lowercased string
# $s0: address that stores computed lowercase string from user input (lowercase) (input)
# $s2: counter for c
# $s3: counter for h
# $s4: counter for a
# $s5: counter for r
# $s6: counter for g
# $s7: counter for e
# $t0: counter for o
# $t1: counter for n
# $t8: register for counter to print out  histogram for each letter
# $v0: register used for passing integers for Syscalls
# $a0: register that stores interactive address during syscalls
# $a1: Register that stores max characters to read from user's string
#
#
#
#/-------labels:-----/  
#
###########input:
# input: Address for storing user's entered string
# string: String we want to convert to lowercase
# newline: nice newline for formatting output
# main: denotes start of main function
#
###################Upper to Lower
# toLowercase: indicates start of every single byte, prior to checks, load current byte into reg
#endConvert, signifies end of Upper to Lower conversion
#continue: goes to next byte of string (for conversion)
#isUppercase: byte needs to be converted to lowercase
#
#
#################Counting 
# cCount: label for incrementing c Counter
# hCount: label for incrementing h Counter
# aCount: label for incrementing a Counter
# rCount: label for incrementing r Counter
# gCount: label for incrementing g Counter
# eCount: label for incrementing e Counter
# oCount: label for incrementing o Counter
# nCount: label for incrementing n Counter
# next: no characters were detected, go to next character for second part
#print: label to denote beginning of print section
#nextLetter begins checking for next letter of lowercase string
#
#####Printing
# c: address for capital letter C output
# h: address for capital letter H output
# a: address for capital letter A output
# r: address for capital letter R output
# g: address for capital letter G output
# e: address for capital letter E output
# o: address for capital letter O output
# n: address for capital letter N output
#
#
#
############Histogram
#num: address for string to print out #
# cPrint: start of loop to print out histogram section for C
# hPrint: start of loop to print out histogram section for H
# arint: start of loop to print out histogram section for A
# rPrint: start of loop to print out histogram section for R
# gPrint: start of loop to print out histogram section for G
# ePrint: start of loop to print out histogram section for E
# oPrint: start of loop to print out histogram section for O
# nPrint: start of loop to print out histogram section for N
# Histh: label that shows start of hist H
# Hista: label that shows start of hist H
# Histr: label that shows start of hist H
# Histg: label that shows start of hist H
# Histe: label that shows start of hist H
# Histo: label that shows start of hist H
# Histn: label that shows start of hist H
# Histdone: label that shows histogram is done, time to exit program





.data  #section for data
 ###########OPTIMIZE: NEWLINES IN STRINGS
#newline: .asciiz "\n"  #newline for simple output
input: .space 500 #address for storing string 
c: .asciiz "\n\nC:" #prints out capital C for output
h: .asciiz "\nH:" #prints out capital H for output
a: .asciiz "\nA:" #prints out capital A for output
r: .asciiz "\nR:" #prints out capital R for output
g: .asciiz "\nG:" #prints out capital G for output
e: .asciiz "\nE:" #prints out capital E for output
o: .asciiz "\nO:" #prints out capital O for output
n: .asciiz "\nN:" #prints out capital N for output
num: .asciiz "#" #print out number sign

.text  #text section of program
main:    #label for main function
	
	

##########read string from user
li $a1, 500 #prepare 500 chars max to be inputted by user
li $v0, 8 #load 8 into $v0 to prepare to read string input from user
la $a0, input #load input address into register $a0
syscall #read string
move $s0, $a0 #move $a0 to $s0 for converting string to lowercase
		
toLowerCase: #label denoting start of toLowercase function
	lb $t2, 0($s0)# take first byte of string pointed to by address
	beqz $t2, endConvert # if first byte is equal to zero, the string is terminated  
	blt $t2, 65, continue   #if current character less than 65, jump to next increment
	#bgt $t2, 114, continue
        ble $t2, 82, isUpperCase #if current value is less than 90, go to converter      
	 continue:  # Continue the iteration
  
  addi $s0, $s0, 1 # Increment the address  
  j toLowerCase #unconditional jump back to lowercase
  
isUpperCase:  addi $t2, $t2, 32         # add 32 to current value (convert to lower)      
     sb $t2, 0($s0)
     addi $s0, $s0, 1  # store it in the string  
     j  toLowerCase #  continue  iteration  as always 
 
  
endConvert: #denotes end of conversion, string is now lowered


	la $s0, input #load string lowercase into $s0
	nextLetter: lb $t7, 0($s0)# Take current byte of string (character) load into $t7 
	beqz $t7, print #if we get to end of string, exit
	blt $t7, 97, next  ####OPTIMIZE
	beq $t7, 99, cCount #if this character is equal to c, go to cCount
	beq $t7, 104, hCount #if this character is equal to h, go to hCount
	beq $t7, 97, aCount #if this character is equal to a, go to aCount
	beq $t7, 114, rCount #if this character is equal to r, go to rCount
	beq $t7, 103, gCount#if this character is equal to g, go to gCount
	beq $t7, 101, eCount#if this character is equal to e, go to eCount
	beq $t7, 111, oCount #if this character is equal to o, go to oCount
	beq $t7, 110, nCount#if this character is equal to n, go to nCount
	j next #jump to nexr character
	
	cCount:addi $s2, $s2, 1 #increment counter by 1
	j next #jump to next character after incrementing
	
	hCount:addi $s3, $s3, 1 #increment counter by 1
	j next #jump to next character after incrementing
	
	aCount:addi $s4, $s4, 1 #increment counter by 1
	j next #jump to next character after incrementing
	
	rCount:addi $s5, $s5, 1 #increment counter by 1
	j next #jump to next character after incrementing
	
	gCount:addi $s6, $s6, 1 #increment counter by 1
	j next #jump to next character after incrementing
	
	eCount:addi $s7, $s7, 1 #increment counter by 1
	j next #jump to next character after incrementing
	
	oCount:addi $t0, $t0, 1 #increment counter by 1
	j next #jump to next character after incrementing
	
	nCount:addi $t1, $t1, 1 #increment counter by 1
	j next #jump to next character after incrementing
	
	 next: #prepare to move to next character
	addi $s0, $s0, 1 #go to next address of string (next char)
	j nextLetter #go back through checks again, at next char
	
    
   print:  #label to denote beginning of print section
   
   #print C
   la $a0, c
   li $v0, 4
   syscall
   #load in counter into address and print to screen
   la $a0, ($s2)
   li $v0, 1
   syscall
   
   #print H
   la $a0, h
   li $v0, 4
   syscall
   #load in counter into address and print to screen
   la $a0, ($s3)
   li $v0, 1
   syscall
   
   
   #print A
   la $a0, a
   li $v0, 4
   syscall
   #load in counter into address and print to screen
   la $a0, ($s4)
   li $v0, 1
   syscall
   
   
   #print R
   la $a0, r
   li $v0, 4
   syscall
   #load in counter into address and print to screen
   la $a0, ($s5)
   li $v0, 1
   syscall
   
   
   #print G
   la $a0, g
   li $v0, 4
   syscall
   #load in counter into address and print to screen
   la $a0, ($s6)
   li $v0, 1
   syscall
   
   
   #load E
   la $a0, e
   li $v0, 4
   syscall
   #load in counter into address and print to screen
   la $a0, ($s7)
   li $v0, 1
   syscall
   
   
   #load O
   la $a0, o
   li $v0, 4
   syscall
   #load in counter into address and print to screen
   la $a0, ($t0)
   li $v0, 1
   syscall
   
   #print N
   la $a0, n
   li $v0, 4
   syscall
   #load in counter into address and print to screen
   la $a0, ($t1)
   li $v0, 1
   syscall
   
   #########print out Hist
   
 #print C
   add $t8, $0, $0#counter =0
   la $a0, c
   li $v0, 4
   syscall
   cPrint: beq $t8, $s2, Histh #start of loop to increment through hist
   #Print out #
   la $a0, num 
   li $v0, 4
   syscall
   addi $t8, $t8, 1 #increment counter
   j cPrint #back to start of loop
   Histh:# print next newline for next hist
   
   #print H
  add $t8, $0, $0#counter=0
   la $a0, h
   li $v0, 4
   syscall
   hPrint: beq $t8, $s3, Hista #start of loop to increment through hist
   # print out #
   la $a0, num
   li $v0, 4
   syscall
   addi $t8, $t8, 1#increment counter
   j hPrint #back to start of loop
   Hista: #print next newline for next hist
   
   
   #Print A
   add $t8, $0, $0 #counter=0
   la $a0, a
   li $v0, 4
   syscall
   aPrint: beq $t8, $s4, Histr #start of next loop
   # print out #
   la $a0, num
   li $v0, 4
   syscall
   addi $t8, $t8, 1 #increment counter
   j aPrint #go back to loop start
   Histr: #time for next hist
   
   
   #Print R
   add $t8, $0, $0#counter =0
   la $a0, r
   li $v0, 4
   syscall
   rPrint: beq $t8, $s5, Histg #start of R increment loop
   #print num sign
   la $a0, num
   li $v0, 4
   syscall
   addi $t8, $t8, 1 #increment counter
   j rPrint #back to loop start
   Histg: #next hist and newline
   
   
   #print G
   add $t8, $0, $0 #re-init count to 0
   la $a0, g
   li $v0, 4
   syscall
   gPrint: beq $t8, $s6, Histe #start of loop for G hist
   #print num sign
   la $a0, num
   li $v0, 4
   syscall
   addi $t8, $t8, 1 #increment loop counter
   j gPrint #back to start of loop
   Histe:# next hist and newline
  
   
   #print out E
   add $t8, $0, $0 #count=0
   la $a0, e
   li $v0, 4
   syscall
   ePrint: beq $t8, $s7, Histo #start of E loop count
   #print out #
   la $a0, num
   li $v0, 4
   syscall
   addi $t8, $t8, 1 #increment count
   j ePrint #back to loop start
   Histo: #newxt hist and newline
   
   
   #print out O
   add $t8, $0, $0 #count=0
   la $a0, o
   li $v0, 4
   syscall
   oPrint: beq $t8, $t0, Histn #next loop for o Hist
   #print out #
   la $a0, num
   li $v0, 4
   syscall
   addi $t8, $t8, 1 #increment count
   j oPrint# back to loop start
   Histn: #next hist and newline
   
   
   #print out N
   add $t8, $0, $0#increment counter
   la $a0, n
   li $v0, 4
   syscall
   nPrint: beq $t8, $t1, Histdone #start of final loop for N hist
   #print out #
   la $a0, num
   li $v0, 4
   syscall
   addi $t8, $t8, 1 #increment loop counter
   j nPrint #back to beginning of this loop
   Histdone: #done
 
   #DONE
   
     li $v0, 10 #exit program
     syscall #exit
