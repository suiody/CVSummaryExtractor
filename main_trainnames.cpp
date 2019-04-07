#include <iostream>
#include <vector>
#include <algorithm>
#include <execinfo.h>
#include "ResumeProcessor/Helpers/CDateHelper.h"
#include "ResumeProcessor/Helpers/CResourceLoader.h"
#include "ResumeProcessor/CResumeProcessor.h"
#include "Common/CTextUtils.h"
#include "Common/CFileUtils.h"
#include "Common/CDebugUtils.h"
#include <boost/filesystem.hpp>
#include <boost/exception/info.hpp>
#include <boost/exception/all.hpp>
#include "Easylogging/easylogging++.h"
#include "Stacktrace/stacktrace.h"
#include <boost/lexical_cast.hpp>



#include <iostream>
#include <fstream>
#include <sstream>

using namespace boost::filesystem;
using namespace Summary_Common;
using namespace Summary_Json;
using namespace std;
using namespace Summary_ResumeProcessor_Helpers;
using namespace Summary_ResumeProcessor;
using namespace boost;

typedef boost::error_info<struct tag_stack_str,std::string> stack_info;

INITIALIZE_EASYLOGGINGPP

int TrainNames(const string &skillsFileName, const string &parentPath);

int main(int argc, char* argv[])
{
    if (argc < 1)
        return 11;

    boost::filesystem::path runPath(argv[0]);
    string parentPath = runPath.parent_path().string();
    string skillsFileName = argv[1];

    if (!CTextUtils::EndsWith(parentPath, "/"))
    {
        parentPath = parentPath + "/";
    }

    return TrainNames(skillsFileName, parentPath);

    return 0;
}

int TrainNames(const string &skillsFileName, const string &parentPath)
{
    CResourceLoader resourceLoader;
    vector<string> skills;
    vector<string> newSkills;

    string existingSkillsFileName;
    string nameResultsText = "";

    if (!CFileUtils::FileExists(skillsFileName))
    {
        return 8;
    }

    SLowerCaseUnorderedSet firstNameLookUp = resourceLoader.LoadIntoLowerCaseHashSet(
            CFileUtils::GetRunPath() + "ResumeProcessor/Data/FirstName.txt", ',');
    SLowerCaseUnorderedSet nonNameLookUp = resourceLoader.LoadIntoLowerCaseHashSet(
            CFileUtils::GetRunPath() + "ResumeProcessor/Data/NonNames.txt", ',');

    newSkills = resourceLoader.LoadExternal(skillsFileName, '\n');

    if (newSkills.size() < 1)
    {
        return 10;
    }

    for (string skill: newSkills)
    {
        string word = CTextUtils::Trim(skill);
        CLowerCaseString lowerWord(word);

        SLowerCaseUnorderedSet::const_iterator gotWord = firstNameLookUp.find(lowerWord);

        if (gotWord != firstNameLookUp.end())
        {
            cout << "Found:" << skill << "\n";
        }
        else
        {
            if ( find(skills.begin(), skills.end(), skill)==skills.end() )
            {
                SLowerCaseUnorderedSet::const_iterator gotWordNonNames = nonNameLookUp.find(lowerWord);

                if ( gotWordNonNames==nonNameLookUp.end() )
                {
                    cout << skill << endl;
                    skills.push_back(skill);
                }
            }
        }
    }

    for (auto i = firstNameLookUp.begin(); i != firstNameLookUp.end(); ++i)
    {
        string output = ((CLowerCaseString) (*i));
        skills.push_back(CTextUtils::FirstLetterUpCase(output));
    }

    sort(skills.begin(), skills.end());

    bool started = false;

    for (auto i = skills.begin(); i != skills.end(); ++i)
    {
        if ((*i).length() > 2)
        {
            nameResultsText += (started ? "," : "") + *i;
            started = true;
            //std::cout << *i << std::endl;
        }
    }

    ofstream namesFile;
    namesFile.open("./FirstName.txt");
    namesFile << nameResultsText;
    namesFile.close();

    return 0;
}