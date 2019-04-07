//
// File CTextUtils.cpp
//
//

#include <stdint.h>
#include <algorithm>
#include <limits>

#include <sstream>
#include <cctype>
#include <algorithm>
#include <clocale>
#include <locale>
#include <vector>
#include <string>
#include <cmath>
#include "CTextUtils.h"
#include "CDebugUtils.h"
#include <boost/locale.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/range/algorithm/replace_if.hpp>
#include "entities.h"


#include <cstring>


/*
 * Summary_Common::CTextUtils::Split
 *
 * Description
 * Splits text by delimiter into string vector
 *
 * Inputs
 * const string &s - text
 * char delim - delimiter
 *
 * Returns
 * vector<string> - splitted string lines
 */
vector<string> Summary_Common::CTextUtils::Split(const string &s, const char delim)
{
    vector<string> elems;
    string delimiterString = "";
    delimiterString += delim;

    boost::split(elems, s, boost::is_any_of(delimiterString));

    if (elems.size() < 1)
    {
        elems.push_back(s);
    }

    return elems;
}

/*
 * Summary_Common::CTextUtils::Split
 *
 * Description
 * Splits text by delimiter into lower case string vector
 *
 * Inputs
 * const string &s - text
 * char delim - delimiter
 *
 * Returns
 * vector<string> - splitted string lines in lower case
 */
vector<Summary_Common::CLowerCaseString> Summary_Common::CTextUtils::Split(const Summary_Common::CLowerCaseString &s,
                                                                           const char delim)
{
    vector<CLowerCaseString> elems;
    string delimiterString = "";
    delimiterString += delim;

    vector<string> entities;
    string allData = (string)s;

    boost::split(entities, allData, boost::is_any_of(delimiterString));

    for (string entity: entities)
    {
        elems.push_back(entity);
    }

    entities.clear();

    if (elems.size() < 1)
    {
        elems.push_back(s);
    }

    return elems;
}

/*
 * Summary_Common::CTextUtils::IsEmptyOrWhiteSpace
 *
 * Description
 * Validates whether the input string is empty or whitespace
 *
 * Inputs
 * const string &inputText - input text
 *
 * Returns
 * true if the input string is empty or whitespace
 */
bool Summary_Common::CTextUtils::IsEmptyOrWhiteSpace(const string &inputText)
{
    return (inputText.compare("") == 0 || inputText.compare(" ") == 0);
}

/*
 * Summary_Common::CTextUtils::FindStringIgnoreCase
 *
 * Description
 * Gets position of a substring withing string ingnoring both strings case
 *
 * Inputs
 * const string &strHaystack - string to search for
 * const string &strNeedle - substring to search for
 *
 * Returns
 * string::size_type - found string position or string::npos if not found
 */
string::size_type Summary_Common::CTextUtils::FindStringIgnoreCase(const string &strHaystack,
                                                                   const string &strNeedle,
                                                                   const int start)
{
    string lowerHayStack = ToLower(strHaystack);
    string lowerNeedle = ToLower(strNeedle);

    return IndexOf(lowerHayStack, lowerNeedle, start);
}

/*
 * Summary_Common::CTextUtils::FindString
 *
 * Description
 * Gets position of a substring withing string
 *
 * Inputs
 * const string &strHaystack - string to search for
 * const string &strNeedle - substring to search for
 *
 * Returns
 * string::size_type - found string position or string::npos if not found
 */
string::size_type Summary_Common::CTextUtils::FindString(const string &strHaystack,
                                                         const string &strNeedle,
                                                         const int start)
{
    return IndexOf(strHaystack, strNeedle, start);
}

/*
 * Summary_Common::CTextUtils::StringContainsIgnoreCase
 *
 * Description
 * Returns true if needle contains in haystack, ignoring the case of both strings
 *
 * Inputs
 * const string &strHaystack - string to search for
 * const string &strNeedle - substring to search for
 *
 * Returns
 * bool - true if needle contains in haystack, ignoring the case of both strings
 */
bool Summary_Common::CTextUtils::StringContainsIgnoreCase(const string &strHaystack, const string &strNeedle)
{
    string lowerHayStack = ToLower(strHaystack);
    string lowerNeedle = ToLower(strNeedle);

    return IndexOf(lowerHayStack, lowerNeedle) != string::npos;
}

/*
 * Summary_Common::CTextUtils::StringContains
 *
 * Description
 * Returns true if needle contains in haystack
 *
 * Inputs
 * const string &strHaystack - string to search for
 * const string &strNeedle - substring to search for
 *
 * Returns
 * bool - true if needle contains in haystack
 */
bool Summary_Common::CTextUtils::StringContains(const string &strHaystack, const string &strNeedle)
{
    //return strstr(strHaystack.c_str(), strNeedle.c_str()) != NULL;
    return IndexOf(strHaystack, strNeedle) != string::npos;
}

/*
 * Summary_Common::CTextUtils::StringContainsMutually
 *
 * Description
 * Returns true if needle contains in haystack or vice versa haystack contains in needle
 *
 * Inputs
 * const string &strHaystack - string to search for
 * const string &strNeedle - substring to search for
 *
 * Returns
 * bool - true if needle contains in haystack
 */
bool Summary_Common::CTextUtils::StringContainsMutually(const string &strHaystack, const string &strNeedle)
{
    string::size_type position = IndexOf(strHaystack, strNeedle);

    if (position == string::npos)
    {
        position = IndexOf(strNeedle, strHaystack);
    }

    return position != string::npos;
}

/*
* Summary_Common::CTextUtils::StringContainsAnyOf
*
* Description
* Returns true if one of needles contains in haystack
*
* Inputs
* const string &strHaystack - string to search for
* initializer_list<T> needles - variable length of string arguments
*
* Returns
* bool - true if one of the needles contains in haystack
*/
bool Summary_Common::CTextUtils::StringContainsAnyOf(const string &strHaystack, const initializer_list<string> &needles)
{
    if (needles.size() < 1)
    {
        return false;
    }

    for (string needle: needles)
    {
        if (StringContains(strHaystack, needle))
        {
            return true;
        }
    }

    return false;
}

/*
* Summary_Common::CTextUtils::StringContainsAnyOf
*
* Description
* All template methods must be declared on .h for avoiding linking issues
* Returns true if one of needles contains in haystack
*
* Inputs
* const string &strHaystack - string to search for
* const vector<string> &needles - vector of string needles
*
* Returns
* bool - true if one of the needles contains in haystack
*/
bool Summary_Common::CTextUtils::StringContainsAnyOf(const string &strHaystack, const vector<string> &needles)
{
    if (needles.size() < 1)
    {
        return false;
    }

    for (string needle: needles)
    {
        if (StringContains(strHaystack, needle))
        {
            return true;
        }
    }

    return false;
}


/*
* Summary_Common::CTextUtils::StringContainsAnyOf
*
* Description
* Returns true if one of needles contains in haystack
*
* Inputs
* const string &strHaystack - string to search for
* initializer_list<T> needles - variable length of string arguments
*
* Returns
* bool - true if one of the needles contains in haystack
*/
bool Summary_Common::CTextUtils::StringIsAnyOf(const string &strHaystack, const initializer_list<string> &needles)
{
    if (needles.size() < 1)
    {
        return false;
    }

    for (string needle: needles)
    {
        if (strHaystack.compare(needle) == 0)
        {
            return true;
        }
    }

    return false;
}

