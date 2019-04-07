//
// File CInputReaderBase.h
//
//

#include <iostream>
#include "CInputReaderBase.h"
#include "../Common/CDebugUtils.h"

using namespace Summary_Common;

/*
 * Summary_Model::CInputReaderBase::ReadIntoList
 *
 * Description
 * Method for reading input file into string vector. Does return empty result if could not handle file
 *
 * Inputs
 *
 * Returns
 * Empty or filled vector<string>
 */
vector<string> Summary_Model::CInputReaderBase::ReadIntoList(string location)
{
    CDebugUtils::DebugCout("CInputReaderBase", "ReadIntoList", "");
    vector<string> list;

    if (CanHandle(location))
    {
        return Handle(location);
    }
    return list;
}

/*
 * Summary_Model::CInputReaderBase::ReadIntoWideList
 *
 * Description
 * Method for reading input file into string vector. Does return empty result if could not handle file
 *
 * Inputs
 *
 * Returns
 * Empty or filled vector<string>
 */
vector<wstring> Summary_Model::CInputReaderBase::ReadIntoWideList(string location)
{
    CDebugUtils::DebugCout("CInputReaderBase", "ReadIntoList", "");
    vector<wstring> list;

    if (CanHandle(location))
    {
        return HandleWide(location);
    }
    return list;
}
