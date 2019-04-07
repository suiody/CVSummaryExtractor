//
// File CFileUtils.cpp
//
//
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file Boost/LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <unistd.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include "CFileUtils.h"
#include "CTextUtils.h"
#include "../Easylogging/easylogging++.h"
#include "../Stacktrace/Stacktrace.h"
#include <boost/algorithm/string/erase.hpp>

string Summary_Common::CFileUtils::runPath = "";
string Summary_Common::CFileUtils::sourcePath = "";
string Summary_Common::CFileUtils::dataFileName = "";

/*
 * Summary_Common::CFileUtils::GetCurrentPath
 *
 * Description
 * Gets full representation of current path
 *
 * Inputs
 *
 * Returns
 * Current path on the disk
 */
string Summary_Common::CFileUtils::GetCurrentPath()
{
    path fullPath( current_path() );
    return fullPath.string();
}

/*
 * Summary_Common::CFileUtils::PathExists
 *
 * Description
 * Validates if path exists on the disk
 *
 * Inputs
 * string strPath - input path
 *
 * Returns
 * True if path exists
 */
bool Summary_Common::CFileUtils::PathExists(string strPath)
{
    try
    {
        if ( access( strPath.c_str(), 0 ) == 0 )
        {
            struct stat status;
            stat( strPath.c_str(), &status );

            if ( status.st_mode & S_IFDIR )
            {
                //cout << "The directory exists." << endl;
                return true;
            }
            else
            {
                //cout << "The path you entered is a file." << endl;
                return false;
            }
        }
    }
    catch (const std::exception &e)
    {

    }

    return false;
}

/*
 * Summary_Common::CFileUtils::FileExists
 *
 * Description
 * Validates if file exists on the disk
 *
 * Inputs
 * string pathName - input path
 *
 * Returns
 * True if file exists
 */
bool Summary_Common::CFileUtils::FileExists(string pathName)
{
    try
    {
        if ( access( pathName.c_str(), 0 ) == 0 )
        {
            struct stat status;
            stat( pathName.c_str(), &status );

            if ( !(status.st_mode & S_IFDIR) )
            {
                //cout << "The file exists." << endl;
                return true;
            }
            else
            {
                //cout << "The path you entered is a directory." << endl;
                return false;
            }
        }
    }
    catch (const std::exception &e)
    {

    }

    return false;
}

/*
 * Summary_Common::CFileUtils::CreateHashedFolder
 *
 * Description
 * Creates new subfolder with random characters on it
 *
 * Inputs
 * string pathName - path where subfolder will be created
 *
 * Returns
 * Full path of the newly created folder
 */
string Summary_Common::CFileUtils::CreateHashedFolder(string pathName)
{
    string currentPath;
    string command;
    if (!PathExists(pathName))
    {
        currentPath = "./";
    }
    size_t hashCode = hash<string>()(RandomString(10));

    if (CTextUtils::EndsWith(pathName, "/"))
    {
        currentPath = pathName + to_string(hashCode) + "/";
    }
    else
    {
        currentPath = pathName + "/" + to_string(hashCode) + "/";
    }

    command = "mkdir " + currentPath;

    if (!CFileUtils::PathExists(currentPath))
    {
        system(command.c_str());
    }

    return currentPath;
}

/*
 * Summary_Common::CFileUtils::RandomString
 *
 * Description
 * Gets random string from characters 0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz using given length
 *
 * Inputs
 * int len - string length
 *
 * Returns
 * Random string with given length
 */
string Summary_Common::CFileUtils::RandomString(int len)
{
    unsigned long seed = CFileUtils::Mix(clock(), time(NULL), getpid());
    srand(seed);
    string str = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    int pos;
    while(str.size() != len)
    {
        pos = ((rand() % (str.size() - 1)));
        str.erase (pos, 1);
    }
    return str;
}

/*
 * Summary_Common::CFileUtils::ExecuteCommand
 *
 * Description
 * Executes command on system without any visible output to standard output and error
 *
 * Inputs
 * string command - command to be executed
 *
 * Returns
 *
 */
