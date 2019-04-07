//
// File CBingTranslator.h
//
//

#ifndef CVSUMMARYEXTRACTOR_CBINGTRANSLATOR_H
#define CVSUMMARYEXTRACTOR_CBINGTRANSLATOR_H

#include "ITranslator.h"

namespace Summary_Translator
{
    class CBingTranslator: public ITranslator
    {
    public:
        CBingTranslator();
        virtual string Translate(const vector<string> &inputText, const string &fromLanguage, const string &toLanguage);

    private:
        string GetWebAccessToken();
        string ExtractAccessToken(const string &httpResponseBody);
        string TranslatePart(const string &accessToken, const string &inputText,
                             const string &fromLanguage, const string &toLanguage);
        void AddTranslationPart(const string &accessToken, const string &sendText,
                             const string &fromLanguage, const string &toLanguage);
        string CldCodeToBingCode(const string &cldCode);

        bool translateRequestError;
        bool translationFirstMode;
        int translationCount;
        string translatedOutputText;
    };
}


#endif //CVSUMMARYEXTRACTOR_CBINGTRANSLATOR_H
