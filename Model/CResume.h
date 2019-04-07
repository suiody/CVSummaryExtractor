//
// File CResume.h
//
//

#ifndef CVSUMMARYEXTRACTOR_CRESUME_H
#define CVSUMMARYEXTRACTOR_CRESUME_H

#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <algorithm>
#include "CPosition.h"
#include "CProject.h"
#include "CEducation.h"
#include "CEducationData.h"
#include "CDates.h"
#include "CSchoolData.h"
#include "../Common/CTextUtils.h"

using namespace std;
using namespace Summary_Common;

typedef vector<string> vector_string;

namespace Summary_Model
{
    //
    // CLASS : CResume
    //
    // DESCRIPTION : This class encapsulates resume data
    //
    // Class has first name, last name, gender, e-mail, phone, languages
    // summary, skills, location, work positions, projects, social profiles,
    // educations, awards and courses fields.
    //
    class CResume
    {
    public:
        /*
        * Summary_Model::CResume::CResume
        *
        * Description
        * Constructor
        *
        * Inputs
        *
        * Returns
        *
        */
        CResume()
        {
        }

        /*
        * Summary_Model::CResume::GetFirstName
        *
        * Description
        * Gets first name
        *
        * Inputs
        *
        * Returns
        * First name string
        */
        const string &GetFirstName() const
        {
            return firstName;
        }

        /*
        * Summary_Model::CResume::SetFirstName
        *
        * Description
        * Sets first name
        *
        * Inputs
        * const string &firstName - first name
        *
        * Returns
        *
        */
        void SetFirstName(const string &firstName)
        {
            CResume::firstName = firstName;
        }

        /*
        * Summary_Model::CResume::HasFirstName
        *
        * Description
        * Gets true if first name length > 0
        *
        * Inputs
        *
        * Returns
        * true if first name length > 0
        */
        const bool HasFirstName() const
        {
            return firstName.length() > 0;
        }

        /*
        * Summary_Model::CResume::GetMiddleName
        *
        * Description
        * Gets middle name
        *
        * Inputs
        *
        * Returns
        * Middle name string
        */
        const string &GetMiddleName() const
        {
            return middleName;
        }

        /*
        * Summary_Model::CResume::SetMiddleName
        *
        * Description
        * Sets middle name
        *
        * Inputs
        * const string &middleName - middle name
        *
        * Returns
        *
        */
        void SetMiddleName(const string &middleName)
        {
            CResume::middleName = middleName;
        }

        /*
        * Summary_Model::CResume::HasMiddleName
        *
        * Description
        * Gets true if middle name length > 0
        *
        * Inputs
        *
        * Returns
        * true if middle name length > 0
        */
        const bool HasMiddleName() const
        {
            return middleName.length() > 0;
        }

        /*
        * Summary_Model::CResume::GetLastName
        *
        * Description
        * Gets last name
        *
        * Inputs
        *
        * Returns
        * Last name string
        */
        const string &GetLastName() const
        {
            return lastName;
        }

        /*
        * Summary_Model::CResume::SetLastName
        *
        * Description
        * Sets last name
        *
        * Inputs
        * const string &lastName - last name
        *
        * Returns
        *
        */
        void SetLastName(const string &lastName)
        {
            CResume::lastName = lastName;
        }

        /*
        * Summary_Model::CResume::AddLastName
        *
        * Description
        * Adds last name string portion
        *
        * Inputs
        * const string &lastName - string to be added to last name
        *
        * Returns
        *
        */
        void AddLastName(const string &lastName)
        {
            if (CResume::lastName.length() > 30 || lastName.length() > 30)
            {
                return;
            }
            CResume::lastName += (CResume::lastName.length() > 0 && lastName.length() > 0 ? " " : "") + lastName;
        }

        /*
        * Summary_Model::CResume::HasLastName
        *
        * Description
        * Gets true if last name length > 0
        *
        * Inputs
        *
        * Returns
        * true if last name length > 0
        */
        const bool HasLastName() const
        {
            return lastName.length() > 0;
        }

        /*
        * Summary_Model::CResume::GetGender
        *
        * Description
        * Gets gender
        *
        * Inputs
        *
        * Returns
        * Gender string (male, female)
        */
        const string &GetGender() const
        {
            return gender;
        }

