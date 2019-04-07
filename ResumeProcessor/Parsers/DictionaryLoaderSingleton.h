//
// File DictionaryLoaderSingleton.h
//
//

#ifndef CVSUMMARYEXTRACTOR_DICTIONARYLOADERSINGLETON_H
#define CVSUMMARYEXTRACTOR_DICTIONARYLOADERSINGLETON_H

#include <iostream>

//using namespace std;
#include "IDictionaryLoader.h"
#include "../Helpers/CResourceLoader.h"

using namespace Summary_ResumeProcessor_Helpers;

namespace Summary_ResumeProcessor_Parsers
{
    class DictionaryLoaderSingleton
    {
    public:
        DictionaryLoaderSingleton() {};

        class DictionaryLoader: public IDictionaryLoader
        {
        public:
            DictionaryLoader(CResourceLoader resourceLoader);
            void LoadDictionaries();

        private:
            void PushDegreeAbbreviation(const string &firstLetter,
                                   const string &secondLetter,
                                   bool concatenateSimply);
            void PushDegreeAbbreviation(const string &firstLetter,
                                   const string &secondLetter,
                                   const string &thirdLetter,
                                   bool concatenateSimply);
            CResourceLoader resourceLoader;
            bool dictionariesLoaded;
        };

        static DictionaryLoader dictionaryLoader;

        static DictionaryLoader GetInstance()
        {
            dictionaryLoader.LoadDictionaries();
            return dictionaryLoader;
        }
    };
}


#endif //CVSUMMARYEXTRACTOR_DICTIONARYLOADERSINGLETON_H
