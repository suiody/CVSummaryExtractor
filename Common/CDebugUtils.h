//
// File CDebugUtils.h
//
//

#ifndef CVSUMMARYEXTRACTOR_CDEBUGUTILS_H
#define CVSUMMARYEXTRACTOR_CDEBUGUTILS_H

#include <string>

using namespace std;


namespace Summary_Common
{
    class CDebugUtils
    {
    public:
        static void DebugCout(string className, string methodName, string error);
        static void Sleep(int milliseconds);
    };
}

#endif //CVSUMMARYEXTRACTOR_CDEBUGUTILS_H