        /*
        * Summary_Model::CResume::SetGender
        *
        * Description
        * Sets gender
        *
        * Inputs
        * const string &gender - gender string
        *
        * Returns
        *
        */
        void SetGender(const string &gender)
        {
            CResume::gender = gender;
        }

        /*
        * Summary_Model::CResume::GetBirthDay
        *
        * Description
        * Gets birthday
        *
        * Inputs
        *
        * Returns
        * Birthday in string format
        */
        const string &GetBirthDay() const
        {
            return birthDay;
        }

        /*
        * Summary_Model::CResume::SetBirthDay
        *
        * Description
        * Sets birthday
        *
        * Inputs
        * const string &birthDay - birthday in string format
        *
        * Returns
        *
        */
        void SetBirthDay(const string &birthDay)
        {
            CResume::birthDay = birthDay;
        }

        /*
        * Summary_Model::CResume::GetEmailAddress
        *
        * Description
        * Gets e-mail address
        *
        * Inputs
        *
        * Returns
        *
        */
        const string &GetEmailAddress() const
        {
            return emailAddress;
        }

        /*
        * Summary_Model::CResume::SetEmailAddress
        *
        * Description
        * Sets e-mail address
        *
        * Inputs
        * const string &emailAddress - e-mail
        *
        * Returns
        *
        */
        void SetEmailAddress(const string &emailAddress)
        {
            CResume::emailAddress = emailAddress;
        }

        /*
        * Summary_Model::CResume::GetPhoneNumbers
        *
        * Description
        * Gets on or more phone numbers on one line
        *
        * Inputs
        *
        * Returns
        * Phone numbers string
        */
        const string &GetPhoneNumbers() const
        {
            return phoneNumbers;
        }

        /*
        * Summary_Model::CResume::SetPhoneNumbers
        *
        * Description
        * Sets phone numbers
        *
        * Inputs
        * const string &phoneNumbers - phone numbers
        *
        * Returns
        *
        */
        void SetPhoneNumbers(const string &phoneNumbers)
        {
            CResume::phoneNumbers = phoneNumbers;
        }

        /*
        * Summary_Model::CResume::GetSkype
        *
        * Description
        * Gets skype
        *
        * Inputs
        *
        * Returns
        * Skype string
        */
        const string &GetSkype() const
        {
            return skype;
        }

        /*
        * Summary_Model::CResume::SetSkype
        *
        * Description
        * Sets skype
        *
        * Inputs
        * const string &skype - skype string
        *
        * Returns
        *
        */
        void SetSkype(const string &skype)
        {
            CResume::skype = skype;
        }

        /*
        * Summary_Model::CResume::GetWebsite
        *
        * Description
        * Gets website url
        *
        * Inputs
        *
        * Returns
        * Website url string
        */
        const string &GetWebsite() const
        {
            return website;
        }

        /*
        * Summary_Model::CResume::SettWebsite
        *
        * Description
        * Sets website url
        *
        * Inputs
        * const string &website - website string
        *
        * Returns
        *
        */
        void SetWebsite(const string &website)
        {
            CResume::website = website;
        }

        /*
        * Summary_Model::CResume::GetFacebook
        *
        * Description
        * Gets facebook url
        *
        * Inputs
        *
        * Returns
        * Facebook url string
        */
        const string &GetFacebook() const
        {
            return facebook;
        }

        /*
        * Summary_Model::CResume::SetFacebook
        *
        * Description
        * Sets facebook url
        *
        * Inputs
        * const string &facebook - facebook string
        *
        * Returns
        *
        */
        void SetFacebook(const string &facebook)
        {
            CResume::facebook = facebook;
        }

        /*
        * Summary_Model::CResume::GetTwitter
        *
        * Description
        * Gets twitter url
        *
        * Inputs
        *
        * Returns
        * Twitter url string
        */
        const string &GetTwitter() const
        {
            return twitter;
        }

        /*
        * Summary_Model::CResume::SetTwitter
        *
        * Description
        * Sets twitter url
        *
        * Inputs
        * const string &twitter - twitter string
        *
        * Returns
        *
        */
        void SetTwitter(const string &twitter)
        {
            CResume::twitter = twitter;
        }

