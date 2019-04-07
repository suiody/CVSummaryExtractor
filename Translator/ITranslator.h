//
// File ITranslator.h
//
//

#ifndef CVSUMMARYEXTRACTOR_ITRANSLATOR_H
#define CVSUMMARYEXTRACTOR_ITRANSLATOR_H

#include <string>
#include <vector>

using namespace std;

namespace Summary_Translator
{
    class ITranslator
    {
        virtual string Translate(const vector<string> &inputText, const string &fromLanguage, const string &toLanguage) = 0;
    };
}


#endif //CVSUMMARYEXTRACTOR_ITRANSLATOR_H
