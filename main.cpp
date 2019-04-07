#include <iostream>
#include <vector>
#include <signal.h>
#include <thread>
#include <string>
#include <future>
#include "ResumeProcessor/Helpers/CDateHelper.h"
#include "ResumeProcessor/Helpers/CResourceLoader.h"
#include "ResumeProcessor/CResumeProcessor.h"
#include "ResumeProcessor/Parsers/DictionaryLoaderSingleton.h"
#include "Common/CTextUtils.h"
#include "Common/CFileUtils.h"
#include "Common/CDebugUtils.h"
#include <boost/exception/all.hpp>
#include "Easylogging/easylogging++.h"
#include "Stacktrace/Stacktrace.h"
#include <boost/lexical_cast.hpp>
#include <boost/network.hpp>
#include <boost/function_output_iterator.hpp>

using namespace boost::filesystem;
using namespace Summary_Common;
using namespace Summary_Json;
using namespace std;
using namespace Summary_ResumeProcessor_Helpers;
using namespace Summary_ResumeProcessor_Parsers;
using namespace Summary_ResumeProcessor;
using namespace boost;

void SegfaultSigaction(int signal, siginfo_t *si, void *arg);
void RunCommand(string fileName);
void ExecuteCommand(string command);
bool StringAndLengthCompare(const string &s1, const string &s2);
bool ResourceExists(const string &parentPath, const string &resourcePath);
int TrainSkills(const string &skillsFileName, const string &parentPath);

INITIALIZE_EASYLOGGINGPP

class A
{
protected:
    static int x;
};

class B: public A
{

};

class C: public A
{

};

class D: public B, C
{

};

