//
// File CAwardsParser.h
//
//

#ifndef CVSUMMARYEXTRACTOR_CAWARDSPARSER_H
#define CVSUMMARYEXTRACTOR_CAWARDSPARSER_H

#include "IParser.h"

//using namespace std;

namespace Summary_ResumeProcessor_Parsers
{
    //
    // CLASS : CAwardsParser
    //
    // DESCRIPTION : This class is used for parsing awards information from input string
    //
    //
    class CAwardsParser: public IParser
    {
    public:
        /*
        * Summary_Model::CAwardsParser::CAwardsParser
        *
        * Description
        * Constructor
        *
        * Inputs
        *
        * Returns
        *
        */
        CAwardsParser()
        {
        }

        virtual void Parse(CSection section, CResume &resume);
    };
}

#endif //CVSUMMARYEXTRACTOR_CAWARDSPARSER_H
