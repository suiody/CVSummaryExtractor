//
// File CSection.h
//
//

#ifndef CVSUMMARYEXTRACTOR_CSECTION_H
#define CVSUMMARYEXTRACTOR_CSECTION_H

#include <string>
#include <vector>
#include "ESectionType.h"

using namespace std;

namespace Summary_Model
{
    //
    // CLASS : CSection
    //
    // DESCRIPTION : This class encapsulates education data
    //
    // Class has type and content fields.
    //
    class CSection
    {
    public:
        /*
        * Summary_Model::CSection::CSection
        *
        * Description
        * Constructor
        *
        * Inputs
        *
        * Returns
        *
        */
        CSection()
        {
        }

        /*
        * Summary_Model::CSection::getType
        *
        * Description
        *
        * Inputs
        *
        * Returns
        * Section type
        */
        const ESectionType &getType() const
        {
            return type;
        }

        /*
        * Summary_Model::CSection::SetType
        *
        * Description
        * Sets section type
        *
        * Inputs
        * const ESectionType &type - section type to be set
        *
        * Returns
        *
        */
        void SetType(const ESectionType &type)
        {
            CSection::type = type;
        }

        /*
        * Summary_Model::CSection::GetContent
        *
        * Description
        * Gets section content vector
        *
        * Inputs
        *
        * Returns
        * vector<string> - section content vector
        */
        const vector<string> &GetContent() const
        {
            return content;
        }

        /*
        * Summary_Model::CSection::SetContent
        *
        * Description
        * Sets section content
        *
        * Inputs
        * const vector<string> &content - vector with new content
        *
        * Returns
        *
        */
        void SetContent(const vector<string> &content)
        {
            CSection::content = content;
        }

        /*
        * Summary_Model::CSection::AddContent
        *
        * Description
        * Adds line to the section content
        *
        * Inputs
        * string input - new line to be added
        *
        * Returns
        *
        */
        void AddContent(string input)
        {
            content.push_back(input);
        }

        /*
        * Summary_Model::CSection::ClearContent
        *
        * Description
        * Clears section content
        *
        * Inputs
        *
        * Returns
        *
        */
        void ClearContent()
        {
            content.clear();
        }

        /*
        * Summary_Model::CSection::AddRawContent
        *
        * Description
        * Add on line to section raw content lines
        *
        * Inputs
        * string input - new raw content line
        *
        * Returns
        *
        */
        void AddRawContent(string input)
        {
            rawContent.push_back(input);
        }

        /*
        * Summary_Model::CSection::ClearRawContent
        *
        * Description
        * Clears section raw content lines
        *
        * Inputs
        *
        * Returns
        *
        */
        void ClearRawContent()
        {
            rawContent.clear();
        }

        /*
        * Summary_Model::CSection::GetRawContent
        *
        * Description
        * Gets section string lines raw content
        *
        * Inputs
        *
        * Returns
        * vector<string> with raw content
        */
        const vector<string> &GetRawContent() const
        {
            return rawContent;
        }

        /*
        * Summary_Model::CSection::SetRawContent
        *
        * Description
        * Sets section raw string lines content
        *
        * Inputs
        * (const vector<string> &rawContent - string vector with the new content
        *
        * Returns
        *
        */
        void SetRawContent(const vector<string> &rawContent)
        {
            CSection::rawContent = rawContent;
        }

        bool operator < (const CSection& str) const
        {
            return ((int)type < (int)str.getType());
        }

    private:
        ESectionType type;
        vector<string> content;
        vector<string> rawContent;

    };

}

#endif //CVSUMMARYEXTRACTOR_CSECTION_H
