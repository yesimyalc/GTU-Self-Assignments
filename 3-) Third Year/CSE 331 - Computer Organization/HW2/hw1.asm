.data
#arrays
start_arr: -3,4,12,7,8,-21,43,23,9,2
start_arr2: 12,5,-2,8,5,3,-11,1
start_arr3: 3,10,4,7,9,11
start_arr4: 4,9,2,5,-7,3,12,8
start_arr5: 23,-56,2,34,-21,78
start_arr6: -101,23,45,-67,2,3,1,89
temp_arr: .space 40
result_arr: .space 40
#consol messages
found: .asciiz "Found sequence: "
nl: .asciiz "\n"
comma: .asciiz ","
#file 
finput: .asciiz "input.txt"
foutput: .asciiz "output.txt"
buffer: .asciiz "Array_outpt: ["
buffer2: .space 32
output_end: .asciiz "] size="

.text
#####File Prep#####
###Open Input File###
li $v0, 13 #file open to read command
la $a0, finput #file name
li $a1, 0 #read
li $a2, 0 #mode ignored
syscall 
add $s5, $zero, $v0 #file is saved to s5
###Read From Input File###
###Open Output File###
li $v0, 13 #file open to write result
la $a0, foutput #file name
li $a1, 1#write
li $a2, 0 #mode ignored
syscall
add $s6, $zero, $v0 #file is saved to s6

la $t3, start_arr6
MainLoop:beq $s0, $t3, end
#####Initializations#####
jal find_start_arr
la $s0 ($v0) #s0=start_arr
la $s1, temp_arr #s1=temp_arr
la $s2, result_arr #s2=result_arr
addi $s3, $zero, 0 #s3=size for result_arr. Initialized as 0
####################

###Write to Output File###
li $v0, 15 #write command
add $a0, $zero, $s6 #the file name is transfered
la $a1, buffer #the buffer will be written
li $a2, 14 #buffer length is 14
syscall
####################

#####Finding Array Length#####
sub $t0, $v1, $s0
sra $s7, $t0, 2
####################

#####Loop Start#####
addi $t0, $zero, 0 #t0=i for the for loop. Initialized as 0
Loop: beq $t0, $s7, loop_end #if i=array length end the loop
	addi $s4, $zero, 0 #s4=size_counter for temp_arr. Initialized as 0
	#assign_array_values procedure call for temp_arr[size_counter]=start_arr[i]
	add $a0 $zero, $s1 #a0=temp_arr
	add $a1, $zero, $s4 #a1=size_counter
	add $a2, $zero, $s0 #a2=start_arr
	add $a3, $zero, $t0 #a3=i
	jal assign_array_values
	addi $s4, $s4, 1 #size_counter++
	#printing the very first sequence
	add $a1, $zero, $s4 #a1=size_counter
	add $a2, $zero, $s1 #a2=temp_arr
	jal print_sequence
	#inner for loop start
	addi $t1, $t0, 1 #t1=j for the inner loop. Initialized as i+1
	Loop2: beq $t1, $s7, loop2_end #if j=array length end the loop
		#start_arr[j] calculation
		add $t2, $t1, $t1
		add $t2, $t2, $t2
		add $t2, $t2, $s0
		lb $t2, 0($t2)
		#temp_arr[size_counter-1] calculation
		addi $t3, $s4, -1 #size_counter-1
		add $t3, $t3, $t3
		add $t3, $t3, $t3
		add $t3, $t3, $s1
		lb $t3, 0($t3)
		#if condition for start_arr[j]>temp_arr[size_counter-1]
		slt $t3, $t3, $t2 #star_arr[j]>temp_arr[size_counter-1]
		beq $t3, $zero if_statement_end #if it is not 0 go to if_statement (meaning star_arr[j]>temp_arr[size_counter-1]) 
			#assign_array_values procedure call for temp_arr[size_counter]=start_arr[j]
			add $a0 $zero, $s1 #a0=temp_arr
			add $a1, $zero, $s4 #a1=size_counter
			add $a2, $zero, $s0 #a2=start_arr
			add $a3, $zero, $t1 #a3=j
			jal assign_array_values
			addi $s4, $s4, 1 #size_counter++
			#printing the found sequence
			add $a1, $zero, $s4 #a1=size_counter
			add $a2, $zero, $s1 #a2=temp_arr
			jal print_sequence
		if_statement_end:
		addi $t1, $t1, 1 #j++
		j Loop2
	loop2_end:
	#if condition for size_counter>size
	slt $t2, $s3, $s4 #size_counter>size
	beq $t2, $zero, if_statement2_end #if not zero enter the if statement (meaning size_counter>size)
		#All the contents inside temp_arr will be loaded into result_arr
		add $s3, $0, $zero #size=0, it is reseted
		#for loop start
		addi $t2, $zero, 0 #t2=k. Initialized as 0
		Loop4: beq $t2, $s4, loop4_end # if k=size_counter end the loop
			#temp_arr[k] calculation
			add $t3, $t2, $t2
			add $t3, $t3, $t3
			add $t4, $t3, $s1
			lb $t4, 0($t4)
			#result_arr[k] calculation
			add $t3, $t3, $s2
			#result_arr[k]=temp_arr[k]
			sw $t4, 0($t3)
			addi $s3, $s3 ,1
			addi $t2, $t2, 1
			j Loop4
		loop4_end:
	if_statement2_end:
