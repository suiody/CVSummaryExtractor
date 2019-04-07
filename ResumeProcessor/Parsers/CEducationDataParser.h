//
// File CEducationDataParser.h
//
//

#ifndef CVSUMMARYEXTRACTOR_CEDUCATIONDATAPARSER_H
#define CVSUMMARYEXTRACTOR_CEDUCATIONDATAPARSER_H

#include "IParser.h"
#include <boost/xpressive/xpressive_static.hpp>

using namespace std;


namespace Summary_ResumeProcessor_Parsers
{
    class CEducationDataParser: public IParser
    {
    public:
        CEducationDataParser();

        virtual void Parse(CSection section, CResume &resume);

    private:
        const static vector<string> schoolLookUp;
        const static vector<string> exactSchoolLookUp;
        const static vector<string> courseLookUp;
        const static boost::xpressive::sregex BirthDayRegex;
        const static boost::xpressive::sregex SplitByWhiteSpaceRegex;
        vector<string> taggedDegreesLookUp;

        vector<string> fieldsOfStudyLookUp;
        vector<string> fieldsOfStudyLookUpShort;
        vector<string> universityLookUpLower;
        vector<string> shortTaggedDegreeLookUp;
        vector<string> degreeAbbbreviations;
        vector<string> degreeAbbbreviationsExact;
        vector<string> degreeAbbbreviationsShort;
        vector<string> shortTaggedDegreeLookUpLower;
        vector<string> shortTaggedDegreeLookUpBeginning;
        vector<string> fieldsOfStudyLookUpLower;
        vector<string> fieldsOfStudyLookUpShortLower;
        vector<int> degreeLineNumbers;

        void PushDegreeAbbreviation(const string &firstLetter, const string &secondLetter, bool concatenateSimply);
        void PushDegreeAbbreviation(const string &firstLetter, const string &secondLetter,
                                    const string &thirdLetter, bool concatenateSimply);
        bool ExtractEducation(CResume &resume, vector<string> &sectionRawContentLower,
                              const string &educationKeyWord, bool useKeyWord);
        bool ExtractSchool(CResume &resume, vector<string> &sectionRawContentLower,
                           const string &educationKeyWord, bool useKeyWord);
        int LineDistance(const vector<int> &lineNumbers, int lineIndex, bool positiveOnly, int distance);
    };
}


#endif //CVSUMMARYEXTRACTOR_CEDUCATIONDATAPARSER_H
