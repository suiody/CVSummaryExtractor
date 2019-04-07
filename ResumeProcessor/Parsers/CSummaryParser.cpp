//
// File CSummaryParser.cpp
//
//

#include "CSummaryParser.h"
#include "../../Common/CDebugUtils.h"
#include "../../Common/CTextUtils.h"

using namespace Summary_Common;

/*
 * Summary_ResumeProcessor_Parsers::CSummaryParser::Parse
 *
 * Description
 * Method for parsing summary section and saving results into resume object
 *
 * Inputs
 * CSection section - section data
 * CResume &resume - resume object to be filled during parsing process
 *
 * Returns
 * changed resume object through input parameter resume
 */
void Summary_ResumeProcessor_Parsers::CSummaryParser::Parse(CSection section, CResume &resume)
{
    CDebugUtils::DebugCout("CSummaryParser", "Parse", "");
    string sectionContent = "";
    bool firstLine = true;

    for (string contentLine: section.GetContent())
    {
        if (!firstLine)
        {
            sectionContent += ": ";
        }

        sectionContent += contentLine;

        firstLine = false;
    }

    resume.SetSummaryDescription(sectionContent);
}
