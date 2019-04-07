//
// File CEducationData.h
//
//

#ifndef CVSUMMARYEXTRACTOR_CEDUCATIONDATA_H
#define CVSUMMARYEXTRACTOR_CEDUCATIONDATA_H

#include <string>
#include <vector>
#include "CDates.h"

using namespace std;


namespace Summary_Model
{
    class CEducationData
    {
    public:
        CEducationData()
        {
            index = -1;
            line = "";
            CDates initDates;
            this->dates = initDates;
        }

        /*
        * Summary_Model::CEducationData::AddData
        *
        * Description
        * Adds index with line to the object
        *
        * Inputs
        * int index - line index
        * const string &line - line data
        *
        * Returns
        *
        */
        void AddData(int index, const string &line)
        {
            this->index = index;
            this->line = line;
        }

        /*
        * Summary_Model::CEducationData::GetIndex
        *
        * Description
        * Gets index of the line
        *
        * Inputs
        *
        * Returns
        * Index of the line
        */
        const int GetIndex() const
        {
            return this->index;
        }

        /*
        * Summary_Model::CEducationData::GetLine
        *
        * Description
        * Gets line data
        *
        * Inputs
        *
        * Returns
        * Index of the line
        */
        const string &GetLine() const
        {
            return this->line;
        }

        /*
        * Summary_Model::CEducationData::AddDate
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
            dates.AddDate(date);
        }

        /*
        * Summary_Model::CEducationData::GetDates
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
            return dates.GetDates();
        }

        friend bool operator== ( const CEducationData &n1, const CEducationData &n2)
        {
            return n1.index == n2.index;
        }

        bool operator<( const CEducationData& val ) const
        {
            return index < val.index;
        }

    private:
        int index;
        string line;
        CDates dates;
    };
}


#endif //CVSUMMARYEXTRACTOR_CEDUCATIONDATA_H