/*
* Summary_Common::CTextUtils::StringContainsAnyOf
*
* Description
* All template methods must be declared on .h for avoiding linking issues
* Returns true if one of needles contains in haystack
*
* Inputs
* const string &strHaystack - string to search for
* const vector<string> &needles - vector of string needles
*
* Returns
* bool - true if one of the needles contains in haystack
*/
bool Summary_Common::CTextUtils::StringIsAnyOf(const string &strHaystack, const vector<string> &needles)
{
    if (needles.size() < 1)
    {
        return false;
    }

    for (string needle: needles)
    {
        if (strHaystack.compare(needle) == 0)
        {
            return true;
        }
    }

    return false;
}

/*
 * Summary_Common::CTextUtils::ToLower
 *
 * Description
 * Makes string into lower case
 *
 * Inputs
 * const string &str - input string
 *
 * Returns
 * String in lower case
 */
string Summary_Common::CTextUtils::ToLower(const string &str)
{
    string lowerNeedle;
    transform(str.begin(), str.end(), back_inserter(lowerNeedle), ::tolower);

    return lowerNeedle;
}

/*
 * Summary_Common::CTextUtils::LTrim
 *
 * Description
 * Removes spaces from the left of string
 *
 * Inputs
 * const string &str - input string
 *
 * Returns
 * Left trimmed string
 */
string Summary_Common::CTextUtils::LTrim(const string &str)
{
    string localString = str;
    auto it =  find_if( localString.begin() , localString.end() , [](char ch){ return !isspace<char>(ch , locale::classic() ) ; } );
    localString.erase( localString.begin() , it);
    return localString;
}

/*
 * Summary_Common::CTextUtils::RTrim
 *
 * Description
 * Removes spaces from the right of string
 *
 * Inputs
 * const string &str - input string
 *
 * Returns
 * Right trimmed string
 */
string Summary_Common::CTextUtils::RTrim(const string &str)
{
    string localString = str;
    auto it =  find_if( localString.rbegin() , localString.rend() , [](char ch){ return !isspace<char>(ch , locale::classic() ) ; } );
    localString.erase( localString.begin() + (  localString.rend() - it ) , localString.end() );
    return localString;
}

/*
 * Summary_Common::CTextUtils::Trim
 *
 * Description
 * Removes spaces from the left and right of string
 *
 * Inputs
 * const string &str - input string
 *
 * Returns
 * Left and right trimmed string
 */
string Summary_Common::CTextUtils::Trim(const string &str)
{
    string localString = RTrim(str);
    return LTrim(localString);
}

/*
 * Summary_Common::CTextUtils::LTrimNonText
 *
 * Description
 * Removes spaces from the left and right of string
 *
 * Inputs
 * const string &str - input string
 *
 * Returns
 * Left trimmed string
 */
string Summary_Common::CTextUtils::LTrimNonText(const string &str)
{
    string localString = str;//RTrim(str);
    //localString = LTrim(localString);

    auto it =  find_if( localString.begin() , localString.end() , [](char ch){ return isalpha<char>(ch , locale::classic() ) ; } );
    localString.erase( localString.begin() , it);

    auto it2 =  find_if( localString.begin() , localString.end() , [](char ch){ return isalnum<char>(ch , locale::classic() ) ; } );
    localString.erase( localString.begin() , it2);

    return localString;
}

/*
 * Summary_Common::CTextUtils::RTrimNonText
 *
 * Description
 * Removes spaces from the left and right of string
 *
 * Inputs
 * const string &str - input string
 *
 * Returns
 * Left trimmed string
 */
string Summary_Common::CTextUtils::RTrimNonText(const string &str)
{
    string localString = str;//RTrim(str);
    //localString = LTrim(localString);

    auto it =  find_if( localString.rbegin() , localString.rend() , [](char ch){ return isalpha<char>(ch , locale::classic() ) ; } );
    localString.erase( localString.begin() + (  localString.rend() - it ) , localString.end() );

    auto it2 =  find_if( localString.rbegin() , localString.rend() , [](char ch){ return isalnum<char>(ch , locale::classic() ) ; } );
    localString.erase( localString.begin() + (  localString.rend() - it2 ) , localString.end() );

    return localString;
}

/*
 * Summary_Common::CTextUtils::TrimNonText
 *
 * Description
 * Removes spaces from the left and right of string
 *
 * Inputs
 * const string &str - input string
 *
 * Returns
 * Left trimmed string
 */
string Summary_Common::CTextUtils::TrimNonText(const string &str)
{
    string localString = RTrimNonText(str);
    localString = LTrimNonText(localString);

    return localString;
}

/*
 * Summary_Common::CTextUtils::LTrimChar
 *
 * Description
 * Removes spaces from the left and right of string
 *
 * Inputs
 * const string &str - input string
 *
 * Returns
 * Left trimmed string
 */
string Summary_Common::CTextUtils::LTrimAnyChar(const string &str, const string &charsToRemove)
{
    string localString = str;//RTrim(str);
    //localString = LTrim(localString);
    boost::trim_left_if(localString, boost::is_any_of(charsToRemove));
    //localString.erase(0, localString.find_first_not_of(charToRemove));

    return localString;
}

/*
 * Summary_Common::CTextUtils::LTrimChar
 *
 * Description
 * Removes spaces from the left and right of string
 *
 * Inputs
 * const string &str - input string
 *
 * Returns
 * Left trimmed string
 */
wstring Summary_Common::CTextUtils::LTrimAnyChar(const wstring &str, const wstring &charsToRemove)
{
    wstring localString = str;//RTrim(str);
    //localString = LTrim(localString);
    boost::trim_left_if(localString, boost::is_any_of(charsToRemove));
    //localString.erase(0, localString.find_first_not_of(charToRemove));

    return localString;
}

/*
 * Summary_Common::CTextUtils::RTrimChar
 *
 * Description
 * Removes spaces from the left and right of string
 *
 * Inputs
 * const string &str - input string
 *
 * Returns
 * Right trimmed string
 */
string Summary_Common::CTextUtils::RTrimAnyChar(const string &str, const string &charsToRemove)
{
    string localString = str;//RTrim(str);
    //localString = LTrim(localString);

    //localString.erase(localString.find_last_not_of(charToRemove) + 1);
    boost::trim_right_if(localString, boost::is_any_of(charsToRemove));

    return localString;
}

/*
 * Summary_Common::CTextUtils::RTrimChar
 *
 * Description
 * Removes spaces from the left and right of string
 *
 * Inputs
 * const string &str - input string
 *
 * Returns
 * Right trimmed string
 */
wstring Summary_Common::CTextUtils::RTrimAnyChar(const wstring &str, const wstring &charsToRemove)
{
    wstring localString = str;//RTrim(str);
    //localString = LTrim(localString);

    //localString.erase(localString.find_last_not_of(charToRemove) + 1);
    boost::trim_right_if(localString, boost::is_any_of(charsToRemove));

    return localString;
}

