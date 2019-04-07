//
// File CLowerCaseString.h
//
//

#ifndef CVSUMMARYEXTRACTOR_LOWERCASESTRING_H
#define CVSUMMARYEXTRACTOR_LOWERCASESTRING_H

#include <string>

using namespace std;

namespace Summary_Common
{
    //
    // CLASS : CLowerCaseString
    //
    // DESCRIPTION : This class encapsulates string in lower case format
    //
    // Class objects are used in SLowerCaseUnorderetSet set
    // String case is lowered when creating class object
    // and can be accessed using string operator.
    //
    class CLowerCaseString
    {
    public:
        CLowerCaseString(const string &str);

        operator string&()             {return data;}
        operator string const&() const {return data;}

        void clear();

    private:
        string  data;
    };
}

#endif //CVSUMMARYEXTRACTOR_LOWERCASESTRING_H