void Summary_Common::CFileUtils::ExecuteCommand(string command)
{
    command = command + " 2> /dev/null";
    //cout << command << "\n";
    streambuf* cout_sbuf = cout.rdbuf(); // save original sbuf
    ofstream   fout("/dev/null");
    cout.rdbuf(fout.rdbuf()); // redirect 'cout' to a 'fout'
    streambuf* cerr_sbuf = cerr.rdbuf(); // save original sbuf
    ofstream   eout("/dev/null");
    cerr.rdbuf(eout.rdbuf()); // redirect 'cout' to a 'fout'
    system(command.c_str());

    cout.rdbuf(cout_sbuf); // restore the original stream buffer
    cerr.rdbuf(cerr_sbuf);
    //cout << ":" << command << "\n";
}

/*
 * Summary_Common::CFileUtils::RemovePath
 *
 * Description
 * Removes path from disk
 *
 * Inputs
 * string pathName - path name
 *
 * Returns
 *
 */
void Summary_Common::CFileUtils::RemovePath(string pathName)
{
    if (!CFileUtils::PathExists(pathName))
    {
        return;
    }

    typedef vector<path> vec;             // store paths,
    vec v;                                // so we can sort them later
    string command = "";

    copy(directory_iterator(pathName), directory_iterator(), back_inserter(v));

    for (vec::const_iterator it (v.begin()); it != v.end(); ++it)
    {
        string fileName = (*it).string();

        if (CFileUtils::PathExists(fileName))
        {
            CFileUtils::RemovePath(fileName);
        }
        else
        {
            remove(fileName.c_str());
        }
    }

    if (!CTextUtils::EndsWith(pathName, "\""))
    {
        pathName = "\"" + pathName + "\"";
    }

    //cout << "path: " << pathName << "\n";

    command = "rmdir " + pathName;

    ExecuteCommand(command);
}

/*
 * Summary_Common::CFileUtils::ExecuteTransform
 *
 * Description
 * Gets all files from given path and executes given transform method on files
 *
 * Inputs
 * string pathName - input path
 * string outputPath - output path for the given tranformation
 * function<void(string, string)> transformFunction - tranformation method to be executed
 *
 * Returns
 *
 */
void Summary_Common::CFileUtils::ExecuteTransform(string pathName, string outputPath, function<void(string, string)> transformFunction)
{
    typedef vector<path> vec;             // store paths,
    vec v;                                // so we can sort them later
    string command = "";

    copy(directory_iterator(pathName), directory_iterator(), back_inserter(v));

    sort(v.begin(), v.end(), PathCompare);  // sort, since directory iteration - should implement numeric/string comparison
                                            // is not ordered on some file systems

    for (vec::const_iterator it (v.begin()); it != v.end(); ++it)
    {
        string jpgFileName = (*it).string();
        //cout << jpgFileName << "\n";
        /*boost::filesystem::path p = jpgFileName;

        std::time_t t = boost::filesystem::last_write_time( p ) ;
        std::cout << "On " << std::time( &t ) << " the file " << jpgFileName
        << " was modified the last time!\n";*/

        transformFunction(jpgFileName, outputPath);
    }

    v.clear();
}

/*
 * Summary_Common::CFileUtils::ExecuteConvert
 *
 * Description
 * Converts input file into .txt file as output using Apache Tika
 *
 * Inputs
 * string inputFile - input file
 * string outputFile - output file
 *
 * Returns
 *
 */
void Summary_Common::CFileUtils::ExecuteConvert(string inputFile, string outputFile)
{
    string command = "java -jar \"";
    command += CFileUtils::GetRunPath() + "tika-app-1.9.jar\" --encoding=UTF-8 --text ";
    command += "\"" + inputFile + "\"" + " > " + "\"" + outputFile + "\"";

    CFileUtils::ExecuteCommand(command);

    LOG(INFO) << command;
}