        /*
        * Summary_Model::CResume::GetGooglePlus
        *
        * Description
        * Gets googleplus url
        *
        * Inputs
        *
        * Returns
        * GooglePlus url string
        */
        const string &GetGooglePlus() const
        {
            return googlePlus;
        }

        /*
        * Summary_Model::CResume::SetGooglePlus
        *
        * Description
        * Sets googleplus url
        *
        * Inputs
        * const string &googlePlus - googleplus string
        *
        * Returns
        *
        */
        void SetGooglePlus(const string &googlePlus)
        {
            CResume::googlePlus = googlePlus;
        }

        /*
        * Summary_Model::CResume::GetLinkedIn
        *
        * Description
        * Gets linkedin url
        *
        * Inputs
        *
        * Returns
        * LinkedIn url string
        */
        const string &GetLinkedIn() const
        {
            return linkedIn;
        }

        /*
        * Summary_Model::CResume::SetLinkedIn
        *
        * Description
        * Sets linkedin url
        *
        * Inputs
        * const string &linkedIn - linkedin string
        *
        * Returns
        *
        */
        void SetLinkedIn(const string &linkedIn)
        {
            CResume::linkedIn = linkedIn;
        }

        /*
        * Summary_Model::CResume::GetInstagram
        *
        * Description
        * Gets instragram url
        *
        * Inputs
        *
        * Returns
        * Instagram url string
        */
        const string &GetInstagram() const
        {
            return instagram;
        }

        /*
        * Summary_Model::CResume::SetInstagram
        *
        * Description
        * Sets instagram url
        *
        * Inputs
        * const string &instagram - instragram string
        *
        * Returns
        *
        */
        void SetInstagram(const string &instagram)
        {
            CResume::instagram = instagram;
        }

        /*
        * Summary_Model::CResume::GetLanguages
        *
        * Description
        * Gets languages
        *
        * Inputs
        *
        * Returns
        * Languages on one line
        */
        const string &GetLanguages() const
        {
            return languages;
        }

        /*
        * Summary_Model::CResume::SetLanguages
        *
        * Description
        * Sets languages
        *
        * Inputs
        * const string &languages - language(s) string
        *
        * Returns
        *
        */
        void SetLanguages(const string &languages)
        {
            CResume::languages = languages;
        }

        /*
        * Summary_Model::CResume::GetSummaryDescription
        *
        * Description
        * Gets summary description
        *
        * Inputs
        *
        * Returns
        * Summary description string
        */
        const string &GetSummaryDescription() const
        {
            return summaryDescription;
        }

        /*
        * Summary_Model::CResume::SetSummaryDescription
        *
        * Description
        * Sets summary description string
        *
        * Inputs
        * const string &summaryDescription - summary string
        *
        * Returns
        *
        */
        void SetSummaryDescription(const string &summaryDescription)
        {
            CResume::summaryDescription = summaryDescription;
        }

        /*
        * Summary_Model::CResume::GetSkills
        *
        * Description
        * Gets skills string vector
        *
        * Inputs
        *
        * Returns
        * vector<string> - skills
        */
        const vector<string> &GetSkills() const
        {
            return skills;
        }

        /*
        * Summary_Model::CResume::SetSkills
        *
        * Description
        * Sets skills vector
        *
        * Inputs
        * const vector<string> &skills - string vector
        *
        * Returns
        *
        */
        void SetSkills(const vector<string> &skills)
        {
            CResume::skills = skills;
        }

        /*
        * Summary_Model::CResume::GetLocation
        *
        * Description
        * Gets location
        *
        * Inputs
        *
        * Returns
        * Location string
        */
        const string &GetLocation() const
        {
            return location;
        }

        /*
        * Summary_Model::CResume::SetLocation
        *
        * Description
        * Sets location
        *
        * Inputs
        * const string &location - location string
        *
        * Returns
        *
        */
        void SetLocation(const string &location)
        {
            CResume::location = location;
        }

        /*
        * Summary_Model::CResume::GetCountry
        *
        * Description
        * Gets country
        *
        * Inputs
        *
        * Returns
        * Country string
        */
        const string &GetCountry() const
        {
            return country;
        }

