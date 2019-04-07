//
// File CSkillsParser.cpp
//
//

#include <algorithm>
#include "CSkillsParser.h"
#include "../../Common/CTextUtils.h"
#include "../../Common/CDebugUtils.h"
#include "../../Common/CFileUtils.h"
#include "DictionaryLoaderSingleton.h"
#include "../../Stemmers/porter2_stemmer.h"

using namespace Summary_Common;
using namespace Summary_ResumeProcessor_Helpers;

/*
 * Summary_Model::CSkillsParser::CSkillsParser
 *
 * Description
 * Constructor. Loads skills data from file into string vector
 *
 * Inputs
 * CResourceLoader resourceLoader - input resource loader object
 *
 * Returns
 *
 */
Summary_ResumeProcessor_Parsers::CSkillsParser::CSkillsParser(CResourceLoader resourceLoader)
{
    skillLookUp = DictionaryLoaderSingleton::GetInstance().GetSkills();
}

/*
 * Summary_ResumeProcessor_Parsers::CSkillsParser::Parse
 *
 * Description
 * Method for parsing skills section and saving results into resume object
 *
 * Inputs
 * CSection section - section data
 * CResume &resume - resume object to be filled during parsing process
 *
 * Returns
 * changed resume object through input parameter resume
 */
void Summary_ResumeProcessor_Parsers::CSkillsParser::Parse(CSection section, CResume &resume)
{
    CDebugUtils::DebugCout("CSkillsParser", "Parse", "");
    bool addSkill = false;
//return;
    //Extract maximum 10 skills from only skills section
    ExtractSkills(section.GetContent(), resume, 10);
    //Extract maximum 15 skills from all data
    ExtractSkills(section.GetRawContent(), resume, 15);
    //cout << section.GetContent().size() << endl;
    //cout << section.GetRawContent().size() << endl;

    vector<string> localSkills = resume.GetSkills();

    sort(localSkills.begin(), localSkills.end(), StringAndLengthCompare);

    resume.SetSkills(localSkills);
}

/*
 * Summary_ResumeProcessor_Parsers::CSkillsParser::ExtractSkills
 *
 * Description
 * Method for extracting skills and adding into the resume
 *
 * Inputs
 * const vector<string> &content - section data
 * CResume &resume - resume object to be filled during extracting process
 * int upperLimit - maximum skills in the vector inside resume object
 *
 * Returns
 * changed resume object through input parameter resume
 */