/*
 * Summary_Common::CFileUtils::ExecuteNonStopConvert
 *
 * Description
 * Converts input file into .txt file as output using Apache Tika. Differs from ExecuteConvert
 * by not reloading JVM on execution but instead using Nailgun to keep JVM running.
 *
 * Inputs
 * string inputFile - input file
 * string outputFile - output file
 *
 * Returns
 *
 */
void Summary_Common::CFileUtils::ExecuteNonStopConvert(string inputFile, string outputFile)
{
    string command = "\"";
    command += CFileUtils::GetRunPath() + "ng\" org.apache.tika.cli.TikaCLI --encoding=UTF-8 --text ";
    command += "\"" + inputFile + "\"" + " > " + "\"" + outputFile + "\"";

    CFileUtils::ExecuteCommand(command);

    LOG(INFO) << command;

    //Try the extraction once more if ng fails to connect to server etc.
    if (GetFileSize(outputFile) < 1)
    {
        ExecuteConvert(inputFile, outputFile);
    }
}

/*
 * Summary_Common::CFileUtils::ExecuteExtract
 *
 * Description
 * Extracts all images from file and places into output folder using Apache Tika
 *
 * Inputs
 * string inputFile - input file (.pdf, .doc etc.)
 * string extractFolder - output folder for images
 *
 * Returns
 *
 */
void Summary_Common::CFileUtils::ExecuteExtract(string inputFile, string extractFolder)
{
    string command = "java -jar \"";
    command += CFileUtils::GetRunPath() + "tika-app-1.9.jar\" --extract --extract-dir=";
    command += "\"" + extractFolder + "\" ";
    command += "\"" + inputFile + "\" > /dev/null";

    CFileUtils::ExecuteCommand(command);

    LOG(INFO) << command;
}

/*
 * Summary_Common::CFileUtils::TransformTesseract
 *
 * Description
 * Transform function. Reads input image file, extracts text from it and copies to output path
 * if the image is facial image candidate
 *
 * Inputs
 * string inputFile - image file
 * string outputPath - output path
 *
 * Returns
 *
 */
void Summary_Common::CFileUtils::TransformTesseract(string inputFile, string outputPath)
{
    string command = "";

    if (!CTextUtils::EndsWith(inputFile, ".txt") && !CTextUtils::EndsWith(inputFile, ".TXT"))
    {
        //cout << "   " << *it << '\n';
        string txtFileName = inputFile.substr(0, inputFile.length() - 4);// + "txt";

        command = "tesseract ";
        command += "\"" + inputFile + "\" ";
        command += "\"" + txtFileName + "\" ";
        //cout << command << "\n";
        CFileUtils::ExecuteCommand(command);

        if (outputPath.length() > 0 && CFileUtils::PathExists(outputPath))
        {
            string imageCandidate = CFileUtils::FaceImageCandidate(inputFile);
            if (imageCandidate.length() > 0)
            {
                //cout << imageCandidate << "\n";
                command = "cp ";
                command += "\"" + imageCandidate + "\"" + " " + "\"" + outputPath + "\"";
                CFileUtils::ExecuteCommand(command);
                //cout << command << "\n";

                LOG(INFO) << command;
            }
        }
    }
}

/*
 * Summary_Common::CFileUtils::TransformCat
 *
 * Description
 * Transform function. Concatenates input file into output file by appending text
 *
 * Inputs
 * string inputFile - input file
 * string outputFile - output file to append text from input file
 *
 * Returns
 *
 */
void Summary_Common::CFileUtils::TransformCat(string inputFile, string outputFile)
{
    string command = "";

    if (CTextUtils::EndsWith(inputFile, "txt") || CTextUtils::EndsWith(inputFile, "TXT"))
    {
        command = "cat ";
        command += "\"" + inputFile + "\" >> ";
        command += "\"" + outputFile + "\" ";
        //cout << command << "\n";
        CFileUtils::ExecuteCommand(command);

        LOG(INFO) << command;
    }
}

/*
 * Summary_Common::CFileUtils::GetFileExtension
 *
 * Description
 * Reads file extension
 *
 * Inputs
 * string fileName - file name
 *
 * Returns
 * File extension with dot in first place
 */
