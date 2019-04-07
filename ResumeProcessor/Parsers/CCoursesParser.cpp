//
// File CCoursesParser.cpp
//
//

#include "CCoursesParser.h"
#include "../../Common/CDebugUtils.h"

using namespace Summary_Common;

/*
 * Summary_ResumeProcessor_Parsers::CCoursesParser::Parse
 *
 * Description
 * Method for parsing courses section and saving results into resume object
 *
 * Inputs
 * CSection section - section data
 * CResume &resume - resume object to be filled during parsing process
 *
 * Returns
 * changed resume object through input parameter resume
 */
void Summary_ResumeProcessor_Parsers::CCoursesParser::Parse(CSection section, CResume &resume)
{
    CDebugUtils::DebugCout("CCoursesParser", "Parse", "");
    resume.SetCourses(section.GetContent());
}
