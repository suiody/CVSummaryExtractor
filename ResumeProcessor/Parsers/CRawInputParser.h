//
// File CRawInputParser.h
//
//

#ifndef CVSUMMARYEXTRACTOR_CRAWINPUTPARSER_H
#define CVSUMMARYEXTRACTOR_CRAWINPUTPARSER_H

//include <string>
#include "IParser.h"

//using namespace std;


namespace Summary_ResumeProcessor_Parsers
{
    //
    // CLASS : CRawInputParser
    //
    // DESCRIPTION : This class is used for parsing projects information from input string
    //
    //
    class CRawInputParser: public IParser
    {
    public:
        /*
        * Summary_Model::CRawInputParser::CRawInputParser
        *
        * Description
        * Constructor
        *
        * Inputs
        *
        * Returns
        *
        */
        CRawInputParser()
        {
        }

        virtual void Parse(CSection section, CResume &resume);
    };
}

#endif //CVSUMMARYEXTRACTOR_CRAWINPUTPARSER_H