string Summary_Common::CFileUtils::GetFileExtension(string fileName)
{
    path filePath = fileName;
    return filePath.extension().string();
}

/*
 * Summary_Common::CFileUtils::GetFileSize
 *
 * Description
 * Gets file size
 *
 * Inputs
 * string fileName - file name
 *
 * Returns
 * File length in bytes
 */
long CFileUtils::GetFileSize(string fileName)
{
    struct stat stat_buf;
    int rc = stat(fileName.c_str(), &stat_buf);

    return rc == 0 ? stat_buf.st_size : -1;
}

/*
 * Summary_Common::CFileUtils::PathCompare
 *
 * Description
 * Compares two paths and returns true if first path in string format is less than second path in string format
 *
 * Inputs
 * const path &s1 - path to compare
 * const path &s2 - path to compare
 *
 * Returns
 * True if first path in string format is less than second path in string format
 */
bool Summary_Common::CFileUtils::PathCompare(const path &s1, const path &s2)
{
    string firstPathString = s1.string();
    string secondPathString = s2.string();

    if (firstPathString.find("_image") != string::npos && secondPathString.find("_image") != string::npos)
    {
        return NaturalCompare(firstPathString.substr(firstPathString.find("_image")), secondPathString.substr(secondPathString.find("_image"))) < 0;
    }

    return NaturalCompare(s1.string(), s2.string()) < 0;

    //return t1 < t2;
}

/*
 * Summary_Common::CFileUtils::NaturalCompare
 *
 * Description
 * Compares 2 strings and returns < 0, 0 or > 0 if first is less than second, those are equal or second is less than first
 *
 * Inputs
 * const string &a - first string
 * const string &b - second string
 *
 * Returns
 *  < 0, 0 or > 0 if first string is less than second string, those are equal or second is less than first
 */
int Summary_Common::CFileUtils::NaturalCompare(const string &a, const string &b)
{
    // Remembers which input had the first leading zero
    int lt = 0;
    // Temporaries.  Used for converting natural equivalents ' ' and '0' to '~'
    char ta, tb;
    // Remembers if the previous characters composed an integer value
    bool num = false;
    // Iterate over the strings stopping at the end of the shorter one
    for( unsigned int i=0; i<std::min( a.size(), b.size() ); i++ )
    {
        // If there is a data missmatch.  Look for special circumstances
        if( a[i] != b[i] )
        {
            // Use the '~' replacement unless the input is currently numeric
            ta = a[i] == ' ' || ( a[i] == '0' && !num ) ? '~' : a[i];
            tb = b[i] == ' ' || ( b[i] == '0' && !num ) ? '~' : b[i];
            // Because the strings don't match here, the can't be numeric
            num = false;
            // There is a natural missmatch with no special circumstances, so terminate
            if( ta != tb )
                return ta - tb;
                // The strings match, so check for leading zeros
            else if( lt == 0 )
                lt = a[i] - b[i];
        }
            // Still equivalent, so check for numeric state
        else
            num = ( num && a[i] == '0' ) || (a[i] > '0' && a[i] <= '9');
    }
    // The strings are naturally equivalent, but the longer is naturally greater
    if( a.size() != b.size() )
        return a.size() - b.size();
    // The strings are naturally equivalent, so we return the one with the first leading '0'
    return lt;
}

/*
 * Summary_Common::CFileUtils::ListPath
 *
 * Description
 * Not used at the moment
 *
 * Inputs
 *
 * Returns
 *
 */
