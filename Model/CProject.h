//
// File CProject.h
//
//

#ifndef CVSUMMARYEXTRACTOR_CPROJECT_H
#define CVSUMMARYEXTRACTOR_CPROJECT_H

#include <string>
#include <vector>

using namespace std;

namespace Summary_Model
{
    //
    // CLASS : CProject
    //
    // DESCRIPTION : This class encapsulates project data
    //
    // Class has title, summary, start and end date fields.
    //
    class CProject
    {
    public:
        /*
        * Summary_Model::CProject::CProject
        *
        * Description
        *
        * Inputs
        *
        * Returns
        *
        */
        CProject()
        {
        }

        /*
        * Summary_Model::CProject::GetTitle
        *
        * Description
        * Gets project title
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
        * Summary_Model::CProject::SetTitle
        *
        * Description
        * Sets project title
        *
        * Inputs
        * const string &title - project title
        *
        * Returns
        *
        */
        void SetTitle(const string &title)
        {
            CProject::title = title;
        }

        /*
        * Summary_Model::CProject::GetStartDate
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
        * Summary_Model::CProject::SetStartDate
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
            CProject::startDate = startDate;
        }

        /*
        * Summary_Model::CProject::GetEndDate
        *
        * Description
        * Gets end date
        *
        * Inputs
        *
        * Returns
        *
        */
        const string &GetEndDate() const
        {
            return endDate;
        }

        /*
        * Summary_Model::CProject::SetEndDate
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
            CProject::endDate = endDate;
        }

        /*
        * Summary_Model::CProject::GetSummary
        *
        * Description
        * Gets summary lines
        *
        * Inputs
        *
        * Returns
        * vector<string> - summary vector
        */
        const vector<string> &GetSummary() const
        {
            return summary;
        }

        /*
        * Summary_Model::CProject::SetSummary
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
            CProject::summary = summary;
        }

        /*
        * Summary_Model::CProject::AddSummary
        *
        * Description
        * Adds summary line
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
        vector<string> summary;
    };
}

#endif //CVSUMMARYEXTRACTOR_CPROJECT_H
