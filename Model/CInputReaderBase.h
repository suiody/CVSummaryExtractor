//
// File CInputReaderBase.cpp
//
//

#ifndef CVSUMMARYEXTRACTOR_CINPUTREADERBASE_H
#define CVSUMMARYEXTRACTOR_CINPUTREADERBASE_H

#include "IInputReader.h"

namespace Summary_Model
{
    //
    // CLASS : CInputReaderBase
    //
    // DESCRIPTION : This class is base class for reading input from any format
    //
    //
    class CInputReaderBase: public IInputReader
    {
    public:
        vector<string> ReadIntoList(string location);
        vector<wstring> ReadIntoWideList(string location);

    protected:
        virtual bool CanHandle(const string &location) = 0;
        virtual vector<string> Handle(const string &location) = 0;
        virtual vector<wstring> HandleWide(const string &location) = 0;
    };
}


#endif //CVSUMMARYEXTRACTOR_CINPUTREADERBASE_H