        /*
        * Summary_Model::CResume::SetCountry
        *
        * Description
        * Sets country
        *
        * Inputs
        * const string &country - country string
        *
        * Returns
        *
        */
        void SetCountry(const string &country)
        {
            CResume::country = country;
        }

        /*
        * Summary_Model::CResume::HasCountry
        *
        * Description
        * Gets true if country length > 0
        *
        * Inputs
        *
        * Returns
        * true if country length > 0
        */
        const bool HasCountry() const
        {
            return country.length() > 0;
        }

        /*
        * Summary_Model::CResume::GetReligion
        *
        * Description
        * Gets religion
        *
        * Inputs
        *
        * Returns
        * Religion string
        */
        const string &GetReligion() const
        {
            return religion;
        }

        /*
        * Summary_Model::CResume::SetReligion
        *
        * Description
        * Sets religion
        *
        * Inputs
        * const string &religion - religion string
        *
        * Returns
        *
        */
        void SetReligion(const string &religion)
        {
            CResume::religion = religion;
        }

        /*
        * Summary_Model::CResume::GetTextLanguageName
        *
        * Description
        * Gets overall text language name
        *
        * Inputs
        *
        * Returns
        * Text language name string
        */
        const string &GetTextLanguageName() const
        {
            return textLanguageName;
        }

        /*
        * Summary_Model::CResume::SetTextLanguageName
        *
        * Description
        * Sets overall text language name
        *
        * Inputs
        * const string &textLanguageName - text language name
        *
        * Returns
        *
        */
        void SetTextLanguageName(const string &textLanguageName)
        {
            CResume::textLanguageName = textLanguageName;
        }

        /*
        * Summary_Model::CResume::GetTextLanguageCode
        *
        * Description
        * Gets overall text language code
        *
        * Inputs
        *
        * Returns
        * Text language code string
        */
        const string &GetTextLanguageCode() const
        {
            return textLanguageCode;
        }

        /*
        * Summary_Model::CResume::SetTextLanguageCode
        *
        * Description
        * Sets overall text language code
        *
        * Inputs
        * const string &textLanguageCode - text language code
        *
        * Returns
        *
        */
        void SetTextLanguageCode(const string &textLanguageCode)
        {
            CResume::textLanguageCode = textLanguageCode;
        }

        /*
        * Summary_Model::CResume::GetTitle
        *
        * Description
        * Gets person title (mr, mrs etc.)
        *
        * Inputs
        *
        * Returns
        * Person title string
        */
        string GetTitle() const
        {
            return CTextUtils::FirstLetterUpCase(title);
        }

        /*
        * Summary_Model::CResume::SetTitle
        *
        * Description
        * Sets title of the person (mr, mrs, ms etc.)
        *
        * Inputs
        * const string &title - person title
        *
        * Returns
        *
        */
        void SetTitle(const string &title)
        {
            CResume::title = title;
        }

        /*
        * Summary_Model::CResume::GetTextLength
        *
        * Description
        * Gets length of original resume input or translated resume input text
        * Length depends on the source where all data is extracted from. If resume is translated then the
        * source is already translated text and will return length of that text, otherwise length of
        * original text
        *
        * Inputs
        *
        * Returns
        * Person resume input text length
        */
        long const &GetTextLength() const
        {
            return textLength;
        }

        /*
        * Summary_Model::CResume::SetTextLength
        *
        * Description
        * Sets length of the original input resume or translated resume text
        *
        * Inputs
        * long textLength - text length
        *
        * Returns
        *
        */
        void SetTextLength(long textLength)
        {
            CResume::textLength = textLength;
        }

        /*
        * Summary_Model::CResume::GetPositions
        *
        * Description
        * Gets work positions vector
        *
        * Inputs
        *
        * Returns
        * vector<CPosition> - work positions
        */
        const vector<CPosition> &GetPositions() const
        {
            return positions;
        }

        /*
        * Summary_Model::CResume::SetPositions
        *
        * Description
        * Sets positions vector
        *
        * Inputs
        * const vector<CPosition> &positions - work positions
        *
        * Returns
        *
        */
        void SetPositions(const vector<CPosition> &positions)
        {
            CResume::positions = positions;
        }

