//
// File CSectionMatchingService.h
//
//

#ifndef CVSUMMARYEXTRACTOR_CSECTIONMATCHINGSERVICE_H
#define CVSUMMARYEXTRACTOR_CSECTIONMATCHINGSERVICE_H

#include <string>
#include <vector>
#include <map>
#include "../Model/ESectionType.h"

using namespace std;
using namespace Summary_Model;

namespace Summary_ResumeProcessor
{
    //
    // CLASS : CSectionMatchingService
    //
    // DESCRIPTION : This class is used for finding sections from input string
    //
    //
    class CSectionMatchingService
    {
    public:
        ESectionType FindSectionTypeMatching(string input);

    private:
        static map<ESectionType, vector<string>> keyWordRegistry;
    };
}

#endif //CVSUMMARYEXTRACTOR_CSECTIONMATCHINGSERVICE_H
