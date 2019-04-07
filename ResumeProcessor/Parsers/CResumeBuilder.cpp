//
// File CResumeBuilder.cpp
//
//

#include "CResumeBuilder.h"
#include "CPersonalParser.h"
#include "CSummaryParser.h"
#include "CEducationParser.h"
#include "CEducationDataParser.h"
#include "CProjectsParser.h"
#include "CWorkExperienceParser.h"
#include "CSkillsParser.h"
#include "CCoursesParser.h"
#include "CAwardsParser.h"
#include "CRawInputParser.h"
#include "CPopularWordsParser.h"
#include "CTextLanguageParser.h"
#include "../CSectionExtractor.h"
#include "../../Common/CDebugUtils.h"

using namespace Summary_Common;
using namespace Summary_ResumeProcessor;

/*
 * Summary_ResumeProcessor_Parsers::CResumeBuilder::CResumeBuilder
 *
 * Description
 * Constructor. Loads section to parser matching object.
 *
 * Inputs
 * CResourceLoader resourceLoader - input resource loader object
 *
 * Returns
 *
 */
Summary_ResumeProcessor_Parsers::CResumeBuilder::CResumeBuilder(CResourceLoader resourceLoader)
{
    parserRegistry = {
            {ESectionType::Personal, new CPersonalParser(resourceLoader) },
            {ESectionType::Summary, new CSummaryParser() },
            {ESectionType::Education, new CEducationParser() },
            {ESectionType::EducationData, new CEducationDataParser() },
            {ESectionType::Projects, new CProjectsParser() },
            {ESectionType::WorkExperience, new CWorkExperienceParser(resourceLoader) },
            {ESectionType::Skills, new CSkillsParser(resourceLoader) },
            {ESectionType::Courses, new CCoursesParser() },
            {ESectionType::Awards, new CAwardsParser() },
            {ESectionType::RawInput, new CRawInputParser()},
            {ESectionType::PopularWords, new CPopularWordsParser(resourceLoader)},
            {ESectionType::TextLanguage, new CTextLanguageParser()}
    };
}

/*
 * Summary_ResumeProcessor_Parsers::CResumeBuilder::CResumeBuilder
 *
 * Description
 * Destructor. Cleans section to parser matching object.
 *
 * Inputs
 *
 * Returns
 *
 */
Summary_ResumeProcessor_Parsers::CResumeBuilder::~CResumeBuilder()
{
    for (auto & registryItem: parserRegistry)
    {
        delete registryItem.second;
    }
}

/*
 * Summary_ResumeProcessor_Parsers::CResumeBuilder::Build
 *
 * Description
 * Builds resume from sections. If language of input text is not english, then remakes sections vector once more
 * from the translated input
 *
 * Inputs
 * vector<CSection> sections - sections data
 *
 * Returns
 * Built CResume object
 */
CResume Summary_ResumeProcessor_Parsers::CResumeBuilder::Build(vector<CSection> sections)
{
    CDebugUtils::DebugCout("CResumeBuilder", "Build", "");
    CResume resume;
    //If the CV language is english then proceed without any change
    bool isEnglish = true;
    vector<string> originalInput;

    for (CSection section: sections)
    {
        if (section.getType() == ESectionType::TextLanguage)
        {
            parserRegistry[section.getType()]->Parse(section, resume);

            if (resume.GetTextLanguageCode().length() > 0 && resume.GetTextLanguageCode().compare("en") != 0)
            {
                isEnglish = false;

                //Save original input resume
                for (string inputLine: section.GetContent())
                {
                    originalInput.push_back(inputLine);
                }
            }

            section.ClearContent();
            break;
        }
    }

    //Extract sections once more from translated input
    if (!isEnglish && resume.GetTranslationSize() > 0)
    {
        CSectionExtractor sectionExtractor;
        sections = sectionExtractor.ExtractFrom(resume.GetTranslation());
    }

    sort(sections.begin(), sections.end());

    for (CSection section: sections)
    {
        if (section.getType() == ESectionType::TextLanguage)
        {
            continue;
        }

        if (parserRegistry.count(section.getType()) != 0)
        {
            parserRegistry[section.getType()]->Parse(section, resume);

            section.ClearContent();
        }
    }

    //Put back original language input only if it was changed
    if (!isEnglish && resume.GetTranslationSize() > 0)
    {
        resume.SetRawInput(originalInput);
    }

    return resume;
}
