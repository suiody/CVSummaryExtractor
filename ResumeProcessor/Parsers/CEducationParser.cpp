//
// File CEducationParser.cpp
//
//

#include "CEducationParser.h"
#include <boost/xpressive/regex_compiler.hpp>
#include "../../Common/CTextUtils.h"
#include "../../Common/CDebugUtils.h"
#include "../Helpers/CDateHelper.h"
#include "DictionaryLoaderSingleton.h"

using namespace Summary_Common;
using namespace Summary_ResumeProcessor_Helpers;

const vector<string> Summary_ResumeProcessor_Parsers::CEducationParser::courseLookUp = {
        "bachelor",
        "diploma",
        "master",
        "doctor",
        "phd"
};

const vector<string> Summary_ResumeProcessor_Parsers::CEducationParser::schoolLookUp = {
        "high school",
        "secondary school",
        "secondary high school",
        "secondary highschool",
        "technical school",
        "highschool",
        "secondaryschool",
        "technicalschool",
        "elementary school",
        "elementaryschool",
        "department",
        "academy",
        "faculty",
        "institute",
        "institution",
        "university",
        "college",
        "seminary",
        "diploma from"
};

const boost::xpressive::sregex Summary_ResumeProcessor_Parsers::CEducationParser::BirthDayRegex =
        boost::xpressive::sregex::compile
                ("((([012][1-9]|[1-2]0|3[01])|([1-9]))(|th|st|nd|rd)(|\\,|\\.)(|\\s+)(|of)(|\\s+)(Jan(|.)|January|Feb(|.)|February|Mar(|.)|March"
                         "|Apr(|.)|April|May|Jun(|.)|June|Jul(|.)|July|Aug(|.)|August|Sep(|.)|Sept(|.)|September|Oct(|.)|October|Nov(|.)"
                         "|November|Dec(|.)|December)(|\\,)(|\\s+)(19|20)([0-9]{2}))"

                         "|((([012][1-9]|[1-2]0|3[01])|([1-9]))(|th|st|nd|rd)(\\-|\\/|\\.)(|\\s+)(Jan(|.)|January|Feb(|.)|February|Mar(|.)"
                         "|March|Apr(|.)|April|May|Jun(|.)|June|Jul(|.)|July|Aug(|.)|August|Sep(|.)|Sept(|.)|September|Oct(|.)|October"
                         "|Nov(|.)|November|Dec(|.)|December)(\\-|\\/|\\.)(19|20)([0-9]{2}))"

                         "|((19|20)([0-9]{2})(|\\s+)(|\\-|\\/|\\.)(|\\s+)(Jan(|.)|January|Feb(|.)|February|Mar(|.)|March|Apr(|.)"
                         "|April|May|Jun(|.)|June|Jul(|.)|July|Aug(|.)|August|Sep(|.)|Sept(|.)|September|Oct(|.)|October|Nov(|.)|November"
                         "|Dec(|.)|December)(|\\s+)(|\\-|\\/|\\.)(|\\s+)(([012][1-9]|[1-2]0|3[01])|([1-9]))(|th|st|nd|rd))"

                         "|((([012][1-9]|[1-2]0|3[01])|([1-9]))(|th|st|nd|rd)(\\-|\\/|\\.)(Jan(|.)|January|Feb(|.)|February|Mar(|.)|March|Apr(|.)"
                         "|April|May|Jun(|.)|June|Jul(|.)|July|Aug(|.)|August|Sep(|.)|Sept(|.)|September|Oct(|.)|October|Nov(|.)|November"
                         "|Dec(|.)|December)(\\-|\\/|\\.)([0-9]{2}))"

                         "|((Jan(|.)|January|Feb(|.)|February|Mar(|.)|March|Apr(|.)|April|May|Jun(|.)|June|Jul(|.)|July|Aug(|.)|August"
                         "|Sep(|.)|Sept(|.)|September|Oct(|.)|October|Nov(|.)|November|Dec(|.)|December)(|\\s+)(|\\-|\\/|\\.|\\,)(|\\s+)"
                         "([012]?[1-9]|[12]0|3[01])(|th|st|nd|rd)(|\\s+)(|\\-|\\/|\\.|\\-|\\,)(|\\s+)(19|20)([0-9]{2}))"

                         "|((Jan(|.)|January|Feb(|.)|February|Mar(|.)|March|Apr(|.)|April|May|Jun(|.)|June|Jul(|.)|July|Aug(|.)|August"
                         "|Sep(|.)|Sept(|.)|September|Oct(|.)|October|Nov(|.)|November|Dec(|.)|December)\\s+(19|20)([0-9]{2}))"

                         "|(([012]?[1-9]|[12]0|3[01])(|th|st|nd|rd)\\-(Jan(|.)|January|Feb(|.)|February|Mar(|.)|March|Apr(|.)|April|May"
                         "|Jun(|.)|June|Jul(|.)|July|Aug(|.)|August|Sep(|.)|Sept(|.)|September|Oct(|.)|October|Nov(|.)|November|Dec(|.)"
                         "|December)\\-([0-9]{2}))"

                         "|((([012][1-9]|[1-2]0|3[01])|([1-9]))(|\\s+)(\\.|\\-|\\/)(|\\s+)((0[1-9]|1[012])|([1-9]))(|\\s+)(\\.|\\-|\\/"
                         "|\\s)(|\\s+)(19|20)([0-9]{2}))"

                         "|(((0[1-9]|1[012])|([1-9]))(|\\s+)(\\.|\\-|\\/)(|\\s+)(([012][1-9]|[1-2]0|3[01])|([1-9]))(|\\s+)(\\.|\\-|\\/"
                         "|\\s)(|\\s+)(19|20)([0-9]{2}))"

                         "|((19|20)([0-9]{2})(|\\s+)(\\.|\\-|\\/)(|\\s+)(([012][1-9]|[1-2]0|3[01])|([1-9]))(|\\s+)(\\.|\\-|\\/)(|\\s+)"
                         "([012][1-9]|[1-2]0|3[01]))"
                         //yyyy dd mm, yyyy d m, yyyy m d and yyyy mm dd in variations

                         "|((([012][1-9]|[1-2]0|3[01])|([1-9]))(|\\s+)(\\.|\\-|\\/)(|\\s+)((0[1-9]|1[012])|([1-9]))(|\\s+)(\\.|\\-|\\/"
                         "|\\s)(|\\s+)([0-9]{2}))"

                         "|(((0[1-9]|1[012])|([1-9]))(|\\s+)(\\.|\\-|\\/)(|\\s+)(([012][1-9]|[1-2]0|3[01])|([1-9]))(|\\s+)(\\.|\\-|\\/"
                         "|\\s)(|\\s+)([0-9]{2}))"

                         "|((([012][1-9]|[1-2]0|3[01])|([1-9]))(|\\s+)\\/(|\\s+)((0[1-9]|1[012])|([1-9]))(|\\s+)\\/(|\\s+)(19|20)([0-9]{2}))"

                         "|(((0[1-9]|1[012])|([1-9]))(|\\s+)\\/(|\\s+)(([012][1-9]|[1-2]0|3[01])|([1-9]))(|\\s+)\\/(|\\s+)(19|20)([0-9]{2}))"

                         //"|((([012][1-9]|[1-2]0|3[01])|([1-9]))\\/((0[1-9]|1[012])|([1-9]))\\-([0-9]{2}))"

                         //"|((([012][1-9]|[1-2]0|3[01])|([1-9]))\\/((0[1-9]|1[012])|([1-9]))\\s(19|20)([0-9]{2}))"

                         "|(([0-9]{2})\\/((0[1-9]|1[012])|([1-9]))\\/(([012][1-9]|[1-2]0|3[01])|([1-9])))"  //yy/mm/dd

                         "|((([012][1-9]|[1-2]0|3[01])|([1-9]))\\s+(19|20)([0-9]{2}))"

                         "|((19|20)([0-9]{2}))$",
                 boost::xpressive::regex_constants::optimize
                 |boost::xpressive::regex_constants::ECMAScript
                 |boost::xpressive::regex_constants::icase);

