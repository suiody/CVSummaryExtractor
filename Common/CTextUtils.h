//
// File CTextUtils.h
//
//

#ifndef CVSUMMARYEXTRACTOR_CTEXTUTILS_H
#define CVSUMMARYEXTRACTOR_CTEXTUTILS_H

#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <initializer_list>
#include "CLowerCaseString.h"

using namespace std;

namespace Summary_Common
{
    //
    // CLASS : CTextUtils
    //
    // DESCRIPTION : This class encapsulates methods for string operations
    //
    // Class consists of string methods split, lower case string split, trim etc.
    //
    class CTextUtils
    {
    public:
        static vector<string> Split(const string &s, const char delim);
        static vector<CLowerCaseString> Split(const CLowerCaseString &s, const char delim);
        static bool IsEmptyOrWhiteSpace(const string &inputText);
        static string::size_type FindStringIgnoreCase(const string &strHaystack, const string &strNeedle, const int start = 0);
        static string::size_type FindString(const string &strHaystack, const string &strNeedle, const int start = 0);
        static bool StringContainsIgnoreCase(const string &strHaystack, const string &strNeedle);
        static bool StringContains(const string &strHaystack, const string &strNeedle);
        static bool StringContainsMutually(const string &strHaystack, const string &strNeedle);
        static bool StringContainsAnyOf(const string &strHaystack, const initializer_list<string> &needles);
        static bool StringContainsAnyOf(const string &strHaystack, const vector<string> &needles);
        static bool StringIsAnyOf(const string &strHaystack, const initializer_list<string> &needles);
        static bool StringIsAnyOf(const string &strHaystack, const vector<string> &needles);

        static string ToLower(const string &str);

        static string LTrim(const string &str);
        static string RTrim(const string &str);
        static string Trim(const string &str);
        static string LTrimNonText(const string &str);
        static string RTrimNonText(const string &str);
        static string TrimNonText(const string &str);
        static string LTrimAnyChar(const string &str, const string &charsToRemove);
        static wstring LTrimAnyChar(const wstring &str, const wstring &charsToRemove);
        static string RTrimAnyChar(const string &str, const string &charsToRemove);
        static wstring RTrimAnyChar(const wstring &str, const wstring &charsToRemove);
        static string TrimAnyChar(const string &str, const string &charsToRemove);
        static wstring TrimAnyChar(const wstring &str, const wstring &charsToRemove);
        static string TrimNonAlphanumeric(const string &str);

        static string RemoveQuotation(const string &str);
        static bool StartsWith(string const &fullString, string const &start);
        static bool EndsWith(string const &fullString, string const &ending);
        static bool ContainsWithExactEnding(const string &line, const string &needle);
        static bool ContainsWithExactStart(const string &line, const string &needle);
        static bool ContainsWithExactStartAndEnding(const string &line, const string &needle);
        static string OnlyAlphanumericString(string inputString);
        static string OnlyAlphanumericStringWithSpaces(string inputString);
        static string OnlyLetters(string inputString);
        static string OnlyLettersWithSpaces(string inputString);
        static string Remove(string inputString, char charToRemove);
        static string RemoveSubstring(const string &inputString, const string &charsToRemove);
        static string Replace(string inputString, char charToRemove, char charToReplace);
        static wstring Replace(wstring inputString, char charToRemove, char charToReplace);
        static string ReplaceAnyChar(const string &str, const string &charsToRemove, char charToReplace);
        static wstring ReplaceAnyChar(const wstring &str, const wstring &charsToRemove, wchar_t charToReplace);
        static string ReplaceEscapeCharacters(const string &input);
        static wstring ReplaceEscapeCharacters(const wstring &input);
        static string ReplaceNonAlphanumeric(string inputString, char charToReplace);
        static string ReplaceNonAlphanumeric(string inputString, char charToReplace, const vector<char> &keepChars);
        static string RemoveUnknownCharacters(string inputString);
        static string FirstLetterUpCase(string input);
        static string OnlyFirstLetterUpCase(string input);
        static wstring WideString(string const& s, locale loc);
        static string NarrowString(wstring const& text, locale loc);
        static string UrlEncode(const string &inputString);
        static string HtmlDecode(const string &inputString);
        static long StringCount(const string &strHaystack, const string &strNeedle);
        static vector<string> SortMapByCounts(const map<string, int> &mapWithCounts, int maxResults);
        static map<string, int> GetSortedMapByCounts(const map<string, int> &mapWithCounts, int maxResults);
        static bool VectorContainsMutually(const vector<string> &haystack, const string &needle, bool alreadyLowerCase);
        static string RemoveUnusedSpaces(const string &inputString);

        static string TrimNameCharacters(const string &input);
        static void AddVectorMutuallyExclusively(vector<string> &array, const string &newString);
    private:
        static bool IsQuotation(char c);
        static string Char2Hex( char dec );

        static inline size_t IndexOf(const string& data, const string& pattern, const int start = 0)
        {
            if (pattern.empty())
                return string::npos;
            else if (data.empty())
                return string::npos;
            else if (pattern.size() > data.size())
                return string::npos;

            const char* itr = search(data.data() + start,
                                          data.data() + data.size(),
                                          pattern.data(),
                                          pattern.data() + pattern.size());

            return ((data.data() + data.size()) == itr) ? string::npos : distance(data.data(),itr);
        }
    };
}

#endif //CVSUMMARYEXTRACTOR_CTEXTUTILS_H
