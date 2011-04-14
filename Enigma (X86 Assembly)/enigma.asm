.data
  ROTOR_FILE: .asciiz "data/rotors.txt"
  INIT_FILE: .asciiz "data/init.txt"
  PLAINTEXT_FILE: .asciiz "data/plaintext.txt"
  OUTPUT_FILE: .asciiz "data/ciphertext.txt"
  LOG_FILE: .asciiz "data/log.txt"

	buffer: .space 4096

  D1L: .space 64  
  D2L: .space 64  
  D3L: .space 64  

  D1R: .space 64  
  D2R: .space 64  
  D3R: .space 64  

  DR: .space 32

  d1: .space 1
  d2: .space 1
  d3: .space 1

  all: .space 512


.text
	.globl main

main:
  # jal print_drums
  # jal rotate_drums
  # jal print_drums
  jal init_drums
  jal encrypt_file

  # li $a0, 65
  # jal encrypt_char
  # move $a0, $v0
  # li $v0, 1
  # syscall
  # jal encrypt_char
  # move $a0, $v0
  # li $v0, 1
  # syscall

  ## print_drum
  # la $a0, D1L
  # li $a1, 64
  # jal print_drum

  ## print_drum
  # la $a0, D1L
  # li $a1, 64
  # jal print_drum
	
  exit:
    li $v0, 10
    syscall

  encrypt_file:
    ## prologue
    sub $sp, $sp, 8 # allocate space on stack
    sw $ra, 8($sp)  # save return address
    sw $s0, 4($sp)  # save caller frame pointer
    add $s0, $sp, 8 # set procedure frame pointer

    ## open ciphertext for writing
    li $v0, 13 # open
    la $a0, OUTPUT_FILE # file name
    li $a1, 1  # write
    li $a2, 0  # mode is ignored 
    syscall # returns fs in $v0
    move $s4, $v0 # save file descriptor 

    li $v0, 13 # open
    la $a0, PLAINTEXT_FILE # file name
    li $a1, 0  # O_RDONLY
    li $a2, 0  # mode is ignored 
    syscall # returns fs in $v0
    move $s1, $v0 # save file descriptor to s0

    encrypt_file_loop:
      li $v0, 14 # read from file
      move $a0, $s1 # fd 
      la $a1, buffer # buffer where to write
      li $a2, 1# number of chars to be read
      syscall 
      move $s3, $v0 # save number of chars read

      lb $a0, buffer
      # li $v0, 1
      # syscall
      bne $a0, 10, not_newline


      newline:
        ## write it out
        # li $v0, 15
        # move $a0, $s4
        # la $a1, buffer
        # li $a2, 1
        # syscall

        ## write it to stdout
        li $v0, 11
        syscall

        bnez $s3, encrypt_file_loop

      not_newline:
        jal encrypt_char
        move $a0, $v0

        ## write it to stdout
        li $v0, 11
        syscall
        ## write it out
        # li $v0, 15
        # move $a0, $s4
        # la $a1, buffer
        # li $a2, 1
        # syscall

        bnez $s3, encrypt_file_loop

    ## close file
    move $a0, $s1
    li $v0, 16 # close file
    syscall 

    ## close file
    move $a0, $s4
    li $v0, 16 # close file
    syscall 

    ## epilogue
    lw $ra, ($s0)   # restore return address
    move $t0, $s0   # save frame pointer temporarily
    lw $s0, -4($s0) # restore frame pointer
    move $sp, $8
    jr $ra
  ## a0 - character
  ## v0 - index
  index:
    ## prologue
    sub $sp, $sp, 8 # allocate space on stack
    sw $ra, 8($sp)  # save return address
    sw $16, 4($sp)  # save caller frame pointer
    add $16, $sp, 8 # set procedure frame pointer
    
    sub $v0, $a0, 32

    ## epilogue
    lw $ra, ($16)   # restore return address
    move $t0, $16   # save frame pointer temporarily
    lw $16, -4($16) # restore frame pointer
    move $sp, $8
    jr $ra

  ## a0 - index
  ## v0 - character
  character:
    ## prologue
    sub $sp, $sp, 8 # allocate space on stack
    sw $ra, 8($sp)  # save return address
    sw $16, 4($sp)  # save caller frame pointer
    add $16, $sp, 8 # set procedure frame pointer
    
    add $v0, $a0, 32

    ## epilogue
    lw $ra, ($16)   # restore return address
    move $t0, $16   # save frame pointer temporarily
    lw $16, -4($16) # restore frame pointer
    move $sp, $8
    jr $ra

  ## a0 - index
  ## v0 - encrypted index
  encrypt:
    ## prologue
    sub $sp, $sp, 8 # allocate space on stack
    sw $ra, 8($sp)  # save return address
    sw $16, 4($sp)  # save caller frame pointer
    add $16, $sp, 8 # set procedure frame pointer

    move $t0, $a0
    lb $t0, D1R($t0)
    lb $t0, D2R($t0)
    lb $t0, D3R($t0)

    lb $t0, DR($t0)

    lb $t0, D3L($t0)
    lb $t0, D2L($t0)
    lb $t0, D1L($t0)

    move $v0, $t0

    jal rotate_drums

    ## epilogue
    lw $ra, ($16)   # restore return address
    move $t0, $16   # save frame pointer temporarily
    lw $16, -4($16) # restore frame pointer
    move $sp, $8
    jr $ra

  ## a0 - character
  ## v0 - encrypted character
  encrypt_char:
    ## prologue
    sub $sp, $sp, 8 # allocate space on stack
    sw $ra, 8($sp)  # save return address
    sw $16, 4($sp)  # save caller frame pointer
    add $16, $sp, 8 # set procedure frame pointer

    jal index
    ## v0 holds index
    move $a0, $v0
    jal encrypt
    ## v0 holds encrypted index
    move $a0, $v0
    jal character
    ## v0 holds encrypted character

    ## epilogue
    lw $ra, ($16)   # restore return address
    move $t0, $16   # save frame pointer temporarily
    lw $16, -4($16) # restore frame pointer
    move $sp, $8
    jr $ra

  print_drums:
    sub $sp, $sp, 8
    sw $ra, 8($sp) # save return address
    sw $16, 4($sp) # save caller frame pointer
    add $16, $sp, 8 # set procedure frame pointer

    ## print_drum D1L
    la $a0, D1L
    li $a1, 64
    jal print_drum
    ## print_drum D1R
    la $a0, D1R
    jal print_drum
    ## print_drum D2L
    la $a0, D2L
    jal print_drum
    ## print_drum D2R
    la $a0, D2R
    jal print_drum
    ## print_drum D3L
    la $a0, D3L
    jal print_drum
    ## print_drum D3L
    la $a0, D3R
    jal print_drum
    ## print_drum DR
    la $a0, DR
    li $a1, 32
    jal print_drum

    lw $ra, ($16) # restore return address
    move $t0, $16 # save frame pointer temporarily
    lw $16, -4($16) # restore frame pointer
    move $sp, $8
    jr $ra


  rotate_drums:
    sub $sp, $sp, 8 # allocate space on stack
    sw $ra, 8($sp)  # save return address
    sw $16, 4($sp)  # save caller frame pointer
    add $16, $sp, 8 # set procedure frame pointer

    ## shl(D1L); shl(D1R);
    la $a0, D1L
    jal shl
    la $a0, D1R
    jal shl

    ## d1++
    lb $t0, d1
    add $t0, $t0, 1
    sb $t0, d1

    blt $t0, 64, rotate_drums_exit ## d1 < 64

    ## d1 = 0
    li $t0, 0
    sb $t0, d1

    ## shl(D2L); shl(D2R);
    la $a0, D2L
    jal shl
    la $a0, D2R
    jal shl

    ## d2++
    lb $t0, d2
    add $t0, $t0, 1
    sb $t0, d2

    blt $t0, 64, rotate_drums_exit ## d2 < 64

    ## d2 = 0
    li $t0, 0
    sb $t0, d2

    ## shl(D3L); shl(D3R);
    la $a0, D3L
    jal shl
    la $a0, D3R
    jal shl

    ## d3++
    lb $t0, d3
    add $t0, $t0, 1
    sb $t0, d3

    blt $t0, 64, rotate_drums_exit ## d3 < 64

    ## d3 = 0
    li $t0, 0
    sb $t0, d3



    rotate_drums_exit:
      lw $ra, ($16)   # restore return address
      move $t0, $16   # save frame pointer temporarily
      lw $16, -4($16) # restore frame pointer
      move $sp, $8
      jr $ra



  # $a0 - drum address
  shl:
    sub $sp, $sp, 8 # allocate space on stack
    sw $ra, 8($sp)  # save return address
    sw $16, 4($sp)  # save caller frame pointer
    add $16, $sp, 8 # set procedure frame pointer

    lb $t0, ($a0) # t0 = drum[0]

    li $t1, 0
    ## 64 times
    shl_loop:
      lb $t2, 1($a0)
      sb $t2, ($a0)  # drum[i] = drum[i+1]
      add $a0, $a0, 1  

      add $t1, $t1, 1
      bne $t1, 63, shl_loop

    sb $t0, ($a0)

    lw $ra, ($16)   # restore return address
    move $t0, $16   # save frame pointer temporarily
    lw $16, -4($16) # restore frame pointer
    move $sp, $8
    jr $ra

  # $a0 - drum address
  # $a1 - drum length
  print_drum:
    sub $sp, $sp, 8
    sw $ra, 8($sp) # save return address
    sw $16, 4($sp) # save caller frame pointer
    add $16, $sp, 8 # set procedure frame pointer

    move $t0, $a0
    move $t1, $a1
    li $t2, 0           # counter

    print_drum_loop:
      ## print t2
      li $v0, 1
      move $a0, $t2
      syscall

      ## print '-'
      li $v0, 11
      li $a0, 45
      syscall
      
      ## print drum[t2++]
      li $v0, 1         # print_int
      lb $a0, ($t0)     # 
      syscall

      ## print newline
      li $v0, 11
      li $a0, 10
      syscall

      add $t0, $t0, 1
      add $t2, $t2, 1
      sub $t1, $t1, 1   #
      bnez $t1, print_drum_loop

    lw $ra, ($16) # restore return address
    move $t0, $16 # save frame pointer temporarily
    lw $16, -4($16) # restore frame pointer
    move $sp, $8 # restore stack pointer
    jr $ra

  # assign values to arrays
  init_drums:
    ## prologue
    sub $sp, $sp, 8 # allocate space on stack
    sw $ra, 8($sp)  # save return address
    sw $16, 4($sp)  # save caller frame pointer
    add $16, $sp, 8 # set procedure frame pointer

    li $t0, 0
    sb $t0, d1
    sb $t0, d2
    sb $t0, d3

    li $v0, 13 # open
    la $a0, ROTOR_FILE # file name
    li $a1, 0  # O_RDONLY
    li $a2, 0  # mode is ignored 
    syscall # returns fs in $v0
    
    move $t0, $v0 # save file descriptor to 

    li $v0, 14 # read from file
    move $a0, $t0 # fd 
    la $a1, buffer # buffer where to write
    li $a2, 4096 # number of chars to be read
    syscall 

    move $t1, $v0 # number of chars read

    # print number of chars read
    # li $v0, 1
    # move $a0, $t1
    # syscall

    li $v0, 16 # close file
    syscall 

    la $t2, buffer
    la $t7, all # $t7 is index of 'all' array

    ## inserts all read numbers into 'all' array
    loop: 
      # first digit
      lb $t3, ($t2)
      sub $t3, 48
      add $t2, 1

      # second digit
      lb $t4, ($t2)
      sub $t4, 48
      add $t2, 2

      # first number in $t5
      li $t5, 10
      mul $t5, $t3, $t5
      add $t5, $t5, $t4

      # third digit
      lb $t3, ($t2)
      sub $t3, 48
      add $t2, 1

      # fourth digit
      lb $t4, ($t2)
      sub $t4, 48
      add $t2, 2

      # second number in $t6
      li $t6, 10
      mul $t6, $t3, $t6
      add $t6, $t6, $t4

      sb $t5, ($t7)
      add $t7, 1
      sb $t6, ($t7)
      add $t7, 1

      sub $t1, 6
      bgt $t1, 0, loop

    assign:
      # assign D1L & D1R
      li $t0, 0 # all index
      li $t1, 0 # loop index
      loop2:
        # load first byte to D1L
        lb $t2, all($t0) # t2 = all[t0]
        add $t0, 1 
        lb $t3, all($t0) # t3 = all[t0+1]
        add $t0, 1 

        sb $t3, D1L($t2) # D1L[t2] = t3
        sb $t2, D1R($t3) # D1R[t3] = t2

        add $t1, $t1, 1
        bne $t1, 64, loop2

      # assign D2L & D2R
      li $t1, 0 # loop index
      loop3:
        # load first byte to D1L
        lb $t2, all($t0) # t2 = all[t0]
        add $t0, 1 
        lb $t3, all($t0) # t3 = all[t0+1]
        add $t0, 1 

        sb $t3, D2L($t2) # D1L[t2] = t3
        sb $t2, D2R($t3) # D1R[t3] = t2

        add $t1, $t1, 1
        bne $t1, 64, loop3

      # assign D3L & D3R
      li $t1, 0 # loop index
      loop4:
        # load first byte to D1L
        lb $t2, all($t0) # t2 = all[t0]
        add $t0, 1 
        lb $t3, all($t0) # t3 = all[t0+1]
        add $t0, 1 

        sb $t3, D3L($t2) # D1L[t2] = t3
        sb $t2, D3R($t3) # D1R[t3] = t2

        add $t1, $t1, 1
        bne $t1, 64, loop4

      # assign DR
      li $t1, 0 # loop index
      loop5:
        # load first byte to D1L
        lb $t2, all($t0) # t2 = all[t0]
        add $t0, 1 
        lb $t3, all($t0) # t3 = all[t0+1]
        add $t0, 1 

        sb $t3, DR($t2) # DR[t2] = t3
        sb $t2, DR($t3) # DR[t3] = t2

        add $t1, $t1, 1
        bne $t1, 32, loop5

    ## epilogue
    lw $ra, ($16)   # restore return address
    move $t0, $16   # save frame pointer temporarily
    lw $16, -4($16) # restore frame pointer
    move $sp, $8
    jr $ra
		
