//
// File CSectionExtractor.h
//
//

#ifndef CVSUMMARYEXTRACTOR_CSECTIONEXTRACTOR_H
#define CVSUMMARYEXTRACTOR_CSECTIONEXTRACTOR_H

#include <iostream>
#include <string>
#include <regex>
#include <vector>
#include "CSectionMatchingService.h"
#include "../Model/CSection.h"

using namespace Summary_Model;
using namespace std;

namespace Summary_ResumeProcessor
{
    //
    // CLASS : CSectionExtractor
    //
    // DESCRIPTION : This class encapsulates section extracting from input string
    //
    //
    class CSectionExtractor
    {
    public:
        /*
        * Summary_Model::CSectionExtractor::CSectionExtractor
        *
        * Description
        *
        * Inputs
        *
        * Returns
        *
        */
        CSectionExtractor()
        {
            sectionMatchingService = new CSectionMatchingService();
        }

        /*
        * ~Summary_Model::CSectionExtractor::CSectionExtractor
        *
        * Description
        *
        * Inputs
        *
        * Returns
        *
        */
        virtual ~CSectionExtractor()
        {
            delete sectionMatchingService;
        }

        vector<CSection> ExtractFrom(vector<string> rawInput);

    private:
        const int SectionTitleNumberOfWordsLimit = 4;
        CSectionMatchingService *sectionMatchingService;

        ESectionType FindSectionType(string input);
    };
}

#endif //CVSUMMARYEXTRACTOR_CSECTIONEXTRACTOR_H