const boost::xpressive::sregex Summary_ResumeProcessor_Parsers::CEducationParser::SplitByWhiteSpaceRegex =
        boost::xpressive::sregex::compile ("\\s+|\\,|\\:|\\.|\\;|\u2028|\\(|\\)|\\/|\\-", boost::xpressive::regex_constants::optimize);

/*
* Summary_Model::CEducationParser::CEducationParser
*
* Description
* Constructor
*
* Inputs
*
* Returns
*
*/
Summary_ResumeProcessor_Parsers::CEducationParser::CEducationParser()
{
    fieldsOfStudyLookUp = DictionaryLoaderSingleton::GetInstance().GetFieldsOfStudy();
    shortTaggedDegreeLookUp = DictionaryLoaderSingleton::GetInstance().GetShortTaggedDegrees();
}

/*
 * Summary_ResumeProcessor_Parsers::CEducationParser::Parse
 *
 * Description
 * Method for parsing education section and saving results (school, start/end date) and course data into resume object
 *
 * Inputs
 * CSection section - section data
 * CResume &resume - resume object to be filled during parsing process
 *
 * Returns
 * changed resume object through input parameter resume
 */
void Summary_ResumeProcessor_Parsers::CEducationParser::Parse(CSection section, CResume &resume)
{
    CDebugUtils::DebugCout("CEducationParser", "Parse", "");
    int i = 0;
    bool educationNull = true;
    bool educationFound = false;

    string lastEducationKeyword = "";
    int educationCounter = 0;

    vector<string> sectionRawContentLower;
    vector<string> educationKeywords;


    while (i < section.GetContent().size())
    {
        string line = CTextUtils::Trim(section.GetContent()[i]);
        string school = ParseSchool(line);

        if (CTextUtils::IsEmptyOrWhiteSpace(school))
        {
            string lowerLine = CTextUtils::ToLower(line);

            if (!educationNull)
            {
                ParseStartAndEndDate(line, resume.GetCurrentEducation());
                string course = "";

                for (string s : courseLookUp )
                {
                    if (CTextUtils::StringContains(lowerLine, s))
                    {
                        course = s;
                        break;
                    }
                }

                if (!CTextUtils::IsEmptyOrWhiteSpace(course))
                {
                    resume.GetCurrentEducation().SetCourse(line);
                }
            }
        }
        else
        {
            if (!educationNull && CTextUtils::IsEmptyOrWhiteSpace(resume.GetCurrentEducation().GetCourse()))
            {
                resume.GetCurrentEducation().SetSchool(resume.GetCurrentEducation().GetSchool() + ", " + school);
            }
            else
            {
                CEducation currentEducation;
                currentEducation.SetSchool(school);
                educationNull = false;

                ParseStartAndEndDate(line, currentEducation);
                resume.AddEducation(currentEducation);
            }
        }

        i++;
    }
}

