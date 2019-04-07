//
// File CPopularWordsParser.cpp
//
//

#include "CPopularWordsParser.h"
#include "DictionaryLoaderSingleton.h"
#include "../../Common/CDebugUtils.h"
#include "../../Common/CTextUtils.h"
#include "../../Common/CFileUtils.h"

using namespace Summary_Common;
using namespace Summary_ResumeProcessor_Helpers;

/*
 * Summary_ResumeProcessor_Parsers::CPopularWordsParser::CPopularWordsParser
 *
 * Description
 * Constructor. Inits stop words vector
 *
 * Inputs
 * CResourceLoader resourceLoader - input resource loader object
 *
 * Returns
 *
 */
Summary_ResumeProcessor_Parsers::CPopularWordsParser::CPopularWordsParser(CResourceLoader resourceLoader)
{
    stopWordsLookup = DictionaryLoaderSingleton::GetInstance().GetStopWords();
    firstNameLookUp = DictionaryLoaderSingleton::GetInstance().GetFirstNames();
}

/*
 * Summary_ResumeProcessor_Parsers::CPopularWordsParser::CPopularWordsParser
 *
 * Description
 * Method for parsing popular words section and saving results into resume object. Saves only words that are not
 * stopwords and if length > 2
 *
 * Inputs
 * CSection section - section data
 * CResume &resume - resume object to be filled during parsing process
 *
 * Returns
 * changed resume object through input parameter resume
 */
void Summary_ResumeProcessor_Parsers::CPopularWordsParser::Parse(CSection section, CResume &resume)
{
    CDebugUtils::DebugCout("CRawInputParser", "Parse", "");
    int i = 0;
    vector<char> keepChars;
    keepChars.push_back('-');
    vector<string> words;

    while (i < section.GetContent().size())
    {
        string line = section.GetContent()[i];
        line = CTextUtils::ReplaceNonAlphanumeric(line, ' ', keepChars);

        //cout << line << "\n";

        for (string word: CTextUtils::Split(line, ' '))
        {
            word = CTextUtils::ToLower(word);
            word = CTextUtils::TrimNameCharacters(word);
            word = CTextUtils::TrimAnyChar(word, "0123456789");
            word = CTextUtils::TrimNameCharacters(word);

            bool isStopWord = false;

            if (word.length() < 3)
            {
                isStopWord = true;
            }

            if (!isStopWord)
            {
                for (string stopWord: stopWordsLookup)
                {
                    if (word.compare(stopWord) == 0)
                    {
                        isStopWord = true;
                        break;
                    }
                }
            }

            if (!isStopWord)
            {
                CLowerCaseString lowerWord(word);

                SLowerCaseUnorderedSet::const_iterator gotWord = firstNameLookUp.find(lowerWord);

                if (gotWord == firstNameLookUp.end())
                {
                    resume.AddWord(word);
                    words.push_back(word);
                }
            }
        }

        i++;
    }

    string lastWord = "";
    for (string word: words)
    {
        if (lastWord.length() > 0)
        {
            resume.AddWordPair(lastWord + " " + word);
        }
        lastWord = word;
    }
}