/*
 * Summary_Common::CTextUtils::TrimChar
 *
 * Description
 * Removes spaces from the left and right of string
 *
 * Inputs
 * const string &str - input string
 *
 * Returns
 * Right and left trimmed string
 */
string Summary_Common::CTextUtils::TrimAnyChar(const string &str, const string &charsToRemove)
{
    string localString = RTrimAnyChar(str, charsToRemove);
    localString = LTrimAnyChar(localString, charsToRemove);

    return localString;
}

/*
 * Summary_Common::CTextUtils::TrimChar
 *
 * Description
 * Removes spaces from the left and right of string
 *
 * Inputs
 * const string &str - input string
 *
 * Returns
 * Right and left trimmed string
 */
wstring Summary_Common::CTextUtils::TrimAnyChar(const wstring &str, const wstring &charsToRemove)
{
    wstring localString = RTrimAnyChar(str, charsToRemove);
    localString = LTrimAnyChar(localString, charsToRemove);

    return localString;
}

/*
 * Summary_Common::CTextUtils::TrimNonAlphanumeric
 *
 * Description
 * Removes spaces from the left and right of string
 *
 * Inputs
 * const string &str - input string
 *
 * Returns
 * Left and right trimmed string
 */
string Summary_Common::CTextUtils::TrimNonAlphanumeric(const string &str)
{
    string localString = str;//RTrim(str);
    //localString = LTrim(localString);

    auto it =  find_if( localString.begin() , localString.end() , [](char ch){ return isalnum<char>(ch , locale::classic() ) ; } );
    localString.erase( localString.begin() , it);

    auto it2 =  find_if( localString.rbegin() , localString.rend() , [](char ch){ return isalnum<char>(ch , locale::classic() ) ; } );
    localString.erase( localString.begin() + (  localString.rend() - it2 ) , localString.end() );

    return localString;
}

/*
 * Summary_Common::CTextUtils::RemoveQuotation
 *
 * Description
 * Removes double quotes from string
 *
 * Inputs
 * const string &str - input string
 *
 * Returns
 * Transformed string
 */
string Summary_Common::CTextUtils::RemoveQuotation(const string &str)
{
    string localString = str;
    localString.erase(remove_if(localString.begin(), localString.end(), IsQuotation), localString.end());

    return localString;
}

/*
 * Summary_Common::CTextUtils::OnlyAlphanumericString
 *
 * Description
 * Gets string with only alphanumeric characters (and without space) in it
 *
 * Inputs
 * string inputString - input string
 *
 * Returns
 * String with only alphanumeric characters
 */
string Summary_Common::CTextUtils::OnlyAlphanumericString(string inputString)
{
    size_t i = 0;
    size_t len = inputString.length();
    while(i < len)
    {
        if (!isalnum(inputString[i]))
        {
            inputString.erase(i, 1);
            len--;
        }
        else
        {
            i++;
        }
    }
    return inputString;
}

/*
 * Summary_Common::CTextUtils::OnlyAlphanumericStringWithSpaces
 *
 * Description
 * Gets string with only alphanumeric characters and with space in it
 *
 * Inputs
 * string inputString - input string
 *
 * Returns
 * String with only alphanumeric characters and with space in it
 */
string Summary_Common::CTextUtils::OnlyAlphanumericStringWithSpaces(string inputString)
{
    //std::string inputStringUTF8 = boost::locale::conv::from_utf<char>(inputString,"UTF-8");
    //locale localeUTF8("en_US.UTF-8");

    size_t i = 0;
    size_t len = inputString.length();

    while(i < len)
    {
        if (!isalnum(inputString[i]) && inputString[i] != ' ')
        {
            inputString.erase(i, 1);
            len--;
        }
        else
        {
            i++;
        }
    }
    return inputString;
}

/*
 * Summary_Common::CTextUtils::OnlyLettersWithSpaces
 *
 * Description
 * Gets string with only letters (ABCDEFGHIJKLMNOPQRSTUVWXYZ, abcdefghijklmnopqrstuvwxyz) and spaces in it
 *
 * Inputs
 * string inputString - input string
 *
 * Returns
 * String with only letters and spaces in it
 */
string Summary_Common::CTextUtils::OnlyLettersWithSpaces(string inputString)
{
    inputString = Summary_Common::CTextUtils::OnlyAlphanumericStringWithSpaces(inputString);
    size_t i = 0;
    size_t len = inputString.length();
    while(i < len)
    {
        if (isdigit(inputString[i]))
        {
            inputString.erase(i, 1);
            len--;
        }
        else
        {
            i++;
        }
    }
    return inputString;
}

/*
 * Summary_Common::CTextUtils::EndsWith
 *
 * Description
 * Validates whether the string ends with specific ending or not
 *
 * Inputs
 * string const &fullString - string to check for ending
 * string const &ending - ending to search for
 *
 * Returns
 * True if string has specific ending
 */
bool Summary_Common::CTextUtils::StartsWith(string const &fullString, string const &start)
{
    if (fullString.length() >= start.length())
    {
        return (0 == fullString.compare (0, start.length(), start));
    }
    else
    {
        return false;
    }
}

/*
 * Summary_Common::CTextUtils::EndsWith
 *
 * Description
 * Validates whether the string ends with specific ending or not
 *
 * Inputs
 * string const &fullString - string to check for ending
 * string const &ending - ending to search for
 *
 * Returns
 * True if string has specific ending
 */
bool Summary_Common::CTextUtils::EndsWith(string const &fullString, string const &ending)
{
    if (fullString.length() >= ending.length())
    {
        return (0 == fullString.compare (fullString.length() - ending.length(), ending.length(), ending));
    }
    else
    {
        return false;
    }
}

/*
 * Summary_Common::CTextUtils::ContainsWithExactEnding
 *
 * Description
 * Validates whether the string contains a needle and this needle is at the end of string or is not in the start
 * of another word
 *
 * Inputs
 * const string &line - string to check for needle
 * const string &needle - needle
 *
 * Returns
 * True if string contains the needle and it has exact ending in the string
 */
bool Summary_Common::CTextUtils::ContainsWithExactEnding(const string &line, const string &needle)
{
    string::size_type namePosition = CTextUtils::FindString(line, needle);
    if (namePosition != string::npos &&
        (namePosition + needle.length() == line.length() || !isalnum(line.at(namePosition + needle.length()))))
    {
        return true;
    }
    return false;
}

/*
 * Summary_Common::CTextUtils::ContainsWithExactStart
 *
 * Description
 * Validates whether the string contains a needle and this needle is at the start of string or is not in the end
 * of another word
 *
 * Inputs
 * const string &line - string to check for needle
 * const string &needle - needle
 *
 * Returns
 * True if string contains the needle and it has exact start in the string
 */
bool Summary_Common::CTextUtils::ContainsWithExactStart(const string &line, const string &needle)
{
    string::size_type namePosition = CTextUtils::FindString(line, needle);
    if (namePosition != string::npos &&
        (namePosition == 0 || !isalpha(line.at(namePosition -1))))
    {
        return true;
    }
    return false;
}

