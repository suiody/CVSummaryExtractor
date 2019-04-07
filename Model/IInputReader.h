//
// File IInputReader.h
//
//

#ifndef CVSUMMARYEXTRACTOR_IINPUTREADER_H
#define CVSUMMARYEXTRACTOR_IINPUTREADER_H

#include <string>
#include <vector>

using namespace std;

namespace Summary_Model
{
    //
    // CLASS : IInputReader
    //
    // DESCRIPTION : This class is interface for reading input data into string vector
    //
    //
    class IInputReader
    {
    public:
        virtual vector<string> ReadIntoList(string location);
    };
}

#endif //CVSUMMARYEXTRACTOR_IINPUTREADER_H
