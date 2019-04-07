//
// File CResumeProcessor.cpp
//
//

#include "CResumeProcessor.h"
#include "CInputReaderFactory.h"
#include "CSectionExtractor.h"
#include "Helpers/CResourceLoader.h"
#include "Parsers/CResumeBuilder.h"
#include "../Common/CDebugUtils.h"

using namespace Summary_ResumeProcessor_Helpers;
using namespace Summary_ResumeProcessor_Parsers;
using namespace Summary_Json;
using namespace Summary_Common;

/*
 * Summary_ResumeProcessor::CResumeProcessor::CResumeProcessor
 *
 * Description
 * Constructor
 *
 * Inputs
 * IOutputFormatter *outputFormatter - output formatter object
 *
 * Returns
 *
 */
Summary_ResumeProcessor::CResumeProcessor::CResumeProcessor(IOutputFormatter *outputFormatter)
{
    _outputFormatter = outputFormatter;
    CInputReaderFactory inputReaderFactory;

    _inputReaders = inputReaderFactory.LoadInputReaders();
}

/*
 * Summary_ResumeProcessor::CResumeProcessor::Process
 *
 * Description
 * Reads data from text file, processes it into sections, parses the sections data and returns output using output
 * formatter given in constructor
 *
 * Inputs
 * string location - input CV text file location
 *
 * Returns
 * Parsed CV summary output
 *
 */
string Summary_ResumeProcessor::CResumeProcessor::Process(string location)
{
    CDebugUtils::DebugCout("CResumeProcessor", "Process", "");
    string formatted = "";

    vector<string> rawInput = _inputReaders.ReadIntoList(location);

    CSectionExtractor sectionExtractor;
    vector<CSection> sections = sectionExtractor.ExtractFrom(rawInput);

    CResourceLoader resourceLoader;
    CResumeBuilder resumeBuilder(resourceLoader);
    CResume resume = resumeBuilder.Build(sections);

    formatted = _outputFormatter->Format(resume);

    return formatted;
}