        /*
        * Summary_Model::CResume::GetProjects
        *
        * Description
        * Gets involved projects
        *
        * Inputs
        *
        * Returns
        * vector<CProject> - projects vector
        */
        const vector<CProject> &GetProjects() const
        {
            return projects;
        }

        /*
        * Summary_Model::CResume::SetProjects
        *
        * Description
        * Sets projects vector
        *
        * Inputs
        * const vector<CProject> &projects - projects vector
        *
        * Returns
        *
        */
        void SetProjects(const vector<CProject> &projects)
        {
            CResume::projects = projects;
        }

        /*
        * Summary_Model::CResume::GetSocialProfiles
        *
        * Description
        * Gets social profiles string vector
        *
        * Inputs
        *
        * Returns
        * vector<string> - social profiles
        */
        const vector<string> &GetSocialProfiles() const
        {
            return socialProfiles;
        }

        /*
        * Summary_Model::CResume::SetSocialProfiles
        *
        * Description
        * Sets social profiles vector
        *
        * Inputs
        * const vector<string> &socialProfiles - social profiles
        *
        * Returns
        *
        */
        void SetSocialProfiles(const vector<string> &socialProfiles)
        {
            CResume::socialProfiles = socialProfiles;
        }

        /*
        * Summary_Model::CResume::GetEducations
        *
        * Description
        * Gets education objects vector
        *
        * Inputs
        *
        * Returns
        * vector<CEducation> - educations
        */
        const vector<CEducation> &GetEducations() const
        {
            return educations;
        }

        /*
        * Summary_Model::CResume::SetEducations
        *
        * Description
        * Sets education objects vector
        *
        * Inputs
        * const vector<CEducation> &educations
        *
        * Returns
        *
        */
        void SetEducations(const vector<CEducation> &educations)
        {
            CResume::educations = educations;
        }

        /*
        * Summary_Model::CResume::GetCourses
        *
        * Description
        * Gets courses string vector
        *
        * Inputs
        *
        * Returns
        * vector<string> - courses
        */
        const vector<string> &GetCourses() const
        {
            return courses;
        }

        /*
        * Summary_Model::CResume::SetCourses
        *
        * Description
        * Sets courses string vector
        *
        * Inputs
        * const vector<string> &courses - courses vector
        *
        * Returns
        *
        */
        void SetCourses(const vector<string> &courses)
        {
            CResume::courses = courses;
        }

        /*
        * Summary_Model::CResume::GetAwards
        *
        * Description
        * Gets awards string vector
        *
        * Inputs
        *
        * Returns
        * vector<string> - awards vector
        */
        const vector<string> &GetAwards() const
        {
            return awards;
        }

        /*
        * Summary_Model::CResume::SetAwards
        *
        * Description
        * Sets awards vector
        *
        * Inputs
        * const vector<string> &awards - awards
        *
        * Returns
        *
        */
        void SetAwards(const vector<string> &awards)
        {
            CResume::awards = awards;
        }

        /*
        * Summary_Model::CResume::AddSkill
        *
        * Description
        * Adds one skill to the collection (vector)
        *
        * Inputs
        * string skill - skill to be added
        *
        * Returns
        *
        */
        bool AddSkill(const string &skill)
        {
            if ( find(skills.begin(), skills.end(), skill)==skills.end() )
            {
                skills.push_back(skill);
                return true;
            }

            return false;
        }

        /*
        * Summary_Model::CResume::AddSkillCount
        *
        * Description
        * Adds skill with its count to the skills map
        *
        * Inputs
        * string skill - skill to be added
        * int count - skill count
        *
        * Returns
        *
        */
        void AddSkillCount(const string &skill, int count)
        {
            skillCounts[skill] = count;
        }

        const vector<string> GetSkillsByPopularity() const;

        /*
        * Summary_Model::CResume::AddPosition
        *
        * Description
        * Adds position object to vector
        *
        * Inputs
        * CPosition position - position to be added
        *
        * Returns
        *
        */
        void AddPosition(const CPosition &position)
        {
            positions.push_back(position);
        }

