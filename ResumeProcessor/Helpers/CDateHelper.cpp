//
// File CDateHelper.cpp
//
//
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file Boost/LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <boost/regex.hpp>
#include <boost/xpressive/regex_compiler.hpp>
#include "CDateHelper.h"
#include "../../Common/CDebugUtils.h"

using namespace Summary_Common;

const string Summary_ResumeProcessor_Helpers::CDateHelper::ShortMonth = "Jan|Feb|Mar|Apr|May|Jun|Jul|Aug|Sep|Oct|Nov|Dec";
const string Summary_ResumeProcessor_Helpers::CDateHelper::FullMonth = "January|February|March|April|May|June|July|August|September|October|November|December";
const boost::xpressive::sregex Summary_ResumeProcessor_Helpers::CDateHelper::StartAndEndDateRegex = boost::xpressive::sregex::compile ("((" + ShortMonth + "|" + FullMonth + ")(|\\s|\\s{2})(\\d{2,4})|(\\d{1,2})(|\\/)(\\d{1,2})\\/(\\d{2,4})|(\\d{1,2})(|\\.)(\\d{1,2})\\.(\\d{2,4})|Current|Now|Present|current|now|present)|\\d{4}", boost::xpressive::regex_constants::optimize);

/*
 * Summary_ResumeProcessor_Helpers::CDateHelper::ParseStartAndEndDate
 *
 * Description
 * Gets start and end date from input string
 *
 * Inputs
 * string input - string line
 *
 * Returns
 * CPeriod object with dates or with empty strings as dates
 *
 */
CPeriod Summary_ResumeProcessor_Helpers::CDateHelper::ParseStartAndEndDate(string input)
{
    CDebugUtils::DebugCout("CDateHelper", "ParseStartAndEndDate", "");
    bool first = true;
    string periodStart = "";
    string periodEnd = "";

    boost::xpressive::sregex_iterator it(input.begin(), input.end(), StartAndEndDateRegex);
    boost::xpressive::sregex_iterator end;
    for (; it != end; ++it)
    {
        //std::cout << it->str() << "\n";
        if (first)
        {
            periodStart = it->str();
        }
        else
        {
            periodEnd = it->str();
        }

        first = false;
    }

    return CPeriod(periodStart, periodEnd);
}
