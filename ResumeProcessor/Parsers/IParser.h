//
// File IParser.h
//
//

#ifndef CVSUMMARYEXTRACTOR_IPARSER_H
#define CVSUMMARYEXTRACTOR_IPARSER_H

#include "../../Model/CSection.h"
#include "../../Model/CResume.h"

using namespace Summary_Model;

namespace Summary_ResumeProcessor_Parsers
{
    //
    // CLASS : IParser
    //
    // DESCRIPTION : This class is base class for parsing any kind of information from input text
    //
    //
    class IParser
    {
    public:
        virtual void Parse(CSection section, CResume &resume);
    };
}

#endif //CVSUMMARYEXTRACTOR_IPARSER_H
