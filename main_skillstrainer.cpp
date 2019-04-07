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
vector<string> addresses;
vector<string> *skills;

void ParseNextFile(string nextUrl, string currentFile, string outputFile);
bool StringAndLengthCompare(const string &s1, const string &s2);

INITIALIZE_EASYLOGGINGPP

int main(int argc, char* argv[])
{
    CResourceLoader resourceLoader;
    vector<string> skills = resourceLoader.Load(
            CFileUtils::GetRunPath() + "ResumeProcessor/Data/Skills.txt", '\n');
    vector<string> nameResults;
    string nameResultsText;

    string outputFile = "Skills.txt";

    vector<string> newSkills;
    newSkills.push_back("Equipment Cleaner");
    newSkills.push_back("Technical Management");
    newSkills.push_back("Structure Analysis");
    newSkills.push_back("Project Estimation");
    newSkills.push_back("Site Inspection");
    newSkills.push_back("Cost Control");
    newSkills.push_back("Project Architect");
    newSkills.push_back("Public Communications");

    for (string skill: newSkills)
    {
        if ( find(skills.begin(), skills.end(), skill)==skills.end() )
        {
            skills.push_back(skill);
        }
    }

    sort(skills.begin(), skills.end(), StringAndLengthCompare);

    ofstream wof("./" + outputFile, ios::out | ios::app);
    //wof.imbue(std::locale("en_US.UTF-8"));
    string fileContents = "";

    for ( int i = 0; i < skills.size(); i++ )
    {

        string sentence = skills.at(i);
        //wstring widestr = std::wstring(sentence.begin(), sentence.end());
        //const wchar_t* widecstr = widestr.c_str();
        fileContents += sentence;
        fileContents += "\n";
        cout << sentence << endl;
        //wof.write(widecstr, widestr.length());
    }

    wof << fileContents.c_str();
    //wof.flush();
    wof.close();


    return 0;
}

/*
 * StringAndLengthCompare
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
bool StringAndLengthCompare(const string &s1, const string &s2)
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