//
// File CSectionMatchingService.cpp
//
//
#include <iostream>
#include "CSectionMatchingService.h"
#include "../Common/CTextUtils.h"
#include "../Common/CDebugUtils.h"

using namespace Summary_Common;

map<ESectionType, vector<string>> Summary_ResumeProcessor::CSectionMatchingService::keyWordRegistry = {
        { ESectionType::Education, {"education", "study", "school","degree","institution", "academic", "qualification"} },
        { ESectionType::Courses, {"coursework", "course"} },
        { ESectionType::Summary, {"summary","profile"} },
        { ESectionType::WorkExperience, {"experience", "work", "employment"} },
        { ESectionType::Projects, {"project"} },
        { ESectionType::Skills, {"skill"/*, "ability", "tool"*/} },
        { ESectionType::Awards, {"award", "certification", "certificate"} }
};

/*
 * Summary_ResumeProcessor::CSectionMatchingService::FindSectionTypeMatching
 *
 * Description
 * Finds possible section where input string belongs to. Returns Unknown if no section found
 *
 * Inputs
 * string input - string line
 *
 * Returns
 * ESectionType (Education, Courses, Summary, WorkExperience, Projects, Skills, Awards or Unknown)
 *
 */
ESectionType Summary_ResumeProcessor::CSectionMatchingService::FindSectionTypeMatching(string input)
{
    CDebugUtils::DebugCout("CSectionMatchingService", "FindSectionTypeMatching", "");
    map<ESectionType, vector<string>>::iterator keyIter;

    input = CTextUtils::Trim(input);

    for (keyIter = keyWordRegistry.begin(); keyIter != keyWordRegistry.end(); ++keyIter)
    {
        for (string value : keyIter->second)
        {
            string newValue = CTextUtils::Trim(value);

            if (CTextUtils::StringContainsIgnoreCase(input, value))
            {
                return keyIter->first;
            }
        }
    }

    return Unknown;
}
