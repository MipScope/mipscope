# *********************************************************************
# PROG: Life
# AUTH: Travis Fischer
# ACCT: tfischer
# DATE: 11/4/2006
# 
# Outputs a textual representation of Conway's game, Life, a simulation 
# of a population of lifeforms over a sequence of generations. Both the
# size of the population and the number of generations are customizable 
# via constants.
# *********************************************************************


.data

# ---------
# Constants
# ---------
PRINT_STRING  = 4
PRINT_INTEGER = 1
NULL          = 0

DEAD      = 0
JUST_DIED = 1
ALIVE     = 2
JUST_BORN = 3

# --------------------------------
# User-Defined Life Grid Constants
# --------------------------------
ROWS        = 8      # the number of Rows in the Grid (8 default)
COLUMNS     = 10     # the number of Columns in the Grid (10 default)
GRID_SIZE   = 80     # ROWS * COLUMNS (80 default)

GENERATIONS = 5      # Number of Generations to simulate

# ----------------------------
# Global Variable Declarations
# ----------------------------
curGenString:        .asciiz           "Generation "
curGenString2:       .asciiz           ": \n"

# NOTE: I use strings to represent the states because it's much easier to 
# change how organisms look.  Instead of using numbers (which the handout 
# specifies that we're supposed to), it looks cooler to use other, obscure 
# ascii characters to represent the different states.
deadString:          .asciiz           "0"
justDiedString:      .asciiz           "3" #
aliveString:         .asciiz           "1" # 
justBornString:      .asciiz           "2" #| 

newLine:             .asciiz           "\n"
grid1:               .byte             0:GRID_SIZE
endSimulationString: .asciiz           "\nEnd of Life  :("
grid2:               .byte             0:GRID_SIZE

curGrid:             .word             0 # pointer to currently active grid
oppGrid:             .word             0 # pointer to currently inactive grid

.text

__start:    

# Initialize global curGrid and oppGrid, used extensively throughout the program
# ------------------------------------------------------------------------------
            la $t0, grid1
            la $t1, grid2
            sw $t0, curGrid
            sw $t1, oppGrid

# Add Default ALIVE cells to the curGrid  (establishing original state of population)
# -----------------------------------------------------------------------------------
            #jal add_cells_for_testing
            
            
            li $a0, 1
            li $a1, 0
            jal addLiveCell
             
            li $a0, 0
            li $a1, 2
            jal addLiveCell
           
            li $a0, 1
            li $a1, 2
            jal addLiveCell

            li $a0, 2
            li $a1, 2
            jal addLiveCell

            li $a0, 2
            li $a1, 1
            jal addLiveCell
            

# Register Usage
# --------------
# $s0 = current row
# $s1 = current column
# $s2 = current generation
# $s3 = next state for current cell
# $s4 = whether or not to change state (don't change state for beginning generation; instead, 
#       just print out initial cells
# $v0 = whether or not curCell is alive
# $v1 = neighbor count for curCell
            
            li $s4, 0
            li $s2, GENERATIONS
            add $s2, $s2, 1
            
genLoop:       blez $s2, endSimulation       # while(curGen--) {
               sub $s2, $s2, 1
               
               la $a0, curGenString
               li $v0, PRINT_STRING
               syscall
               
               li $t0, GENERATIONS
               sub $a0, $t0, $s2
               li $v0, PRINT_INTEGER
               syscall
               
               la $a0, curGenString2
               li $v0, PRINT_STRING          # printf("Generation %d:\n", curGen);
               syscall

               
               li $s0, 0
   
   rowLoop:       li $t0, ROWS
                  bge $s0, $t0, endRow
                  #blez $s0, endRow           # while(curRow--) {
                  #sub $s0, $s0, 1

                  li $s1, 0

      colLoop:       li $t0, COLUMNS
                     bge $s1, $t0, endCol
                     #blez $s1, endCol        # while(curCol--) {
                     #sub $s1, $s1, 1
                     
                     move $a0, $s0
                     move $a1, $s1
                     
                     bnez $s4, getNextState
                     
                     # just printing out state (not changing state)
                     mul $t6, $a0, COLUMNS
                     add $t6, $t6, $a1          # $t6 = row * COLUMNS + col (offset into curGrid)

                     lw $t7, curGrid
                     add $t6, $t6, $t7
                     lb $a2, ($t6)              # Retrieve the state of the cell at the given offset
                     j stateChangeOff
                    
      
      getNextState:
                     jal countNeighbors      # $v1 = number of neighbors
                     
                     move $a0, $s0
                     move $a1, $s1
                     jal isCellAlive         # $v0 = (curCell->isAlive);
                     
                     beqz $v0, curCellIsDead # Handle dead vs alive cells differently
                     
                     # curCell is ALIVE or JUST_BORN
                     # -----------------------------
                     li $s3, ALIVE           # nextState = ALIVE; // by default
                     
                     seq $t0, $v1, 2
                     seq $t1, $v1, 3
                     or  $t0, $t0, $t1       # $t0 = (neighborCount == 2 || neighborCount == 3)

                     bnez $t0, stateSet      # no ALIVE/DEAD state change (possibly JUST_BORN->ALIVE change)

                     li $s3, JUST_DIED       # curCell was ALIVE & has now JUST_DIED
                     j stateSet
                     
      curCellIsDead: # curCell is DEAD or JUST_DIED
                     # ----------------------------
                     li $s3, DEAD            # nextState = DEAD; // by default
                     
                     li $t0, 3               # curCell is not alive, so if (neighborCount != 3) 
                     bne $v1, $t0, stateSet # then nextState is set (DEAD)
                     
                     li $s3, JUST_BORN       # else nextState = JUST_BORN (since neighborCount == 3)
                     
      stateSet:      move $a0, $s0
                     move $a1, $s1
                     move $a2, $s3
                     
                     jal setOppCell          # oppGrid[$a0][$a1] = nextState ($a2);
      
      stateChangeOff:               
                     jal printState          # Print out a textual representation of the new state
                     
                     add $s1, $s1, 1
                     j colLoop
               
      endCol:     
                  jal printNewLine           # Print a '\n' after each column
                              
                  add $s0, $s0, 1
                  j rowLoop

   endRow:     
               beqz $s4, swapGridsOff
               jal swapGrids                 # Swap curGrid with oppGrid (& vice-versa)
   
   swapGridsOff:
               jal printNewLine
               
               li $s4, 1
               j genLoop                     # Generations loop
            
