//
// File IInputReaderFactory.cpp
//
//

#include "IInputReaderFactory.h"

/*
 * Summary_Model::IInputReaderFactory::LoadInputReaders
 *
 * Description
 * Base class method to load input reader
 *
 * Inputs
 *
 * Returns
 * Base class object for input readers
 */
Summary_Model::IInputReader Summary_Model::IInputReaderFactory::LoadInputReaders()
{
    return Summary_Model::IInputReader();
}
