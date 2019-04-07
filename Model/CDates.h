//
// File CDates.h
//
//

#ifndef CVSUMMARYEXTRACTOR_CDATES_H
#define CVSUMMARYEXTRACTOR_CDATES_H

#include <string>
#include <vector>

using namespace std;


namespace Summary_Model
{
    //
    // CLASS : CDates
    //
    // DESCRIPTION : This class encapsulates string vector with dates data
    //
    // Class has dates string vector as private member.
    //
    class CDates
    {
    public:
        CDates()
        {

        }

        /*
        * Summary_Model::CDates::CDates
        *
        * Description
        * Constructor. Adds date to the string vector
        *
        * Inputs
        * const string &date - input date string
        *
        * Returns
        *
        */
        CDates(const string &date)
        {
            dates.push_back(date);
        }

        /*
        * Summary_Model::CDates::AddDate
        *
        * Description
        * Adds date to the string vector
        *
        * Inputs
        * const string &date - input date string
        *
        * Returns
        *
        */
        void AddDate(const string &date)
        {
            dates.push_back(date);
        }

        /*
        * Summary_Model::CDates::GetDates
        *
        * Description
        * Gets string vector with dates information
        *
        * Inputs
        *
        * Returns
        * Dates vector
        */
        const vector<string> &GetDates() const
        {
            return dates;
        }

    private:
        vector<string> dates;
    };
}


#endif //CVSUMMARYEXTRACTOR_CDATES_H
