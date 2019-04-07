//
// File CEducationParser.h
//
//

#ifndef CVSUMMARYEXTRACTOR_CEDUCATIONPARSER_H
#define CVSUMMARYEXTRACTOR_CEDUCATIONPARSER_H

#include "IParser.h"
#include <boost/xpressive/xpressive_static.hpp>

using namespace std;

namespace Summary_ResumeProcessor_Parsers
{
    //
    // CLASS : CEducationParser
    //
    // DESCRIPTION : This class is used for parsing education information from input string
    //
    //
    class CEducationParser: public IParser
    {
    public:
        CEducationParser();

        virtual void Parse(CSection section, CResume &resume);

    private:
        const static vector<string> schoolLookUp;
        const static vector<string> courseLookUp;
        const static boost::xpressive::sregex BirthDayRegex;
        const static boost::xpressive::sregex SplitByWhiteSpaceRegex;
        vector<string> taggedDegreesLookUp;

        vector<string> fieldsOfStudyLookUp;
        vector<string> shortTaggedDegreeLookUp;
        vector<string> degreeAbbbreviations;
        vector<string> degreeAbbbreviationsExact;
        vector<string> shortTaggedDegreeLookUpLower;
        vector<string> shortTaggedDegreeLookUpBeginning;
        vector<string> fieldsOfStudyLookUpLower;
        vector<int> degreeLineNumbers;

        static string ParseSchool(string line);
        static void ParseStartAndEndDate(string line, CEducation &currentEducation);

        bool ExtractEducation(CResume &resume, vector<string> &sectionRawContentLower,
                              const string &educationKeyWord, bool useKeyWord);
        bool ExtractSchool(CResume &resume, vector<string> &sectionRawContentLower,
                           const string &educationKeyWord, bool useKeyWord);
        int LineDistance(const vector<int> &lineNumbers, int lineIndex, bool positiveOnly, int distance);
    };
}

#endif //CVSUMMARYEXTRACTOR_CEDUCATIONPARSER_H