endSimulation:
            
            la $a0, endSimulationString      # Print out a string notifying user that life has ended
            li $a1, PRINT_STRING
            jal printLn
            
            done



# *********************************************************************
#  // Swaps curGrid with oppGrid (or vice-versa, if you'd prefer)
#  void swapGrids(void) {
#     void *tempCur ($t0) = curGrid;
#     void *tempOpp ($t1) = oppGrid;
#     
#     curGrid = tempOpp;
#     oppGrid = tempCur;
#  }
# 
# clobbers: $t0-$t1
# *********************************************************************
swapGrids:

            sub $sp, $sp, 4            # decrement stack pointer
            sw $ra, 4($sp)             # push ra
            
            lw $t0, curGrid
            lw $t1, oppGrid

            sw $t0, oppGrid
            sw $t1, curGrid
            
            lw $ra, 4($sp)             # pop ra
            add $sp, $sp, 4            # increment stack pointer
            jr $ra                     # return to caller


# *********************************************************************
#  // Returns the number of alive cells neighboring a given cell in the 
#  // curGrid (including wrapping around edges of the grid)
#  int ($v1) countNeighbors(int row ($a0), int col ($a1)) {
#     register const int count ($t2)  = 0;
#     register const int posRow ($t3) = row + ROWS;
#     register const int posCol ($t4) = col + COLUMNS;
#     
#     for(int i ($t0) = -1; i <= 1; i++) {
#        int r ($a0) = (rowPos + i) % ROWS;
#        
#        for(int j ($t1) = -1; j <= 1; j++) {
#           if (i != 0 || j != 0) {
#              int c ($a1) = (colPos + j) % COLUMNS;
#              
#              count += isCellAlive(r, c);
#           }
#        }
#     }
#     
#     return count;
#  }
# 
# clobbers: $t0-$t7, $a0-$a1, $v0-$v1
# *********************************************************************
countNeighbors:

            sub $sp, $sp, 4            # decrement stack pointer
            sw $ra, 4($sp)             # push ra
            
            add $t3, $a0, ROWS
            add $t4, $a1, COLUMNS
            
            li $t2, 0                  # Neighbor count
            li $t5, 1                  # for termination of 'for' loops
            li $t0, -1                 
            
nForI:         bgt $t0, $t5, nEndForI     # for(i = -1; i <= 1; i++) {
               li $t1, -1                 
              
               add $a0, $t3, $t0          # r = row + ROWS + i
               rem $a0, $a0, ROWS         # Wrap neighbor's row using modular arithmetic
              
   nForJ:         bgt $t1, $t5, nEndForJ     # for(j = -1; j <= 1; j++) {
                  
                  seq $t6, $t0, 0
                  seq $t7, $t1, 0
                  and $t6, $t6, $t7
                  bnez $t6, nEndIf           # ensure that (i != 0 || j != 0) since we only want 
                                             # to count neighboring cells
                  add $a1, $t4, $t1          # c = col + COLUMNS + j
                  rem $a1, $a1, COLUMNS      # Wrap neighbor's col using modular arithmetic
                  
                  jal isCellAlive
                  add $t2, $t2, $v0          # count += isCellAlive(r, c);
                  
   nEndIf:        add $t1, $t1, 1
                  j nForJ
   
   nEndForJ:                                 # end of j for loop

               add $t0, $t0, 1
               j nForI
            
nEndForI:                                 # endof i for loop
            
            move $v1, $t2              # return count;
            
            lw $ra, 4($sp)             # pop ra
            add $sp, $sp, 4            # increment stack pointer
            jr $ra                     # return to caller
 