        /*
        * Summary_Model::CResume::GetCurrentPosition
        *
        * Description
        * Gets reference to the last position object in vector
        *
        * Inputs
        *
        * Returns
        * CPosition object
        */
        CPosition &GetCurrentPosition()
        {
            return positions.back();
        }

        /*
        * Summary_Model::CResume::AddProject
        *
        * Description
        * Adds project to the projects vector
        *
        * Inputs
        * CProject project - project to be added
        *
        * Returns
        *
        */
        void AddProject(const CProject &project)
        {
            projects.push_back(project);
        }

        /*
        * Summary_Model::CResume::GetCurrentProject
        *
        * Description
        * Gets latest project object from the vector
        *
        * Inputs
        *
        * Returns
        * CProject object
        */
        CProject &GetCurrentProject()
        {
            return projects.back();
        }

        /*
        * Summary_Model::CResume::AddSocialProfile
        *
        * Description
        * Adds social profile to the profiles
        *
        * Inputs
        * string profile - profile string
        *
        * Returns
        *
        */
        void AddSocialProfile(const string &profile)
        {
            if (find(socialProfiles.begin(), socialProfiles.end(), profile) == socialProfiles.end())
            {
                socialProfiles.push_back(profile);
            }
        }

        /*
        * Summary_Model::CResume::AddEducation
        *
        * Description
        * Adds education object to the vector
        *
        * Inputs
        * CEducation education - education object
        *
        * Returns
        *
        */
        void AddEducation(const CEducation &education)
        {
            educations.push_back(education);
        }

        /*
        * Summary_Model::CResume::GetCurrentEducation
        *
        * Description
        * Gets latest education object from the vector
        *
        * Inputs
        *
        * Returns
        * CEducation - education object
        */
        CEducation &GetCurrentEducation()
        {
            return educations.back();
        }

        /*
        * Summary_Model::CResume::AddCourse
        *
        * Description
        * Adds course to the courses vector
        *
        * Inputs
        * string course - course to be added
        *
        * Returns
        *
        */
        void AddCourse(const string &course)
        {
            courses.push_back(course);
        }

        /*
        * Summary_Model::CResume::AddAward
        *
        * Description
        * Adds award to the awards vector
        *
        * Inputs
        * string award - award to be added
        *
        * Returns
        *
        */
        void AddAward(const string &award)
        {
            awards.push_back(award);
        }

        /*
        * Summary_Model::CResume::GetRawInput
        *
        * Description
        * Gets raw input string lines vector
        *
        * Inputs
        *
        * Returns
        * vector<string> - raw input vector
        */
        const vector<string> &GetRawInput() const
        {
            return rawInput;
        }

        /*
        * Summary_Model::CResume::SetRawInput
        *
        * Description
        * Sets raw input string lines vector
        *
        * Inputs
        * const vector<string> &rawInput - raw input vector
        *
        * Returns
        *
        */
        void SetRawInput(const vector<string> &rawInput)
        {
            vector <string> replacedInput;

            for (string line: rawInput)
            {
                replacedInput.push_back(CTextUtils::ReplaceEscapeCharacters(line));
            }

            CResume::rawInput = replacedInput;
        }

        /*
        * Summary_Model::CResume::AddRawInput
        *
        * Description
        * Adds one line to the raw input string lines vector
        *
        * Inputs
        * const string &line - raw input line
        *
        * Returns
        *
        */
        void AddRawInput(const string &line)
        {
            rawInput.push_back(CTextUtils::ReplaceEscapeCharacters(line));
        }

        /*
        * Summary_Model::CResume::GetTranslation
        *
        * Description
        * Gets translation string lines vector
        *
        * Inputs
        *
        * Returns
        * vector<string> - translation vector
        */
        const vector<string> &GetTranslation() const
        {
            return translation;
        }

        /*
        * Summary_Model::CResume::SetTranslation
        *
        * Description
        * Sets translation string lines vector
        *
        * Inputs
        * const vector<string> &translation - translation vector
        *
        * Returns
        *
        */
        void SetTranslation(const vector<string> &translation)
        {
            CResume::translation = translation;
        }

        /*
        * Summary_Model::CResume::AddTranslation
        *
        * Description
        * Adds one line to the translation string lines vector
        *
        * Inputs
        * const string &line - translation line
        *
        * Returns
        *
        */
        void AddTranslation(const string &line)
        {
            translation.push_back(line);
        }

