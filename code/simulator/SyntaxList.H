/* ---------------------------------------------- *\
   file: SyntaxList.H
   auth: Travis Fischer, Tim O'Donnell
   acct: tfischer, tim
   date: 4/8/2007
\* ---------------------------------------------- */
#ifndef __SYNTAXLIST_H__
#define __SYNTAXLIST_H__

// Not counting anything w/ coprocessors
const char *const Instructions[] = {
   // ALU Operations
   "abs", 
   "add", "addu", "addi", "addiu", 
   "and", "andi", 
   "clo", "clz", 
   "div", "divu", 
   "mult", "multu", 
   "mul", "mulo", "mulou", "madd", "maddu", "msub", 
   "neg", "negu", 
   "nor", "not", 
   "or", "ori", 
   "rem", "remu", 
   "sll", "sllv", 
   "sra", "srav", "srl", "srlv", 
   "rol", "ror", 
   "sub", "subu", 
   "xor", "xori", 
   
   // Constant-Manipulating Instructions
   "lui", "li", 
   "slt", "sltu", "slti", "sltiu", 
   "seq", "sge", "sgeu", "sgt", "sgtu", 
   "sle", "sleu", "sne", 

   // Branch Instructions
   "b", 
   //... coprocessor stuff
   "beq", 
   "bgez", "bgezal", "bgtz", 
   "blez", 
   "bltzal", "bltz", 
   "bne", 
   "beqz", "bge", "bgeu", "bgt", "bgtu", 
   "ble", "bleu", "blt", "bltu", 
   "bnez", 

   // Jump Instructions
   "j", 
   "jal", 
   "jalr", 
   "jr", 

   // Trap Instructions
   "teq", "teqi", "tge", "tgeu", "tgei", "tgeiu", 
   "tlt", "tltu", "tlti", "tltiu", 

   // Load Instructions
   "la", 
   "lb", "lbu", "lh", "lhu", 
   "lw", "lwl", "lwr", 
   "ld", "ulh", "ulhu", "ulw", 
   
   // Store Instructions
   "sb", "sh", "sw", "swl", "swr", 
   "sd", "ush", "usw", 
   
   // Data Movement Instructions
   "move", "mfhi", "mflo", "mthi", "mtlo", 
   "movn", "movz", 
   "movf", "movt", // needed?

   // Floating Point Instructions
   // add later, time-permitting

   // Exceptions and Interrupt Handlers
   "eret", 
   "syscall", 
   "break", 
   "nop", 
   
   NULL
};

const char *const Directives[] = {
   "align", "ascii", "asciiz", "byte", 
   "data", // "rdata", "sdata", 
//   "float", "double", 
   "extern", 
   "globl", "half", 
   "ktext", //set", 
   "space", "text", "word", 
   
   NULL
};

const char *const Registers[] = {
   "\\$r?[0-2]?\\d",  // $r10
   "\\$r?3[0-1]",     // $31

   "\\$a[0-3t]",       // $a0-3
   "\\$v[0-1]",        // $v0-1
   "\\$t[0-7]",        // $t0-7
   "\\$s[0-8]",        // $s0-8
   "\\$k[0-1]",        // $k0-1
   
   "\\$gp",            // $gp
   "\\$ra",            // $ra
   
   "\\$sp",            // $sp

   NULL
};

const char *LabelSyntax   = "[a-zA-Z_][a-zA-Z_0-9]*\\s?:";
const char *CommentSyntax = "#[^\n]*";

const char *const SpecialSyntax[] = {
   "\\bdone\\b", 
   "\\b__start\\s?:", 
   "\\bmain\\s?:", 

   NULL
};

#endif // __SYNTAXLIST_H__