bool Summary_Common::CFileUtils::ListPath(string pathName, int level)
{
    if (!PathExists(pathName))
    {
        return false;
    }

    typedef vector<path> vec;             // store paths,
    vec v;                                // so we can sort them later
    string command = "";
    bool pathListed = false;
    bool filesExisted = false;
    bool subsExisted = false;

    copy(directory_iterator(pathName), directory_iterator(), back_inserter(v));

    for (vec::const_iterator it (v.begin()); it != v.end(); ++it)
    {
        string fileName = (*it).string();
        if (level == 3 && CFileUtils::FileExists(fileName))
        {
            //cout << level << "   " << *it << '\n';
        }

        subsExisted = true;
        if (CFileUtils::PathExists(fileName))
        {
            if (CFileUtils::ListPath(fileName, level + 1))
            {
                if (level == 2)
                {
                    //cout << "minor" << "\n";
                    return true;
                }
                if (level == 1)
                {
                    //cout << "minor.second" << "\n";
                    pathListed = true;
                }

            }
        }
        else
        {
            if (level == 3 && CFileUtils::FileExists(fileName))
            {
                //cout << "true" << "\n";
                string imageCandidate = CFileUtils::FaceImageCandidate(fileName);
                if (imageCandidate.length() > 0)
                {
                    //cout << imageCandidate << "\n";
                }

                filesExisted = true;
                //return true;
            }
        }
        if (level == 1 && !pathListed)
        {
            //cout << "false.second" << "\n";
        }
        if (level == 2)
        {
            //cout << "false" << "\n";
            if (CFileUtils::FileExists(fileName))
            {
                //remove((*it).string().c_str());
            }
            else if (CFileUtils::PathExists(fileName))
            {
                //CFileUtils::RemovePath((*it).string());
            }
        }

        //remove(fileName.c_str());
    }

    if (level == 2 && !subsExisted)
    {
        //cout << "remove " << pathName << "\n";
        //CFileUtils::RemovePath(pathName);
    }

    if (level == 3 && filesExisted)
    {
        return true;
    }

    if (!CTextUtils::EndsWith(pathName, "\""))
    {
        pathName = "\"" + pathName + "\"";
    }

    if (!(level == 3))
    {
        return false;
    }

    return true;
}

/*
 * Summary_Common::CFileUtils::FaceImageCandidate
 *
 * Description
 * Gets image that is possible facial image candidate by seeing if ocr'ed text is not too long
 *
 * Inputs
 * string fileName - image file name. Must be .png, .jpg, .jpeg, .bmp, .tif or .gif
 *
 * Returns
 * The same image file name if it can be face image
 */
string Summary_Common::CFileUtils::FaceImageCandidate(string fileName)
{
    path filePath = fileName;
    string textFileName = "";
    string fileExtension = filePath.extension().string();

    if (fileExtension.length() < 4)
    {
        return "";
    }
    fileExtension = CTextUtils::ToLower(fileExtension);
    if (fileExtension.compare(".png") == 0 ||
        fileExtension.compare(".jpg") == 0 ||
        fileExtension.compare(".jpeg") == 0 ||
        fileExtension.compare(".bmp") == 0 ||
        fileExtension.compare(".tif") == 0 ||
        fileExtension.compare(".gif") == 0)
    {
        textFileName = fileName.substr(0, fileName.length() - fileExtension.length()) + ".txt";
        //cout << fileName << "\n";
        if (CFileUtils::FileExists(textFileName))
        {
            ifstream fileStream(textFileName);
            stringstream fileBuffer;
            string fileContents;
            fileBuffer << fileStream.rdbuf();
            fileContents = fileBuffer.str();

            if (CTextUtils::IsEmptyOrWhiteSpace(fileContents))
            {
                return fileName;
            }

            fileContents = CTextUtils::OnlyAlphanumericString(fileContents);
            //cout << "cont:" << fileContents << "\n";

            if (fileContents.length() < 10)
            {
                return fileName;
            }
        }
    }

    return "";
}

/*
 * Summary_Common::CFileUtils::IsPathWritable
 *
 * Description
 * Validates if path is writable by creating a random file for the moment
 *
 * Inputs
 * string pathName - path name
 *
 * Returns
 * True if path is writable
 */