        /*
        * Summary_Model::CResume::GetTranslationSize
        *
        * Description
        * Gets translation size of all lines
        *
        * Inputs
        *
        * Returns
        * Translation text size
        */
        long GetTranslationSize()
        {
            long translationSize = 0;

            for (string line: translation)
            {
                translationSize += line.length();
            }

            return translationSize;
        }

        /*
        * Summary_Model::CResume::GetWords
        *
        * Description
        * Gets document word and occurrence count map
        *
        * Inputs
        *
        * Returns
        * map<string, int> - words map
        */
        const map<string, int> &GetWords() const
        {
            return words;
        }

        /*
        * Summary_Model::CResume::SetWords
        *
        * Description
        * Gets document word and occurrence count map
        *
        * Inputs
        * const map<string, int> &words - word map
        *
        * Returns
        *
        */
        void SetWords(const map<string, int> &words)
        {
            CResume::words = words;
        }

        void AddWord(const string &word);
        const vector<string> GetWordsByPopularity() const;
        const map<string, int> GetWordsMapByPopularity() const;
        void AddWordPair(const string &wordPair);
        const vector<string> GetWordPairsByPopularity() const;
        const map<string, int> GetWordPairsMapByPopularity() const;

        const vector<string> &GetUrlCandidates() const
        {
            return urlCandidates;
        }

        void SetUrlCandidates(const vector<string> &urlCandidates)
        {
            CResume::urlCandidates = urlCandidates;
        }

        void AddUrlCandidate(const string &urlCandidate)
        {
            urlCandidates.push_back(urlCandidate);
        }

        const vector<string> &GetSchoolDegrees() const
        {
            return schoolDegrees;
        }

        void SetSchoolDegrees(const vector<string> &schoolDegrees)
        {
            CResume::schoolDegrees = schoolDegrees;
        }

        void AddSchoolDegree(const string &schoolDegree)
        {
            CTextUtils::AddVectorMutuallyExclusively(schoolDegrees, schoolDegree);
        }

        /*
        * Summary_Model::CResume::GetSchoolFields
        *
        * Description
        * Gets middle name
        *
        * Inputs
        *
        * Returns
        * Middle name string
        */
        const vector<string> &GetSchoolFields() const
        {
            return schoolFields;
        }

        void SetSchoolFields(const vector<string> &schoolFields)
        {
            CResume::schoolFields = schoolFields;
        }

        void AddSchoolField(const string &schoolField)
        {
            bool canAdd = true;

            for (string field: schoolFields)
            {
                if (CTextUtils::StringContains(field, CTextUtils::Trim(schoolField)))
                {
                    canAdd = false;

                    break;
                }
            }

            if (canAdd)
            {
                schoolFields.push_back(schoolField);
            }
        }

        /*
        * Summary_Model::CResume::GetEducationDegrees
        *
        * Description
        * Gets middle name
        *
        * Inputs
        *
        * Returns
        * Middle name string
        */
        vector <string> GetEducationDegrees() const
        {
            vector <string> output;

            //sort(educationDegrees.begin(), educationDegrees.end(), Xgreater());

            for (auto &degreeObject : educationDegrees )
            {
                string outputString = degreeObject.GetLine();
                //output.push_back(degreeObject.GetLine() + ";" + to_string(degreeObject.GetIndex()));
                for (string date: degreeObject.GetDates())
                {
                    outputString += ";" + date;
                }
                output.push_back(outputString);
            }

            return output;
        }

        void SetEducationDegrees(vector<CEducationData> &educationDegrees)
        {
            CResume::educationDegrees = educationDegrees;
        }

