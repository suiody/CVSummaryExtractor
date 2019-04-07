//
// File Stacktrace.h
//
//

#ifndef CVSUMMARYEXTRACTOR_STACKTRACE_H
#define CVSUMMARYEXTRACTOR_STACKTRACE_H

#include <stdio.h>
#include <stdlib.h>
#include <execinfo.h>
#include <cxxabi.h>
#include "../Common/CFileUtils.h"
#include "../Easylogging/easylogging++.h"

using namespace Summary_Common;

/*
 * PrintStacktrace
 *
 * Description
 * Prints stack trace into error log using max_frames parameters as maximum lines for stack trace
 *
 * Inputs
 * unsigned int max_frames - maximum lines for stack trace, defaults to 64
 *
 * Returns
 *
 */
static inline void PrintStacktrace(unsigned int max_frames = 64)
{
    string sourceFile = CFileUtils::GetSourcePath();

    try
    {
        void *array[max_frames];
        size_t size = backtrace(array, sizeof(array) / sizeof(array[0]));
        char ** traceMessages = backtrace_symbols(array, size);
        for (int i = 0; i < size; i++)
        {
            if (traceMessages[i] != nullptr)
            {
                LOG(ERROR) << sourceFile <<traceMessages[i];
            }
        }
        if (traceMessages != nullptr)
        {
            free(traceMessages);
        }
    }
    catch (std::exception e)
    {

    }
}

#endif //CVSUMMARYEXTRACTOR_STACKTRACE_H