/*
 * Summary_ResumeProcessor_Parsers::CEducationParser::ParseSchool
 *
 * Description
 * Extracts school data from string
 *
 * Inputs
 * string line - string line to be parsed
 *
 * Returns
 * School data
 *
 */
string Summary_ResumeProcessor_Parsers::CEducationParser::ParseSchool(string line)
{
    string school = "";
    string lowerLine = CTextUtils::ToLower(line);

    for (string s : schoolLookUp )
    {
        if (CTextUtils::StringContains(lowerLine, s))
        {
            school = s;
            break;
        }
    }

    if (CTextUtils::IsEmptyOrWhiteSpace(school))
    {
        return "";
    }

    int endOfSchoolIndex = CTextUtils::FindString(line, "\t");
    if (endOfSchoolIndex == string::npos)
    {
        endOfSchoolIndex = line.size() - 1;
    }

    return CTextUtils::Trim(line.substr(0, endOfSchoolIndex + 1));
}

/*
 * Summary_ResumeProcessor_Parsers::CEducationParser::ParseStartAndEndDate
 *
 * Description
 * Extracts start and end date from string and saves into CEducation object
 *
 * Inputs
 * string line - string line to be parsed
 * CEducation &currentEducation - reference to education object that is changed
 *
 * Returns
 * Changes in CEducation object given as input
 */
void Summary_ResumeProcessor_Parsers::CEducationParser::ParseStartAndEndDate(string line, CEducation &currentEducation)
{
    CPeriod startAndEndDate = CDateHelper::ParseStartAndEndDate(line);

    if (startAndEndDate.GetStart().compare("") != 0)
    {
        //cout << startAndEndDate.GetStart() << "\n";
        currentEducation.SetStartDate(startAndEndDate.GetStart());
        currentEducation.SetEndDate(startAndEndDate.GetEnd());
    }
}