bool Summary_Common::CFileUtils::IsPathWritable(string pathName)
{
    bool isPathWritable = false;
    string fileName = CFileUtils::RandomString(15) + ".txt";
    if (pathName.compare(".") == 0 || pathName.compare("./") == 0)
    {
        pathName = CFileUtils::GetCurrentPath();
    }
    //string currentPath = CFileUtils::GetCurrentPath();
    if (!CTextUtils::EndsWith(pathName, "/"))
    {
        pathName += "/";
    }

    fileName = pathName + fileName;

    RemoveFile(fileName);

    try
    {
        // try to write in the location
        ofstream outfile (fileName.c_str());
        outfile.close();

        if(!outfile.fail() && !outfile.bad())
        {
            isPathWritable = true;
        }
    }
    catch (exception& e)
    {
        LOG(ERROR) << CFileUtils::GetSourcePath() << e.what();

        PrintStacktrace();
    }

    RemoveFile(fileName);

    return isPathWritable;
}

/*
 * Summary_Common::CFileUtils::RemoveFile
 *
 * Description
 * Removes file from disk
 *
 * Inputs
 * string fileName - file name to remove
 *
 * Returns
 *
 */
void Summary_Common::CFileUtils::RemoveFile(string fileName)
{
    try
    {
        // remove a possibly existing test file
        if (CFileUtils::FileExists(fileName))
        {
            remove(fileName.c_str());
        }
    }
    catch (exception& e)
    {
        LOG(ERROR) << CFileUtils::GetSourcePath() << e.what();

        PrintStacktrace();
    }
}

/*
 * Summary_Common::CFileUtils::Mix
 *
 * Description
 * Calculates new long value from three input values. Used for better results in randomizing numbers
 *
 * Inputs
 * unsigned long a - first long
 * unsigned long b - second long
 * unsigned long c - third long
 *
 * Returns
 * Mixed number
 */
unsigned long Summary_Common::CFileUtils::Mix(unsigned long a, unsigned long b, unsigned long c)
{
    a=a-b;  a=a-c;  a=a^(c >> 13);
    b=b-c;  b=b-a;  b=b^(a << 8);
    c=c-a;  c=c-b;  c=c^(b >> 13);
    a=a-b;  a=a-c;  a=a^(c >> 12);
    b=b-c;  b=b-a;  b=b^(a << 16);
    c=c-a;  c=c-b;  c=c^(b >> 5);
    a=a-b;  a=a-c;  a=a^(c >> 3);
    b=b-c;  b=b-a;  b=b^(a << 10);
    c=c-a;  c=c-b;  c=c^(b >> 15);
    return c;
}

/*
 * Summary_Common::CFileUtils::ExtractFileName
 *
 * Description
 * Extracts pure file name without directory names. Not working on Windows
 *
 * Inputs
 * string pathName - file path to extract file name from
 *
 * Returns
 * Pure file name
 */
string CFileUtils::ExtractFileName(string pathName)
{
    string fileName = "";
    if (FileExists(pathName))
    {
        size_t pos = pathName.find_last_of("/"); //Works not on Windows
        if(pos != std::string::npos)
        {
            fileName.assign(pathName.begin() + pos + 1, pathName.end());
        }
        else
        {
            fileName = pathName;
        }
    }

    return fileName;
}

/*
 * Summary_Common::CFileUtils::ExtractFacialImages
 *
 * Description
 * Copies facial image candidates from
 *
 * Inputs
 * const string &createdFolder - temporary folder where temporary contets is to be extracted
 * const string &fileName - document file with images in it
 * const string &extractImagesPath - folder where facial image candidates are to be stored
 *
 * Returns
 *
 */
void CFileUtils::ExtractFacialImages(const string &createdFolder, const string &fileName, const string &extractImagesPath)
{
    string tesseractFolder = CreateHashedFolder(createdFolder);

    //Extract images
    CFileUtils::ExecuteExtract(fileName, tesseractFolder);

    //Find facial image candidates by making text, seeing if text is not too long and copying right
    //images to the output path
    CFileUtils::ExecuteTransform(tesseractFolder, extractImagesPath, &CFileUtils::TransformTesseract);

    //CFileUtils::ExecuteTransform(tesseractFolder, outputFileName, &CFileUtils::TransformCat);

    //Remove temp folders and files
    CFileUtils::RemovePath(tesseractFolder);
}