/*
 * Summary_Common::CTextUtils::ContainsWithExactStartAndEnding
 *
 * Description
 * Validates whether the string contains a needle and this needle is at the start of string / is not in the end
 * of another word plus is in the end of string or is not in the start of another word
 *
 * Inputs
 * const string &line - string to check for needle
 * const string &needle - needle
 *
 * Returns
 * True if string contains the needle and it has exact start and ending in the string
 */
bool Summary_Common::CTextUtils::ContainsWithExactStartAndEnding(const string &line, const string &needle)
{
    string::size_type namePosition = CTextUtils::FindString(line, needle);
    if (namePosition != string::npos &&
        (namePosition == 0 || !isalpha(line.at(namePosition -1))) &&
        (namePosition + needle.length() == line.length() || !isalnum(line.at(namePosition + needle.length()))))
    {
        return true;
    }
    return false;
}

/*
 * Summary_Common::CTextUtils::Remove
 *
 * Description
 * Removes all occurrences of a character from string
 *
 * Inputs
 * string inputString - input string
 * char charToRemove - character to be removed
 *
 * Returns
 * String without specific characters
 */
string Summary_Common::CTextUtils::Remove(string inputString, char charToRemove)
{
    size_t i = 0;
    size_t len = inputString.length();
    while(i < len)
    {
        if (inputString[i] == charToRemove)
        {
            inputString.erase(i, 1);
            len--;
        }
        else
        {
            i++;
        }
    }
    return inputString;
}

/*
 * Summary_Common::CTextUtils::RemoveSubstring
 *
 * Description
 * Removes all occurrences of a string from string
 *
 * Inputs
 * const string &inputString - input string
 * const string &charsToRemove - character to be removed
 *
 * Returns
 * String without specific string occurrences
 */
string Summary_Common::CTextUtils::RemoveSubstring(const string &inputString, const string &charsToRemove)
{
    return boost::erase_all_copy(inputString, charsToRemove);
}

/*
 * Summary_Common::CTextUtils::Replace
 *
 * Description
 * Replaces all occurrences of one character to another within string
 *
 * Inputs
 * string inputString - input string
 * char charToRemove - character to be removed
 * char charToReplace - character to be replaced
 *
 * Returns
 * String with all specific characters replaced to other characters
 */
string Summary_Common::CTextUtils::Replace(string inputString, char charToRemove, char charToReplace)
{
    size_t i = 0;
    size_t len = inputString.length();
    while(i < len)
    {
        if (inputString[i] == charToRemove)
        {
            inputString[i] = charToReplace;
        }

        i++;
    }
    return inputString;
}

/*
 * Summary_Common::CTextUtils::Replace
 *
 * Description
 * Replaces all occurrences of one character to another within string
 *
 * Inputs
 * string inputString - input string
 * char charToRemove - character to be removed
 * char charToReplace - character to be replaced
 *
 * Returns
 * String with all specific characters replaced to other characters
 */
wstring Summary_Common::CTextUtils::Replace(wstring inputString, char charToRemove, char charToReplace)
{
    size_t i = 0;
    size_t len = inputString.length();
    while(i < len)
    {
        if (inputString[i] == charToRemove)
        {
            inputString[i] = charToReplace;
        }

        i++;
    }
    return inputString;
}

/*
 * Summary_Common::CTextUtils::RemoveAnyChar
 *
 * Description
 * Removes any characters in str, defined in charsToRemove
 *
 * Inputs
 * const string &str - input string
 *
 * Returns
 * Right and left trimmed string
 */
string Summary_Common::CTextUtils::ReplaceAnyChar(const string &str, const string &charsToRemove, char charToReplace)
{
    string localString = str;
    //boost::erase_all(localString, charsToRemove);
    localString = boost::replace_if(localString, boost::is_any_of(charsToRemove), charToReplace);

    return localString;
}

/*
 * Summary_Common::CTextUtils::RemoveAnyChar
 *
 * Description
 * Removes any characters in str, defined in charsToRemove
 *
 * Inputs
 * const wstring &str - input string
 *
 * Returns
 * Characters replaced in wstring
 */
wstring Summary_Common::CTextUtils::ReplaceAnyChar(const wstring &str, const wstring &charsToRemove, wchar_t charToReplace)
{
    wstring localString = str;
    //boost::erase_all(localString, charsToRemove);
    localString = boost::replace_if(localString, boost::is_any_of(charsToRemove), charToReplace);

    return localString;
}

/*
 * Summary_Common::CTextUtils::ReplaceEscapeCharacters
 *
 * Description
 * Replaces all occurrences of certain escape characters with spaces
 *
 * Inputs
 * string inputString - input string
 *
 * Returns
 * String with all escape characters \a\b\f\r\v replaced to space
 */
string Summary_Common::CTextUtils::ReplaceEscapeCharacters(const string &inputString)
{
    string localInput = inputString;

    localInput = CTextUtils::ReplaceAnyChar(localInput, "\a\b\f\r\v", ' ');

    return localInput;
}

/*
 * Summary_Common::CTextUtils::ReplaceEscapeCharacters
 *
 * Description
 * Replaces all occurrences of certain escape characters with spaces
 *
 * Inputs
 * string inputString - input string
 *
 * Returns
 * String with all escape characters \a\b\f\r\v replaced to space
 */
wstring Summary_Common::CTextUtils::ReplaceEscapeCharacters(const wstring &inputString)
{
    wstring localInput = inputString;

    localInput = CTextUtils::ReplaceAnyChar(localInput, L"\a\b\f\r\v", ' ');

    return localInput;
}

/*
 * Summary_Common::CTextUtils::ReplaceNonAlphanumeric
 *
 * Description
 * Replaces all occurrences of one non alphanumeric character to another within string
 *
 * Inputs
 * string inputString - input string
 * char charToReplace - character to be replaced
 *
 * Returns
 * String with all non alphanumeric characters replaced to other characters
 */
string Summary_Common::CTextUtils::ReplaceNonAlphanumeric(string inputString, char charToReplace)
{
    size_t i = 0;
    size_t len = inputString.length();
    while(i < len)
    {
        if (!isalnum(inputString[i]))
        {
            inputString[i] = charToReplace;
        }

        i++;
    }
    return inputString;
}

/*
 * Summary_Common::CTextUtils::ReplaceNonAlphanumeric
 *
 * Description
 * Replaces all occurrences of one non alphanumeric character to another within string except for those in
 * keepChars vector
 *
 * Inputs
 * string inputString - input string
 * char charToReplace - character to be replaced
 * const vector<char> &keepChars - non alphanumeric characters to keep
 *
 * Returns
 * String with all non alphanumeric characters replaced to other characters
 */
string Summary_Common::CTextUtils::ReplaceNonAlphanumeric(string inputString, char charToReplace, const vector<char> &keepChars)
{
    size_t i = 0;
    size_t len = inputString.length();
    while(i < len)
    {
        bool keepChar = false;
        if (!isalnum(inputString[i]))
        {
            for (char charToKeep: keepChars)
            {
                if (charToKeep == inputString[i])
                {
                    keepChar = true;
                    break;
                }
            }

            if (!keepChar)
            {
                inputString[i] = charToReplace;
            }
        }

        i++;
    }
    return inputString;
}

