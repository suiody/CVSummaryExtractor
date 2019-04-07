//
// File CWorkExperienceParser.h
//
//
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file Boost/LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef CVSUMMARYEXTRACTOR_CWORKEXPERIENCEPARSER_H
#define CVSUMMARYEXTRACTOR_CWORKEXPERIENCEPARSER_H

#include "IParser.h"
#include <vector>
#include "../Helpers/CResourceLoader.h"
#include <boost/regex.hpp>
#include <boost/xpressive/xpressive_static.hpp>

using namespace std;
using namespace Summary_ResumeProcessor_Helpers;

namespace Summary_ResumeProcessor_Parsers
{
    //
    // CLASS : CWorkExperienceParser
    //
    // DESCRIPTION : This class is used for parsing work experience (projects) information from input string
    //
    //
    class CWorkExperienceParser: public IParser
    {
    public:
        CWorkExperienceParser(CResourceLoader resourceLoader);

        virtual void Parse(CSection section, CResume &resume);

    private:
        string FindJobTitle(string line);

        const static boost::xpressive::sregex SplitByWhiteSpaceRegex;
        vector<string> jobLookUp;
        vector<string> countryLookUp;
        vector<string> jobLookUpLower;
        vector<string> countryLookUpLower;
    };
}

#endif //CVSUMMARYEXTRACTOR_CWORKEXPERIENCEPARSER_H
