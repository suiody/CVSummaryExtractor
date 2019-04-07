//
// File CLowerCaseString.cpp
//
//

#include <algorithm>
#include "CLowerCaseString.h"
#include "CDebugUtils.h"

/*
 * Summary_Common::CLowerCaseString::CLowerCaseString
 *
 * Description
 * Constructor. Initializes input string in lower case and saves in data property
 *
 * Inputs
 * const string &str - input string
 *
 * Returns
 *
 */
Summary_Common::CLowerCaseString::CLowerCaseString(const string &str)
{
    transform(str.begin(), str.end(), back_inserter(data), ::tolower);
}

/*
 * Summary_Common::CLowerCaseString::clear
 *
 * Description
 * Clears data property
 *
 * Inputs
 *
 * Returns
 *
 */
void Summary_Common::CLowerCaseString::clear()
{
    data = "";
}
