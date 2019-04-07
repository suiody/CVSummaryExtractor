//
// File CPosition.h
//
//

#ifndef CVSUMMARYEXTRACTOR_CPOSITION_H
#define CVSUMMARYEXTRACTOR_CPOSITION_H

#include <string>
#include <vector>

using namespace std;

namespace Summary_Model
{
    //
    // CLASS : CPosition
    //
    // DESCRIPTION : This class encapsulates position data
    //
    // Class has title, company, summary, start and end date fields.
    //
    class CPosition
    {

    public:
        /*
        * Summary_Model::CPosition::CPosition
        *
        * Description
        * Constructor
        * Inputs
        *
        * Returns
        *
        */
        CPosition()
        {
        }

        /*
        * Summary_Model::CPosition::GetTitle
        *
        * Description
        * Gets position title
        *
        * Inputs
        *
        * Returns
        * Title as string
        */
        const string &GetTitle() const
        {
            return title;
        }

        /*
        * Summary_Model::CPosition::SetTitle
        *
        * Description
        * Sets position title
        *
        * Inputs
        * const string &title - position title
        *
        * Returns
        *
        */
        void SetTitle(const string &title)
        {
            CPosition::title = title;
        }

        /*
        * Summary_Model::CPosition::GetStartDate(
        *
        * Description
        * Gets start date
        *
        * Inputs
        *
        * Returns
        * Start date as string
        */
        const string &GetStartDate() const
        {
            return startDate;
        }

        /*
        * Summary_Model::CPosition::SetStartDate
        *
        * Description
        * Sets start date
        *
        * Inputs
        * const string &startDate - start date
        *
        * Returns
        *
        */
        void SetStartDate(const string &startDate)
        {
            CPosition::startDate = startDate;
        }

        /*
        * Summary_Model::CPosition::GetEndDate
        *
        * Description
        * Gets end date
        *
        * Inputs
        *
        * Returns
        * End date as string
        */
        const string &GetEndDate() const
        {
            return endDate;
        }

        /*
        * Summary_Model::CPosition::SetEndDate
        *
        * Description
        * Sets end date
        *
        * Inputs
        * const string &endDate - end date
        *
        * Returns
        *
        */
        void SetEndDate(const string &endDate)
        {
            CPosition::endDate = endDate;
        }

        /*
        * Summary_Model::CPosition::GetCompany
        *
        * Description
        * Gets company information
        *
        * Inputs
        *
        * Returns
        * Company as string
        */
        const string &GetCompany() const
        {
            return company;
        }

        /*
        * Summary_Model::CPosition::SetCompany
        *
        * Description
        * Sets company in formation
        *
        * Inputs
        * const string &company - company
        *
        * Returns
        *
        */
        void SetCompany(const string &company)
        {
            CPosition::company = company;
        }

        /*
        * Summary_Model::CPosition::GetSummary
        *
        * Description
        * Gets summary lines
        *
        * Inputs
        *
        * Returns
        * vector<string> - summary lines
        */
        const vector<string> &GetSummary() const
        {
            return summary;
        }

        /*
        * Summary_Model::CPosition::SetSummary
        *
        * Description
        * Sets summary lines
        *
        * Inputs
        * const vector<string> &summary - summary lines
        *
        * Returns
        *
        */
        void SetSummary(const vector<string> &summary)
        {
            CPosition::summary = summary;
        }

        /*
        * Summary_Model::CPosition::AddSummary
        *
        * Description
        * Adds summary to the existing lines
        *
        * Inputs
        * string inputSummary - summary line
        *
        * Returns
        *
        */
        void AddSummary(string inputSummary)
        {
            summary.push_back(inputSummary);
        }

    private:
        string title;
        string startDate;
        string endDate;
        string company;
        vector<string> summary;
    };
}

#endif //CVSUMMARYEXTRACTOR_CPOSITION_H
