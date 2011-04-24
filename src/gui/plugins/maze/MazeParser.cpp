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
   file: MazeParser.C
   auth: Travis Fischer, Tim O'Donnell
   acct: tfischer, tim
   date: 8/14/2007
\* ---------------------------------------------- */
#include "MazeParser.H"
#include "../../Utilities.H"
#include "Maze.H"

#define fail(why) {cerr << why << endl; return false;}
#define failLineNum(why,num) {cerr << why << " at line " << num << endl; return false;}
//This isn't safe. I really ought to fix it. It's fine for now though
#define mazeArrayAt(row, col) mazeArray[(row) * fileWidth - \
	((row)==0 ? 0 : forgotTrailingSpace) + (col)]

#include <QFile>
#include <QTextStream>
/*
 * Given a .mze file, produces a maze object with the same information. This
 * involves extracting wall information for each cell in the grid, as well as
 * the source and destination. All Cells begin as empty. Note: Dimensions and
 * indices prefixed with "file" refer to the char array of the file, wheras
 * those starting with "maze" refer to the rows and columns of the eventual
 * Maze object
 */
bool MazeParser::parse(const QString &fileName, Maze *maze) {
   QFile file(fileName);
   if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
      return false;

   QByteArray arr = file.readAll();
   int fileSize = arr.size();
   char *mazeArray = arr.data();
   
/*   int fd;
   if((fd = open(fileName.toAscii().data(),O_RDWR)) == -1) fail("Can't open file.");
   //Get the file length
   int fileSize = lseek(fd,0,SEEK_END);
   char *mazeArray;
   //cerr << "1\n";
   //Map the file to our address space
   if((int) (mazeArray = (char *) mmap(0,fileSize,PROT_READ,MAP_PRIVATE,fd,0)) ==-1) fail("mmap");*/
   int fileWidth = 0;
   //This flags if the file has the final space at the end of the first row or not
   int forgotTrailingSpace=0;
   //cerr << "2\n";
   //Figure out the width of the maze in the file
   do{
      //Also make sure the top row is well formed
      if(fileWidth % 2 == 0){
         if(mazeArray[fileWidth] == '#') fail("Misaligned destination point"); 
         if(mazeArray[fileWidth] != ' ') fail("Remember to include spaces in top row");
      }else{
         if(mazeArray[fileWidth] != '_' &&
			   mazeArray[fileWidth] != '#') fail("Malformed top row");
		}
		fileWidth++;
	}while(mazeArray[fileWidth] != '\n' && fileWidth < fileSize);
	if(fileWidth % 2 == 0){
		//If there's no trailing space, the first row of the file
		//will be shorter than the others. We mark a flag used to
		//adjust indexing later, and adjust the width
		forgotTrailingSpace = 1;
		fileWidth++;
	}
	int mazeWidth = fileWidth / 2;
	int mazeHeight = fileSize / fileWidth -1;
	fileWidth++;//For the newline
   
   //cerr << "parseW: " << mazeWidth << ", " << mazeHeight << endl;

	//Maze *maze = new Maze(mazeWidth,mazeHeight);
   maze->init(mazeWidth, mazeHeight);
	
   //cerr << "3\n";
	//Now we go through and get the info for each cell in the grid
	int mazeRow, mazeCol, fileRow, fileCol;
	for(mazeRow = 0, fileRow = 1; mazeRow < mazeHeight; mazeRow++, fileRow ++){
   //cerr << "mazeRow: " << mazeRow << endl;

		for(mazeCol = 0, fileCol = 1; mazeCol < mazeWidth; mazeCol++, fileCol += 2){
			bool is_goal = false;
         //cerr << "mazeCol: " << mazeCol << endl;
			//cout << ' ' << mazeArrayAt(fileRow-1,fileCol)<< endl <<
				//mazeArrayAt(fileRow,fileCol-1)<< 
			   //mazeArrayAt(fileRow,fileCol)  <<
				//mazeArrayAt(fileRow,fileCol+1)<< endl;

			MazeStatus status = EMPTY;
			//Start with no walls
			WallSet walls = 0;
			//We now check for each wall in turn
			char northWall = mazeArrayAt(fileRow-1,fileCol);
			//cout << "north" << northWall << endl;
			if(northWall == '_'){
				walls |= NORTH;
			}else if(northWall == '#'){
				//Destinations should only be in the top row
				if(mazeRow == 0) is_goal = true;
				else failLineNum("Misplaced Destination",fileRow);
			}else if(northWall != ' '){
				cout << "north" << endl;
				failLineNum("Malformed maze",fileRow);
			}
         //cerr << "north: " << northWall << endl;

			char southWall = mazeArrayAt(fileRow,fileCol);
			if(southWall == '_'){
				walls |= SOUTH;
			}else if(southWall == '*'){
				status = CURRENT;
				//The source should be at the bottom
				if(mazeRow == mazeHeight -1) maze->setSource(mazeCol);
				else failLineNum("Misplaced Source",fileRow);
			}else if(southWall != ' '){
				cout << "south " << southWall << " " << fileCol << forgotTrailingSpace <<endl;
				failLineNum("Malformed maze",fileRow);
			}
			
         //cerr << "south: " << southWall << endl;
			char westWall = mazeArrayAt(fileRow,fileCol-1);
			if(westWall == '|'){
				walls |= WEST;
			}else if(mazeCol == 0){
				failLineNum("Edge of maze not walled in",fileRow);
			}else if(westWall != ' '){
				//we shouldn't have anything but
				//bars and spaces here.
				failLineNum("Malformed maze",fileRow);
			}


         //cerr << "west: " << westWall << endl;
			char eastWall = mazeArrayAt(fileRow,fileCol+1);
			if(eastWall == '|'){
				walls |= EAST;
			}else if(mazeCol == mazeWidth-1){
				failLineNum("Edge of maze not walled in",fileRow);
			}else if(eastWall != ' '){
				//we shouldn't have anything but
				//bars and spaces here.
				failLineNum("Malformed maze",fileRow);
			}

         //cerr << "east: " << eastWall << endl;
			Cell *cell;
			if (is_goal)
				cell = new GoalCell(status,walls);
			else
				cell = new NormalCell(status,walls);
			//cout << walls << endl;
			maze->setCell(mazeRow,mazeCol,cell);
		}
		//cout << "next row" << endl;
	}
	if(maze->getSource() == -1) fail("No source point specified");

   file.close();
	//close(fd);
	//maze->to_s();
	return true;
}

