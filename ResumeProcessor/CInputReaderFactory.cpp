//
// File CInputReaderFactory.cpp
//
//

#include "CInputReaderFactory.h"
#include "../PlainReader/CPlainTextInputReader.h"
#include "../Common/CDebugUtils.h"

using namespace Summary_Common;

/*
 * Summary_ResumeProcessor::CInputReaderFactory::LoadInputReaders
 *
 * Description
 * Loads plain text reader
 *
 * Inputs
 *
 * Returns
 * CPlainTextInputReader object
 *
 */
CPlainTextInputReader Summary_ResumeProcessor::CInputReaderFactory::LoadInputReaders()
{
    CDebugUtils::DebugCout("CInputReaderFactory", "LoadInputReaders", "");
    CPlainTextInputReader inputReader;
    return inputReader;
}
