//
// File CPlainTextInputReader.cpp
//
//

#include <algorithm>
#include <iostream>
#include <fstream>
#include <locale>
#include "../Common/CTextUtils.h"
#include "../Common/CDebugUtils.h"
#include "../Common/CFileUtils.h"
#include "CPlainTextInputReader.h"
#include "../Easylogging/easylogging++.h"
#include "../Stacktrace/Stacktrace.h"

/*#include <sstream>
#include <fstream>
#include <codecvt>*/

using namespace Summary_Common;
//std::wstring readFile(const char* filename);
/*
 * Summary_PlainReader::CPlainTextInputReader::CanHandle
 *
 * Description
 * Validates whether text file reading can be handled
 *
 * Inputs
 * const string &location - text file path
 *
 * Returns
 * true if file is text file (has .txt ending)
 */
bool Summary_PlainReader::CPlainTextInputReader::CanHandle(const string &location)
{
    CDebugUtils::DebugCout("CPlainTextInputReader", "CanHandle", "");
    string localLocation = location;
    transform(localLocation.begin(), localLocation.end(), localLocation.begin(), ::tolower);
    return HasEnding(location, "txt");
}

/*
 * Summary_PlainReader::CPlainTextInputReader::CanHandle
 *
 * Description
 * Reads all text lines from file into string vector
 *
 * Inputs
 * const string &location - text file path
 *
 * Returns
 * vector<string> filled with text lines from file
 */
vector<string> Summary_PlainReader::CPlainTextInputReader::Handle(const string &location)
{
    CDebugUtils::DebugCout("CPlainTextInputReader", "Handle", "");
    vector<string> lines;
    string line;

    ifstream inputFile;

    try
    {
        inputFile.imbue(locale("en_US.UTF-8"));
        inputFile.open(location.c_str(), ifstream::in);

        if (inputFile.is_open())
        {
            while ( getline (inputFile,line) )
            {
                //cout << line << '\n';
                line = CTextUtils::ReplaceEscapeCharacters(line);
                line = CTextUtils::Replace(line, '\t', ' ');
                lines.push_back(line);
            }
            inputFile.close();
        }
    }
    catch (exception &e)
    {
        LOG(ERROR) << CFileUtils::GetSourcePath() << e.what();

        PrintStacktrace();
        //inputFile.close();
    }

    return lines;
}


/*
 * Summary_PlainReader::CPlainTextInputReader::CanHandleWide
 *
 * Description
 * Reads all text lines from file into wstring vector
 *
 * Inputs
 * const string &location - text file path
 *
 * Returns
 * vector<wstring> filled with text lines from file
 */
vector<wstring> Summary_PlainReader::CPlainTextInputReader::HandleWide(const string &location)
{
    CDebugUtils::DebugCout("CPlainTextInputReader", "Handle", "");
    vector<wstring> lines;
    wstring line;

    //std::locale utf8_locale(std::locale(), new gel::stdx::utf8cvt<true>);
    //std::locale utf8_locale(std::locale(), new utf8cvt<true>);
    //std::locale::global(locale("en_US.UTF-8"));
    wifstream inputFile;

    try
    {
        //ifstream myfile (location);
        inputFile.imbue(locale("en_US.UTF-8"));
        inputFile.open(location.c_str(), wifstream::in);

        if (inputFile.is_open())
        {
            while ( getline (inputFile,line) )
            {
                //wcout << line << '\n';
                lines.push_back(line);
            }
            inputFile.close();
        }
    }
    catch (exception &e)
    {
        LOG(ERROR) << CFileUtils::GetSourcePath() << e.what();

        PrintStacktrace();
        //inputFile.close();
    }

    return lines;
}

/*
 * Summary_PlainReader::CPlainTextInputReader::HasEnding
 *
 * Description
 * Validates if file has proper ending or ending plus closing "
 *
 * Inputs
 * string const &fullString - file name
 * string const &ending - file ending to validate for
 *
 * Returns
 * true if file has proper ending or ending plus closing "
 */
bool Summary_PlainReader::CPlainTextInputReader::HasEnding(string const &fullString, string const &ending)
{
    CDebugUtils::DebugCout("CPlainTextInputReader", "HasEnding", fullString);
    return CTextUtils::EndsWith(fullString, ending) || CTextUtils::EndsWith(fullString, ending + "\"");
}

/*std::wstring readFile(const char* filename)
{
    std::wifstream wif(filename);
    std::locale elppUnicodeLocale("");
    wif.imbue(std::locale(elppUnicodeLocale, new std::codecvt_utf8<wchar_t>));
    std::wstringstream wss;
    wss << wif.rdbuf();
    return wss.str();
}*/
