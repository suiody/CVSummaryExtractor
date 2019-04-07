//
// File CInputReaderFactory.h
//
//

#ifndef CVSUMMARYEXTRACTOR_CINPUTREADERFACTORY_H
#define CVSUMMARYEXTRACTOR_CINPUTREADERFACTORY_H

#include "../Model/IInputReaderFactory.h"
#include "../PlainReader/CPlainTextInputReader.h"

using namespace std;
using namespace Summary_Model;
using namespace Summary_PlainReader;

namespace Summary_ResumeProcessor
{
    //
    // CLASS : CInputReaderFactory
    //
    // DESCRIPTION : This class is used for loading input readers, at the moment only plain text reader
    //
    //
    class CInputReaderFactory
    {
    public:
        CPlainTextInputReader LoadInputReaders();
    };
}

#endif //CVSUMMARYEXTRACTOR_CINPUTREADERFACTORY_H
