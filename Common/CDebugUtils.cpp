//
// File CDebugUtils.cpp
//
//

#include <stdlib.h>
#include <iostream>
#include <unistd.h>
#include "CDebugUtils.h"
#include "CFileUtils.h"

/*
 * Summary_Common::CDebugUtils::DebugCout
 *
 * Description
 * Outputs information to standard output using className, methodname and error text
 *
 * Inputs
 * string className - class name
 * string methodName - method name
 * string error - error text
 *
 * Returns
 *
 */
void Summary_Common::CDebugUtils::DebugCout(string className, string methodName, string error)
{
    string logDirName = "./logs";
    string command = "mkdir " + logDirName;

    /*if (!CFileUtils::PathExists(logDirName))
    {
        system(command.c_str());
    }*/

    //cout << className << ":" << methodName << ":" << error << "\n";
}

/*
 * Summary_Common::CDebugUtils::Sleep
 *
 * Description
 * Sleeps n milliseconds
 *
 * Inputs
 * int milliseconds counter (n)
 *
 * Returns
 *
 */
void Summary_Common::CDebugUtils::Sleep(int milliseconds)
{
    usleep(milliseconds * 1000);
}