/*
 * Summary_Common::CTextUtils::RemoveUnknownCharacters
 *
 * Description
 * Removes characters that may be not seen but may cause trouble in string find and other
 * operations. For example �
 *
 * Inputs
 * string inputString - input string
 *
 * Returns
 * String without unknown characters like �
 */
string Summary_Common::CTextUtils::RemoveUnknownCharacters(string inputString)
{
    //std::string inputStringUTF8 = boost::locale::conv::to_utf<char>(inputString,"Latin1");
    //locale localeUTF8("en_US.UTF-8");

    size_t i = 0;
    //wstring convertedString = widen(inputString, localeUTF8);
    size_t len = inputString.length();
    //cout << convertedString.length() << " " << inputString.length() << endl;

    while(i < len)
    {
        if (static_cast<int>(inputString[i]) < 0 /* !isalnum(convertedString[i], localeUTF8)*/)
        {
            inputString.erase(i, 1);
            len--;
        }
        else
        {
            i++;
        }
    }
    return ReplaceEscapeCharacters(inputString);//narrow(convertedString, localeUTF8);
}

/*
 * Summary_Common::CTextUtils::FirstLetterUpCase
 *
 * Description
 * Gets string with first letter in upper case
 *
 * Inputs
 * string input - input string
 *
 * Returns
 * String with first letter in upper case
 */
string Summary_Common::CTextUtils::FirstLetterUpCase(string input)
{
    string output = input;

    if (output.length() > 0)
    {
        output[0] = toupper(output[0]);
    }

    return output;
}

/*
 * Summary_Common::CTextUtils::OnlyFirstLetterUpCase
 *
 * Description
 * Gets string with first letter in upper case and others in lower case for each word in string
 *
 * Inputs
 * string input - input string
 *
 * Returns
 * String with first letter in upper case and others in lower case for each word in string
 */
string Summary_Common::CTextUtils::OnlyFirstLetterUpCase(string input)
{
    string output = input;
    bool isWordStart = true;

    if (output.length() > 0)
    {
        for (int i = 0; i < input.length(); i++)
        {
            //cout << input[i] << endl;
            //cout << static_cast<int>(input[i]) << endl;
            if (isWordStart)
            {
                output[i] = toupper(output[i]);
                isWordStart = false;
            }
            else
            {
                output[i] = tolower(output[i]);
            }

            if (output[i] == ' ' ||
                    (output[i] == 95 ) ||                               //Low line
                    (output[i] == 45 )                                  //Hyphen-minus
               )
            {
                isWordStart = true;
            }

            //TODO: validate more codes
            if (i > 0 &&
                    (
                        (output[i] == -81 && output[i - 1] == -62) ||   //Macron
                        (output[i] == -119 && output[i - 1] == -53) ||  //Modifier letter macron
                        (output[i] == -115 && output[i - 1] == -53) ||  //Modifier letter low macron
                        (output[i] == -124 && output[i - 1] == -52) ||  //Combining macron
                        (output[i] == -125 && output[i - 1] == -52) ||  //Combining overline
                        (output[i] == -79 && output[i - 1] == -52) ||   //Combining macron below
                        (output[i] == -78 && output[i - 1] == -52) ||   //Combining low line
                        (output[i] == -75 && output[i - 1] == -52) ||   //Combining short stroke overlay
                        (output[i] == -74 && output[i - 1] == -52) ||   //Combining long stroke overlay
                        (output[i] == -98 && output[i - 1] == -51) ||   //Combining double macron
                        (output[i] == -97 && output[i - 1] == -51) ||   //Combining double macron below
                        (0==1)
                    )
                )
            {
                isWordStart = true;
            }

            //TODO: validate more codes
            if (i > 1 &&
                    (
                            (output[i] == -112 && output[i - 1] == -128 && output[i - 2] == -30) ||
                            (output[i] == -111 && output[i - 1] == -128 && output[i - 2] == -30) ||
                            (output[i] == -110 && output[i - 1] == -128 && output[i - 2] == -30) ||
                            (output[i] == -109 && output[i - 1] == -128 && output[i - 2] == -30) ||
                            (output[i] == -108 && output[i - 1] == -128 && output[i - 2] == -30) ||
                            (output[i] == -107 && output[i - 1] == -128 && output[i - 2] == -30) ||
                            (output[i] == -66 && output[i - 1] == -128 && output[i - 2] == -30) ||
                            (output[i] == -125 && output[i - 1] == -127 && output[i - 2] == -30) ||
                            (output[i] == -69 && output[i - 1] == -127 && output[i - 2] == -30) ||
                            (output[i] == -117 && output[i - 1] == -126 && output[i - 2] == -30) ||
                            (output[i] == -110 && output[i - 1] == -120 && output[i - 2] == -30)
                    )
                )
            {
                isWordStart = true;
            }
        }

    }

    return output;
}

/*
 * Summary_Common::CTextUtils::IsQuotation
 *
 * Description
 * Returns true if selected character is double quote character
 *
 * Inputs
 * char c - input character
 *
 * Returns
 * True on double quote character
 */
bool Summary_Common::CTextUtils::IsQuotation(char c)
{
    switch(c)
    {
        case '"':
        {
            return true;
        }
        default:
        {
            return false;
        }
    }
}

/* ===================================================================================================
 *
 * This section is for conversion methods to and from widestring, methods are not in use at the moment
 */

//This function is not working if input string has special symbols like  in it
wstring Summary_Common::CTextUtils::WideString(string const& s, locale loc)
{
    char_traits<wchar_t>::state_type state = { 0 };
    //cout << "R:" << s << endl;
    typedef codecvt<wchar_t, char, char_traits<wchar_t>::state_type >
            ConverterFacet;

    ConverterFacet const& converter(use_facet<ConverterFacet>(loc));

    char const* nextToRead = s.data();
    wchar_t buffer[BUFSIZ];
    wchar_t* nextToWrite;
    codecvt_base::result result;
    wstring wresult;

    while ((result
                    = converter.in
                    (state,
                     nextToRead, s.data()+s.size(), nextToRead,
                     buffer, buffer+sizeof(buffer)/sizeof(*buffer), nextToWrite))
           == codecvt_base::partial)
    {
        wresult.append(buffer, nextToWrite);
    }

    if (result == codecvt_base::error)
    {
        throw runtime_error("Encoding error");
    }
    wresult.append(buffer, nextToWrite);
    return wresult;
}

//Not tested well
string Summary_Common::CTextUtils::NarrowString(wstring const& text, locale loc)
{
    //std::locale const loc("");
    wchar_t const* from = text.c_str();
    size_t const len = text.size();
    vector<char> buffer(len + 1);
    use_facet<ctype<wchar_t> >(loc).narrow(from, from + len, '_', &buffer[0]);
    return string(&buffer[0], &buffer[len]);
}

/* ===================================================================================================
 *
 * This section is for testing with different text search algorithms, methods are not actively in use
 */