# *********************************************************************
#  // Returns whether or not the given cell is alive in the curGrid
#  bool ($v0) isCellAlive(int row ($a0), int col ($a1)) {
#     register const char state ($t6) = *(curGrid + row * COLUMNS + col);
#     
#     return (state == ALIVE || state == JUST_BORN);
#  }
# 
# clobbers: $t6-$t7, $v0
# *********************************************************************
isCellAlive:

            sub $sp, $sp, 4            # decrement stack pointer
            sw $ra, 4($sp)             # push ra
            
            mul $t6, $a0, COLUMNS
            add $t6, $t6, $a1          # $t6 = row * COLUMNS + col (offset into curGrid)
            
            lw $t7, curGrid
            add $t6, $t6, $t7
            lb $t6, ($t6)              # Retrieve the state of the cell at the given offset
            
            sge $v0, $t6, ALIVE        # return (state >= ALIVE); // ALIVE = 2 and JUST_BORN = 3
            
            lw $ra, 4($sp)             # pop ra
            add $sp, $sp, 4            # increment stack pointer
            jr $ra                     # return to caller
           

# *********************************************************************
#  // Sets the cell at (row, col) to the given state (inactive Grid)
#  void setOppCell(int row ($a0), int col ($a1), int state ($a2)) {
#     *(oppGrid + row * COLUMNS + col) = state;
#  }
# 
# clobbers: $t0-$t1
# *********************************************************************
setOppCell:

            sub $sp, $sp, 4            # decrement stack pointer
            sw $ra, 4($sp)             # push ra
            
            mul $t1, $a0, COLUMNS
            add $t1, $t1, $a1          # $t1 = row * COLUMNS + col (offset into oppGrid)
            
            lw $t0, oppGrid
            add $t0, $t0, $t1
            # input state stored in $a2
            sb $a2, ($t0)              # Set cell at given offset to the given state
            
            lw $ra, 4($sp)             # pop ra
            add $sp, $sp, 4            # increment stack pointer
            jr $ra                     # return to caller


# *********************************************************************
#  // Prints a textual representation of the given state
#  void printState(const char state ($a2)) {
#     cout << stateStrings[state];
#  }
# 
# clobbers: $t0, $a0, $v0
# *********************************************************************
printState:

            sub $sp, $sp, 4            # decrement stack pointer
            sw $ra, 4($sp)             # push ra
            
            #move $a0, $a2
            #li $v0, PRINT_INTEGER
            #syscall
            
            # Determine which of the 4 strings to print
            la $a0, deadString
            beqz $a2, stringKnown
            
            la $a0, justDiedString
            li $t0, JUST_DIED
            beq $a2, $t0, stringKnown

            la $a0, aliveString
            li $t0, ALIVE
            beq $a2, $t0, stringKnown
            
            la $a0, justBornString
            
stringKnown: # $a0 now contains the address of the string to print
            
            li $v0, PRINT_STRING
            syscall
            
            lw $ra, 4($sp)             # pop ra
            add $sp, $sp, 4            # increment stack pointer
            jr $ra                     # return to caller


# *********************************************************************
#  // Adds an ALIVE cell at the given (row, col) to the curGrid
#  void addLiveCell(int row ($a0), int col ($a1)) {
#     *(curGrid + row * COLUMNS + col) = ALIVE;
#  }
# 
# clobbers: $t0-$t1
# *********************************************************************
addLiveCell:

            sub $sp, $sp, 4            # decrement stack pointer
            sw $ra, 4($sp)             # push ra
            
            mul $t1, $a0, COLUMNS
            add $t1, $t1, $a1          # $t1 = row * COLUMNS + col (offset into curGrid)

            lw $t0, curGrid
            add $t0, $t0, $t1
            li $t1, ALIVE
            sb $t1, ($t0)              # Initialize cell to be ALIVE
            
            lw $ra, 4($sp)             # pop ra
            add $sp, $sp, 4            # increment stack pointer
            jr $ra                     # return to caller


# *********************************************************************
#  // Prints a string or an integer followed by a newline to stdout
#  void printLn(void *addr ($a0), int outputType ($a1)) {
#     if (outputType == 0) // integer
#        cout << *addr;
#     else // outputType should = 4 for a string
#        cout << addr;
#     
#     cout << endl;
#  }
# 
# clobbers: $a0, $v0
# *********************************************************************
printLn:

            sub $sp, $sp, 4            # decrement stack pointer
            sw $ra, 4($sp)             # push ra
            
            move $v0, $a1
            syscall                    # Print the original string passed in
            
            jal printNewLine           # Add a newline at the end the string
            
            lw $ra, 4($sp)             # pop ra
            add $sp, $sp, 4            # increment stack pointer
            jr $ra                     # return to caller


# *********************************************************************
#  // Prints a newline string to stdout
#  void printNewLine(void) {
#     cout << endl;
#  }
# 
# clobbers: $a0, $v0
# *********************************************************************
printNewLine:
            
            sub $sp, $sp, 4            # decrement stack pointer
            sw $ra, 4($sp)             # push ra
            
            la $a0, newLine
            li $v0, PRINT_STRING
            syscall
            
            lw $ra, 4($sp)             # pop ra
            add $sp, $sp, 4            # increment stack pointer
            jr $ra                     # return to caller
            
