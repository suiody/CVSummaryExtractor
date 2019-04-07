//
// File ESectionType.h
//
//

#ifndef CVSUMMARYEXTRACTOR_ESECTIONTYPE_H
#define CVSUMMARYEXTRACTOR_ESECTIONTYPE_H

namespace Summary_Model
{
    //
    // ENUM : ESectionType
    //
    // DESCRIPTION : This enum lists all resume section types
    //
    //
    enum ESectionType
    {
        Unknown = 1,
        Personal = 2,
        Summary = 3,
        Education = 4,
        WorkExperience = 5,
        Projects = 6,
        Skills = 7,
        Courses = 8,
        Awards = 9,
        RawInput = 10,
        PopularWords = 11,
        TextLanguage = 12,
        EducationData = 13
    };
}

#endif //CVSUMMARYEXTRACTOR_ESECTIONTYPE_H
