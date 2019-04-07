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
    SLowerCaseUnorderedSet firstNameLookUp = resourceLoader.LoadIntoLowerCaseHashSet(
            CFileUtils::GetRunPath() + "ResumeProcessor/Data/Skills.txt", ',');
    vector<string> nameResults;
    string nameResultsText;
    skills = new vector<string>();
    string outputFile = "skills.txt";

    //addresses.push_back("http://jobsearch.about.com/od/list/fl/list-of-skills-resume.htm");

    string currentFile = "skills_current.txt";

    ParseNextFile("http://jobsearch.about.com/od/list/fl/list-of-skills-resume.htm", currentFile, outputFile);

    sort(skills->begin(), skills->end(), StringAndLengthCompare);

    ofstream wof("./" + outputFile, ios::out | ios::app);
    //wof.imbue(std::locale("en_US.UTF-8"));
    string fileContents = "";

    for ( int i = 0; i < skills->size(); i++ )
    {

        string sentence = skills->at(i);
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

    delete skills;

    return 0;
}

void ParseNextFile(string nextUrl, string currentFile, string outputFile)
{
    string currentFilePath = "./" + currentFile;
    if ( find(addresses.begin(), addresses.end(), nextUrl)==addresses.end() )
    {
        addresses.push_back(nextUrl);

        if (CFileUtils::FileExists(currentFilePath))
        {
            cout << currentFilePath << " exists" << "\n";
        }
        else
        {
            cout << currentFilePath << " does not exist" << "\n";

            CFileUtils::ExecuteCommand("wget \"" + nextUrl + "\" -O " + currentFile);
            //currentFile = "./" + currentFile;

            if (CFileUtils::FileExists(currentFilePath))
            {
                cout << currentFilePath << " exists" << "\n";

                ifstream inputFile;
                stringstream buffer;
                string dataString;

                try
                {
                    //ifstream myfile (location);
                    inputFile.open(currentFilePath, ifstream::in);

                    if (inputFile.is_open())
                    {
                        buffer << inputFile.rdbuf();
                        inputFile.close();
                    }
                }
                catch (std::exception &ex)
                {
                    cout << currentFilePath << " reading error " << ex.what();
                    //inputFile.close();
                }

                dataString = buffer.str();
                buffer.clear();

                remove(currentFilePath.c_str());

                size_t nPos = dataString.find("<li>", 0); // fist occurrence

                while (nPos != string::npos)
                {
                    //cout << dataString.substr(nPos + 1, 20) << "\n";
                    string word = "";
                    size_t nPos1 = dataString.find(">", nPos + 1);
                    size_t nPos2 = dataString.find("</li>", nPos1 + 1);

                    if (nPos1 != string::npos && nPos2 != string::npos)
                    {
                        word = dataString.substr(nPos1 + 1, nPos2 - nPos1 - 1);
                        word = CTextUtils::Trim(word);
                        CLowerCaseString lowerWord(word);

                        //cout << word << "\n";

                        size_t nPoshref1 = word.find("http://jobsearch.about.com/od/list/", 0);
                        if (nPoshref1 == string::npos)
                        {
                            nPoshref1 = word.find("http://jobsearch.about.com/od/skills/", 0);
                        }
                        if (nPoshref1 == string::npos)
                        {
                            nPoshref1 = word.find("http://jobsearch.about.com/od/job-title-samples/", 0);
                        }

                        if (nPoshref1 != string::npos)
                        {
                            size_t nPoshref2 = word.find("\"", nPoshref1 + 1);

                            if (nPoshref2 != string::npos)
                            {
                                cout << word.substr(nPoshref1, nPoshref2 - nPoshref1) << endl;
                                ParseNextFile(word.substr(nPoshref1, nPoshref2 - nPoshref1), currentFile, outputFile);
                            }
                        }

                        if (word.find("<", 0) == string::npos)
                        {
                            if ( find(skills->begin(), skills->end(), word)==skills->end() )
                            {
                                skills->push_back(word);

                                cout << word << endl;

                                /*std::wofstream wof("./" + outputFile, ios::out | ios::app);
                                wof.imbue(std::locale("en_US.UTF-8"));
                                wof << word.c_str() << endl;
                                wof.close();*/
                            }
                        }
                    }

                    nPos = dataString.find("<li>", nPos + 1);
                }

            }
            else
            {
                cout << currentFile << " does not exist" << "\n";
            }
        }
    }
    else
    {
        return;
    }
}

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