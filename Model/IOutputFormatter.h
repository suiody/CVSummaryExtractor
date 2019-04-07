//
// File IOutputFormatter.h
//
//

#ifndef CVSUMMARYEXTRACTOR_IOUTPUTFORMATTER_H
#define CVSUMMARYEXTRACTOR_IOUTPUTFORMATTER_H

#include <string>
#include "CResume.h"

using namespace std;

namespace Summary_Model
{
    //
    // CLASS : IOutputFormatter
    //
    // DESCRIPTION : This class is base class for outputting parser results into any format
    //
    //
    class IOutputFormatter
    {
    public:
        /*
        * Summary_Model::IOutputFormatter::IOutputFormatter
        *
        * Description
        * Constructor
        *
        * Inputs
        *
        * Returns
        *
        */
        IOutputFormatter()
        {
        }

        virtual string Format(const CResume &resume);
    };
}

#endif //CVSUMMARYEXTRACTOR_IOUTPUTFORMATTER_H