/*
vector<size_t> knuthMorrisPrattTable(const string & needle);
size_t knuthMorrisPrattSearch(const string & haystack, const string & needle, const vector<size_t> & table);
bool offsetMatch(const string & haystack, const string & needle, size_t offset);
size_t rabinKarpMatcher(const string & haystack, const string & needle);
size_t naiveSearch(const string & haystack, const string & needle);
const char*  volnitsky(const char*  S,  const char*  Se,  const char*  SS,  const char*  SSe);


vector<size_t> knuthMorrisPrattTable(const string & needle)
{
    vector<size_t> table(needle.size() + 1, -1);
    for(size_t index = 1; index <= needle.size(); index++)
    {
        size_t position = table[index - 1];

        while(position != -1 && needle[position] != needle[index - 1])
            position = table[position];

        table[index] = position + 1;
    }

    return table;
}


size_t knuthMorrisPrattSearch(const string & haystack, const string & needle, const vector<size_t> & table)
{
    size_t haystackIndex = 0;
    size_t needleIndex = 0;

    size_t haystackSize = haystack.size();
    size_t needleSize = needle.size();

    while(haystackIndex < haystackSize)
    {
        while(needleIndex != -1 && (needleIndex == needleSize || needle[needleIndex] != haystack[haystackIndex]))
            needleIndex = table[needleIndex];

        needleIndex++;
        haystackIndex++;

        if(needleIndex == needleSize)
            return (haystackIndex - needleSize);
    }

    return string::npos;
}

bool offsetMatch(const string & haystack, const string & needle, size_t offset)
{
    size_t needleCount = needle.size();
    size_t index;

    for(index = 0; index < needleCount && needle[index] == haystack[offset + index]; index++);

    return index == needleCount;
}

size_t rabinKarpMatcher(const string & haystack, const string & needle)
{
    if (needle.empty())
        return std::string::npos;
    else if (haystack.empty())
        return std::string::npos;
    else if (needle.size() > haystack.size())
        return std::string::npos;

    static const uint64_t radixLength = 256ULL;
    static const uint64_t prime = 72057594037927931ULL;

    //vector<size_t> matches;

    size_t needleLength = needle.size();
    size_t haystackLength = haystack.size();
    size_t lastIndex = haystackLength - needleLength;

    uint64_t differenceHash = pow(radixLength, (uint64_t)(needleLength - 1));// % prime;
    differenceHash = differenceHash % prime;

    size_t needleHash = 0;
    size_t firstHaystackHash = 0;

    size_t index;

    //cout << haystack << endl;
    //cout << needle << endl;

    // preprocessing
    for(index = 0; index < needleLength; index++)
    {
        needleHash = (radixLength * needleHash + needle[index]) % prime;
        //cout << index << " " << needleHash << endl;
        firstHaystackHash = (radixLength * firstHaystackHash + haystack[index]) % prime;
    }

    vector<uint64_t> haystackHashes;
    haystackHashes.reserve(lastIndex + 1);
    haystackHashes.push_back(firstHaystackHash);

    // matching
    for(index = 0; index <= lastIndex; index++)
    {
        if(needleHash == haystackHashes[index])
        if(offsetMatch(haystack, needle, index))
            return index;

        if(index < lastIndex)
        {
            uint64_t newHaystackHash = (radixLength * (haystackHashes[index] - haystack[index] * differenceHash) + haystack[index + needleLength]) % prime;
            haystackHashes.push_back(newHaystackHash);
        }
    }

    return string::npos;
}

size_t naiveSearch(const string & haystack, const string & needle)
{
    // thanks Jeff!
    if(needle.size() > haystack.size())
        return string::npos;

    size_t needleSize = needle.size();
    size_t maximumIndex = haystack.size() - needleSize;

    size_t needleIndex;
    for(size_t haystackIndex = 0; haystackIndex <= maximumIndex; haystackIndex++)
    {
        for(needleIndex = 0; needleIndex < needleSize && needle[needleIndex] == haystack[haystackIndex + needleIndex]; needleIndex++);

        if(needleIndex == needleSize)
            return haystackIndex;
    }

    return string::npos;
}

const char*  volnitsky(const char*  S,  const char*  Se,  const char*  SS,  const char*  SSe)
{
    // only for little-endian platforms and where access to misaligned W is allowed

    typedef         uint16_t        W_t;
    typedef         uint8_t         offset_t;

    const size_t    W_size   = sizeof(W_t);
    const size_t    S_size   = Se - S;
    const size_t    SS_size  = SSe - SS;
    const size_t    step     = SS_size - W_size + 1;

    // args sizes check
    if  (
            S_size   <   20000       //  see startup costs;  algo limit: 2*SS_size
            ||  SS_size  <   2*W_size-1
            ||  SS_size  >=  std::numeric_limits<offset_t>::max()
            )
        return  std::search(S,Se,SS,SSe);       // fall-back to std::search

    // make hash
    const size_t    H_size     = 64*1024;
    offset_t        H[H_size]  = {0};       // {0} - initializes ALL buckets with 0
    for  ( int  i = SS_size-W_size;    i >= 0;   i-- )
    {
        size_t  h  =  *(W_t*) (SS+i)  % H_size;
        while (H[h])    h = (h+1) % H_size;             // find free cell
        H[h] = i+1;
    }

    // step through text
    const char*   P = S + SS_size - W_size;
    for  (;   P <= Se-SS_size;   P += step)
    {
        for  (size_t  h = *(W_t*) P  %  H_size;    H[h];    h = (h+1) % H_size )
        {
            const char*    R  =  P - (H[h] - 1);
            for  ( size_t i=0;  i<SS_size;  i++)
            {
                if  (R[i] != SS[i])   goto next_hash_cell;
            }
            return R;                       // found
            next_hash_cell:;
        }
    }

    // check tail
    return  std::search(P-step+1,Se,SS,SSe);
};
*/

/* ===================================================================================================
 *
 * Boyer-Moore-Hoorspool
 *
 *
 */
/*
 * Originally written by Joel Yliluoma <joel.yliluoma@iki.fi>
 * http://en.wikipedia.org/wiki/Talk:Boyer%E2%80%93Moore_string_search_algorithm#Common_functions
 *
 * Copyright (c) 2008 Joel Yliluoma
 * Copyright (c) 2010 Hongli Lai
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */
//typedef std::vector<size_t> occtable_type;

/* This function creates an occ table to be used by the search algorithms. */
/* It only needs to be created once per a needle to search. */
/*
const occtable_type
CreateOccTable(const unsigned char* needle, size_t needle_length)
{
    occtable_type occ(UCHAR_MAX+1, needle_length); // initialize a table of UCHAR_MAX+1 elements to value needle_length

    // Populate it with the analysis of the needle
    // But ignoring the last letter
    if(needle_length >= 1)
    {
        const size_t needle_length_minus_1 = needle_length-1;
        for(size_t a=0; a<needle_length_minus_1; ++a)
            occ[needle[a]] = needle_length_minus_1 - a;
    }
    return occ;
}*/

/* A Boyer-Moore-Horspool search algorithm. */
/* If it finds the needle, it returns an offset to haystack from which
 * the needle was found. Otherwise, it returns haystack_length.
 */
