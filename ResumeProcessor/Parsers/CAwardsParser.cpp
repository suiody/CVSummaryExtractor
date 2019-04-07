//
// File CAwardsParser.cpp
//
//

#include "CAwardsParser.h"
#include "../../Common/CDebugUtils.h"

using namespace Summary_Common;

/*
 * Summary_ResumeProcessor_Parsers::CAwardsParser::Parse
 *
 * Description
 * Method for parsing awards section and saving results into resume object
 *
 * Inputs
 * CSection section - section data
 * CResume &resume - resume object to be filled during parsing process
 *
 * Returns
 * changed resume object through input parameter resume
 */
void Summary_ResumeProcessor_Parsers::CAwardsParser::Parse(CSection section, CResume &resume)
{
    CDebugUtils::DebugCout("CAwardsParser", "Parse", "");
    resume.SetAwards(section.GetContent());
}