bool Summary_ResumeProcessor_Parsers::CEducationParser::ExtractEducation(CResume &resume,
                                                                        vector<string> &sectionRawContentLower,
                                                                        const string &educationKeyWord,
                                                                        bool useKeyWord)
{
    bool educationFound = false;
    string educationKeyWordLower = CTextUtils::ToLower(educationKeyWord);
    int lineCount = -1;

    for (string line: sectionRawContentLower)
    {
        bool lineAdded = false;
        string newLine = line;
        lineCount ++;
        line = CTextUtils::TrimAnyChar(line, " &:+;./()[]{}\\\t\"'^?>=<+*&%$#!_-");
        line = line + "   ";

        //If keyword needs to be found then search from the first positions of line
        if (useKeyWord && (CTextUtils::StartsWith(line, educationKeyWordLower) ||
                           CTextUtils::StartsWith(line.substr(1), educationKeyWordLower) ||
                           CTextUtils::StartsWith(line.substr(2), educationKeyWordLower)||
                           CTextUtils::StartsWith(line.substr(3), educationKeyWordLower)))
        {
            educationFound = true;
        }

        if (useKeyWord && !educationFound)
        {
            continue;
        }

        if (LineDistance(degreeLineNumbers, lineCount, true, 2) == 0)
        {
            continue;
        }


        for (string lookDegreeFromBeginning: shortTaggedDegreeLookUpBeginning)
        {
            //This keyword was found in the beginning of the text and serves as a clue that we have further aducational data
            if ((CTextUtils::StartsWith(line, lookDegreeFromBeginning) ||
                 CTextUtils::StartsWith(line.substr(1), lookDegreeFromBeginning) ||
                 CTextUtils::StartsWith(line.substr(2), lookDegreeFromBeginning)||
                 CTextUtils::StartsWith(line.substr(3), lookDegreeFromBeginning)))
            {
                degreeLineNumbers.push_back(lineCount);
            }

            //Further educational data is available right after the keyword itself
            if (((newLine.length() > lookDegreeFromBeginning.length() && CTextUtils::StartsWith(line, lookDegreeFromBeginning)) ||
                 (newLine.length() - 1 > lookDegreeFromBeginning.length() && CTextUtils::StartsWith(line.substr(1), lookDegreeFromBeginning) ) ||
                 (newLine.length() - 2 > lookDegreeFromBeginning.length() && CTextUtils::StartsWith(line.substr(2), lookDegreeFromBeginning))||
                 (newLine.length() - 3 > lookDegreeFromBeginning.length() && CTextUtils::StartsWith(line.substr(3), lookDegreeFromBeginning))))
            {
                resume.AddEducationDegree(lineCount, line);
                lineAdded = true;
                educationFound = true;
                break;
            }
        }


        for (string lookDegreeFromBeginning: shortTaggedDegreeLookUpBeginning)
        {
            //cout << lookDegreeFromBeginning << endl;
            if (((newLine.length() > lookDegreeFromBeginning.length() && CTextUtils::StartsWith(line, lookDegreeFromBeginning)) ||
                 (newLine.length() - 1 > lookDegreeFromBeginning.length() && CTextUtils::StartsWith(line.substr(1), lookDegreeFromBeginning) ) ||
                 (newLine.length() - 2 > lookDegreeFromBeginning.length() && CTextUtils::StartsWith(line.substr(2), lookDegreeFromBeginning))||
                 (newLine.length() - 3 > lookDegreeFromBeginning.length() && CTextUtils::StartsWith(line.substr(3), lookDegreeFromBeginning))))
            {
                resume.AddEducationDegree(lineCount, line);
                degreeLineNumbers.push_back(lineCount);
                lineAdded = true;
                educationFound = true;
                break;
            }
        }

        if (lineAdded)
        {
            continue;
        }

        /*for (string school: schoolLookUp)
        {
            if (CTextUtils::StringContains(newLine, school))
            {
                resume.AddEducationDegree(line);
                degreeLineNumbers.push_back(lineCount);
                lineAdded = true;
                educationFound = true;
                break;
            }
        }

        if (lineAdded)
        {
            continue;
        }*/

        if (line.length() > 0 && line.find(" ") != string::npos)
        {
            newLine = CTextUtils::RemoveUnusedSpaces(line) + " ";
        }

        for (string shortTaggedDegree: shortTaggedDegreeLookUpLower)
        {
            //string lowerDegree = CTextUtils::ToLower(shortTaggedDegree);

            if (CTextUtils::StringContains(newLine, shortTaggedDegree))
            {
                resume.AddEducationDegree(lineCount, line);
                degreeLineNumbers.push_back(lineCount);
                lineAdded = true;
                educationFound = true;
                break;
            }
        }

        if (lineAdded)
        {
            continue;
        }

        for (string fieldOfStudy: fieldsOfStudyLookUpLower)
        {
            //string lowerFieldOfStudy = CTextUtils::ToLower(fieldOfStudy);

            if (CTextUtils::StringContains(newLine, fieldOfStudy))
            {
                for (string degreeAbbreviation: degreeAbbbreviations)
                {
                    if (CTextUtils::ContainsWithExactStart(newLine, degreeAbbreviation))
                    {
                        //string originalLine = line;
                        /*string::size_type firstPosition = newLine.find(fieldOfStudy);
                        string::size_type secondPosition = newLine.find(degreeAbbreviation);
                        string::size_type oldPosition;
                        string::size_type lastPosition;
                        string::size_type lastDotPosition;

                        if (firstPosition > secondPosition)
                        {
                            lastPosition = firstPosition + fieldOfStudy.length();
                            firstPosition = secondPosition;
                        }
                        else
                        {
                            lastPosition = secondPosition + degreeAbbreviation.length();
                        }

                        //cout << lastPosition << endl;
                        oldPosition = firstPosition;

                        secondPosition = newLine.find_last_of(".", firstPosition);
                        lastDotPosition = newLine.find(".", lastPosition);

                        if (secondPosition != string::npos)
                        {
                            firstPosition = secondPosition;
                            secondPosition = newLine.find(" ", firstPosition);

                            if (secondPosition < oldPosition)
                            {
                                firstPosition = secondPosition;
                            }
                        }
                        else
                        {
                            firstPosition = 0;
                        }

                        if (lastDotPosition != string::npos && lastDotPosition > firstPosition)
                        {
                            newLine = CTextUtils::TrimAnyChar(newLine.substr(firstPosition, lastDotPosition - firstPosition), " ;-_:/\\");
                        }
                        else
                        {
                            newLine = CTextUtils::TrimAnyChar(newLine.substr(firstPosition), " ;-_:/\\");
                        }*/


                        resume.AddEducationDegree(lineCount, newLine);
                        degreeLineNumbers.push_back(lineCount);
                        lineAdded = true;
                        educationFound = true;

                        //cout << degreeAbbreviation << endl;

                        break;
                    }
                }

                //Have already this line, check for any other fields of study or degrees inside that line later
                if (lineAdded)
                {
                    break;
                }
            }
        }

        if (lineAdded)
        {
            continue;
        }

        if (CTextUtils::StringContains(newLine, "degree"))
        {
            for (string degreeAbbreviation: degreeAbbbreviations)
            {
                if (CTextUtils::ContainsWithExactStart(newLine, degreeAbbreviation + "degree") ||
                    CTextUtils::ContainsWithExactStart(newLine, degreeAbbreviation + " degree"))
                {
                    resume.AddEducationDegree(lineCount, line);
                    degreeLineNumbers.push_back(lineCount);
                    educationFound = true;
                    lineAdded = true;
                    //cout << degreeAbbreviation << endl;
                    //cout << "========" << endl;
                    break;
                }
            }
        }
    }

    return educationFound;
}