/*
size_t SearchInHorspool(const unsigned char* haystack, size_t haystack_length,
                        const occtable_type& occ,
                        const unsigned char* needle,
                        const size_t needle_length)
{
    if(needle_length > haystack_length) return haystack_length;
    if(needle_length == 1)
    {
        const unsigned char* result = (const unsigned char*)std::memchr(haystack, *needle, haystack_length);
        return result ? size_t(result-haystack) : haystack_length;
    }

    const size_t needle_length_minus_1 = needle_length-1;

    const unsigned char last_needle_char = needle[needle_length_minus_1];

    size_t haystack_position=0;
    while(haystack_position <= haystack_length-needle_length)
    {
        const unsigned char occ_char = haystack[haystack_position + needle_length_minus_1];

        // The author modified this part. Original algorithm matches needle right-to-left.
        // This code calls memcmp() (usually matches left-to-right) after matching the last
        // character, thereby incorporating some ideas from
        // "Tuning the Boyer-Moore-Horspool String Searching Algorithm"
        // by Timo Raita, 1992.
        if(last_needle_char == occ_char
           && std::memcmp(needle, haystack+haystack_position, needle_length_minus_1) == 0)
        {
            return haystack_position;
        }

        haystack_position += occ[occ_char];
    }
    return haystack_length;
}*/

/*
 * Summary_Common::CTextUtils::Char2Hex
 *
 * Description
 * Returns two-digit hex code
 *
 * Inputs
 * char dec - input character
 *
 * Returns
 * String representation of hex
 */
string Summary_Common::CTextUtils::Char2Hex(char dec)
{
    char dig1 = (dec&0xF0)>>4;
    char dig2 = (dec&0x0F);
    if ( 0<= dig1 && dig1<= 9) dig1+=48;            //0,48inascii
    if (10<= dig1 && dig1<=15) dig1+=97-10;         //a,97inascii
    if ( 0<= dig2 && dig2<= 9) dig2+=48;
    if (10<= dig2 && dig2<=15) dig2+=97-10;

    string r;
    r.append( &dig1, 1);
    r.append( &dig2, 1);

    return r;
}

/*
 * Summary_Common::CTextUtils::UrlEncode
 *
 * Description
 * Returns url-encoded string
 *
 * Inputs
 * const string &inputString - string to be encoded. Can be UTF-8 string
 *
 * Returns
 * Url-encoded string
 */
string Summary_Common::CTextUtils::UrlEncode(const string &inputString)
{
    string escaped="";
    int max = inputString.length();
    for(int i=0; i<max; i++)
    {
        if ( (48 <= inputString[i] && inputString[i] <= 57) ||//0-9
             (65 <= inputString[i] && inputString[i] <= 90) ||//abc...xyz
             (97 <= inputString[i] && inputString[i] <= 122) || //ABC...XYZ
             (inputString[i]=='~' || inputString[i]=='!' || inputString[i]=='*' ||
                     inputString[i]=='(' || inputString[i]==')' || inputString[i]=='\'')
                )
        {
            escaped.append( &inputString[i], 1);
        }
        else if (inputString[i] == '\n')
        {
            escaped.append("%0d%0a");  //The 0d carriage return is needed for having translated result on multiline
        }
        else
        {
            escaped.append("%");
            escaped.append( Char2Hex(inputString[i]) );//converts char 255 to string "ff"
        }
    }
    return escaped;
}

/*
 * Summary_Common::CTextUtils::HtmlDecode
 *
 * Description
 * Returns string with HTML characters decoded from for example &amp to & etc.
 *
 * Inputs
 * const string &inputString - string to be HTML decoded
 *
 * Returns
 * HTML-decoded string
 */
string Summary_Common::CTextUtils::HtmlDecode(const string &inputString)
{
    //string html = inputString;
    string htmlDecoded;

    char *htmlDecodedChars = new char[inputString.length() + 1];

    decode_html_entities_utf8(htmlDecodedChars, inputString.c_str());

    htmlDecoded = htmlDecodedChars;

    delete htmlDecodedChars;

    return inputString;
}

/*
 * Summary_Common::CTextUtils::StringCount
 *
 * Description
 * Returns string with HTML characters decoded from for example &amp to & etc.
 *
 * Inputs
 * const string &strHaystack - string to search value for
 * const string &strNeedle - string value we count in first string
 *
 * Returns
 * needle count in haystack
 */
long Summary_Common::CTextUtils::StringCount(const string &strHaystack, const string &strNeedle)
{
    string::size_type stringPosition = CTextUtils::FindString(strHaystack, strNeedle, -1);
    int count = 0;

    while (stringPosition != -1)
    {
        stringPosition = CTextUtils::FindString(strHaystack, strNeedle, stringPosition + strNeedle.size());
        count++;
    }

    return count;
}

/*
 * Summary_Common::CTextUtils::SortMapByCounts
 *
 * Description
 * Sorts map string keys and returns string vector where key=value pair's value is the biggest
 *
 * Inputs
 * const map<string, int> &mapWithCounts - map<string, int>, map<string, long> etc.
 * int maxResults - maximum results in output
 *
 * Returns
 * Sorted input map keys by value result
 */
vector<string> Summary_Common::CTextUtils::SortMapByCounts(const map<string, int> &mapWithCounts, int maxResults)
{
    vector<string> popularItems;
    vector<pair<string,int>> items(mapWithCounts.begin(), mapWithCounts.end());
    int count = 0;

    auto cmp = [](pair<string,int> const & a, pair<string,int> const & b)
    {
        return a.second != b.second?  a.second > b.second : a.first > b.first;
    };

    sort(items.begin(), items.end(), cmp);

    for(vector<pair<string,int>>::const_iterator it = items.begin(); it != items.end(); ++it)
    {
        //cout << it->first << " " << it -> second << "\n";
        popularItems.push_back(it -> first);
        count ++;

        if (count >  maxResults)
        {
            break;
        }
    }

    return popularItems;
}

/*
 * Summary_Common::CTextUtils::SortMapByCounts
 *
 * Description
 * Sorts map string keys and returns map where key=value pair's value is the biggest
 *
 * Inputs
 * const map<string, int> &mapWithCounts - map<string, int>, map<string, long> etc.
 * int maxResults - maximum results in output
 *
 * Returns
 * Sorted input map keys, values by value result
 */
map<string, int> Summary_Common::CTextUtils::GetSortedMapByCounts(const map<string, int> &mapWithCounts, int maxResults)
{
    vector<string> popularItems;
    vector<pair<string,int>> items(mapWithCounts.begin(), mapWithCounts.end());
    map<string, int> returnMap;
    int count = 0;

    auto cmp = [](pair<string,int> const & a, pair<string,int> const & b)
    {
        return (int)a.second != (int)b.second?  (int)a.second > (int)b.second : a.first.compare(b.first) > 0;
    };

    sort(items.begin(), items.end(), cmp);

    for(vector<pair<string,int>>::const_iterator it = items.begin(); it != items.end(); ++it)
    {
        //cout << it->first << " " << it -> second << "\n";
        returnMap.insert(pair<string,int>(it->first, it->second));
        count ++;

        if (count >  maxResults)
        {
            break;
        }
    }

    return returnMap;
}

