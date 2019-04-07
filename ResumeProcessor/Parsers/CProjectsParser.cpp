//
// File CProjectsParser.cpp
//
//

#include "CProjectsParser.h"
#include "../../Common/CTextUtils.h"
#include "../../Common/CDebugUtils.h"
#include "../Helpers/CDateHelper.h"

using namespace Summary_Common;
using namespace Summary_ResumeProcessor_Helpers;

/*
 * Summary_ResumeProcessor_Parsers::CProjectsParser::Parse
 *
 * Description
 * Method for parsing projects section and saving results (start/end date, title) into resume object
 *
 * Inputs
 * CSection section - section data
 * CResume &resume - resume object to be filled during parsing process
 *
 * Returns
 * changed resume object through input parameter resume
 */
void Summary_ResumeProcessor_Parsers::CProjectsParser::Parse(CSection section, CResume &resume)
{
    CDebugUtils::DebugCout("CProjectsParser", "Parse", "");
    int i = 0;
    bool currentProjectNull = true;

    while (i < section.GetContent().size())
    {
        string line = section.GetContent()[i];

        CPeriod startAndEndDate = CDateHelper::ParseStartAndEndDate(line);

        if (startAndEndDate.GetStart().compare("") == 0)
        {
            if (!currentProjectNull)
            {
                resume.GetCurrentProject().AddSummary(line);
            }
            else if (CTextUtils::FindString(line, ":") != string::npos)
            {
                vector<string> elements = CTextUtils::Split(line + "_", ':');
                CProject project;
                project.SetTitle(CTextUtils::Trim(elements[0]));
                //cout << line << "_" << "\n";
                //cout << elements.size() << "\n";

                project.AddSummary(CTextUtils::Trim(elements[1]));

                resume.AddProject(project);
            }
        }
        else
        {
            CProject currentProject;
            currentProject.SetStartDate(startAndEndDate.GetStart());
            currentProject.SetEndDate(startAndEndDate.GetEnd());

            string title = line.substr(0, line.find(startAndEndDate.GetStart()));
            title = CTextUtils::Trim(title);
            currentProject.SetTitle(title);

            resume.AddProject(currentProject);

            currentProjectNull = false;
        }

        i++;
    }
}