addi $t0, $t0, 1 #i++
j Loop
loop_end: 
####################

#printing the found sequence
add $a1, $zero, $s3
add $a2, $zero, $s2
jal print_sequence

la $t3, start_arr6
#print new line
li $v0, 4
la $a0, nl
syscall
j MainLoop
end:
#####File Closing#####
#Close File
li $v0, 16
add $a0, $zero, $s6
syscall
#Close File
li $v0, 16
add $a0, $zero, $s5
syscall
####################

#####Exit#####
li $v0 10
syscall
####################





#####Assign_Array_Values Procedure#####
###B[j]=A[i]###
assign_array_values:
#A[i] calculation
add $t2, $a3, $a3 #i*2
add $t2, $t2, $t2 #t2=i*4 for addressing
add $t2, $t2, $a2 #A+i*4 = A[i]
lb $t2, 0($t2) #t2=content of A[i]
#B[j] calculation
add $t3, $a1, $a1 #j*2
add $t3, $t3, $t3 #t3=j*4
add $t3, $t3, $a0 #B+j*4 = B[j]
sw $t2, 0($t3) #B[j]=A[i]
jr $ra
####################

#####Print_Sequence Procedure#####
print_sequence: 
#Print "Found Sequence"
addi $sp, $sp, -4 #opening space in stack
sw $ra, 0($sp) #ra is saved in case itoa procedure needs to be called
li $v0, 4
la $a0, found
syscall
#for loop to print values
add $t2, $zero, 0 #t2=m for the loop. Initialized as 0
add $t4, $zero, $a1 #the size
add $t5, $zero, $a2 #the array
Loop3: beq $t2, $t4, print_loop_end #if m=the size exit the loop
	#array[m] calculation
	add $t3, $t2, $t2
	add $t3, $t3, $t3
	add $t3, $t3, $t5
	lb $t3, 0($t3)
	#printing the value
	li $v0, 1
	add $a0, $zero, $t3
	syscall
	bne $s2, $t5 skip_write_value #if the array is not the result array, skip write value
	#writing the value into output file
	add $a0, $zero, $t3 #a0=array[m]
	jal itoa #value gets converted into string
	add $t7, $zero, $v0 #value info in v0 is saved to t7
	#array[m] calculation again because i lost the value
	add $t3, $t2, $t2
	add $t3, $t3, $t3
	add $t3, $t3, $t5
	lb $t3, 0($t3)
	add $a0, $zero, $t3 #a0=array[m]
	jal count_digit #the count of digits in array[m] is counted
	add $t8, $zero, $v0 #count of digits is stored in t8
	li $v0, 15
	add $a0, $zero, $s6
	add $a1, $zero, $t7
	move $a2, $t8
	syscall #array[m] is written into the output file
	skip_write_value:
	add $t3, $t4, -1 #t3=the size-1 to determine if comma is needed
	beq $t2, $t3  print_loop_end #if m=the size-1 then comma is not needed so exit the loop
	li $v0, 4
	la $a0, comma
	syscall
	bne $s2, $t5 skip_write_comma #if the array is not the result array, skip write comma
	#write comma to the output file
	li $v0, 15
	add $a0, $zero, $s6
	la $a1, comma
	li $a2, 1
	syscall
	skip_write_comma:
	addi $t2, $t2, 1 #m++
	j Loop3
