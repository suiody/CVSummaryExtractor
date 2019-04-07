//
// File JsonOutputFormatter.cpp
//
//

#include <iostream>
#include "JsonOutputFormatter.h"
#include "../Common/CTextUtils.h"
#include "../Common/CDebugUtils.h"

using namespace Summary_Common;

/*
 * Summary_Json::JsonOutputFormatter::Format
 *
 * Description
 * Reads data of the resume and outputs results as json to standard output
 *
 * Inputs
 * const CResume &resume - resume data
 *
 * Returns
 * Empty string
 */
string Summary_Json::JsonOutputFormatter::Format(const CResume &resume)
{
    CDebugUtils::DebugCout("JsonOutputFormatter", "Format", "");
    map<string, int> popularWordsMap = resume.GetWordsMapByPopularity();
    map<string, int> popularWordPairsMap = resume.GetWordPairsMapByPopularity();
    int index = 0;
    int positionCount = resume.GetPositions().size();
    int projectCount = resume.GetProjects().size();
    int educationCount = resume.GetEducations().size();
    int schoolCount = resume.GetSchools().size();
    int wordsMapCount = popularWordsMap.size();
    int wordPairsMapCount = popularWordPairsMap.size();

    cout << "{\n";
    AddString("\t", "first_name", resume.GetFirstName(), "\n", false);
    AddString("\t", "middle_name", resume.GetMiddleName(), "\n", false);
    AddString("\t", "last_name", resume.GetLastName(), "\n", false);
    AddString("\t", "birthday", resume.GetBirthDay(), "\n", false);
    AddString("\t", "gender", resume.GetGender(), "\n", false);
    AddString("\t", "email", resume.GetEmailAddress(), "\n", false);
    AddString("\t", "phone", resume.GetPhoneNumbers(), "\n", false);
    AddString("\t", "skype", resume.GetSkype(), "\n", false);
    AddString("\t", "website_url", resume.GetWebsite(), "\n", false);
    AddString("\t", "facebook_url", resume.GetFacebook(), "\n", false);
    AddString("\t", "twitter_url", resume.GetTwitter(), "\n", false);
    AddString("\t", "googleplus_url", resume.GetGooglePlus(), "\n", false);
    AddString("\t", "linkedin_url", resume.GetLinkedIn(), "\n", false);
    AddString("\t", "instagram_url", resume.GetInstagram(), "\n", false);
    AddStringList("\t", "\t", "social_profiles", resume.GetSocialProfiles(), "\n", false);
    AddStringList("\t", "\t", "url_candidates", resume.GetUrlCandidates(), "\n", false);
    AddString("\t", "title", resume.GetTitle(), "\n", false);
    AddString("\t", "religion", resume.GetReligion(), "\n", false);
    AddString("\t", "languages", resume.GetLanguages(), "\n", false);
    AddString("\t", "text_language_code", resume.GetTextLanguageCode(), "\n", false);
    AddString("\t", "text_language_name", resume.GetTextLanguageName(), "\n", false);
    AddString("\t", "description", resume.GetSummaryDescription(), "\n", false);
    AddStringList("\t", "\t", "skills", resume.GetSkillsByPopularity(), "\n", false);

    AddWordCountMap("words", popularWordsMap);
    AddWordCountMap("word_pairs", popularWordPairsMap);

    AddString("\t", "country", resume.GetCountry(), "\n", false);
    AddString("\t", "location", resume.GetLocation(), "\n", false);
    AddArrayHeader("\t", "positions", positionCount, "\n");
    index = 0;
    for (CPosition position: resume.GetPositions())
    {
        AddStartBrace("\t\t", "\n");
        AddString("\t\t\t", "title", position.GetTitle(), "\n", false);
        AddString("\t\t\t", "date_from", position.GetStartDate(), "\n", false);
        AddString("\t\t\t", "date_to", position.GetEndDate(), "\n", false);
        AddString("\t\t\t", "company", position.GetCompany(), "\n", false);
        AddStringList("\t\t\t", "\t", "summary", position.GetSummary(), "\n", true);
        //cout << "\t\t" << "}"<< (index < positionCount - 1 ? "," : "") << "\n";
        AddEndBrace("\t\t", index, positionCount, "\n");
        index++;
    }
    AddArrayFooter("\t", positionCount, "\n", false);
    index = 0;
    AddArrayHeader("\t", "projects", projectCount, "\n");
    for (CProject project: resume.GetProjects())
    {
        //cout << "\t\t" << "{" << "\n";
        AddStartBrace("\t\t", "\n");
        AddString("\t\t\t", "title", project.GetTitle(), "\n", false);
        AddString("\t\t\t", "date_from", project.GetStartDate(), "\n", false);
        AddString("\t\t\t", "date_to", project.GetEndDate(), "\n", false);
        AddStringList("\t\t\t", "\t", "summary", project.GetSummary(), "\n", true);
        AddEndBrace("\t\t", index, projectCount, "\n");
        index++;
    }
    AddArrayFooter("\t", projectCount, "\n", false);
    //AddStringList("\t", "\t", "education_degrees", resume.GetEducationDegrees(), "\n", false);
    /*AddStringList("\t", "\t", "school_degrees", resume.GetSchoolDegrees(), "\n", false);
    AddStringList("\t", "\t", "school_fields", resume.GetSchoolFields(), "\n", false);
    AddStringList("\t", "\t", "school_names", resume.GetSchoolNames(), "\n", false);*/
    index = 0;
    AddArrayHeader("\t", "educations", educationCount, "\n");
    for (CEducation entity: resume.GetEducations())
    {
        AddStartBrace("\t\t", "\n");
        AddString("\t\t\t", "school", entity.GetSchool(), "\n", false);
        AddString("\t\t\t", "course", entity.GetCourse(), "\n", false);
        AddString("\t\t\t", "date_from", entity.GetStartDate(), "\n", false);
        AddString("\t\t\t", "date_to", entity.GetEndDate(), "\n", true);
        AddEndBrace("\t\t", index, resume.GetEducations().size(), "\n");
        index++;
    }
    AddArrayFooter("\t", educationCount, "\n", false);

    index = 0;
    /*AddArrayHeader("\t", "schools", schoolCount, "\n");
    for (CSchoolData entity: resume.GetSchools())
    {
        AddStartBrace("\t\t", "\n");
        AddString("\t\t\t", "school_name", entity.GetSchoolName(), "\n", false);
        AddString("\t\t\t", "school_degree", entity.GetSchoolDegree(), "\n", false);
        AddString("\t\t\t", "school_field", entity.GetSchoolField(), "\n", false);
        AddString("\t\t\t", "date_from", entity.GetDateFrom(), "\n", false);
        AddString("\t\t\t", "date_to", entity.GetDateTo(), "\n", true);

        AddEndBrace("\t\t", index, resume.GetSchools().size(), "\n");
        index++;
    }
    AddArrayFooter("\t", schoolCount, "\n", false);*/
    AddStringList("\t", "\t", "courses", resume.GetCourses(), "\n", false);
    AddStringList("\t", "\t", "awards", resume.GetAwards(), "\n", false);
    AddLong("\t", "resume_length", resume.GetTextLength(), "\n", false);
    AddStringList("\t", "\t", "input", resume.GetRawInput(), "\n", false);
    AddStringList("\t", "\t", "translation", resume.GetTranslation(), "\n", true);
    cout << "}\n";
    //AddStringList("\t", "\t", "skills", resume.GetSkillsByPopularity(), "\n", false);
    //AddLong("\t", "resume_length", resume.GetTextLength(), "\n", false);
    //AddStringList("\t", "\t", "input", resume.GetRawInput(), "\n", true);
    //AddString("\t", "title", resume.GetTitle(), "\n", false);
    /*if (resume.GetTitle().length() > 0)
    {
        cout << "+++" << endl;
    }*/
    return "";
}