        void AddEducationDegree(int mapIndex, const string &educationDegree)
        {
            //educationDegrees.push_back(educationDegree);
            bool canAdd = true;
            bool willReplace = false;
            string degreeToBoDeleted = "";
            int index = 0, lineIndex = -1;
            vector<CEducationData>::iterator it;

            CEducationData foundObject;

//cout << educationDegrees.size() << endl;
            for (auto &degreeObject : educationDegrees )
            {
                string degree = degreeObject.GetLine();
                lineIndex = degreeObject.GetIndex();

                if (CTextUtils::StringContains(degree, CTextUtils::Trim(educationDegree)))
                {
                    //cout << degree << endl;
                    //cout << educationDegree << endl;
                    canAdd = false;

                    if (degree.length() > 50 && educationDegree.length() < 50)
                    {
                        degreeToBoDeleted = degree;
                        willReplace = true;
                    }

                    foundObject = degreeObject;

                    break;
                }
                index++;
            }

            if (willReplace)
            {
                it = find(educationDegrees.begin(), educationDegrees.end(), foundObject);

                if (it != educationDegrees.end())
                {
                    educationDegrees.erase( it );
                }
            }

            if (canAdd || willReplace)
            {
                CEducationData newObject;

                newObject.AddData(mapIndex, CTextUtils::Trim(educationDegree));

                educationDegrees.push_back(newObject);
            }

            sort(educationDegrees.begin(), educationDegrees.end());
        }

        void AddEducationDegreeYear(const string &degree, string year)
        {
            for (CEducationData &degreeObject : educationDegrees )
            {
                if (degreeObject.GetLine().compare(degree) == 0 || CTextUtils::StartsWith(degreeObject.GetLine(), degree))
                {
                    //educationDegrees[degreeObject.first] += ";" + year;
                    degreeObject.AddDate(year);
                    break;
                }
            }
        }

        void AddSchoolName(const string &schoolName)
        {
            CTextUtils::AddVectorMutuallyExclusively(schoolNames, schoolName);
        }

        /*
        * Summary_Model::CResume::GetSchoolNames
        *
        * Description
        * Gets middle name
        *
        * Inputs
        *
        * Returns
        * Middle name string
        */
        const vector<string> &GetSchoolNames() const
        {
            return schoolNames;
        }

        void SetSchoolNames(const vector<string> &schoolNames)
        {
            CResume::schoolNames = schoolNames;
        }

        void AddSchool(const CSchoolData &school)
        {
            schools.push_back(school);
        }

        /*
        * Summary_Model::CResume::GetMiddleName
        *
        * Description
        * Gets middle name
        *
        * Inputs
        *
        * Returns
        * Middle name string
        */
        const vector<CSchoolData> &GetSchools() const
        {
            return schools;
        }

        void SetSchools(const vector<CSchoolData> &schools)
        {
            CResume::schools = schools;
        }

        /*vector <string> GetEducationDegreeYears(const string &degree) const
        {
            vector <string> output;

            for (auto &degreeObject : educationDegreeYears )
            {
                if (degreeObject.first.compare(degree) == 0)
                {
                    //output.push_back(degreeObject.second);
                    break;
                }
            }

            return output;
        }

        vector <string> GetAllEducationDegreeYears(const string &degree) const
        {
            vector <string> output;

            for (auto &degreeObject : educationDegreeYears )
            {
                //output.push_back(degreeObject.second);
            }

            return output;
        }*/

    private:
        string firstName;
        string middleName;
        string lastName;
        string gender;
        string birthDay;
        string emailAddress;
        string phoneNumbers;
        string skype;
        string website;
        string facebook;
        string twitter;
        string googlePlus;
        string linkedIn;
        string instagram;
        string languages;
        string summaryDescription;
        vector<string> skills;
        string location;
        string country;
        string religion;
        string textLanguageName;
        string textLanguageCode;
        string title;
        long textLength;
        vector<CPosition> positions;
        vector<CProject> projects;
        vector<string> socialProfiles;
        vector<CEducation> educations;
        vector<CEducationData> educationDegrees;
        vector<CSchoolData> schools;
        //unordered_map<string, CDates> educationDegreeYears;
        vector<string> schoolDegrees;
        vector<string> schoolFields;
        vector<string> schoolNames;
        vector<string> courses;
        vector<string> awards;
        vector<string> rawInput;
        vector<string> translation;
        vector<string> urlCandidates;

    private:
        map<string, int> words;
        map<string, int> wordPairs;
        map<string, int> skillCounts;
        //vector<string> stemmedWords;
    };
}

#endif //CVSUMMARYEXTRACTOR_CRESUME_H
