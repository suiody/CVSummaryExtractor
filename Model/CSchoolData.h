//
// File CSchoolData.h
//
//

#ifndef CVSUMMARYEXTRACTOR_CSCHOOLDATA_H
#define CVSUMMARYEXTRACTOR_CSCHOOLDATA_H

#include <string>

using namespace std;


namespace Summary_Model
{
    class CSchoolData
    {
    public:
        CSchoolData(int index)
        {
            schoolName = "";
            schoolDegree = "";
            schoolLevel = "";
            schoolField = "";
            dateFrom = "";
            dateTo = "";
            this->index = index;
        }

        /*
        * Summary_Model::CSchoolData::AddSchoolName
        *
        * Description
        * Adds school name string
        *
        * Inputs
        * const string &schoolName - school name string
        *
        * Returns
        */
        void AddSchoolName(const string &schoolName)
        {
            this->schoolName = schoolName;
        }

        /*
        * Summary_Model::CSchoolData::GetSchoolName
        *
        * Description
        * School name string
        *
        * Inputs
        *
        * Returns
        * School name string
        */
        const string &GetSchoolName() const
        {
            return this->schoolName;
        }

        /*
        * Summary_Model::CSchoolData::AddSchoolDegree
        *
        * Description
        * Adds school degree string
        *
        * Inputs
        * const string &schoolDegree - school degree string
        *
        * Returns
        */
        void AddSchoolDegree(const string &schoolDegree)
        {
            this->schoolDegree = schoolDegree;
        }

        /*
        * Summary_Model::CSchoolData::GetSchoolDegree
        *
        * Description
        * School degree string
        *
        * Inputs
        *
        * Returns
        * School degree of study string
        */
        const string &GetSchoolDegree() const
        {
            return this->schoolDegree;
        }

        /*
        * Summary_Model::CSchoolData::AddSchoolLevel
        *
        * Description
        * Adds school level string
        *
        * Inputs
        * const string &schoolLevel - school level string
        *
        * Returns
        */
        void AddSchoolLevel(const string &schoolLevel)
        {
            this->schoolLevel = schoolLevel;
        }

        /*
        * Summary_Model::CSchoolData::GetSchoolLevel
        *
        * Description
        * School level string
        *
        * Inputs
        *
        * Returns
        * School level of study string
        */
        const string &GetSchoolLevel() const
        {
            return this->schoolLevel;
        }

        /*
        * Summary_Model::CSchoolData::AddDateFrom
        *
        * Description
        * Adds school field string
        *
        * Inputs
        * const string &schoolField - school field string
        *
        * Returns
        */
        void AddSchoolField(const string &schoolField)
        {
            this->schoolField = schoolField;
        }

        /*
        * Summary_Model::CSchoolData::GetSchoolField
        *
        * Description
        * School field string
        *
        * Inputs
        *
        * Returns
        * School field of study string
        */
        const string &GetSchoolField() const
        {
            return this->schoolField;
        }

        /*
        * Summary_Model::CSchoolData::AddDateFrom
        *
        * Description
        * Adds date from string
        *
        * Inputs
        * const string &dateFrom - date to string
        *
        * Returns
        */
        void AddDateFrom(const string &dateFrom)
        {
            this->dateFrom = dateFrom;
        }

        /*
        * Summary_Model::CSchoolData::GetDateFrom
        *
        * Description
        * Gets date from string
        *
        * Inputs
        *
        * Returns
        * date from string
        */
        const string &GetDateFrom() const
        {
            return this->dateFrom;
        }

        /*
        * Summary_Model::CSchoolData::AddDateTo
        *
        * Description
        * Adds date to string
        *
        * Inputs
        * const string &dateTo - date to string
        *
        * Returns
        */
        void AddDateTo(const string &dateTo)
        {
            this->dateTo = dateTo;
        }

        /*
        * Summary_Model::CSchoolData::GetDateTo
        *
        * Description
        * Gets date to string
        *
        * Inputs
        *
        * Returns
        * date to string
        */
        const string &GetDateTo() const
        {
            return this->dateTo;
        }

        /*
        * Summary_Model::CSchoolData::GetIndex
        *
        * Description
        * Gets school data index
        *
        * Inputs
        *
        * Returns
        * index
        */
        const int GetIndex() const
        {
            return this->index;
        }

        friend bool operator== ( const CSchoolData &n1, const CSchoolData &n2)
        {
            return n1.index == n2.index;
        }

        bool operator<( const CSchoolData& val ) const
        {
            return index < val.index;
        }

    private:

        string schoolName;
        string schoolDegree;
        string schoolLevel;
        string schoolField;
        string dateFrom;
        string dateTo;
        int index;
    };
}


#endif //CVSUMMARYEXTRACTOR_CSCHOOLDATA_H