/*
 * Summary_Common::CTextUtils::VectorContainsMutually
 *
 * Description
 * Goes through haystack vector values and sees if any of the values contains the needle
 * or the needle contains any of the vector values
 *
 * Inputs
 * const vector<string> &haystack - vector of string values
 * const string &needle - needle to search for
 * bool alreadyLowerCase - if true then string vector is in lower case
 *
 * Returns
 * true if needle contains in vector or any vector values contain needle
 */
bool Summary_Common::CTextUtils::VectorContainsMutually(const vector<string> &haystack,
                                                        const string &needle,
                                                        bool alreadyLowerCase)
{
    bool contains = false;
    string needleLower = CTextUtils::ToLower(needle);

    for (string needleCandidate: haystack)
    {
        string needleCandidateLower = needleCandidate;

        if (!alreadyLowerCase)
        {
            needleCandidateLower = CTextUtils::ToLower(needleCandidate);
        }

        //This profile already contained in profiles list
        if (CTextUtils::StringContains(needleCandidateLower, needleLower))
        {
            contains = true;
            break;
        }

        //New profile contains one of the older, replace older with longer new profile
        if (CTextUtils::StringContains(needleLower, needleCandidateLower))
        {
            contains = true;
            break;
        }
    }

    return contains;
}

/*
 * Summary_Common::CTextUtils::RemoveUnusedSpaces
 *
 * Description
 * Removes unused spaces and tabs, keeping only one space between each words
 *
 * Inputs
 * const string &inputString - input string
 *
 * Returns
 * Words with only one space between each word
 */
string Summary_Common::CTextUtils::RemoveUnusedSpaces(const string &inputString)
{
    if (inputString.find(" ") != string::npos)
    {
        vector<string> splits = Split(inputString, ' ');
        string returnString = "";

        for (string part: splits)
        {
            if (part.length() < 1)
            {
                continue;
            }

            returnString += (returnString.length() > 0 ? " ": "");
            returnString += CTextUtils::TrimAnyChar(part, "\t");
        }

        return returnString;
    }
    else
    {
        return inputString;
    }
}

/*
 * Summary_Common::CTextUtils::CPersonalParser::TrimNameCharacters
 *
 * Description
 * Removes unwanted characters from the text and trims from space, -, nonb space and macron
 *
 * Inputs
 * const string &name - input name text
 *
 * Returns
 * changed text string
 */
string Summary_Common::CTextUtils::TrimNameCharacters(const string &input)
{
    string localInput = input;
    wstring inputStringUTF8 = boost::locale::conv::to_utf<wchar_t>(localInput, "UTF-8");
    bool canEraseDoubleQuotes = true;

    //localName = CTextUtils::ReplaceAnyChar(localName, "&:+;./()[]{}\\\t\"“”¡¢£¤¥¦§¨©ª", ' ');
    //Many different - signs TODO: replace with code equivalents
    /*if (localInput.find("’") != string::npos ||
            localInput.find("–") != string::npos||
            localInput.find("—") != string::npos ||
            localInput.find("‘") != string::npos ||
            localInput.find("-") != string::npos)
    {
        canEraseDoubleQuotes = false;
    }
    //localName = CTextUtils::ReplaceAnyChar(localName, "’", '\'');

    //localName = CTextUtils::ReplaceAnyChar(localName, "\'\'", '\'');

    localInput = CTextUtils::ReplaceAnyChar(localInput, "&:+;./()[]{}\\\t\"^?>=<+*&%$#!_", ' ');
                                                                            //Do not remove just any character
                                                                            //for those can be used in combination
    localInput = CTextUtils::ReplaceEscapeCharacters(localInput);

    if (canEraseDoubleQuotes)
    {
        localInput = CTextUtils::ReplaceAnyChar(localInput, "“”", ' ');
    }

    //localName = CTextUtils::ReplaceAnyChar(localName, "«¬®°±²³µ¶·¸ ¹º»¼ ½¾¿ × ÷ ~ ^?>=<+*&%$#!_", ' ');
    //localName = CTextUtils::ReplaceAnyChar(localName, "’´`’", '\'');
    localInput = CTextUtils::Replace(localInput, ',', ' ');  //In question if name is in format lastname comma firstname
    localInput = CTextUtils::TrimAnyChar(localInput, "-");   //Minus and macron
    localInput = CTextUtils::TrimAnyChar(localInput, "–");
    localInput = CTextUtils::TrimAnyChar(localInput, "  ");  //Space plus possibly nonb space
     */

    inputStringUTF8 = CTextUtils::ReplaceAnyChar(inputStringUTF8, L"…&:+;./()[]{}\\\t\"^?>=<+*&%$#!_", ' ');
    inputStringUTF8 = CTextUtils::ReplaceAnyChar(inputStringUTF8, L"’´`’’", '\'');
    inputStringUTF8 = CTextUtils::ReplaceEscapeCharacters(inputStringUTF8);
    inputStringUTF8 = CTextUtils::ReplaceAnyChar(inputStringUTF8, L"“”", ' ');

    inputStringUTF8 = CTextUtils::Replace(inputStringUTF8, ',', ' ');  //In question if name is in format lastname comma firstname
    inputStringUTF8 = CTextUtils::TrimAnyChar(inputStringUTF8, L"-");   //Minus and macron
    inputStringUTF8 = CTextUtils::TrimAnyChar(inputStringUTF8, L"–");
    inputStringUTF8 = CTextUtils::TrimAnyChar(inputStringUTF8, L"  ");  //Space plus possibly nonb space

    localInput = boost::locale::conv::from_utf<wchar_t>(inputStringUTF8, "UTF-8");

    localInput = CTextUtils::Replace(localInput, ',', ' ');  //In question if name is in format lastname comma firstname
    localInput = CTextUtils::TrimAnyChar(localInput, "-");   //Minus and macron
    localInput = CTextUtils::TrimAnyChar(localInput, "–");
    localInput = CTextUtils::TrimAnyChar(localInput, "  ");  //Space plus possibly nonb space

    return localInput;
}

/*
 * Summary_Common::CTextUtils::AddVectorMutuallyExclusively
 *
 * Description
 * Adds string to vector if vector values does not contain this string and string does not
 * contain any vector value
 *
 * Inputs
 * const vector<string> &haystack - vector of string values
 * const string &needle - needle to search for
 * bool alreadyLowerCase - if true then string vector is in lower case
 *
 * Returns
 * true if needle contains in vector or any vector values contain needle
 */
void Summary_Common::CTextUtils::AddVectorMutuallyExclusively(vector<string> &array, const string &newString)
{
    bool canAdd = true;
    string trimmedNewString = CTextUtils::Trim(newString);

    for (string degree: array)
    {
        string trimmedDegree = CTextUtils::Trim(degree);

        if (CTextUtils::StringContainsMutually(degree, trimmedNewString))
        {
            canAdd = false;

            break;
        }
    }

    if (canAdd)
    {
        array.push_back(trimmedNewString);
    }
}
