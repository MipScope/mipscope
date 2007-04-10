/* ---------------------------------------------- *\
   file: Utilities.H
   auth: Travis Fischer, Tim O'Donnell
   acct: tfischer, tim
   date: 4/8/2007
\* ---------------------------------------------- */
#ifndef __UTILITIES_H__
#define __UTILITIES_H__

#include <iostream>
using namespace std;


#define PROJECT_NAME       "MipScope"


#define safeDelete(p)         {if ((p)) { delete (p); (p) = NULL; }}
#define safeDeleteArray(p)    {if ((p)) { delete[] (p); (p) = NULL; }}

#define safeFree(p)           {if ((p)) { free((p)); (p) = NULL; }}

#define println(__s_) \
   {std::cerr << __FILE__ << " (" << __LINE__ << "): " << (__s_) << '\n';} \

#endif // __UTILITIES_H__