void Summary_ResumeProcessor_Parsers::CSkillsParser::ExtractSkills(const vector<string> &content, CResume &resume, int upperLimit)
{
    if (content.size() < 1 || resume.GetSkills().size() >= upperLimit)
    {
        return;
    }
//return;
    bool addSkill = false;
    vector<string> newContent;
    vector<string> useContent;
    vector<char> keepChars;

    keepChars.push_back('\'');
    keepChars.push_back('+');
    keepChars.push_back('#');
    keepChars.push_back(',');
    keepChars.push_back(';');
    keepChars.push_back('.');

    string allText = "";

    //return;

    for (string line: content)
    {
        string newLine = "";

        line = CTextUtils::ReplaceNonAlphanumeric(line, ' ', keepChars);
        line = CTextUtils::ToLower(line);
        //line = CTextUtils::Replace(line, ' ', ' ');

        vector<string> lineWords = CTextUtils::Split(line, ' ');

        for (string lineWord: lineWords)
        {
            //Porter2Stemmer::stem(lineWord);
            //cout << lineWord << ":" << lineWord.length() << endl;
            string trimmedWord = CTextUtils::Trim(lineWord);
            if (trimmedWord.length() > 0)
            {
                newLine += (CTextUtils::Trim(newLine).length() > 0 ? " " : "");
                newLine += trimmedWord;
            }
            //cout << newLine << endl;
        }

        if (newLine.length() > 0)
        {
            //cout << newLine << endl;
            newContent.push_back(newLine);
            allText += newLine + "\n";
        }
    }

    useContent = newContent;

    for (string skill: skillLookUp)
    {
        //string newSkill = "";
        string lowerSkill = "";

        //if (skill.find(' ') != string::npos)
        if (1 == 2)
        {
            vector<string> lineWords = CTextUtils::Split(skill, ' ');

            for (string lineWord: lineWords)
            {
                Porter2Stemmer::stem(lineWord);
                //newSkill += (newSkill.length() > 0 ? " " : "");
                //newSkill += lineWord;
                //cout << lineWord << endl;
            }

            //useContent = newContent;
        }
        else
        {
            //newSkill = CTextUtils::ToLower(skill);
            lowerSkill = CTextUtils::ToLower(skill);
            //useContent = content;
        }

        if (lowerSkill.length() < 1)
        {
            continue;
        }

        bool newSkillContainsSpace = CTextUtils::StringContains(lowerSkill, " ");

        for (string line: useContent)
        {
            //newSkill = skill;
            //size_t nPos = CTextUtils::FindStringIgnoreCase(line, newSkill);
            size_t nPos = CTextUtils::FindString(line, lowerSkill);

            if (nPos != string::npos)
            {

                if ((line.length() <= nPos + lowerSkill.length() ||
                    newSkillContainsSpace ||
                    //line.at(nPos + newSkill.length()) == ' ' ||
                    !isalnum(line.at(nPos + lowerSkill.length()))) &&
                    (nPos == 0 || line.at(nPos - 1) == ' '))
                {
                    addSkill = true;

                    for (string existingSkill: lowerSkills)
                    {
                        //existingSkill = CTextUtils::ToLower(existingSkill);

                        if (CTextUtils::StringContains(existingSkill, lowerSkill))
                        {
                            addSkill = false;
                            break;
                        }
                    }

                    if (addSkill)
                    {
                        if (resume.AddSkill(skill))
                        {
                            int skillCount = CTextUtils::StringCount(allText, lowerSkill);

                            lowerSkills.push_back(lowerSkill);
                            //allText.count(lowerSkill, 0);
                            //cout << skill << "::" << skillCount << endl;
                            resume.AddSkillCount(skill, skillCount);

                            if (resume.GetSkills().size() >= upperLimit)
                            {
                                break;
                            }
                        }
                    }
                }
            }
            else
            {
                //Could not find skill directly, try its different forms
                string newLowerSkill = lowerSkill;
                addSkill = true;
                if (newLowerSkill.length() > 3 && CTextUtils::EndsWith(newLowerSkill, "ies"))
                {
                    newLowerSkill = newLowerSkill.substr(0, newLowerSkill.length() - 3) + "y";
                }
                else if (newLowerSkill.length() > 1 && CTextUtils::EndsWith(newLowerSkill, "s"))
                {
                    newLowerSkill = newLowerSkill.substr(0, newLowerSkill.length() - 1);
                }
                else if (newLowerSkill.length() > 1 && CTextUtils::EndsWith(newLowerSkill, "y"))
                {
                    newLowerSkill = newLowerSkill.substr(0, newLowerSkill.length() - 1) + "ies";
                }
                else
                {
                    addSkill = false;
                }

                if (addSkill)
                {
                    size_t oldnPos = nPos;
                    size_t nPos = CTextUtils::FindString(line, newLowerSkill);

                    if (nPos != string::npos)
                    {
                        if ((line.length() <= nPos + newLowerSkill.length() ||
                             newSkillContainsSpace ||
                             //line.at(nPos + newSkill.length()) == ' ' ||
                             !isalnum(line.at(nPos + newLowerSkill.length()))) &&
                            (nPos == 0 || line.at(nPos - 1) == ' '))
                        {
                            addSkill = true;

                            for (string existingSkill: lowerSkills)
                            {
                                //existingSkill = CTextUtils::ToLower(existingSkill);

                                if (CTextUtils::StringContains(existingSkill, lowerSkill))
                                {
                                    addSkill = false;
                                    break;
                                }

                                if (CTextUtils::StringContains(existingSkill, newLowerSkill))
                                {
                                    addSkill = false;
                                    break;
                                }
                            }

                            if (addSkill)
                            {
                                if (resume.AddSkill(skill))
                                {
                                    lowerSkills.push_back(lowerSkill);

                                    int skillCount = CTextUtils::StringCount(allText, newLowerSkill);

                                    //AddSkillCount(newLowerSkill, 1)
                                    //cout << newLowerSkill << ":" << skill << ":" << skillCount << endl;
                                    resume.AddSkillCount(skill, skillCount);

                                    if (resume.GetSkills().size() >= upperLimit)
                                    {
                                        break;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }

        if (resume.GetSkills().size() >= upperLimit)
        {
            break;
        }
    }
}

/*
 * Summary_ResumeProcessor_Parsers::CSkillsParser::StringAndLengthCompare
 *
 * Description
 * Method for comparing input texts length and if those are equal then compare texts
 *
 * Inputs
 * const string &s1 - first text
 * const string &s2 - second text
 *
 * Returns
 * true if first string is less than the other in length or just less than the other in string compare
 */
bool Summary_ResumeProcessor_Parsers::CSkillsParser::StringAndLengthCompare(const string &s1, const string &s2)
{
    if (s1.length() == s2.length())
    {
        return s1.compare(s2) < 0;
    }
    else
    {
        return s1.length() > s2.length();
    }
}