int main(int argc, char* argv[])
{
    struct sigaction sa;
    //DictionaryLoaderSingleton::GetInstance();//.GetFirstNames().size() << endl;
    //cout << DictionaryLoaderSingleton::GetInstance().GetMap().size() << endl;
    D d;
    d.x = 0;

    //Initializes structure for catching segmentation fault errors
    memset(&sa, 0, sizeof(sa));
    sigemptyset(&sa.sa_mask);
    sa.sa_sigaction = SegfaultSigaction;
    sa.sa_flags   = SA_SIGINFO;

    string currentPath = "";
    string fileName = "";
    string outputFileName = "";
    string createdFolder = "";
    string fileExtension = "";
    string extractImagesPath = "";
    string skillsFileName = "";
    bool fileFormatSupported = false;
    bool isTextFile = false;
    bool mainProcessError = false;
    boost::filesystem::path runPath(argv[0]);
    string parentPath = runPath.parent_path().string();

    if (!CTextUtils::EndsWith(parentPath, "/"))
    {
        parentPath = parentPath + "/";
    }
    //Saves program full path
    CFileUtils::SetRunPath(parentPath);
    //Saves input file full path
    CFileUtils::SetSourcePath(argc > 1 ? (string)argv[1] : "");

    //Inits all dictionaries
    DictionaryLoaderSingleton::GetInstance();

    //Inits default configuration for logger
    el::Configurations defaultConf;

    defaultConf.setGlobally(el::ConfigurationType::ToStandardOutput, "false");
    defaultConf.setGlobally(el::ConfigurationType::Filename, parentPath + "logs/CVSummaryExtractor.log");

    defaultConf.set(el::Level::Global,
                    el::ConfigurationType::ToStandardOutput, "false");
    defaultConf.set(el::Level::Global,
                    el::ConfigurationType::Filename, parentPath + "logs/CVSummaryExtractor.log");
    el::Loggers::reconfigureLogger("default", defaultConf);

    START_EASYLOGGINGPP(argc, argv);
    LOG(INFO) << "Started " << CFileUtils::GetSourcePath();

    //Redirects sigmentation fault errors to function SegfaultSigaction
    sigaction(SIGSEGV, &sa, NULL);

    CDebugUtils::DebugCout("main", "main", "testing");

    //Validates if everything needed is placed where tool itself is placed
    if (!CFileUtils::PathExists(parentPath + "ResumeProcessor/Data/"))
    {
        LOG(INFO) << "User error: No existing path " + parentPath + "ResumeProcessor/Data/";
        cout << "{\"error\":\"" << "No existing path " + parentPath + "ResumeProcessor/Data/" << "\"}\n";
        return 1;
    }
    if (!ResourceExists(parentPath, "ResumeProcessor/Data/Countries.txt") ||
        !ResourceExists(parentPath, "ResumeProcessor/Data/FirstName.txt") ||
        !ResourceExists(parentPath, "ResumeProcessor/Data/JobTitles.txt") ||
        !ResourceExists(parentPath,  "ResumeProcessor/Data/Skills.txt") ||
        !ResourceExists(parentPath, "ResumeProcessor/Data/StopWords.txt") ||
        !ResourceExists(parentPath, "ResumeProcessor/Data/Religions.txt") ||
        !ResourceExists(parentPath, "ResumeProcessor/Data/NonNames.txt")
            )
    {
        return 2;
    }

    if (!ResourceExists(parentPath, "ResumeProcessor/Data/LastName.txt"))
    {
        return 2;
    }

    if (argc < 2)
    {
        LOG(INFO) << "User error: Not enough arguments";
        cout << "{\"help\":\"Usage: CVSummaryExtractor FILENAME [-extract_image_path PATH]\"}\n";
        return 3;
    }
    fileName = argv[1];

    //Validates command line arguments
    if (CTextUtils::ToLower(fileName).find("-extract_image_path") != string::npos)
    {
        LOG(INFO) << "User error: Please provide filename as first argument";
        cout << "{\"error\":\"Please provide filename as first argument]\"}\n";
        return 4;
    }

    //Validates command line arguments
    if (CTextUtils::ToLower(fileName).find("-train_skills") != string::npos)
    {
        if (argc > 1)
        {
            skillsFileName = argv[2];

            //Trains skills dictionary and returns the result
            return TrainSkills(skillsFileName, parentPath);
        }
        else
        {
            LOG(INFO) << "User error: skills file name not provided";
            cout << "{\"error\":\"" << "Skills file name not provided" << "\"}\n";
            return 9;
        }
    }

    if (!CFileUtils::FileExists(fileName))
    {
        LOG(INFO) << "User error: File " << fileName << " does not exist";
        cout << "{\"error\":\"" << "File " << fileName << " does not exist" << "\"}\n";
        return 5;
    }

    string supportedFormats = "txt,pdf,doc,docx,html,htm,xml,odt,rtf";

    for (string supportedFormat: CTextUtils::Split(supportedFormats, ','))
    {
        if (CTextUtils::EndsWith(CTextUtils::ToLower(fileName), supportedFormat))
        {
            fileFormatSupported = true;

            if (supportedFormat.compare("txt") == 0)
            {
                isTextFile = true;
            }

            break;
        }
    }

    //If not text file then need to extract temporary results to temp path and location where
    //this subfolder is created must be accessible for that
    if (!isTextFile && !CFileUtils::IsPathWritable(parentPath))
    {
        LOG(INFO) << "Path is not writable";
        cout << "{\"error\":\"" << "Path " << parentPath << " is not writable" << "\"}\n";
        return 6;
    }

    if (!fileFormatSupported)
    {
        LOG(INFO) << "User error: no support for file extension";
        cout << "{\"error\":\"" << "Supported file endings are " << supportedFormats << "\"}\n";
    }
    else
    {
        //This time all facial image candidates should be copied to specific location
        if (argc > 2 && (strcmp(argv[2], "-extract_image_path") == 0))
        {
            char * extractPath = argv[3];

            extractImagesPath = extractPath;

            if (!CFileUtils::PathExists(extractImagesPath))
            {
                LOG(INFO) << "Ended with extract images path does not exist";
                cout << "{\"error\":\"" << "Provided path does not exist" << "\"}\n";
                return 7;
            }
        }

        //Just testing this tool
        if ((argc > 2 && (strcmp(argv[2], "-parallel_test") == 0)) ||
            (argc > 4 && (strcmp(argv[4], "-parallel_test") == 0)))
        {
            cout << "Testing CVSummaryExtractor in parellel" << endl;

            for (int i = 0; i < 10; ++i)
            {
                cout << "-------Create thread------- " << i << endl;

                RunCommand(fileName);
            }

            CDebugUtils::Sleep(1000);
            return 0;
        }

        JsonOutputFormatter *outputFormatter = NULL;

        try
        {
            CFileUtils::SetDataFileName(CFileUtils::ExtractFileName(fileName));
            //Given was not a text file, transform it and extract results from transformation
            if (!isTextFile)
            {
                currentPath = parentPath;

                fileExtension = CFileUtils::GetFileExtension(fileName);

                if (fileExtension.length() > 0)
                {
                    outputFileName = fileName.substr(0, fileName.size() - fileExtension.size()) + ".txt";
                }
                else
                {
                    outputFileName += ".txt";
                }

                //Temporary file name
                outputFileName = outputFileName.substr(outputFileName.find_last_of("/\\") + 1);

                //Create new temp folder for storing temp text file
                createdFolder = CFileUtils::CreateHashedFolder(currentPath);
                //tesseractFolder = CFileUtils::CreateHashedFolder(createdFolder);
                outputFileName = createdFolder + outputFileName;

                //Uses Apache Tika to convert input to .txt
                CFileUtils::ExecuteNonStopConvert(fileName, outputFileName);

                //If needed to copy facial image candidates to the output folder
                if (extractImagesPath.length() > 0)
                {
                    CFileUtils::ExtractFacialImages(createdFolder, fileName, extractImagesPath);
                }

                fileName = outputFileName;
            }

            outputFormatter = new JsonOutputFormatter();
            CResumeProcessor processor(outputFormatter);
            processor.Process(fileName);
        }
        catch (const std::exception &e)
        {
            //Log error description and stack trace into log, output error information to standard output
            cout << "{\"error\":\"" << boost::diagnostic_information(e) << "\"}\n";
            LOG(ERROR) << CFileUtils::GetSourcePath() << ": " << boost::diagnostic_information(e);

            PrintStacktrace();

            mainProcessError = true;
        }

        if (outputFormatter != NULL)
        {
            delete outputFormatter;
        }

        if (!isTextFile)
        {
            CFileUtils::RemovePath(createdFolder);
        }

        LOG(INFO) << "Ended " << CFileUtils::GetSourcePath();
    }

    return mainProcessError ? 100: 0;
}

