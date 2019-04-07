//
// File CSummaryParser.h
//
//

#ifndef CVSUMMARYEXTRACTOR_CSUMMARYPARSER_H
#define CVSUMMARYEXTRACTOR_CSUMMARYPARSER_H

#include "IParser.h"

//using namespace std;

namespace Summary_ResumeProcessor_Parsers
{
    //
    // CLASS : CSummaryParser
    //
    // DESCRIPTION : This class is used for parsing summary information from input string
    //
    //
    class CSummaryParser: public IParser
    {
    public:
        /*
        * Summary_Model::CSummaryParser::CSummaryParser
        *
        * Description
        * Constructor
        *
        * Inputs
        *
        * Returns
        *
        */
        CSummaryParser()
        {
        }

        virtual void Parse(CSection section, CResume &resume);
    };
}

#endif //CVSUMMARYEXTRACTOR_CSUMMARYPARSER_H
