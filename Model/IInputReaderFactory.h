//
// File IInputReaderFactory.h
//
//

#ifndef CVSUMMARYEXTRACTOR_IINPUTREADERFACTORY_H
#define CVSUMMARYEXTRACTOR_IINPUTREADERFACTORY_H

#include "IInputReader.h"

namespace Summary_Model
{
    //
    // CLASS : IInputReaderFactory
    //
    // DESCRIPTION : This class is base class for loading input data reader(s)
    //
    // Class objects are used in SLowerCaseUnorderetSet set
    // String case is lowered when creating class object
    // and can be accessed using string operator.
    //
    class IInputReaderFactory
    {
    public:
        virtual IInputReader LoadInputReaders();
    };
}

#endif //CVSUMMARYEXTRACTOR_IINPUTREADERFACTORY_H
