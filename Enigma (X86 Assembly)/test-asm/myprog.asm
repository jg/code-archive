# Sample MIPS program that writes to a new file.
# by Kenneth Vollmar and Pete Sanderson

.data
fout: .asciiz "testout.txt"
# filename for output
buffer: .asciiz "The quick brown fox jumps over the lazy dog."

.text
  .globl main

main:
  # open file (for writing) a file that does not exist
  li $v0, 13
  # system call for open file into $v0
  la $a0, fout
  # output file name
  li $a1, 1
  # Open for writing (flags are 0: read, 1: write)
  li $a2, 0
  # mode is ignored
  syscall
  # open a file (file descriptor returned in $v0)
  move $s6, $v0
  # save the file descriptor for later use

  # write to file just opened
  li $v0, 15
  # system call for write to file into $v0
  move $a0, $s6
  # file descriptor
  la $a1, buffer
  # address of buffer from which to write
  li $a2, 44
  # hardcoded buffer length
  syscall
  # write to file

  # close file
  li $v0, 16
  move $a0, $s6
  syscall

 
  li $v0, 10
  syscall

# system call for close file into $v0
# put file descriptor back into $a0 to close file
# close file
