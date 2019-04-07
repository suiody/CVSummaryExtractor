//
// File CEducationDataParser.cpp
//
//

#include "CEducationDataParser.h"
#include <boost/xpressive/regex_compiler.hpp>
#include "../../Common/CTextUtils.h"
#include "../../Common/CDebugUtils.h"
#include "../Helpers/CDateHelper.h"
#include "DictionaryLoaderSingleton.h"

using namespace Summary_Common;
using namespace Summary_ResumeProcessor_Helpers;

const vector<string> Summary_ResumeProcessor_Parsers::CEducationDataParser::courseLookUp = {
        "bachelor",
        "diploma",
        "master",
        "doctor",
        "phd"
};

const vector<string> Summary_ResumeProcessor_Parsers::CEducationDataParser::schoolLookUp = {
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

const vector<string> Summary_ResumeProcessor_Parsers::CEducationDataParser::exactSchoolLookUp = {
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
        "academy",
        "institute",
        "institution",
        "university",
        "college",
        "seminary",
};

const boost::xpressive::sregex Summary_ResumeProcessor_Parsers::CEducationDataParser::BirthDayRegex =
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

const boost::xpressive::sregex Summary_ResumeProcessor_Parsers::CEducationDataParser::SplitByWhiteSpaceRegex =
        boost::xpressive::sregex::compile ("\\s+|\\,|\\:|\\.|\\;|\u2028|\\(|\\)|\\/|\\-", boost::xpressive::regex_constants::optimize);

Summary_ResumeProcessor_Parsers::CEducationDataParser::CEducationDataParser()
{
    fieldsOfStudyLookUp = DictionaryLoaderSingleton::GetInstance().GetFieldsOfStudy();
    fieldsOfStudyLookUpShort = DictionaryLoaderSingleton::GetInstance().GetFieldsOfStudyShort();
    shortTaggedDegreeLookUp = DictionaryLoaderSingleton::GetInstance().GetShortTaggedDegrees();

    degreeAbbbreviationsExact.push_back("associate");
    degreeAbbbreviationsExact.push_back("diploma");
    degreeAbbbreviationsExact.push_back("bachelor");
    degreeAbbbreviationsExact.push_back("master");
    degreeAbbbreviationsExact.push_back("masters");
    degreeAbbbreviationsExact.push_back("doctor");
    degreeAbbbreviationsExact.push_back("doctors");
    //degreeAbbbreviationsExact.push_back("doctor equivalency");
    //degreeAbbbreviationsExact.push_back("doctors equivalency");
    //degreeAbbbreviationsExact.push_back("resident"); //TODO: What to do with resident/residency degrees
    //degreeAbbbreviationsExact.push_back("residency");

    degreeAbbbreviationsExact.push_back("doctorate");
    degreeAbbbreviationsExact.push_back("post graduate");
    degreeAbbbreviationsExact.push_back("postgraduate");

    degreeAbbbreviationsExact.push_back("d.e.e.e");
    degreeAbbbreviationsExact.push_back("d. e. e. e");
    degreeAbbbreviationsExact.push_back("deee ");
    degreeAbbbreviationsExact.push_back("deee)");
    degreeAbbbreviationsExact.push_back("deee,");

    degreeAbbbreviations = DictionaryLoaderSingleton::GetInstance().GetDegreeAbbbreviations();
    degreeAbbbreviationsShort = DictionaryLoaderSingleton::GetInstance().GetDegreeAbbbreviationsShort();
    for (string abbreviation: degreeAbbbreviationsShort)
    {
        degreeAbbbreviationsExact.push_back(abbreviation);
    }

    universityLookUpLower = DictionaryLoaderSingleton::GetInstance().GetUniversitiesLower();

    //degreeAbbbreviationsExact.push_back(DictionaryLoaderSingleton::GetInstance().GetDegreeAbbbreviationsShort());

    for (string shortTaggedDegree: shortTaggedDegreeLookUp)
    {
        string lowerDegree = CTextUtils::ToLower(shortTaggedDegree);

        shortTaggedDegreeLookUpLower.push_back(lowerDegree);
    }

    for (string fieldOfStudy: fieldsOfStudyLookUp)
    {
        //cout << degree << endl;
        string lowerFieldOfStudy = CTextUtils::ToLower(fieldOfStudy);
        fieldsOfStudyLookUpLower.push_back(lowerFieldOfStudy);
    }

    for (string fieldOfStudy: fieldsOfStudyLookUpShort)
    {
        //cout << degree << endl;
        string lowerFieldOfStudy = CTextUtils::ToLower(fieldOfStudy);
        fieldsOfStudyLookUpShortLower.push_back(lowerFieldOfStudy);
    }
}

/*
 * Summary_ResumeProcessor_Parsers::CEducationDataParser::Parse
 *
 * Description
 * Method for parsing education data section and saving results into resume object
 *
 * Inputs
 * CSection section - section data
 * CResume &resume - resume object to be filled during parsing process
 *
 * Returns
 * changed resume object through input parameter resume
 */
void Summary_ResumeProcessor_Parsers::CEducationDataParser::Parse(CSection section, CResume &resume)
{
    CDebugUtils::DebugCout("CEducationParser", "Parse", "");
    int i = 0;
    bool educationNull = true;
    bool educationFound = false;

    string lastEducationKeyword = "";
    int educationCounter = 0;

    vector<string> sectionRawContentLower;
    vector<string> educationKeywords;

    educationKeywords.push_back("education");
    educationKeywords.push_back("qualification");
    educationKeywords.push_back("academic");
    educationKeywords.push_back("academia");
    educationKeywords.push_back("e d u c a t i o n");
    educationKeywords.push_back("scholastics");
    educationKeywords.push_back("graduation");
    educationKeywords.push_back("professional development");
    educationKeywords.push_back("professional background");
    educationKeywords.push_back("summary of qualification");
    educationKeywords.push_back("formal qualifications");
    educationKeywords.push_back("relevant education");
    educationKeywords.push_back("other education");
    educationKeywords.push_back("additional education");
    educationKeywords.push_back("professional education");
    educationKeywords.push_back("continuing education");
    educationKeywords.push_back("technical qualification");
    /*educationKeywords.push_back("e d u c a t i o n");
    educationKeywords.push_back("e d u c a t i o n");*/
    educationKeywords.push_back("scholarship");
    educationKeywords.push_back("degree");
    educationKeywords.push_back("doctorate");
    educationKeywords.push_back("study");
    educationKeywords.push_back("foreign study");
    educationKeywords.push_back("professional training");
    educationKeywords.push_back("college");
    educationKeywords.push_back("school");
    educationKeywords.push_back("skills");
    educationKeywords.push_back("apprenticeship");
    educationKeywords.push_back("global experience");
    educationKeywords.push_back("related coursework");
    educationKeywords.push_back("thesis title");

    for (string line: section.GetRawContent())
    {
        //cout << line << endl;
        if (line.length() < 1)
        {
            continue;
        }
//cout << line << endl;
        sectionRawContentLower.push_back(CTextUtils::ToLower(CTextUtils::RemoveUnusedSpaces(line)));
    }

    if (taggedDegreesLookUp.size() < 1)
    {
        taggedDegreesLookUp = DictionaryLoaderSingleton::GetInstance().GetNonNames();
    }


    for (string educationKeyword: educationKeywords)
    {
        if (!educationFound)
        {
            educationFound = ExtractEducation(resume, sectionRawContentLower, educationKeyword, true);
            if (educationFound)
            {
                //cout << educationKeyword << endl;
                lastEducationKeyword = educationKeyword;
                break;
            }
        }

        if (educationCounter > 4)
        {
            //cout << educationKeyword << endl;
        }

        educationCounter++;
    }

    if (!educationFound)
    {
        educationFound = ExtractEducation(resume, sectionRawContentLower, "", false);
        if (educationFound)
        {
            //cout << "found at last" << endl;
        }
    }

    if (educationFound)
    {
        if (lastEducationKeyword.length() > 0)
        {
            ExtractSchool(resume, sectionRawContentLower, lastEducationKeyword, true);
        }
        else
        {
            ExtractSchool(resume, sectionRawContentLower, "", false);
        }
    }
    else
    {
        ExtractSchool(resume, sectionRawContentLower, "", false);
    }

    vector<CSchoolData> schoolDatas;
    CSchoolData currentSchoolData(0);

    //cout << "c:" << resume.GetEducationDegrees().size() << endl;
    for (string educationDegree: resume.GetEducationDegrees())
    {
        //cout << educationDegree << endl;
        vector<string> Dates;
        bool containedSchoolDegree = false;

        string trimmedEducationDegree = CTextUtils::TrimNameCharacters(educationDegree);
        for (string degreeAbbreviation: degreeAbbbreviationsExact)
        {
            if (CTextUtils::ContainsWithExactStartAndEnding(educationDegree, degreeAbbreviation))
            {
                resume.AddSchoolDegree(degreeAbbreviation);
                currentSchoolData.AddSchoolDegree(degreeAbbreviation);

                containedSchoolDegree = true;
            }
        }

        if (containedSchoolDegree)
        {
            for (string fieldOfStudy: fieldsOfStudyLookUpLower)
            {
                if (CTextUtils::ContainsWithExactStartAndEnding(educationDegree, fieldOfStudy))
                {
                    resume.AddSchoolField(fieldOfStudy);
                    currentSchoolData.AddSchoolField(fieldOfStudy);
                }
            }

            for (string fieldOfStudy: fieldsOfStudyLookUpShortLower)
            {
                if (CTextUtils::ContainsWithExactStartAndEnding(educationDegree, fieldOfStudy))
                {
                    resume.AddSchoolField(fieldOfStudy);

                    if (currentSchoolData.GetSchoolField().length() < 1)
                    {
                        currentSchoolData.AddSchoolField(fieldOfStudy);
                    }

                    //cout << "+++++ " << fieldOfStudy << endl;
                }
            }
        }

        /*for (string school: exactSchoolLookUp)
        {
            if (CTextUtils::ContainsWithExactEnding(educationDegree, school))
            {
                resume.AddSchoolName(school);
                currentSchoolData.AddSchoolName(school);
                //cout << school << endl;
            }
        }*/

        if (
                find_if(educationDegree.begin(), educationDegree.end(), ::isdigit) != educationDegree.end())
        {
            //cout << educationDegree << endl;
            //cout << trimmedEducationDegree << endl;

            boost::xpressive::sregex_token_iterator i(trimmedEducationDegree.begin(), trimmedEducationDegree.end(), SplitByWhiteSpaceRegex, -1);
            boost::xpressive::sregex_token_iterator j;

            while(i != j)
            {
                string word = *i++;
                //cout << word << endl;

                boost::xpressive::sregex_iterator itWord(word.begin(), word.end(), BirthDayRegex);
                boost::xpressive::sregex_iterator endWord;
                for (; itWord != endWord; ++itWord)
                {
                    //cout << "word:" << itWord->str() << endl;
                    bool canAdd = true;
                    CTextUtils::AddVectorMutuallyExclusively(Dates, itWord->str());
                }
            }

            boost::xpressive::sregex_iterator it(trimmedEducationDegree.begin(), trimmedEducationDegree.end(), BirthDayRegex);
            boost::xpressive::sregex_iterator end;
            for (; it != end; ++it)
            {
                //cout << it->str() << endl;
                //Dates.push_back(it->str());
                CTextUtils::AddVectorMutuallyExclusively(Dates, it->str());
            }
        }

        if (Dates.size() > 0)
        {
            //cout << Dates.size() << "______________" << endl;
            //cout << Dates[0] << endl;
            //cout << Dates[1] << endl;
            //cout << date << endl;
            //cout << educationDegree << endl;
            for (string date: Dates)
            {
                resume.AddEducationDegreeYear(educationDegree, date);
                if (currentSchoolData.GetDateFrom().length() == 0)
                {
                    currentSchoolData.AddDateFrom(date);
                }
                else
                {
                    currentSchoolData.AddDateTo(date);
                }
                //cout << date << endl;
            }
        }

        if (containedSchoolDegree)
        {
            int index = currentSchoolData.GetIndex();
            schoolDatas.push_back(currentSchoolData);

            CSchoolData currentSchoolData(index + 1);
        }

    }

    for (CSchoolData &data: schoolDatas)
    {
        if (data.GetSchoolName().length() == 0)
        {
            for (string educationDegree: resume.GetEducationDegrees())
            {
                bool schoolAdded = false;
                //cout << educationDegree << endl;

                for (string universityName: universityLookUpLower)
                {
                    //cout << universityName << endl;
                    if (CTextUtils::StringContains(educationDegree, universityName))
                    {
                        bool canAdd = true;
                        for (CSchoolData &dataTemp: schoolDatas)
                        {

                        }

                        data.AddSchoolName(universityName);
                        schoolAdded = true;
                        break;
                    }
                }

                if (!schoolAdded)
                {
                    for (string school: exactSchoolLookUp)
                    {
                        if (CTextUtils::ContainsWithExactEnding(educationDegree, school))
                        {
                            bool canAdd = true;
                            for (CSchoolData &dataTemp: schoolDatas)
                            {

                            }

                            data.AddSchoolName(school);

                            break;
                        }
                    }
                }
            }
        }
        resume.AddSchool(data);
    }
}

/*
 * Summary_ResumeProcessor_Parsers::CEducationDataParser::ExtractEducation
 *
 * Description
 * Extracts education data (lines with degrees and fields of study) and saves into resume object.
 * If already found then returns immediately
 *
 * Inputs
 * CResume &resume - reference to the resume object
 * vector<string> &sectionRawContentLower - section string vector in lower case
 * const string &educationKeyWord - education keyword to look educational data after
 * bool useKeyWord - if true then uses the keyword in education search
 *
 * Returns
 * changed resume object through input parameter resume
 */
bool Summary_ResumeProcessor_Parsers::CEducationDataParser::ExtractEducation(CResume &resume,
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
                //cout << lookDegreeFromBeginning << endl;
                degreeLineNumbers.push_back(lineCount);
            }

            //Further educational data is available right after the keyword itself
            if (((newLine.length() > lookDegreeFromBeginning.length() && CTextUtils::StartsWith(line, lookDegreeFromBeginning)) ||
                 (newLine.length() - 1 > lookDegreeFromBeginning.length() && CTextUtils::StartsWith(line.substr(1), lookDegreeFromBeginning) ) ||
                 (newLine.length() - 2 > lookDegreeFromBeginning.length() && CTextUtils::StartsWith(line.substr(2), lookDegreeFromBeginning))||
                 (newLine.length() - 3 > lookDegreeFromBeginning.length() && CTextUtils::StartsWith(line.substr(3), lookDegreeFromBeginning))))
            {
                //cout << lookDegreeFromBeginning << endl;
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
                //cout << lookDegreeFromBeginning << endl;
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

            if (CTextUtils::ContainsWithExactStartAndEnding(newLine, shortTaggedDegree))
            {
                //cout << shortTaggedDegree << endl;
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
                    if (CTextUtils::ContainsWithExactStartAndEnding(newLine, degreeAbbreviation))
                    {
                        //cout << degreeAbbreviation << ":" << newLine << endl;
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

/*
 * Summary_ResumeProcessor_Parsers::CEducationDataParser::ExtractSchool
 *
 * Description
 * Extracts school data
 *
 * Inputs
 * CResume &resume - reference to the resume object
 * vector<string> &sectionRawContentLower - section string vector in lower case
 * const string &educationKeyWord - education keyword to look educational data after
 * bool useKeyWord - if true then uses the keyword in education search
 *
 * Returns
 * changed resume object through input parameter resume
 */
bool Summary_ResumeProcessor_Parsers::CEducationDataParser::ExtractSchool(CResume &resume,
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
                //cout << school << endl;
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

/*
 * Summary_ResumeProcessor_Parsers::CEducationDataParser::LineDistance
 *
 * Description
 * Gets back if the distance between lines got from input vector and line defined with its index
 * falls into predefined maximum distance value
 *
 * Inputs
 * const vector<int> &lineNumbers - predefined line indexes vector
 * int lineIndex - current line index to check for
 * bool positiveOnly - if true then counts with lines only after each line in array
 * int distance - maximum distance that are allowed between lines
 *
 * Returns
 * -1 if no predefined line numbers, 0 line index too far away, 1 if any of line indexes from array
 * is not more away than predefined distance
 */
int Summary_ResumeProcessor_Parsers::CEducationDataParser::LineDistance(const vector<int> &lineNumbers, int lineIndex,
                                                                        bool positiveOnly, int distance)
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
