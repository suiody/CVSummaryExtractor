//
// File CEducation.h
//
//

#ifndef CVSUMMARYEXTRACTOR_CEDUCATION_H
#define CVSUMMARYEXTRACTOR_CEDUCATION_H

#include <string>

using namespace std;

namespace Summary_Model
{
    //
    // CLASS : CEducation
    //
    // DESCRIPTION : This class encapsulates education data
    //
    // Class has school, course, start and end date fields.
    //
    class CEducation
    {

    public:
        /*
        * Summary_Model::CEducation::CEducation
        *
        * Description
        * Constructor
        *
        * Inputs
        *
        * Returns
        *
        */
        CEducation()
        {
        }

        /*
        * Summary_Model::CEducation::GetSchool
        *
        * Description
        * Gets school information
        * Inputs
        *
        * Returns
        * School as string
        */
        const string &GetSchool() const
        {
            return school;
        }

        /*
        * Summary_Model::CEducation::SetSchool
        *
        * Description
        * Sets school information
        *
        * Inputs
        * const string &school - school data
        *
        * Returns
        *
        */
        void SetSchool(const string &school)
        {
            CEducation::school = school;
        }

        /*
        * Summary_Model::CEducation::GetCourse
        *
        * Description
        * Gets course
        *
        * Inputs
        *
        * Returns
        * Course as string
        */
        const string &GetCourse() const
        {
            return course;
        }

        /*
        * Summary_Model::CEducation::SetCourse
        *
        * Description
        * Sets course
        *
        * Inputs
        * const string &course - course data
        *
        * Returns
        *
        */
        void SetCourse(const string &course)
        {
            CEducation::course = course;
        }

        /*
        * Summary_Model::CEducation::GetStartDate
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
        * Summary_Model::CEducation::SetStartDate
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
            CEducation::startDate = startDate;
        }

        /*
        * Summary_Model::CEducation::GetEndDate
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
        * Summary_Model::CEducation::SetEndDate
        *
        * Description
        * Sets end date
        *
        * Inputs
        * const string &endDate - end date as string
        *
        * Returns
        *
        */
        void SetEndDate(const string &endDate)
        {
            CEducation::endDate = endDate;
        }

    private:
        string school;
        string course;
        string startDate;
        string endDate;
    };
}

#endif //CVSUMMARYEXTRACTOR_CEDUCATION_H