/*
 * Summary_Json::JsonOutputFormatter::AddString
 *
 * Description
 * Adds field name and value with prefix and suffix and comma after the string if it is not the last element in list
 * For example "school":"Banha University",
 *
 * Inputs
 * string prefix - prefix, typically a tab or tabs
 * string fieldName - field name, will be surrounded in double quotes
 * string value - field value, will be surrounded in double quotes
 * string suffix - suffix, typically a new line
 * bool lastElement - if true then no comma added, else comma will be added before suffix
 *
 * Returns
 * Outputs result to standard output
 *
 */
void Summary_Json::JsonOutputFormatter::AddString(string prefix, string fieldName, string value, string suffix,
                                                  bool lastElement)
{
    cout << prefix << "\"" << CTextUtils::RemoveQuotation(fieldName) << "\":" << "\"" <<
                              CTextUtils::RemoveQuotation(value) << "\"" << (lastElement ? "" : ",") << suffix;
}

/*
 * Summary_Json::JsonOutputFormatter::AddLong
 *
 * Description
 * Adds field name and value with prefix and suffix and comma after the string if it is not the last element in list
 * For example "school":"Banha University",
 *
 * Inputs
 * string prefix - prefix, typically a tab or tabs
 * string fieldName - field name, will be surrounded in double quotes
 * string value - field value, will be surrounded in double quotes
 * string suffix - suffix, typically a new line
 * bool lastElement - if true then no comma added, else comma will be added before suffix
 *
 * Returns
 * Outputs result to standard output
 *
 */
void Summary_Json::JsonOutputFormatter::AddLong(string prefix, string fieldName, long value, string suffix,
                                                  bool lastElement)
{
    cout << prefix << "\"" << CTextUtils::RemoveQuotation(fieldName) << "\":" << "\"" << value << "\"" <<
            (lastElement ? "" : ",") << suffix;
}

