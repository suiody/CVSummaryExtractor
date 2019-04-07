//
// File IResourceLoader.h
//
//

#ifndef CVSUMMARYEXTRACTOR_IRESOURCELOADER_H
#define CVSUMMARYEXTRACTOR_IRESOURCELOADER_H

#include <string>
#include <unordered_set>
#include <vector>
#include "../../Common/SLowerCaseUnorderedSet.h"

using namespace std;
using namespace Summary_Common;

namespace Summary_ResumeProcessor_Helpers
{
    //
    // CLASS : IResourceLoader
    //
    // DESCRIPTION : This class is base class for reading input data into vector or unordered_set
    //
    // Class objects are used in SLowerCaseUnorderetSet set
    // String case is lowered when creating class object
    // and can be accessed using string operator.
    //
    class IResourceLoader
    {
    public:
        virtual unordered_set<string> LoadIntoHashSet(string resourceName, char delimiter) = 0;
        virtual SLowerCaseUnorderedSet LoadIntoLowerCaseHashSet(string resourceName, char delimiter) = 0;
        virtual vector<string> Load(string resourceName, char delimiter) = 0;
        virtual vector<string> LoadExternal(string resourceName, char delimiter) = 0;
        virtual string LoadExternalToString(string resourceName) = 0;
        virtual vector<CLowerCaseString> LoadLowerCase(string resourceName, char delimiter) = 0;
    };
}

#endif //CVSUMMARYEXTRACTOR_IRESOURCELOADER_H
