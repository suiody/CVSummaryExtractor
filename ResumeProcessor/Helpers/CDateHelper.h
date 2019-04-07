//
// File CDateHelper.h
//
//
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file Boost/LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef CVSUMMARYEXTRACTOR_CDATEHELPER_H
#define CVSUMMARYEXTRACTOR_CDATEHELPER_H

#include <string>
#include <iostream>

#include <boost/xpressive/xpressive_static.hpp>
#include "../../Model/CPeriod.h"

using namespace std;
using namespace Summary_Model;

namespace Summary_ResumeProcessor_Helpers
{
    //
    // CLASS : CDateHelper
    //
    // DESCRIPTION : This class is used for parsing start and end date from input string
    //
    //
    class CDateHelper
    {
    public:
        static CPeriod ParseStartAndEndDate(string input);
    private:
        const static string ShortMonth;
        const static string FullMonth;
        const static boost::xpressive::sregex StartAndEndDateRegex;
    };
}

#endif //CVSUMMARYEXTRACTOR_CDATEHELPER_H
