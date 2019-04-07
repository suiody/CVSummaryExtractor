//
// File JsonOutputFormatter.h
//
//

#ifndef CVSUMMARYEXTRACTOR_JSONOUTPUTFORMATTER_H
#define CVSUMMARYEXTRACTOR_JSONOUTPUTFORMATTER_H

#include <string>
#include "../Model/IOutputFormatter.h"

using namespace std;
using namespace Summary_Model;

namespace Summary_Json
{
    //
    // CLASS : JsonOutputFormatter
    //
    // DESCRIPTION : This class encapsulates outputting Json to cout
    //
    // Class methods are used when outputting resume data to cout
    //
    class JsonOutputFormatter: public IOutputFormatter
    {
    public:
        JsonOutputFormatter()
        {
        }

        virtual string Format(const CResume &resume);

    private:
        void AddString(string prefix, string fieldName, string value, string suffix, bool lastElement);
        void AddLong(string prefix, string fieldName, long value, string suffix, bool lastElement);
        void AddStringList(string prefix, string fieldPrefix, string header, vector<string> values, string suffix,
                           bool lastElement);
        void AddArrayHeader(string prefix, string header, int arrayCount, string suffix);
        void AddArrayFooter(string prefix, int arrayCount, string suffix, bool lastElement);
        void AddStartBrace(string prefix, string suffix);
        void AddEndBrace(string prefix, int index, int arrayCount, string suffix);
        void AddWordCountMap(const string &header, map<string, int> popularWordsMap);
    };
}

#endif //CVSUMMARYEXTRACTOR_JSONOUTPUTFORMATTER_H
