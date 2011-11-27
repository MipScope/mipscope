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
   file: Utilities.H
   auth: Travis Fischer, Tim O'Donnell
   acct: tfischer, tim
   date: 4/8/2007
\* ---------------------------------------------- */
#ifndef __UTILITIES_H__
#define __UTILITIES_H__

#include <iostream>
#include <QStatusBar>
using namespace std;
#include "Options.H"

#define PROJECT_NAME       "MipScope"
#define PROJECT_VERSION    "0.3.3"

#define FILE_FILTER        tr("MIPS Files (*.s *.a *.mal);;All Files (*)")
#define STATUS_DELAY       (12000)
#define IMAGES             ":/"//"gui/images"
//#define LARGE_ICONS
#define SMALL_ICONS

#ifdef LARGE_ICONS
   #define ICON_SIZE          (32)
   #define ICON_DIR           "32x32"
#else
   #define ICON_SIZE          (22)
   #define ICON_DIR           "22x22"
#endif

//#define ICONS              IMAGES "/" ICON_DIR
#define ICONS                IMAGES

#define EXTENDED_FADE_INTERVAL   (33)
#define EXTENDED_FADE_DURATION   (EXTENDED_FADE_INTERVAL * 10)

extern QStatusBar *STATUS_BAR;

/*enum DEBUG_MODE {
   M_NORMAL = 0, M_DEBUGGING = 1, M_RUNNING = (M_DEBUGGING | 2), M_PAUSED = (M_DEBUGGING | 4), 
};*/

// userState() in QTextBlock
enum BlockState { B_DEFAULT = -1, B_NORMAL = 0, B_BREAKPOINT = 1, 
   B_CURRENT_PC = 2, B_BREAKPOINT_CURRENT_PC = (B_BREAKPOINT | B_CURRENT_PC)
};

#define safeDelete(p)         {if ((p)) { delete (p); (p) = NULL; }}
#define safeDeleteArray(p)    {if ((p)) { delete[] (p); (p) = NULL; }}

#define safeFree(p)           {if ((p)) { free((p)); (p) = NULL; }}

#define println(__s_) \
   {std::cerr << __FILE__ << " (" << __LINE__ << "): " << (__s_) << '\n';} \

#include "../simulator/typedefs.H"
#if 0
#endif // guard to ensure errors in utilities.H don't show up in weird places
extern bool VERBOSE;

#endif // __UTILITIES_H__

