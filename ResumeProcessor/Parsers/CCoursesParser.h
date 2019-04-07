//
// File CCoursesParser.h
//
//

#ifndef CVSUMMARYEXTRACTOR_CCOURSESPARSER_H
#define CVSUMMARYEXTRACTOR_CCOURSESPARSER_H

#include "IParser.h"

//using namespace std;

namespace Summary_ResumeProcessor_Parsers
{
    //
    // CLASS : CCoursesParser
    //
    // DESCRIPTION : This class is used for parsing courses information from input string
    //
    //
    class CCoursesParser: public IParser
    {
    public:
        /*
        * Summary_Model::CCoursesParser::CCoursesParser
        *
        * Description
        * Constructor
        *
        * Inputs
        *
        * Returns
        *
        */
        CCoursesParser()
        {
        }

        virtual void Parse(CSection section, CResume &resume);
    };
}

#endif //CVSUMMARYEXTRACTOR_CCOURSESPARSER_H
