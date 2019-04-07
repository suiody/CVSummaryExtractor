//
// File IDictionaryLoader.h
//
//

#ifndef CVSUMMARYEXTRACTOR_IDICTIONARYLOADER_H
#define CVSUMMARYEXTRACTOR_IDICTIONARYLOADER_H

#include <string>
#include <vector>
#include <map>
#include "../../Common/SLowerCaseUnorderedSet.h"

using namespace std;
using namespace Summary_Common;

namespace Summary_ResumeProcessor_Parsers
{
    class IDictionaryLoader
    {
    public:
        IDictionaryLoader()
        {

        }

    public:
        virtual const vector<string> &GetJobs() const {return jobLookUp;};
        virtual const vector<string> &GetUniversitiesLower() const {return universityLookUpLower;};
        virtual const vector<string> &GetCountries() const {return countryLookUp;};
        virtual const vector<string> &GetJobsLower() const {return jobLookUpLower;};
        virtual const vector<string> &GetCountriesLower() const {return countryLookUpLower;};
        virtual const map<string, string> &GetCountriesMap() const {return countryMap;};
        virtual const vector<string> &GetReligions() const {return religionLookUp;};
        virtual const vector<string> &GetStopWords() const {return stopWordsLookup;};
        virtual const vector<string> &GetNonNames() const {return nonNamesLookup;};
        virtual const vector<string> &GetSkills() const {return skillLookUp;};
        virtual const vector<string> &GetFieldsOfStudy() const {return fieldsOfStudyLookUp;};
        virtual const vector<string> &GetFieldsOfStudyShort() const {return fieldsOfStudyLookUpShort;};
        virtual const vector<string> &GetShortTaggedDegrees() const {return shortTaggedDegreesLookUp;};
        virtual const vector<string> &GetDegreeAbbbreviations() const {return degreeAbbbreviations;};
        virtual const vector<string> &GetDegreeAbbbreviationsShort() const {return degreeAbbbreviationsShort;};
        virtual const SLowerCaseUnorderedSet &GetFirstNames() const  {return firstNameLookUp;};

        vector<string> jobLookUp;
        vector<string> universityLookUpLower;
        vector<string> countryLookUp;
        vector<string> jobLookUpLower;
        vector<string> countryLookUpLower;
        map<string, string> countryMap;
        vector<string> religionLookUp;
        vector<string> stopWordsLookup;
        vector<string> nonNamesLookup;
        vector<string> skillLookUp;
        vector<string> fieldsOfStudyLookUp;
        vector<string> fieldsOfStudyLookUpShort;
        vector<string> shortTaggedDegreesLookUp;
        vector<string> degreeAbbbreviations;
        vector<string> degreeAbbbreviationsShort;
        SLowerCaseUnorderedSet firstNameLookUp;
    };
}


#endif //CVSUMMARYEXTRACTOR_IDICTIONARYLOADER_H
