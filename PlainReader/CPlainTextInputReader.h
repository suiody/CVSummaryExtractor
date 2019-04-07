//
// File CPlainTextInputReader.h
//
//

#ifndef CVSUMMARYEXTRACTOR_CPLAINTEXTINPUTREADER_H
#define CVSUMMARYEXTRACTOR_CPLAINTEXTINPUTREADER_H

#include "../Model/CInputReaderBase.h"

using namespace Summary_Model;

namespace Summary_PlainReader
{
    //
    // CLASS : CPlainTextInputReader
    //
    // DESCRIPTION : This class reads plain text data from input
    //
    class CPlainTextInputReader: public CInputReaderBase
    {
    protected:
        virtual bool CanHandle(const string &location) override;

        virtual vector<string> Handle(const string &location) override;
        virtual vector<wstring> HandleWide(const string &location) override;

    private:
        bool HasEnding(string const &fullString, string const &ending);
    };
}

#endif //CVSUMMARYEXTRACTOR_CPLAINTEXTINPUTREADER_H