/*
 * Summary_Json::JsonOutputFormatter::AddStringList
 *
 * Description
 * Adds an array of strings
 *
 * Inputs
 * string prefix - prefix, typically a tab or tabs
 * string fieldPrefix - field name, will be surrounded in double quotes
 * string header - header
 * vector<string> values - field values, will be surrounded in double quotes
 * string suffix - suffix, typically a new line
 * bool lastElement - if true then no comma added, else comma will be added before suffix
 *
 * Returns
 *
 */
void Summary_Json::JsonOutputFormatter::AddStringList(string prefix, string fieldPrefix, string header,
                                                      vector<string> values, string suffix, bool lastElement)
{
    int index = 0;
    cout << prefix << "\"" << CTextUtils::RemoveQuotation(header) << "\"" << ":";
    cout << "[" << (values.size() > 0 ? "\n" : "");
    for (string value: values)
    {
        cout << prefix << fieldPrefix << "\"" << CTextUtils::RemoveQuotation(value) << "\"" << (index < values.size() - 1 ? "," : "") << suffix;
        index ++;
    }
    cout << (values.size() > 0 ? prefix : "") << "]" << (lastElement ? "" : ",") << suffix;
}

/*
 * Summary_Json::JsonOutputFormatter::AddArrayHeader
 *
 * Description
 * Adds header to the array
 *
 * Inputs
 * string prefix - prefix, typically a tab or tabs
 * string header - array header name
 * int arrayCount - elements count in array
 * string suffix - suffix, typically a new line
 *
 * Returns
 *
 */
void Summary_Json::JsonOutputFormatter::AddArrayHeader(string prefix, string header, int arrayCount, string suffix)
{
    cout << prefix << "\"" << CTextUtils::RemoveQuotation(header) << "\"" << ":";
    cout << "[" << (arrayCount > 0 ? suffix : "");
}

/*
 * Summary_Json::JsonOutputFormatter::AddArrayFooter
 *
 * Description
 * Adds footer to the array
 *
 * Inputs
 * string prefix - prefix, typically a tab or tabs
 * int arrayCount - elements count in array
 * string suffix - suffix, typically a new line
 * bool lastElement - if true then no comma added, else comma will be added before suffix
 *
 * Returns
 *
 */
void Summary_Json::JsonOutputFormatter::AddArrayFooter(string prefix, int arrayCount, string suffix, bool lastElement)
{
    cout << (arrayCount > 0 ? prefix : "") << "]" << (lastElement ? "" : ",") << suffix;
}

/*
 * Summary_Json::JsonOutputFormatter::AddStartBrace
 *
 * Description
 * Adds start brace
 *
 * Inputs
 * string prefix - prefix, typically a tab or tabs
 * string suffix - suffix, typically a new line
 *
 * Returns
 *
 */
void Summary_Json::JsonOutputFormatter::AddStartBrace(string prefix, string suffix)
{
    cout << prefix << "{" << suffix;
}

/*
 * Summary_Json::JsonOutputFormatter::AddEndBrace
 *
 * Description
 * Adds end brace, mostly used for array elements
 *
 * Inputs
 * string prefix - prefix, typically a tab or tabs
 * int index - index in an array
 * int arrayCount - total count of the array
 * string suffix - suffix, typically a new line
 *
 * Returns
 *
 */
void Summary_Json::JsonOutputFormatter::AddEndBrace(string prefix, int index, int arrayCount, string suffix)
{
    cout << prefix << "}"<< (index < arrayCount - 1 ? "," : "") << suffix;
}

/*
 * Summary_Json::JsonOutputFormatter::AddWordCountMap
 *
 * Description
 * Adds word count map into the jsaon
 *
 * Inputs
 * const string &header - header of map array
 * map<string, int> popularWordsMap - map of string keys and integer values
 *
 * Returns
 *
 */
void Summary_Json::JsonOutputFormatter::AddWordCountMap(const string &header, map<string, int> popularWordsMap)
{
    int wordsMapCount = popularWordsMap.size();
    auto cmp = [](pair<string,int> const & a, pair<string,int> const & b)
    {
        return (int)a.second != (int)b.second?  (int)a.second > (int)b.second : a.first.compare(b.first) > 0;
    };
    vector<pair<string,int>> items(popularWordsMap.begin(), popularWordsMap.end());
    sort(items.begin(), items.end(), cmp);

    AddArrayHeader("\t", header, wordsMapCount, "\n");
    int index = 0;
    for(vector<pair<string,int>>::const_iterator it = items.begin(); it != items.end(); ++it)
    {
        AddStartBrace("\t\t", "\n");
        AddString("\t\t\t", "word", it->first, "\n", false);
        AddLong("\t\t\t", "count", it->second, "\n", true);
        AddEndBrace("\t\t", index, wordsMapCount, "\n");
        index++;
    }
    AddArrayFooter("\t", wordsMapCount, "\n", false);
}
