//
// File CPopularWordsParser.h
//
//

#ifndef CVSUMMARYEXTRACTOR_CPOPULARWORDSPARSER_H
#define CVSUMMARYEXTRACTOR_CPOPULARWORDSPARSER_H

#include <string>
#include "IParser.h"
#include "../Helpers/CResourceLoader.h"

using namespace std;
using namespace Summary_ResumeProcessor_Helpers;

namespace Summary_ResumeProcessor_Parsers
{
    //
    // CLASS : CProjectsParser
    //
    // DESCRIPTION : This class is used for parsing words in popularity order,
    // excluding stop-words
    //
    class CPopularWordsParser: public IParser
    {
    public:
        CPopularWordsParser(CResourceLoader resourceLoader);
        virtual void Parse(CSection section, CResume &resume);

    private:
        vector<string> stopWordsLookup;
        SLowerCaseUnorderedSet firstNameLookUp;
    };
}

#endif //CVSUMMARYEXTRACTOR_CPOPULARWORDSPARSER_H