/*
 * SegfaultSigaction
 *
 * Description
 * Method gets called if program receives segmentation fault. Logs error about the case
 * and prints stack trace to the log. Finally exits the program.
 *
 * Inputs
 * int signal - signal number the method has catched
 * siginfo_t *si - signal information structure with address information etc.
 * void *arg - additional arguments
 *
 * Returns
 * exits program and returns 11 as exit code of the program.
 *
 */
void SegfaultSigaction(int signal, siginfo_t *si, void *arg)
{
    try
    {
        //Log error description and stack trace into log, output error information to standard output
        cout << "{\"error\":\"" << " segfault at address " << si->si_addr << "\"}\n";
        LOG(ERROR) << CFileUtils::GetSourcePath() << " segfault at address " << si->si_addr;

        PrintStacktrace();
    }
    catch (std::exception e)
    {

    }

    exit(11);
}

/*
 * RunCommand
 *
 * Description
 * Runs CVSummaryExtractor with given file name
 *
 * Inputs
 * string fileName - file name given for the tool
 *
 * Returns
 *
 */
void RunCommand(string fileName)
{
    string command = CFileUtils::GetRunPath() + "CVSummaryExtractor ";
    command += "\"" + fileName + "\"";

    future<void> result( async(std::launch::async, ExecuteCommand, command));
}

/*
 * ExecuteCommand
 *
 * Description
 * Executes given command on the system
 *
 * Inputs
 * string command - given command to be run externally on system
 *
 * Returns
 *
 */
void ExecuteCommand(string command)
{
    CFileUtils::ExecuteCommand(command);
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

/*
 * ResourceExists
 *
 * Description
 * Validates if resource file exists and outputs error message if not
 *
 * Inputs
 * const string &parentPath - parent path
 * const string &resourcePath - resource path
 *
 * Returns
 * true if first file exists on path
 */
bool ResourceExists(const string &parentPath, const string &resourcePath)
{
    string totalPath = parentPath + resourcePath;

    if (!CFileUtils::FileExists(totalPath))
    {
        LOG(INFO) << "User error: No file " + totalPath;
        cout << "{\"error\":\"" << "No file " + totalPath << "\"}\n";
        return false;
    }

    return true;
}

/*
 * TrainSkills
 *
 * Description
 * Adds new skills to the skills dictionary from the file given as input
 *
 * Inputs
 * const string &skillsFileName - parent path
 * const string &parentPath - parent path
 *
 * Returns
 * 0 on success, > 0 on error
 */
int TrainSkills(const string &skillsFileName, const string &parentPath)
{
    CResourceLoader resourceLoader;
    vector<string> skills;
    vector<string> newSkills;

    string existingSkillsFileName;

    if (!CFileUtils::FileExists(skillsFileName))
    {
        LOG(INFO) << "User error: skills file does not exist";
        cout << "{\"error\":\"" << "Provided skills file does not exist" << "\"}\n";
        return 8;
    }

    existingSkillsFileName = parentPath + "ResumeProcessor/Data/Skills.txt";
    skills = resourceLoader.Load(
            existingSkillsFileName, '\n');
    newSkills = resourceLoader.LoadExternal(skillsFileName, '\n');

    if (newSkills.size() < 1)
    {
        LOG(INFO) << "User error: skills file does not contain any skills";
        cout << "{\"error\":\"" << "Provided skills file does not contain skills" << "\"}\n";
        return 10;
    }

    for (string skill: newSkills)
    {
        if ( skill.length() > 0 && find(skills.begin(), skills.end(), skill)==skills.end() )
        {
            skills.push_back(skill);
        }
    }

    sort(skills.begin(), skills.end(), StringAndLengthCompare);

    string fileContents = "";

    for ( int i = 0; i < skills.size(); i++ )
    {
        string sentence = skills.at(i);

        fileContents += sentence;
        fileContents += "\n";
    }

    ofstream skillsOutputStream;
    try
    {
        skillsOutputStream.open(existingSkillsFileName, ios::out);

        if (skillsOutputStream.is_open())
        {
            skillsOutputStream << fileContents.c_str();
            skillsOutputStream.close();
        }
        else
        {
            LOG(INFO) << "Could not open skills dictionary " << existingSkillsFileName << " for writing";
            cout << "{\"error\":\"" << "Could not open skills dictionary "
            << existingSkillsFileName << " for writing" << "\"}\n";
            return 10;
        }
    }
    catch (const std::exception &e)
    {
        //Log error description and stack trace into log, output error information to standard output
        cout << "{\"error\":\"Error during writing to skills dictionary " << existingSkillsFileName
        << ":" << boost::diagnostic_information(e) << "\"}\n";
        LOG(ERROR) << "Error during writing to skills dictionary " << existingSkillsFileName
        << ": " << boost::diagnostic_information(e);

        PrintStacktrace();

        return 11;
    }

    return 0;
}

