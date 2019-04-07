//
// File CPersonalParser.h
//
//
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file Boost/LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef CVSUMMARYEXTRACTOR_CPERSONALPARSER_H
#define CVSUMMARYEXTRACTOR_CPERSONALPARSER_H

#include <string>
#include <unordered_set>
#include <vector>
#include "IParser.h"
#include "../Helpers/CResourceLoader.h"
#include "../../Common/SLowerCaseUnorderedSet.h"
#include <boost/xpressive/xpressive_static.hpp>

using namespace std;
using namespace Summary_ResumeProcessor_Helpers;
using namespace Summary_Common;

namespace Summary_ResumeProcessor_Parsers
{
    //
    // CLASS : CPersonalParser
    //
    // DESCRIPTION : This class is used for parsing personal information from input string
    //
    //
    class CPersonalParser: public IParser
    {
    public:
        CPersonalParser(CResourceLoader resourceLoader);

        virtual void Parse(CSection section, CResume &resume);

    private:
        bool ExtractAddress(CResume &resume, bool addressFound, bool onlyAddressLines, string line);
        bool ExtractCountry(CResume &resume, bool countryFound, string line);
        void ExtractSocialProfiles(CResume &resume, string line);
        void ExtractUrlCandidates(CResume &resume, const string &line);
        bool ExtractPhone(CResume &resume, bool phoneFound, const string &phoneKeyWord,
                          const vector<string> &nonKeywords, bool keyWordAfterAllowed, const string &line);
        bool BulkExtractPhone(const vector<string> &sectionRawContentLower, CResume &resume, bool phoneFound);
        bool ExtractMorePhoneData(const vector<string> &sectionRawContentLower, CResume &resume, bool phoneFound,
                                  const vector<string> &phoneKeyWords, const vector<string> &nonKeywords);
        void PreparePhoneClues(const CResume &resume, const vector<string> &sectionRawContentLower);
        bool ExtractPhoneByClues(const vector<string> &sectionRawContentLower, CResume &resume, bool phoneFound);
        bool ExtractGender(CResume &resume, bool genderFound, const string &line);
        bool ExtractHeaderFirstName(CResume &resume, bool firstNameFound, const string &line);
        bool ExtractFirstAndLastName(CResume &resume, string nameSuggestion, bool firstNameFound,
                                     const string &line, bool requireLastName);
        bool ExtractLastName(CResume &resume, bool lastNameFound, const string &line, const string &lowerFirstName);
        bool ExtractLastNameAfterKeyword(CResume &resume, bool lastNameFound, const string &line);
        bool ExtractAndOverWriteName(CResume &resume, bool lastNameFound, const string &line);
        bool ExtractAndRepairName(CResume &resume, bool lastNameFound, const string &line);
        bool ExtractAndRepairFullName(CResume &resume, bool fullNameFound, const string &line);
        bool ExtractAndRepairNameHelper(CResume &resume, bool lastNameFound, string cleanedOriginalLine);
        bool ExtractEmail(CResume &resume, bool emailFound, const string &line);
        bool ExtractBirthDay(CResume &resume, bool birthDayFound, const string &line, string controlSentence);
        bool ExtractReligion(CResume &resume, bool religionFound, const string &line);
        bool ExtractSkype(CResume &resume, bool religionFound, const string &line);
        bool ExtractWebSite(CResume &resume, bool webSiteFound, const string &line);
        string TrimNameCharacters(const string &input);
        bool ValidateLastName(const string &name);
        bool UrlContainsValidDomain(const string &line, const string &url);
        void PostProcessData(CResume &resume);
        string ExtractCountryFromLine(const string &lowerLine, bool useCode, string keyword);
        bool ContainsCountryCode(const string &lowerLine, const string &countryCode);
        string CountryCodeWithDots(const string &code);
        bool HasPhoneKeyWordIndicator(const string &match, const string::size_type &phoneIndicatorPosition,
                                      const string &lowerLine, const string &lowerPhoneKeyword,
                                      bool keyWordAfterAllowed);
        bool HasPhoneIndicator(const string &match,
                               const string::size_type &phoneIndicatorPosition,
                               const string &lowerLine,
                               const string &lowerIndicator);
        bool AddLastNames(CResume &resume, string lastName);
        void ExchangeFirstAndLastName(CResume &resume);
        void RearrangeAllNames(CResume &resume);
        void PushDegreeAbbreviation(const string &firstLetter, const string &secondLetter, bool concatenateSimply);
        void PushDegreeAbbreviation(const string &firstLetter, const string &secondLetter,
                                    const string &thirdLetter, bool concatenateSimply);
        bool ExtractEducation(CResume &resume, vector<string> &sectionRawContentLower,
                              const string &educationKeyWord, bool useKeyWord);
        bool ExtractSchool(CResume &resume, vector<string> &sectionRawContentLower,
                              const string &educationKeyWord, bool useKeyWord);
        int LineDistance(const vector<int> &lineNumbers, int lineIndex, bool positiveOnly, int distance);

        const static boost::xpressive::sregex EmailRegex;
        const static boost::xpressive::sregex PhoneRegex;
        const static boost::xpressive::sregex SocialProfileRegex;
        const static boost::xpressive::sregex SplitByWhiteSpaceRegex;
        const static boost::xpressive::sregex BirthDayRegex;
        const static boost::xpressive::sregex SkypeRegex;
        const static boost::xpressive::sregex UrlRegex;
        const static boost::xpressive::sregex ShortUrlRegex;
        const static boost::xpressive::sregex DomainNameRegex;
        SLowerCaseUnorderedSet firstNameLookUp;
        vector<string> countryLookUp;
        vector<string> countryLookUpLower;
        map<string, string> countryMap;
        vector<string> religionLookUp;
        vector<string> stopWordsLookup;
        vector<string> nonNamesLookup;
        vector<string> fieldsOfStudyLookUp;
        vector<string> shortTaggedDegreeLookUp;
        vector<string> degreeAbbbreviations;
        vector<string> shortTaggedDegreeLookUpLower;
        vector<string> shortTaggedDegreeLookUpBeginning;
        vector<string> fieldsOfStudyLookUpLower;
        vector<string> phoneIndicators;
        vector<string> exactPhoneIndicators;
        vector<string> notAllowedStartings;
        vector<string> nameKeysVector;
        vector<string> lastNameKeysVector;
        vector<string> fullNameKeysVector;
        vector<string> namePrefixes;
        vector<string> allSocialProfiles;
        vector<string> imageFormatEndings;
        vector<string> siteKeywords;
        vector<string> schoolLookUp;
        vector<string> dropNames;
        vector<int> degreeLineNumbers;
        vector<int> phoneClueLineNumbers;
        bool birthDayStringFound;
        bool religionStringFound;
        bool countryStringFound;
        bool nameKeyFound;

        int religionKeyWordSteps;
        int nameLineCount;
        int lastNameKeyWordSteps;
        int countryKeyWordSteps;
        int phoneKeyWordSteps;
        int genderKeyWordSteps;
    };
}

#endif //CVSUMMARYEXTRACTOR_CPERSONALPARSER_H
