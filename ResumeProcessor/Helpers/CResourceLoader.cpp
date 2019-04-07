//
// File CResourceLoader.cpp
//
//

#include <iostream>
#include <fstream>
#include <sstream>
#include "CResourceLoader.h"
#include "../../Common/CTextUtils.h"
#include "../../Common/CDebugUtils.h"
#include "../../Common/CFileUtils.h"
#include "../../Easylogging/easylogging++.h"
#include "../../Stacktrace/Stacktrace.h"
/*
 * Summary_ResumeProcessor_Helpers::CResourceLoader::LoadIntoHashSet
 *
 * Description
 * Loads text lines from input text file into unordered_set
 *
 * Inputs
 * string resourceName - text file name
 * char delimiter - lines delimiter character
 *
 * Returns
 * unordered_set<string> filled with the textual data
 */
unordered_set<string> Summary_ResumeProcessor_Helpers::CResourceLoader::LoadIntoHashSet(string resourceName,
                                                                                        char delimiter)
{
    CDebugUtils::DebugCout("CResourceLoader", "LoadIntoHashSet", "");
    vector<string> loadedData = Load(resourceName, delimiter);
    unordered_set<string>loadedSet(loadedData.begin(), loadedData.end());
    return loadedSet;
}

/*
 * Summary_ResumeProcessor_Helpers::CResourceLoader::Load
 *
 * Description
 * Loads text lines from input text file into vector
 *
 * Inputs
 * string resourceName - text file name
 * char delimiter - lines delimiter character
 *
 * Returns
 * vector<string> filled with the textual data
 */
vector<string> Summary_ResumeProcessor_Helpers::CResourceLoader::Load(string resourceName, char delimiter)
{
    CDebugUtils::DebugCout("CResourceLoader", "Load", "");
    string dataString;
    vector<string> returnVector;

    if (resourceName.find("ResumeProcessor") == string::npos)
    {
        resourceName = "./ResumeProcessor/Data/" + resourceName;
    }

    dataString = LoadExternalToString(resourceName);

    returnVector = CTextUtils::Split(dataString, delimiter);

    return returnVector;
}

/*
 * Summary_ResumeProcessor_Helpers::CResourceLoader::LoadExternal
 *
 * Description
 * Loads text lines from input text file into vector
 *
 * Inputs
 * string resourceName - text file name
 * char delimiter - lines delimiter character
 *
 * Returns
 * vector<string> filled with the textual data
 */
vector<string> Summary_ResumeProcessor_Helpers::CResourceLoader::LoadExternal(string resourceName, char delimiter)
{
    CDebugUtils::DebugCout("CResourceLoader", "LoadExternal", "");

    string dataString = LoadExternalToString(resourceName);

    vector<string> returnVector = CTextUtils::Split(dataString, delimiter);

    return returnVector;
}

/*
 * Summary_ResumeProcessor_Helpers::CResourceLoader::LoadExternalToString
 *
 * Description
 * Loads text lines from input text file into vector
 *
 * Inputs
 * string resourceName - text file name
 *
 * Returns
 * vector<string> filled with the textual data
 */
string Summary_ResumeProcessor_Helpers::CResourceLoader::LoadExternalToString(string resourceName)
{
    CDebugUtils::DebugCout("CResourceLoader", "LoadExternalToString", "");
    ifstream inputFile;
    stringstream buffer;
    string dataString;
    vector<string> returnVector;

    try
    {
        //ifstream myfile (location);
        inputFile.open(resourceName, ifstream::in);

        if (inputFile.is_open())
        {
            buffer << inputFile.rdbuf();
            inputFile.close();
        }
    }
    catch (exception &e)
    {
        LOG(ERROR) << CFileUtils::GetSourcePath() << ":" << resourceName << " reading error " << e.what();
        PrintStacktrace();
        //inputFile.close();
    }

    dataString = buffer.str().c_str();
    buffer.clear();

    return dataString;
}

/*
 * Summary_ResumeProcessor_Helpers::CResourceLoader::LoadIntoLowerCaseHashSet
 *
 * Description
 * Loads text lines from input text file into unordered_set with the data in lower case
 *
 * Inputs
 * string resourceName - text file name
 * char delimiter - lines delimiter character
 *
 * Returns
 * SLowerCaseUnorderedSet filled with the textual data in lower case
 */
SLowerCaseUnorderedSet Summary_ResumeProcessor_Helpers::CResourceLoader::LoadIntoLowerCaseHashSet(string resourceName,
                                                                                                  char delimiter)
{
    CDebugUtils::DebugCout("CResourceLoader", "LoadIntoLowerCaseHashSet", "");
    vector<CLowerCaseString> loadedData = LoadLowerCase(resourceName, delimiter);
    SLowerCaseUnorderedSet loadedSet(loadedData.begin(), loadedData.end());
    return loadedSet;
}

/*
 * Summary_ResumeProcessor_Helpers::CResourceLoader::LoadLowerCase
 *
 * Description
 * Loads text lines from input text file into vector
 *
 * Inputs
 * string resourceName - text file name
 * char delimiter - lines delimiter character
 *
 * Returns
 * vector<CLowerCaseString> filled with the textual data in lower case
 */
vector<CLowerCaseString> Summary_ResumeProcessor_Helpers::CResourceLoader::LoadLowerCase(string resourceName,
                                                                                         char delimiter)
{
    CDebugUtils::DebugCout("CResourceLoader", "Load", "");
    ifstream inputFile;
    stringstream buffer;
    CLowerCaseString dataString("");
    vector<CLowerCaseString> returnVector;

    if (resourceName.find("ResumeProcessor") == string::npos)
    {
        resourceName = "./ResumeProcessor/Data/" + resourceName;
    }

    try
    {
        //ifstream myfile (location);
        inputFile.open(resourceName, ifstream::in);

        if (inputFile.is_open())
        {
            buffer << inputFile.rdbuf();
            inputFile.close();
        }
    }
    catch (exception &e)
    {
        LOG(ERROR) << CFileUtils::GetSourcePath() << ":" << resourceName << " reading error " << e.what();
        PrintStacktrace();
        //inputFile.close();
    }

    dataString = CLowerCaseString(buffer.str().c_str());
    buffer.clear();

    returnVector = CTextUtils::Split(dataString, delimiter);
    dataString.clear();

    return returnVector;
}
