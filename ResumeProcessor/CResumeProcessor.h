//
// File CResumeProcessor.h
//
//

#ifndef CVSUMMARYEXTRACTOR_CRESUMEPROCESSOR_H
#define CVSUMMARYEXTRACTOR_CRESUMEPROCESSOR_H

#include <string>
#include "../Json/JsonOutputFormatter.h"
#include "../Model/IInputReader.h"
#include "../Model/CInputReaderBase.h"
#include "../PlainReader/CPlainTextInputReader.h"

using namespace std;
using namespace Summary_Model;
using namespace Summary_Json;
using namespace Summary_PlainReader;

namespace Summary_ResumeProcessor
{
    //
    // CLASS : CResumeProcessor
    //
    // DESCRIPTION : This class is used for processing resume data from input string
    //
    //
    class CResumeProcessor
    {
    public:
        CResumeProcessor(IOutputFormatter *outputFormatter);
        string Process(string location);

    private:
        IOutputFormatter *_outputFormatter;
        CPlainTextInputReader _inputReaders;
    };
}

#endif //CVSUMMARYEXTRACTOR_CRESUMEPROCESSOR_H
