//
// File CResumeBuilder.h
//
//

#ifndef CVSUMMARYEXTRACTOR_CRESUMEBUILDER_H
#define CVSUMMARYEXTRACTOR_CRESUMEBUILDER_H

#include <string>
#include <map>
#include "IParser.h"
#include "../../Model/CResume.h"
#include "../Helpers/CResourceLoader.h"

using namespace std;
using namespace Summary_ResumeProcessor_Helpers;
using namespace Summary_Model;

namespace Summary_ResumeProcessor_Parsers
{
    //
    // CLASS : CResumeBuilder
    //
    // DESCRIPTION : This class is used for building resume structure and fill it with data
    //
    //
    class CResumeBuilder
    {
    public:
        CResumeBuilder(CResourceLoader resourceLoader);
        CResume Build(vector<CSection> sections);

        virtual ~CResumeBuilder();

    private:
        map<ESectionType, IParser *> parserRegistry;
    };
}

#endif //CVSUMMARYEXTRACTOR_CRESUMEBUILDER_H
