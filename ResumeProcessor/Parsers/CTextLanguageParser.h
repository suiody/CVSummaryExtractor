//
// File CTextLanguageParser.h
//
//

#ifndef CVSUMMARYEXTRACTOR_CTEXTLANGUAGEPARSER_H
#define CVSUMMARYEXTRACTOR_CTEXTLANGUAGEPARSER_H

//include <string>
#include "IParser.h"

//using namespace std;

namespace Summary_ResumeProcessor_Parsers
{
    //
    // CLASS : CTextLanguageParser
    //
    // DESCRIPTION : This class is used for parsing projects information from input string
    //
    //
    class CTextLanguageParser: public IParser
    {
    public:
        /*
        * Summary_Model::CRawInputParser::CTextLanguageParser
        *
        * Description
        * Constructor
        *
        * Inputs
        *
        * Returns
        *
        */
        CTextLanguageParser()
        {
        }

        virtual void Parse(CSection section, CResume &resume);
    };
}


#endif //CVSUMMARYEXTRACTOR_CTEXTLANGUAGEPARSER_H
