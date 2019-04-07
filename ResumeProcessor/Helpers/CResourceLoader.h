//
// File CResourceLoader.h
//
//

#ifndef CVSUMMARYEXTRACTOR_CRESOURCELOADER_H
#define CVSUMMARYEXTRACTOR_CRESOURCELOADER_H

#include <string>
#include "IResourceLoader.h"

using namespace std;

namespace Summary_ResumeProcessor_Helpers
{
    //
    // CLASS : CResourceLoader
    //
    // DESCRIPTION : This class deals with input data reading into specific format types (vector, unordered_set etc.)
    //
    //
    class CResourceLoader: IResourceLoader
    {
    public:
        virtual unordered_set<string> LoadIntoHashSet(string resourceName, char delimiter);
        virtual SLowerCaseUnorderedSet LoadIntoLowerCaseHashSet(string resourceName, char delimiter);
        virtual vector<string> Load(string resourceName, char delimiter);
        virtual vector<string> LoadExternal(string resourceName, char delimiter);
        virtual string LoadExternalToString(string resourceName);
        virtual vector<CLowerCaseString> LoadLowerCase(string resourceName, char delimiter);
    };
}

#endif //CVSUMMARYEXTRACTOR_CRESOURCELOADER_H
