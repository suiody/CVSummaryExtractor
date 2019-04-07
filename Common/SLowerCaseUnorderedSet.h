//
// File SLowerCaseUnorderedSet.h
//
//

#ifndef CVSUMMARYEXTRACTOR_LOWERCASEUNORDEREDSET_H
#define CVSUMMARYEXTRACTOR_LOWERCASEUNORDEREDSET_H

#include <unordered_set>
#include <algorithm>
#include <iterator>
#include "CLowerCaseString.h"

namespace Summary_Common
{
    //
    // TYPEDEF : SLowerCaseUnorderedSet
    //
    // DESCRIPTION : This type advances unordered_set<CLowerCaseString>
    // when lower case string operations on set are needed
    //
    typedef unordered_set<CLowerCaseString,
            hash<string>,
            equal_to<string> >  SLowerCaseUnorderedSet;
}


#endif //CVSUMMARYEXTRACTOR_LOWERCASEUNORDEREDSET_H
