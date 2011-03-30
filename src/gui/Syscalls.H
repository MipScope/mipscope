/*
 * MipScope <http://mipscope.sourceforge.net>
 * Copyright (C) 2007 Travis Fischer and Tim O'Donnell
 * 
 * Send bugs, features, ideas to
 * Travis Fischer (tfischer@cs.brown.edu) and Tim O'Donnell (tim@cs.brown.edu)
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 *
 *
 */

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

   // Additions / Non-Standard
   // ------------------------
   
   S_CLEAR_OUTPUT = 18
};

#endif // __SYSCALLS_H__

