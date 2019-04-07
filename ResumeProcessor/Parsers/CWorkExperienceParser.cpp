//
// File CWorkExperienceParser.cpp
//
//
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file Boost/LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <boost/xpressive/regex_compiler.hpp>
#include "CWorkExperienceParser.h"
#include "../../Common/CTextUtils.h"
#include "../../Common/CDebugUtils.h"
#include "../../Common/CFileUtils.h"
#include "../Helpers/CDateHelper.h"
#include "DictionaryLoaderSingleton.h"


#include <iostream>
#include <fstream>
#include <sstream>

using namespace Summary_Common;
using namespace Summary_ResumeProcessor_Helpers;

const boost::xpressive::sregex Summary_ResumeProcessor_Parsers::CWorkExperienceParser::SplitByWhiteSpaceRegex = boost::xpressive::sregex::compile("\\s+", boost::xpressive::regex_constants::optimize);

/*
 * Summary_ResumeProcessor_Parsers::CWorkExperienceParser::CWorkExperienceParser
 *
 * Description
 * Constructor
 *
 * Inputs
 * CResourceLoader resourceLoader - input resource loader object
 *
 * Returns
 *
 */
Summary_ResumeProcessor_Parsers::CWorkExperienceParser::CWorkExperienceParser(CResourceLoader resourceLoader)
{
    jobLookUp = DictionaryLoaderSingleton::GetInstance().GetJobs();

    countryLookUp = DictionaryLoaderSingleton::GetInstance().GetCountries();

    jobLookUpLower = DictionaryLoaderSingleton::GetInstance().GetJobsLower();

    countryLookUpLower = DictionaryLoaderSingleton::GetInstance().GetCountriesLower();
}

/*
 * Summary_ResumeProcessor_Parsers::CWorkExperienceParser::Parse
 *
 * Description
 * Method for parsing work experince section and saving results (start/end date, company) into resume object
 *
 * Inputs
 * CSection section - section data
 * CResume &resume - resume object to be filled during parsing process
 *
 * Returns
 * changed resume object through input parameter resume
 */
void Summary_ResumeProcessor_Parsers::CWorkExperienceParser::Parse(CSection section, CResume &resume)
{
    CDebugUtils::DebugCout("CWorkExperienceParser", "Parse", "");
    int i = 0;

    bool currentPositionNull = true;

    while (i < section.GetContent().size())
    {
        string line = section.GetContent()[i];
        string title = FindJobTitle(line);

        if (CTextUtils::IsEmptyOrWhiteSpace(title))
        {
            string lowerLine = CTextUtils::ToLower(line);

            if (!currentPositionNull)
            {
                CPeriod startAndEndDate = CDateHelper::ParseStartAndEndDate(line);

                if (startAndEndDate.GetStart().compare("") != 0)
                {
                    resume.GetCurrentPosition().SetStartDate(startAndEndDate.GetStart());
                    resume.GetCurrentPosition().SetEndDate(startAndEndDate.GetEnd());
                }
                else
                {
                    string country = "";

                    for (int j = 0; j < countryLookUpLower.size(); j++)
                    {
                        if (CTextUtils::StringContains(lowerLine, countryLookUpLower.at(j)))
                        {
                            //cout << "Work experience country:" << s << "\n";
                            country = countryLookUp.at(j);
                            break;
                        }
                    }

                    if (CTextUtils::IsEmptyOrWhiteSpace(country))
                    {
                        resume.GetCurrentPosition().AddSummary(line);
                    }
                    else
                    {
                        resume.GetCurrentPosition().SetCompany(
                                CTextUtils::Trim
                                        (line.substr
                                                (0, CTextUtils::FindString(lowerLine, CTextUtils::ToLower(country)) +
                                                                              country.length())
                                        )
                                                              );
                    }
                }
            }
        }
        else
        {
            currentPositionNull = false;
            CPosition currentPosition;

            currentPosition.SetTitle(title);
            resume.AddPosition(currentPosition);
        }

        i++;
    }
}

/*
 * Summary_ResumeProcessor_Parsers::CWorkExperienceParser::FindJobTitle
 *
 * Description
 * Finds job title from input string
 *
 * Inputs
 * string line - input string line
 *
 * Returns
 * Job title or empty string
 *
 */
string Summary_ResumeProcessor_Parsers::CWorkExperienceParser::FindJobTitle(string line)
{
   int count = 0;/* = CTextUtils::Split(line, ' ');

    if (elements.size() > 4)
    {
        return "";
    }*/

    boost::xpressive::sregex_token_iterator i(line.begin(), line.end(), SplitByWhiteSpaceRegex, -1);
    boost::xpressive::sregex_token_iterator j;

    while(i != j)
    {
        *i++;
        count++;

        if (count > 4)
        {
            return "";
        }
    }

    string lowerLine = CTextUtils::ToLower(line);

    for (int j = 0; j < jobLookUpLower.size(); j++)
    {
        if (CTextUtils::StringContains(lowerLine, jobLookUpLower.at(j)))
        {
            //cout << "Work experience country:" << s << "\n";
            return CTextUtils::Trim(jobLookUp.at(j));
        }
    }

    return "";
}

