//
// File CRawInputParser.cpp
//
//

#include "CRawInputParser.h"
#include "../../Common/CDebugUtils.h"
#include "../../Common/CTextUtils.h"

using namespace Summary_Common;

/*
 * Summary_ResumeProcessor_Parsers::CRawInputParser::Parse
 *
 * Description
 * Method for parsing raw data section and saving results into resume object
 *
 * Inputs
 * CSection section - section data
 * CResume &resume - resume object to be filled during parsing process
 *
 * Returns
 * changed resume object through input parameter resume
 */
void Summary_ResumeProcessor_Parsers::CRawInputParser::Parse(CSection section, CResume &resume)
{
    CDebugUtils::DebugCout("CRawInputParser", "Parse", "");
    int i = 0;

    while (i < section.GetContent().size())
    {
        string line = section.GetContent()[i];

        resume.AddRawInput(line);

        i++;
    }
}
