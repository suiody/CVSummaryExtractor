//
// File CProjectsParser.h
//
//

#ifndef CVSUMMARYEXTRACTOR_CPROJECTSPARSER_H
#define CVSUMMARYEXTRACTOR_CPROJECTSPARSER_H

#include "IParser.h"

//using namespace std;

namespace Summary_ResumeProcessor_Parsers
{
    //
    // CLASS : CProjectsParser
    //
    // DESCRIPTION : This class is used for parsing projects information from input string
    //
    //
    class CProjectsParser: public IParser
    {
    public:
        /*
        * Summary_Model::CProjectsParser::CProjectsParser
        *
        * Description
        *
        * Inputs
        *
        * Returns
        *
        */
        CProjectsParser()
        {
        }

        virtual void Parse(CSection section, CResume &resume);

    private:

    };
}

#endif //CVSUMMARYEXTRACTOR_CPROJECTSPARSER_H
