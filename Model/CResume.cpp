//
// File CResume.cpp
//
//

#include <algorithm>
#include "CResume.h"
#include "../Stemmers/porter2_stemmer.h"

/*
 * Summary_Model::CResume::GetWordsByPopularity
 *
 * Description
 * Gets words in document sorted by most popular words first
 *
 * Inputs
 *
 * Returns
 * vector<string> - words vector
 */
const vector<string> Summary_Model::CResume::GetWordsByPopularity() const
{
    return CTextUtils::SortMapByCounts(words, 100000);
}

/*
 * Summary_Model::CResume::GetWordsMapByPopularity
 *
 * Description
 * Gets words in document sorted by most popular words first
 *
 * Inputs
 *
 * Returns
 * vector<string> - words vector
 */
const map<string, int> Summary_Model::CResume::GetWordsMapByPopularity() const
{
    return CTextUtils::GetSortedMapByCounts(words, 100000);
}

/*
 * Summary_Model::CResume::GetWordPairsByPopularity
 *
 * Description
 * Gets words in document sorted by most popular words first
 *
 * Inputs
 *
 * Returns
 * vector<string> - words vector
 */
const vector<string> Summary_Model::CResume::GetWordPairsByPopularity() const
{
    return CTextUtils::SortMapByCounts(wordPairs, 100000);
}

/*
 * Summary_Model::CResume::GetWordsMapByPopularity
 *
 * Description
 * Gets words in document sorted by most popular words first
 *
 * Inputs
 *
 * Returns
 * vector<string> - words vector
 */
const map<string, int> Summary_Model::CResume::GetWordPairsMapByPopularity() const
{
    return CTextUtils::GetSortedMapByCounts(wordPairs, 100000);
}

/*
 * Summary_Model::CResume::GetSkillsByPopularity
 *
 * Description
 * Gets skills of document sorted by most popular words first
 *
 * Inputs
 *
 * Returns
 * vector<string> - words vector
 */
const vector<string> Summary_Model::CResume::GetSkillsByPopularity() const
{
    return CTextUtils::SortMapByCounts(skillCounts, 15);
}

/*
 * Summary_Model::CResume::AddWord
 *
 * Description
 * Adds word to the map in lower case
 *
 * Inputs
 * const string &word - word to be added
 *
 * Returns
 *
 */
void Summary_Model::CResume::AddWord(const string &word)
{
    if (word.length() < 1)
    {
        return;
    }
    string insertWord = CTextUtils::ToLower(word);
    /*string stemmedWord = CTextUtils::ToLower(word);

    Porter2Stemmer::stem(stemmedWord);

    if ( find(stemmedWords.begin(), stemmedWords.end(), stemmedWord)==stemmedWords.end() )
    {
        words.insert(pair<string,int>(insertWord, 1));
        stemmedWords.push_back(stemmedWord);
    }
    else
    {
        words[insertWord] = words[insertWord] + 1;
    }*/

    if (words.count(insertWord) > 0)
    {
        //cout << word << " exists " << words[word] << "\n";
        words[insertWord] = words[insertWord] + 1;
    }
    else
    {
        //cout << word << " not exists" << "\n";
        words.insert(pair<string,int>(insertWord, 1));
    }
}

/*
 * Summary_Model::CResume::AddWordPair
 *
 * Description
 * Adds word to the map in lower case
 *
 * Inputs
 * const string &word - word to be added
 *
 * Returns
 *
 */
void Summary_Model::CResume::AddWordPair(const string &wordPair)
{
    if (wordPair.length() < 1)
    {
        return;
    }
    string insertWord = CTextUtils::ToLower(wordPair);

    if (wordPairs.count(insertWord) > 0)
    {
        wordPairs[insertWord] = wordPairs[insertWord] + 1;
    }
    else
    {
        wordPairs.insert(pair<string,int>(insertWord, 1));
    }
}
