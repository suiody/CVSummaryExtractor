//
// File CSkillsParser.h
//
//

#ifndef CVSUMMARYEXTRACTOR_CSKILLSPARSER_H
#define CVSUMMARYEXTRACTOR_CSKILLSPARSER_H

#include "IParser.h"
#include "../Helpers/CResourceLoader.h"

using namespace Summary_ResumeProcessor_Helpers;

namespace Summary_ResumeProcessor_Parsers
{
    //
    // CLASS : CSkillsParser
    //
    // DESCRIPTION : This class is used for parsing skill information from input string
    //
    //
    class CSkillsParser: public IParser
    {
    public:
        /*
        * Summary_Model::CSkillsParser::CSkillsParser
        *
        * Description
        *
        * Inputs
        *
        * Returns
        *
        */
        CSkillsParser(CResourceLoader resourceLoader);

        virtual void Parse(CSection section, CResume &resume);

    private:
        static bool StringAndLengthCompare(const string &s1, const string &s2);
        void ExtractSkills(const vector<string> &content, CResume &resume, int upperLimit);

        vector<string> skillLookUp;
        vector<string> lowerSkills;
    };
}

#endif //CVSUMMARYEXTRACTOR_CSKILLSPARSER_H
