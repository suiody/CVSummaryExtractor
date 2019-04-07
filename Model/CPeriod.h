//
// File CPeriod.h
//
//

#ifndef CVSUMMARYEXTRACTOR_CPERIOD_H
#define CVSUMMARYEXTRACTOR_CPERIOD_H

#include <string>

using namespace std;

namespace Summary_Model
{
    //
    // CLASS : CPeriod
    //
    // DESCRIPTION : This class encapsulates period data
    //
    // Class start and end date fields.
    //
    class CPeriod
    {

    public:
        /*
        * Summary_Model::CPeriod::CPeriod
        *
        * Description
        * Constructor
        *
        * Inputs
        * const string &start - start date string
        * const string &end - end date string
        *
        * Returns
        *
        */
        CPeriod(const string &start, const string &end) : start(start), end(end)
        {
        }

        /*
        * Summary_Model::CPeriod::CPeriod
        *
        * Description
        * Gets start date
        *
        * Inputs
        *
        * Returns
        * Start date as string
        */
        const string &GetStart() const
        {
            return start;
        }

        /*
        * Summary_Model::CPeriod::CPeriod
        *
        * Description
        * Sets start date
        *
        * Inputs
         * const string &start - start date
        *
        * Returns
        *
        */
        void SetStart(const string &start)
        {
            CPeriod::start = start;
        }

        /*
        * Summary_Model::CPeriod::CPeriod
        *
        * Description
        * Gets end date
        *
        * Inputs
        *
        * Returns
        * End date as string
        */
        const string &GetEnd() const
        {
            return end;
        }

        /*
        * Summary_Model::CPeriod::CPeriod
        *
        * Description
        * Sets end date
        *
        * Inputs
        * const string &end - end date as string
        *
        * Returns
        *
        */
        void SetEnd(const string &end)
        {
            CPeriod::end = end;
        }

    private:
        string start;
        string end;
    };

}

#endif //CVSUMMARYEXTRACTOR_CPERIOD_H
