//
// File CSectionExtractor.cpp
//
//

#include <iostream>
#include <fstream>
#include <stdlib.h>

#include "CSectionExtractor.h"
#include "../Common/CTextUtils.h"
#include "../Common/CDebugUtils.h"

using namespace Summary_Common;

/*
 * Summary_ResumeProcessor::CSectionExtractor::ExtractFrom
 *
 * Description
 * Cuts input string vector into sections with just raw data
 *
 * Inputs
 * vector<string> rawInput - input strings vector
 *
 * Returns
 * Vector of sections
 *
 */
vector<CSection> Summary_ResumeProcessor::CSectionExtractor::ExtractFrom(vector<string> rawInput)
{
    CDebugUtils::DebugCout("CSectionExtractor", "ExtractFrom", "");
    vector<CSection> sections;
    int i = 0;
    bool haveSkillsSection = false;
    bool haveEducationSection = false;

    if (rawInput.size() < 1)
    {
        return sections;
    }

    CSection personalSection;
    personalSection.SetType(ESectionType::Personal);

    while (i < rawInput.size() - 1 &&
            (FindSectionType(CTextUtils::ToLower(CTextUtils::RemoveUnknownCharacters(rawInput[i]))) == ESectionType::Unknown))
    {
        string line = CTextUtils::RemoveUnknownCharacters(rawInput[i]);
        if (!CTextUtils::IsEmptyOrWhiteSpace(line))
        {
            personalSection.AddContent(CTextUtils::Trim(rawInput[i]));
        }

        i++;
    }

    for (string element: rawInput)
    {
        personalSection.AddRawContent(CTextUtils::Trim(element));
    }

    sections.push_back(personalSection);

    while (i < rawInput.size())
    {
        string input = CTextUtils::ToLower(CTextUtils::RemoveUnknownCharacters(rawInput[i]));
        ESectionType sectionType = FindSectionType(input);

        if (sectionType != ESectionType::Unknown)
        {
            CSection section;
            section.SetType(sectionType);

            while (i < rawInput.size() - 1 &&
                    (FindSectionType(CTextUtils::ToLower(CTextUtils::RemoveUnknownCharacters(rawInput[i + 1]))) == ESectionType::Unknown))
            {
                i++;
                string line = CTextUtils::RemoveUnknownCharacters(rawInput[i]);

                if (!CTextUtils::IsEmptyOrWhiteSpace(line))
                {
                    section.AddContent(CTextUtils::Trim(line));
                }
            }

            if (sectionType == ESectionType::Skills)
            {
                haveSkillsSection = true;
                for (string element: rawInput)
                {
                    section.AddRawContent(CTextUtils::Trim(CTextUtils::RemoveUnknownCharacters(element)));
                }
            }
            else if (sectionType == ESectionType::Education)
            {
                haveEducationSection = true;
                for (string element: rawInput)
                {
                    section.AddRawContent(CTextUtils::Trim(element));
                }
            }

            sections.push_back(section);
        }

        i++;
    }

    CSection section;
    section.SetType(ESectionType::RawInput);
    CSection sectionWords;
    sectionWords.SetType(ESectionType::PopularWords);
    CSection sectionLanguage;
    sectionLanguage.SetType(ESectionType::TextLanguage);
    CSection sectionEducationData;
    sectionEducationData.SetType(ESectionType::EducationData);

    //string summary;

    for (string element: rawInput)
    {
        section.AddContent(CTextUtils::Trim(element));
        sectionWords.AddContent(CTextUtils::Trim(CTextUtils::RemoveUnknownCharacters(element)));
        sectionLanguage.AddContent(CTextUtils::Trim(element));
        sectionEducationData.AddRawContent(CTextUtils::Trim(element));
    }

    sections.push_back(section);
    sections.push_back(sectionWords);
    sections.push_back(sectionLanguage);
    sections.push_back(sectionEducationData);

    if (!haveSkillsSection)
    {
        CSection section;
        section.SetType(ESectionType::Skills);

        for (string element: rawInput)
        {
            section.AddRawContent(CTextUtils::Trim(CTextUtils::RemoveUnknownCharacters(element)));
        }

        sections.push_back(section);
    }

    if (!haveEducationSection)
    {
        CSection section;
        section.SetType(ESectionType::Education);

        for (string element: rawInput)
        {
            section.AddRawContent(CTextUtils::Trim(element));
        }

        sections.push_back(section);
    }

    rawInput.clear();

    return sections;
}

/*
 * Summary_ResumeProcessor::CSectionExtractor::FindSectionType
 *
 * Description
 * Finds possible section type where input string belongs to
 *
 * Inputs
 * string input - string line
 *
 * Returns
 * ESectionType (Education, Courses, Summary, WorkExperience, Projects, Skills, Awards or Unknown)
 *
 */
ESectionType Summary_ResumeProcessor::CSectionExtractor::FindSectionType(string input)
{
    vector<string> elements = CTextUtils::Split(input, ' ');
    return elements.size() < SectionTitleNumberOfWordsLimit ? sectionMatchingService->FindSectionTypeMatching(input) : ESectionType::Unknown ;
}