print_loop_end:
#print new line
li $v0, 4
la $a0, nl
syscall
bne $s2, $t5 skip_write_end #if the array is not the result array, skip write output end
#write output_end and size to output file
li $v0, 15
add $a0, $zero, $s6
la $a1, output_end
li $a2, 7
syscall #print output_end
add $a0, $zero, $s3 #a0=size
jal itoa #size gets converted into string
add $t7, $zero, $v0 #size info in v0 is saved to t7
li $v0, 15
add $a0, $zero, $s6
add $a1, $zero, $t7
li $a2, 1
syscall #print size value
li $v0, 15
add $a0, $zero, $s6
la $a1, nl
li $a2, 1
syscall #print new line
skip_write_end:
lw $ra, 0($sp)
addi $sp, $sp, 4 #restoring stack
jr $ra
####################

#####Find Start Array#####
find_start_arr:
beq $s0, 0, start_arr_b #if currently has no array go to start_arr
la $t0, start_arr
beq $s0, $t0, start_arr2_b #if it is currently start_arr go to start_arr2
la $t0, start_arr2
beq $s0, $t0, start_arr3_b #if it is currently start_arr2 go to start_arr3
la $t0, start_arr3
beq $s0, $t0, start_arr4_b #if it is currently start_arr3 go to start_arr4
la $t0, start_arr4
beq $s0, $t0, start_arr5_b #if it is currently start_arr4 go to start_arr5
la $t0, start_arr5 
beq $s0, $t0, start_arr6_b #if it is currently start_arr5 go to start_arr6

start_arr_b:
la $v0, start_arr
la $v1, start_arr2 #following one registered as start_arr2
jr $ra
start_arr2_b:
la $v0, start_arr2
la $v1, start_arr3 #following one registered as start_arr3
jr $ra
start_arr3_b:
la $v0, start_arr3
la $v1, start_arr4 #following one registered as start_arr4
jr $ra
start_arr4_b:
la $v0, start_arr4
la $v1, start_arr5 #following one registered as start_arr5
jr $ra
start_arr5_b:
la $v0, start_arr5
la $v1, start_arr6 #following one registered as start_arr6
jr $ra
start_arr6_b:
la $v0, start_arr6
la $v1, temp_arr #following one registered as temp_arr
jr $ra
####################

#####Counting Digit Number#####
count_digit:
addi $v0, $zero, 0 #initialize the size as 0
slt $t9, $v0, $a0
beq $t9, 1, dont_add #checks if the number is negative. if so increment the size
addi $v0, $v0, 1
dont_add:
add $t9, $zero, 10
add $t8, $zero, $a0 #t8=the number
DivLoop: beq $t8, $zero, divLoop_end #if the division is not zero keep looping
	div $t8, $t9
	mflo $t8 #t8=division result
	addi $v0, $v0, 1 #count++
	j DivLoop
divLoop_end:
jr $ra
####################

#####Converting Int->String#####
itoa:
      la   $t3, buffer2+30  # pointer to almost-end of buffer
      sb   $0, 1($t3)      # null-terminated str
      li   $t6, '0'  
      sb   $t6, ($t3)     # init. with ascii 0
      li   $t8, 10        # preload 10

      slt  $t7, $a0, $0   # keep the sign
      beq  $a0, $0, iend  # end if 0
      bgtz $a0, loop
      neg  $a0, $a0       # absolute value (unsigned)
loop:
      div  $a0, $t8       # a /= 10
      mflo $a0
      mfhi $t9            # get remainder
      add  $t9, $t9, $t6  # convert to ASCII digit
      sb   $t9, ($t3)     # store it
      sub  $t3, $t3 1    # dec. buf ptr
      bne  $a0, $0, loop  # if not zero, loop
      addi $t3, $t3, 1    # adjust buf ptr
iend:
      beq  $t7, $0, nolz  # was < 0?
      addi $t3, $t3, -1
      li   $t6, '-'
      sb   $t6, ($t3)
nolz:
      move $v0, $t3      # return the address of the string
      jr   $ra          
####################