bool Summary_ResumeProcessor_Parsers::CEducationParser::ExtractSchool(CResume &resume,
                                                                     vector<string> &sectionRawContentLower,
                                                                     const string &educationKeyWord, bool useKeyWord)
{
    bool educationFound = false;
    string educationKeyWordLower = CTextUtils::ToLower(educationKeyWord);
    int lineCount = -1;

    for (string line: sectionRawContentLower)
    {
        bool lineAdded = false;
        line = CTextUtils::TrimAnyChar(line, " &:+;./()[]{}\\\t\"'^?>=<+*&%$#!_-");
        string newLine = line;
        lineCount ++;

        line = line + "   ";
        //If keyword needs to be found then search from the first positions of line
        if (useKeyWord && (CTextUtils::StartsWith(line, educationKeyWordLower) ||
                           CTextUtils::StartsWith(line.substr(1), educationKeyWordLower) ||
                           CTextUtils::StartsWith(line.substr(2), educationKeyWordLower)||
                           CTextUtils::StartsWith(line.substr(3), educationKeyWordLower)))
        {
            educationFound = true;
        }

        if (useKeyWord && !educationFound)
        {
            continue;
        }

        if (LineDistance(degreeLineNumbers, lineCount, false, 2) == 0)
        {
            for (int index: degreeLineNumbers)
            {
                //cout << "+" << index << endl;
            }

            continue;
        }

        for (string school: schoolLookUp)
        {
            if (CTextUtils::ContainsWithExactEnding(newLine, school))
            {
                resume.AddEducationDegree(lineCount, line);
                degreeLineNumbers.push_back(lineCount);
                lineAdded = true;
                educationFound = true;
                //cout << school << endl;
                //cout << lineCount << endl;
                break;
            }
        }
    }

    return educationFound;
}

int Summary_ResumeProcessor_Parsers::CEducationParser::LineDistance(const vector<int> &lineNumbers,
                                                                   int lineIndex,
                                                                   bool positiveOnly,
                                                                   int distance)
{
    int returnValue = 0;
//degreeLineNumbers
    if (lineNumbers.size() < 1)
    {
        return -1;
    }

    for (int index: lineNumbers)
    {
        if (positiveOnly)
        {
            if (lineIndex - index <= distance && lineIndex >= index)
            {
                returnValue = 1;
                break;
            }
        }
        else
        {
            if (abs(lineIndex - index) <= distance)
            {
                returnValue = 1;
                break;
            }
        }
    }

    return returnValue;
}

