/* ---------------------------------------------- *\
   file: Syscalls.H
   auth: Travis Fischer, Tim O'Donnell
   acct: tfischer, tim
   date: 4/27/2007
\* ---------------------------------------------- */
#ifndef __SYSCALLS_H__
#define __SYSCALLS_H__

// Note, probably half of these will be implemented
enum SYSCALL {
   S_PRINT_INT = 1,       // $a0 = integer
   S_PRINT_FLOAT = 2, 
   S_PRINT_DOUBLE = 3, 
   S_PRINT_STRING = 4,    // $a0 = string
   
   S_READ_INT = 5,                          // integer -> $v0
   S_READ_FLOAT = 6, 
   S_READ_DOUBLE = 7, 
   S_READ_STRING = 8,     // $a0 = buffer, $a1 = length
   
   S_SBRK = 9,            // $a0 = amount
   S_EXIT = 10, 
   
   S_PRINT_CHAR = 11,     // $a0 = char
   S_READ_CHAR = 12,                        // char -> $a0
   
   S_OPEN = 13,
   S_READ = 14, 
   S_CLOSE = 15, 
   S_EXIT2 = 16, 

   S_PRINT = 17,           // combination of all prints
};

#endif // __SYSCALLS_H__

