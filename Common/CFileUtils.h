//
// File CFileUtils.h
//
//
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file Boost/LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef CVSUMMARYEXTRACTOR_CFILEUTILS_H
#define CVSUMMARYEXTRACTOR_CFILEUTILS_H

#include <string>
#include <functional>
#include <boost/filesystem.hpp>

using namespace std;
using namespace boost::filesystem;

namespace Summary_Common
{
    //
    // CLASS : CFileUtils
    //
    // DESCRIPTION : This class encapsulates file and path methods
    //
    // Class has methods for detecting file and path existance, creating new path with random name and executing
    // Apache Tika commands along with tesseract ocr scanning or removing path with files
    //
    class CFileUtils
    {
    public:
        /*
        * Summary_Common:: CFileUtils::GetRunPath
        *
        * Description
        * Gets the path where program is run from previously set property runPath
        *
        * Inputs
        *
        * Returns
        *
        */
        static const string &GetRunPath()
        {
            return runPath;
        }

        /*
        * Summary_Common:: CFileUtils::SetRunPath
        *
        * Description
        * Sets the path where program is run
        *
        * Inputs
        * const string &runPathInput - program run path
        *
        * Returns
        * Program running path
        */
        static void SetRunPath(const string &runPathInput)
        {
            runPath = runPathInput;
        }

        /*
        * Summary_Common:: CFileUtils::GetSourcePath
        *
        * Description
        * Gets the source path where input file with text (.txt,.doc etc.) resides with input file name
        * from previously set property sourcePath
        *
        * Inputs
        *
        * Returns
        * Gets the path of input file with file name
        */
        static const string &GetSourcePath()
        {
            return sourcePath;
        }

        /*
        * Summary_Common:: CFileUtils::SetSourcePath
        *
        * Description
        * Sets the path of input file with file name
        *
        * Inputs
        * const string &sourcePathInput - input file path with file name but basically any path
        *
        * Returns
        *
        */
        static void SetSourcePath(const string &sourcePathInput)
        {
            sourcePath = sourcePathInput;
        }

        /*
        * Summary_Common:: CFileUtils::GetDataFileName
        *
        * Description
        * Gets the input data file name (.txt, .pdf or with other extension) of the summary data
        *
        * Inputs
        *
        * Returns
        * Gets input data file name
        */
        static const string &GetDataFileName()
        {
            return dataFileName;
        }

        /*
        * Summary_Common:: CFileUtils::SetDataFileName
        *
        * Description
        * Sets the name of input file
        *
        * Inputs
        * const string &dataFileName - input file name were summary resides
        *
        * Returns
        *
        */
        static void SetDataFileName(const string &dataFileName)
        {
            CFileUtils::dataFileName = dataFileName;
        }

        static string GetCurrentPath();
        static bool PathExists(string pathName);
        static bool FileExists(string pathName);
        static string CreateHashedFolder(string pathName);
        static void ExecuteCommand(string command);
        static void RemovePath(string pathName);
        static void ExecuteTransform(string pathName, string outputPath, function<void(string, string)> transformFunction);
        static void ExecuteConvert(string inputFile, string outputFile);
        static void ExecuteNonStopConvert(string inputFile, string outputFile);
        static void ExecuteExtract(string inputFile, string extractFolder);
        static void TransformTesseract(string inputFile, string outputPath);
        static void TransformCat(string inputFile, string outputFile);
        static string GetFileExtension(string fileName);
        static long GetFileSize(string fileName);
        static bool ListPath(string pathName, int level);
        static string FaceImageCandidate(string fileName);
        static bool IsPathWritable(string pathName);
        static void RemoveFile(string fileName);
        static string ExtractFileName(string pathName);
        static void ExtractFacialImages(const string &createdFolder, const string &fileName, const string &extractImagesPath);
    private:
        static string RandomString(int len);

        static bool PathCompare(const path& s1, const path& s2);
        static int NaturalCompare( const string& a, const string& b );
        static unsigned long Mix(unsigned long a, unsigned long b, unsigned long c);

        static string runPath;
        static string sourcePath;
        static string dataFileName;
    };
}

#endif //CVSUMMARYEXTRACTOR_CFILEUTILS_H
