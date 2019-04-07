//
// File DictionaryLoaderSingleton.cpp
//
//

#include "DictionaryLoaderSingleton.h"
#include "../../Common/CFileUtils.h"
#include "../../Common/CTextUtils.h"

using namespace std;
using namespace Summary_Common;
using namespace Summary_ResumeProcessor_Parsers;

CResourceLoader resourceLoader;
DictionaryLoaderSingleton::DictionaryLoader temp(resourceLoader);
DictionaryLoaderSingleton::DictionaryLoader DictionaryLoaderSingleton::dictionaryLoader = temp;

DictionaryLoaderSingleton::DictionaryLoader::DictionaryLoader(CResourceLoader resourceLoader)
{
    DictionaryLoaderSingleton::DictionaryLoader::resourceLoader = resourceLoader;
    dictionariesLoaded = false;
}

/*
 * DictionaryLoaderSingleton::DictionaryLoader::LoadDictionaries
 *
 * Description
 * Loads dictionaries data from files
 *
 * Inputs
 *
 * Returns
 *
 */
void DictionaryLoaderSingleton::DictionaryLoader::LoadDictionaries()
{
    string runPath = CFileUtils::GetRunPath();

    if (runPath.length() < 1 || dictionariesLoaded)
    {
        return;
    }

    vector<string> countriesVector;
    vector<string> universitiesVector;

    //cout << "RP: " << CFileUtils::GetRunPath() << endl;
    DictionaryLoaderSingleton::DictionaryLoader::resourceLoader = resourceLoader;
    stopWordsLookup = resourceLoader.Load(runPath + "ResumeProcessor/Data/StopWords.txt", '\n');
    skillLookUp = resourceLoader.Load(runPath + "ResumeProcessor/Data/Skills.txt", '\n');
    jobLookUp = resourceLoader.Load(runPath + "ResumeProcessor/Data/JobTitles.txt", ',');
    universitiesVector = resourceLoader.Load(runPath + "ResumeProcessor/Data/Universities.txt", '\n');
    countriesVector = resourceLoader.Load(runPath + "ResumeProcessor/Data/Countries.txt", '|');

    firstNameLookUp = resourceLoader.LoadIntoLowerCaseHashSet(runPath + "ResumeProcessor/Data/FirstName.txt", ',');
    religionLookUp = resourceLoader.Load(runPath + "ResumeProcessor/Data/Religions.txt", '\n');
    nonNamesLookup = resourceLoader.Load(runPath + "ResumeProcessor/Data/NonNames.txt", '\n');
    fieldsOfStudyLookUp = resourceLoader.Load(runPath + "ResumeProcessor/Data/Degrees.txt", '\n');
    fieldsOfStudyLookUpShort = resourceLoader.Load(runPath + "ResumeProcessor/Data/ShortDegrees.txt", '\n');
    shortTaggedDegreesLookUp = resourceLoader.Load(runPath + "ResumeProcessor/Data/ShortTaggedDegrees.txt", '\n');

    for (string countryCombination: countriesVector)
    {
        vector<string> country = CTextUtils::Split(countryCombination, ';');

        if (country.size() > 0)
        {
            //Country name
            countryLookUp.push_back(country[0]);
            countryLookUpLower.push_back(CTextUtils::ToLower(country[0]));
        }

        if (country.size() > 1 && country[1].length() > 0)
        {
            //Country code & name
            countryMap.insert(pair<string, string>(country[1], country[0]));
        }
    }

    for (string countryCombination: countriesVector)
    {
        vector<string> country = CTextUtils::Split(countryCombination, ';');

        for (int i = 2; i < 5; i++)
        {
            if (country.size() > i)
            {
                //Do not add any secondary key if it's already added
                if (country[i].length() > 0 && countryMap.count(country[i]) < 1)
                {
                    //Country code & name
                    countryMap.insert(pair<string, string>(country[i], country[0]));
                }
            }
            else
            {
                break;
            }
        }
    }

    for (string university: universitiesVector)
    {
        universityLookUpLower.push_back(CTextUtils::ToLower(university));
    }

    for (string job: jobLookUp)
    {
        jobLookUpLower.push_back(CTextUtils::ToLower(job));
    }

    degreeAbbbreviations.push_back("associate degree");
    degreeAbbbreviations.push_back("diploma");
    degreeAbbbreviations.push_back("bachelor");
    degreeAbbbreviations.push_back("master of");
    degreeAbbbreviations.push_back("master’s");
    degreeAbbbreviations.push_back("master´s");
    degreeAbbbreviations.push_back("master's");
    degreeAbbbreviations.push_back("master in");
    degreeAbbbreviations.push_back("masters of");
    degreeAbbbreviations.push_back("masters in");
    degreeAbbbreviations.push_back("master diploma");
    degreeAbbbreviations.push_back("masters diploma");
    degreeAbbbreviations.push_back("masters level");
    degreeAbbbreviations.push_back("master level");
    degreeAbbbreviations.push_back("master equivalency");
    degreeAbbbreviations.push_back("masters equivalency");
    degreeAbbbreviations.push_back("doctor´s");
    degreeAbbbreviations.push_back("doctor's");
    degreeAbbbreviations.push_back("doctor’s");
    degreeAbbbreviations.push_back("doctors in");
    //degreeAbbbreviations.push_back("resident"); //TODO: What to do with resident/residency degrees?
    //degreeAbbbreviations.push_back("residency");

    degreeAbbbreviations.push_back("doctorate");
    degreeAbbbreviations.push_back("post graduate");
    degreeAbbbreviations.push_back("postgraduate");
    degreeAbbbreviations.push_back("secondary school");

    degreeAbbbreviationsShort.push_back("m.r.c.p");
    degreeAbbbreviationsShort.push_back("m. r. c. p");
    degreeAbbbreviationsShort.push_back("mrcp ");
    degreeAbbbreviationsShort.push_back("mrcp)");
    degreeAbbbreviationsShort.push_back("mrcp,");

    degreeAbbbreviationsShort.push_back("f.r.c.p");
    degreeAbbbreviationsShort.push_back("f. r. c. p");
    degreeAbbbreviationsShort.push_back("frcp ");
    degreeAbbbreviationsShort.push_back("frcp)");
    degreeAbbbreviationsShort.push_back("frcp,");

    degreeAbbbreviationsShort.push_back("l.r.c.p");
    degreeAbbbreviationsShort.push_back("l. r. c. p");
    degreeAbbbreviationsShort.push_back("lrcp ");
    degreeAbbbreviationsShort.push_back("lrcp)");
    degreeAbbbreviationsShort.push_back("lrcp,");

    degreeAbbbreviationsShort.push_back("m.b.b.s");
    degreeAbbbreviationsShort.push_back("m. b. b. s");
    degreeAbbbreviationsShort.push_back("mbbs ");
    degreeAbbbreviationsShort.push_back("mbbs)");
    degreeAbbbreviationsShort.push_back("mbbs,");

    degreeAbbbreviationsShort.push_back("m.c.g.i");
    degreeAbbbreviationsShort.push_back("m. c. g. i");
    degreeAbbbreviationsShort.push_back("mcgi ");
    degreeAbbbreviationsShort.push_back("mcgi)");
    degreeAbbbreviationsShort.push_back("mcgi,");

    degreeAbbbreviationsShort.push_back("m.a.l.d");
    degreeAbbbreviationsShort.push_back("m. a. l. d");
    degreeAbbbreviationsShort.push_back("mald ");
    degreeAbbbreviationsShort.push_back("mald)");
    degreeAbbbreviationsShort.push_back("mald,");

    degreeAbbbreviationsShort.push_back("m.q.b.m.s");
    degreeAbbbreviationsShort.push_back("m. q. b. m. s");
    degreeAbbbreviationsShort.push_back("mqbms ");
    degreeAbbbreviationsShort.push_back("mqbms)");
    degreeAbbbreviationsShort.push_back("mqbms,");

    degreeAbbbreviationsShort.push_back("m.p.p.m");
    degreeAbbbreviationsShort.push_back("m. p. p. m");
    degreeAbbbreviationsShort.push_back("mppm");
    degreeAbbbreviationsShort.push_back("mppm)");
    degreeAbbbreviationsShort.push_back("mppm,");

    degreeAbbbreviationsShort.push_back("m.b.b.s");
    degreeAbbbreviationsShort.push_back("m. b. b. s");
    degreeAbbbreviationsShort.push_back("mbbs");
    degreeAbbbreviationsShort.push_back("mbbs)");
    degreeAbbbreviationsShort.push_back("mbbs,");

    degreeAbbbreviationsShort.push_back("b.l.i.s");
    degreeAbbbreviationsShort.push_back("b. l. i. s");
    degreeAbbbreviationsShort.push_back("blis ");
    degreeAbbbreviationsShort.push_back("blis)");
    degreeAbbbreviationsShort.push_back("blis,");

    degreeAbbbreviationsShort.push_back("a.s.");
    degreeAbbbreviationsShort.push_back("a. s.");
    degreeAbbbreviationsShort.push_back("a.s)");
    degreeAbbbreviationsShort.push_back("a.s,");
    degreeAbbbreviationsShort.push_back("a.s-");

    degreeAbbbreviationsShort.push_back("m.s.");
    degreeAbbbreviationsShort.push_back("m. s.");
    degreeAbbbreviationsShort.push_back("m.s)");
    degreeAbbbreviationsShort.push_back("m.s,");
    degreeAbbbreviationsShort.push_back("m.s-");

    degreeAbbbreviationsShort.push_back("s.s.l.c");
    degreeAbbbreviationsShort.push_back("s. s. l. c");
    degreeAbbbreviationsShort.push_back("sslc");
    degreeAbbbreviationsShort.push_back("sslc)");
    degreeAbbbreviationsShort.push_back("sslc,");

    degreeAbbbreviationsShort.push_back("b.s.m.b.e");
    degreeAbbbreviationsShort.push_back("b. s. m. b. e");
    degreeAbbbreviationsShort.push_back("bsmbe");
    degreeAbbbreviationsShort.push_back("bsmbe)");
    degreeAbbbreviationsShort.push_back("bsmbe,");

    degreeAbbbreviationsShort.push_back("h.v.a.c");
    degreeAbbbreviationsShort.push_back("h. v. a. c");
    degreeAbbbreviationsShort.push_back("hvac");
    degreeAbbbreviationsShort.push_back("hvac)");
    degreeAbbbreviationsShort.push_back("hvac,");

    degreeAbbbreviationsShort.push_back("s.s.s.c");
    degreeAbbbreviationsShort.push_back("s. s. s. c");
    degreeAbbbreviationsShort.push_back("sssc");
    degreeAbbbreviationsShort.push_back("sssc)");
    degreeAbbbreviationsShort.push_back("sssc,");

    degreeAbbbreviationsShort.push_back("b.s.c.e");
    degreeAbbbreviationsShort.push_back("b. s. c. e");
    degreeAbbbreviationsShort.push_back("bsce");
    degreeAbbbreviationsShort.push_back("bsce)");
    degreeAbbbreviationsShort.push_back("bsce,");

    PushDegreeAbbreviation("s", "s", "c", true);

    PushDegreeAbbreviation("d", "phil", true);
    PushDegreeAbbreviation("d", "lit", true);
    PushDegreeAbbreviation("d", "litt", true);
    PushDegreeAbbreviation("d", "mus", true);
    PushDegreeAbbreviation("d", "min", true);
    PushDegreeAbbreviation("psy", "d", true);
    PushDegreeAbbreviation("th", "m", true);
    PushDegreeAbbreviation("th", "d", true);
    PushDegreeAbbreviation("ed", "d", true);
    PushDegreeAbbreviation("ph", "d", true);
    PushDegreeAbbreviation("ch", "e", true);
    PushDegreeAbbreviation("cp", "e", true);
    PushDegreeAbbreviation("env", "e", true);
    PushDegreeAbbreviation("nav", "e", true);
    PushDegreeAbbreviation("sys", "e", true);
    PushDegreeAbbreviation("nuc", "i", "e", true);
    PushDegreeAbbreviation("au", "d", true);
    PushDegreeAbbreviation("pharm", "d", true);
    PushDegreeAbbreviation("b", "e", false);
    PushDegreeAbbreviation("b", "s", true);
    PushDegreeAbbreviation("b", "f", "a", true);
    PushDegreeAbbreviation("b", "g", "s", true);
    PushDegreeAbbreviation("b", "b", "a", true);
    PushDegreeAbbreviation("b", "h", "k", true);
    PushDegreeAbbreviation("b", "h", "e", true);
    PushDegreeAbbreviation("b", "h", "sc", true);
    PushDegreeAbbreviation("b", "i", "d", true);
    PushDegreeAbbreviation("ju", "dr", true);
    PushDegreeAbbreviation("b", "j", true);
    PushDegreeAbbreviation("s", "b", true);
    PushDegreeAbbreviation("b", "arch", true);
    PushDegreeAbbreviation("b", "tech", true);
    PushDegreeAbbreviation("b", "th", true);
    PushDegreeAbbreviation("b", "tec", true);
    PushDegreeAbbreviation("b", "acc", true);
    PushDegreeAbbreviation("b", "acy", true);
    PushDegreeAbbreviation("b", "adm", true);
    PushDegreeAbbreviation("b", "bus", true);
    PushDegreeAbbreviation("b", "com", true);
    PushDegreeAbbreviation("b", "comm", true);
    PushDegreeAbbreviation("b", "ed", false);
    PushDegreeAbbreviation("b", "dent", true);
    PushDegreeAbbreviation("b", "eng", true);
    PushDegreeAbbreviation("b", "math", true);
    PushDegreeAbbreviation("b", "mus", true);
    PushDegreeAbbreviation("b", "pharm", true);
    PushDegreeAbbreviation("b", "med", true);
    PushDegreeAbbreviation("b", "chir", true);
    PushDegreeAbbreviation("b", "agr", "ec", true);
    PushDegreeAbbreviation("b", "lib", "stud", true);
    PushDegreeAbbreviation("b", "med", "sc", true);
    PushDegreeAbbreviation("b", "soc", "sci", true);
    PushDegreeAbbreviation("b", "sc", true);
    PushDegreeAbbreviation("m", "sc", true);
    PushDegreeAbbreviation("m", "st", true);
    PushDegreeAbbreviation("m", "arch", false);
    PushDegreeAbbreviation("b", "a", true);
    PushDegreeAbbreviation("m", "b", true);
    PushDegreeAbbreviation("m", "d", true);

    PushDegreeAbbreviation("d", "o", false);
    PushDegreeAbbreviation("d", "sc", true);
    PushDegreeAbbreviation("d", "c", false);
    PushDegreeAbbreviation("d", "c", "l", true);
    PushDegreeAbbreviation("d", "c", "m", true);
    PushDegreeAbbreviation("d", "v", "m", true);
    PushDegreeAbbreviation("d", "d", "s", true);
    PushDegreeAbbreviation("d", "m", "a", true);
    PushDegreeAbbreviation("d", "m", "d", true);
    PushDegreeAbbreviation("d", "p", "m", true);
    PushDegreeAbbreviation("d", "p", "s", true);
    PushDegreeAbbreviation("d", "p", "t", true);
    PushDegreeAbbreviation("dr", "p", "h", true);
    PushDegreeAbbreviation("d", "d", true);
    PushDegreeAbbreviation("d", "p", true);
    PushDegreeAbbreviation("d", "eng", true);
    PushDegreeAbbreviation("d", "env", true);
    PushDegreeAbbreviation("d", "b", "a", false);
    PushDegreeAbbreviation("d", "e", "a", true);
    PushDegreeAbbreviation("d", "s", "w", true);
    PushDegreeAbbreviation("b", "d", true);
    PushDegreeAbbreviation("b", "m", "e", true);
    PushDegreeAbbreviation("b", "c", "s", true);
    PushDegreeAbbreviation("b", "d", "s", true);
    PushDegreeAbbreviation("b", "ec", true);
    PushDegreeAbbreviation("b", "c", "l", true);
    PushDegreeAbbreviation("b", "c", "a", true);
    PushDegreeAbbreviation("s", "c", "e", true);
    PushDegreeAbbreviation("b", "b", "a", true);
    PushDegreeAbbreviation("b", "f", "a", true);
    PushDegreeAbbreviation("b", "s", "n", true);
    PushDegreeAbbreviation("b", "s", "w", true);
    PushDegreeAbbreviation("b", "p", "e", true);
    PushDegreeAbbreviation("b", "a", "o", true);
    PushDegreeAbbreviation("b", "sc", "agr", true);
    PushDegreeAbbreviation("o", "d", true);
    PushDegreeAbbreviation("m", "a", true);
    PushDegreeAbbreviation("m", "a", "t", true);
    PushDegreeAbbreviation("m", "l", "s", true);

    PushDegreeAbbreviation("m", "s", "n", false);
    PushDegreeAbbreviation("m", "s", "e", false);
    PushDegreeAbbreviation("m", "s", "t", true);
    PushDegreeAbbreviation("m", "s", "w", true);
    PushDegreeAbbreviation("m", "f", "a", true);
    PushDegreeAbbreviation("m", "t", "s", true);
    PushDegreeAbbreviation("m", "arch", false);
    PushDegreeAbbreviation("m", "acc", true);
    PushDegreeAbbreviation("m", "res", true);
    PushDegreeAbbreviation("m", "bus", true);
    PushDegreeAbbreviation("m", "com", true);
    PushDegreeAbbreviation("m", "des", true);
    PushDegreeAbbreviation("m", "th", true);
    PushDegreeAbbreviation("m", "div", true);
    PushDegreeAbbreviation("m", "ed", true);
    PushDegreeAbbreviation("m", "litt", true);
    PushDegreeAbbreviation("m", "eng", true);
    PushDegreeAbbreviation("m", "sci", true);
    PushDegreeAbbreviation("m", "bio", true);
    PushDegreeAbbreviation("m", "chem", true);
    PushDegreeAbbreviation("m", "phys", true);
    PushDegreeAbbreviation("m", "math", true);
    PushDegreeAbbreviation("m", "mus", true);
    PushDegreeAbbreviation("m", "geol", true);
    PushDegreeAbbreviation("m", "phil", true);
    PushDegreeAbbreviation("m", "res", true);
    PushDegreeAbbreviation("m", "tech", true);
    PushDegreeAbbreviation("eng", "d", true);
    PushDegreeAbbreviation("ll", "d", true);
    PushDegreeAbbreviation("ll", "b", true);
    PushDegreeAbbreviation("ll", "m", true);
    PushDegreeAbbreviation("s", "s", "d", false);
    PushDegreeAbbreviation("s", "s", "l", false);
    PushDegreeAbbreviation("s", "s", "p", true);
    PushDegreeAbbreviation("m", "prof", "studs", true);
    PushDegreeAbbreviation("m", "med", "sc", true);
    PushDegreeAbbreviation("m", "med", "sci", true);
    PushDegreeAbbreviation("m", "e", "sci", true);
    PushDegreeAbbreviation("m", "s", "sc", true);
    PushDegreeAbbreviation("m", "r", "e", true);
    PushDegreeAbbreviation("m", "p", "a", true);
    PushDegreeAbbreviation("m", "p", "h", false);
    PushDegreeAbbreviation("m", "p", "m", true);
    PushDegreeAbbreviation("m", "p", "p", true);
    PushDegreeAbbreviation("m", "p", "t", true);
    PushDegreeAbbreviation("m", "p", "d", true);
    PushDegreeAbbreviation("m", "p", "s", true);
    PushDegreeAbbreviation("m", "m", "t", true);
    PushDegreeAbbreviation("m", "b", "a", true);
    PushDegreeAbbreviation("m", "b", "i", true);
    PushDegreeAbbreviation("m", "b", "t", true);
    PushDegreeAbbreviation("m", "h", "a", true);
    PushDegreeAbbreviation("m", "h", "l", true);
    PushDegreeAbbreviation("l", "h", "d", true);
    PushDegreeAbbreviation("l", "l", "a", true);
    PushDegreeAbbreviation("l", "l", "b", true);
    PushDegreeAbbreviation("l", "l", "d", true);
    PushDegreeAbbreviation("l", "l", "m", true);
    PushDegreeAbbreviation("l", "d", "s", true);
    PushDegreeAbbreviation("l", "s", "s", true);
    PushDegreeAbbreviation("ed", "d", true);
    PushDegreeAbbreviation("m", "e", false);
    PushDegreeAbbreviation("ed", "s", true);
    PushDegreeAbbreviation("j", "d", false);
    PushDegreeAbbreviation("j", "s", "d", false);
    PushDegreeAbbreviation("j", "u", "d", false);
    PushDegreeAbbreviation("ch", "b", true);
    PushDegreeAbbreviation("ch", "e", false);
    PushDegreeAbbreviation("ch", "m", true);
    PushDegreeAbbreviation("s", "j", "d", true);
    PushDegreeAbbreviation("s", "t", "b", true);
    PushDegreeAbbreviation("s", "t", "m", true);
    PushDegreeAbbreviation("s", "t", "l", true);
    PushDegreeAbbreviation("s", "t", "d", true);
    PushDegreeAbbreviation("ph", "b", true);
    PushDegreeAbbreviation("ph", "l", true);
    PushDegreeAbbreviation("p", "d", true);
    PushDegreeAbbreviation("p", "d", "eng", true);
    PushDegreeAbbreviation("j", "c", "b", true);
    PushDegreeAbbreviation("j", "c", "d", true);
    PushDegreeAbbreviation("j", "c", "l", true);
    PushDegreeAbbreviation("j", "c", "d", true);
    PushDegreeAbbreviation("d", "a", "e", true);
    PushDegreeAbbreviation("d", "h", "sc", true);
    PushDegreeAbbreviation("b", "e", false);
    PushDegreeAbbreviation("d", "a", true);
    PushDegreeAbbreviation("a", "a", true);
    PushDegreeAbbreviation("a", "b", false);
    PushDegreeAbbreviation("a", "e", true);
    PushDegreeAbbreviation("c", "e", true);
    PushDegreeAbbreviation("e", "e", true);
    PushDegreeAbbreviation("pod", "d", true);
    PushDegreeAbbreviation("e", "c", "s", true);
    PushDegreeAbbreviation("a", "a", "s", true);
    PushDegreeAbbreviation("a", "mus", "a", true);
    PushDegreeAbbreviation("l", "mus", "a", true);
    PushDegreeAbbreviation("a", "b", "a", true);
    PushDegreeAbbreviation("a", "b", "s", true);
    PushDegreeAbbreviation("a", "o", "s", true);
    PushDegreeAbbreviation("a", "a", "a", true);
    PushDegreeAbbreviation("a", "p", "s", true);
    PushDegreeAbbreviation("a", "s", "n", true);
    PushDegreeAbbreviation("h", "n", "c", true);
    PushDegreeAbbreviation("h", "n", "d", true);
    PushDegreeAbbreviation("o", "n", "c", true);

    PushDegreeAbbreviation("fd", "a", true);
    PushDegreeAbbreviation("fd", "ed", true);
    PushDegreeAbbreviation("fd", "eng", true);
    PushDegreeAbbreviation("fd", "mus", true);
    PushDegreeAbbreviation("fd", "bus", true);
    PushDegreeAbbreviation("fd", "sc", true);
    PushDegreeAbbreviation("fd", "tech", true);
    PushDegreeAbbreviation("fd", "tec", true);

    for (string abbreviation: degreeAbbbreviationsShort)
    {
        degreeAbbbreviations.push_back(abbreviation);
    }

    dictionariesLoaded = true;
}

/*
 * DictionaryLoaderSingleton::DictionaryLoader::PushDegreeAbbreviation
 *
 * Description
 * Concatenates two letters in a very different way and puts the result into degreeabbreviationsshort vector
 *
 * Inputs
 * const string &firstLetter - first letter to concat
 * const string &secondLetter - second letter to concat
 * bool concatenateSimply - if true than concatenates also without any spaces and adds that into vector
 *
 * Returns
 *
 */
void DictionaryLoaderSingleton::DictionaryLoader::PushDegreeAbbreviation(const string &firstLetter,
                                                                               const string &secondLetter,
                                                                               bool concatenateSimply)
{
    if (concatenateSimply)
    {
        degreeAbbbreviationsShort.push_back(firstLetter + secondLetter);
    }
    //
    degreeAbbbreviationsShort.push_back(firstLetter + secondLetter + ")");
    degreeAbbbreviationsShort.push_back(firstLetter + secondLetter + "(");
    degreeAbbbreviationsShort.push_back(firstLetter + secondLetter + ",");
    degreeAbbbreviationsShort.push_back(firstLetter + secondLetter + ".");
    degreeAbbbreviationsShort.push_back(firstLetter + secondLetter + "-");
    degreeAbbbreviationsShort.push_back(firstLetter + secondLetter + ":");

    degreeAbbbreviationsShort.push_back(firstLetter + " " + secondLetter);
    /*degreeAbbbreviationsShort.push_back(firstLetter + " " + secondLetter + ")");
    degreeAbbbreviationsShort.push_back(firstLetter + " " + secondLetter + "(");
    degreeAbbbreviationsShort.push_back(firstLetter + " " + secondLetter + ",");
    degreeAbbbreviationsShort.push_back(firstLetter + " " + secondLetter + ".");
    degreeAbbbreviationsShort.push_back(firstLetter + " " + secondLetter + "-");
    degreeAbbbreviationsShort.push_back(firstLetter + " " + secondLetter + ":");*/

    degreeAbbbreviationsShort.push_back(firstLetter + "-" + secondLetter);
    /*degreeAbbbreviationsShort.push_back(firstLetter + "-" + secondLetter + " ");
    degreeAbbbreviationsShort.push_back(firstLetter + "-" + secondLetter + ")");
    degreeAbbbreviationsShort.push_back(firstLetter + "-" + secondLetter + "(");
    degreeAbbbreviationsShort.push_back(firstLetter + "-" + secondLetter + ",");
    degreeAbbbreviationsShort.push_back(firstLetter + "-" + secondLetter + ".");
    degreeAbbbreviationsShort.push_back(firstLetter + "-" + secondLetter + "-");
    degreeAbbbreviationsShort.push_back(firstLetter + "-" + secondLetter + ":");*/

    degreeAbbbreviationsShort.push_back(firstLetter + "." + secondLetter);
    /*degreeAbbbreviationsShort.push_back(firstLetter + "." + secondLetter + " ");
    degreeAbbbreviationsShort.push_back(firstLetter + "." + secondLetter + ")");
    degreeAbbbreviationsShort.push_back(firstLetter + "." + secondLetter + "(");
    degreeAbbbreviationsShort.push_back(firstLetter + "." + secondLetter + ",");
    degreeAbbbreviationsShort.push_back(firstLetter + "." + secondLetter + ".");
    degreeAbbbreviationsShort.push_back(firstLetter + "." + secondLetter + "-");
    degreeAbbbreviationsShort.push_back(firstLetter + "." + secondLetter + ":");*/

    degreeAbbbreviationsShort.push_back(firstLetter + ". " + secondLetter);
    /*degreeAbbbreviationsShort.push_back(firstLetter + ". " + secondLetter + " ");
    degreeAbbbreviationsShort.push_back(firstLetter + ". " + secondLetter + ")");
    degreeAbbbreviationsShort.push_back(firstLetter + ". " + secondLetter + "(");
    degreeAbbbreviationsShort.push_back(firstLetter + ". " + secondLetter + ",");
    degreeAbbbreviationsShort.push_back(firstLetter + ". " + secondLetter + ".");
    degreeAbbbreviationsShort.push_back(firstLetter + ". " + secondLetter + "-");
    degreeAbbbreviationsShort.push_back(firstLetter + ". " + secondLetter + ":");*/
}

/*
 * DictionaryLoaderSingleton::DictionaryLoader::PushDegreeAbbreviation
 *
 * Description
 * Concatenates two letters in a very different way and puts the result into degreeabbreviationsshort vector
 *
 * Inputs
 * const string &firstLetter - first letter to concat
 * const string &secondLetter - second letter to concat
 * const string &thirdLetter - third letter to concat
 * bool concatenateSimply - if true than concatenates also without any spaces and adds that into vector
 *
 * Returns
 *
 */
void DictionaryLoaderSingleton::DictionaryLoader::PushDegreeAbbreviation(const string &firstLetter,
                                                                               const string &secondLetter,
                                                                               const string &thirdLetter,
                                                                               bool concatenateSimply)
{
    if (concatenateSimply)
    {
        degreeAbbbreviationsShort.push_back(firstLetter + secondLetter + thirdLetter);
    }
    //
    degreeAbbbreviationsShort.push_back(firstLetter + secondLetter + thirdLetter + ")");
    degreeAbbbreviationsShort.push_back(firstLetter + secondLetter + thirdLetter + ",");
    degreeAbbbreviationsShort.push_back(firstLetter + secondLetter + thirdLetter + ".");
    degreeAbbbreviationsShort.push_back(firstLetter + secondLetter + thirdLetter + "-");
    degreeAbbbreviationsShort.push_back(firstLetter + " " + secondLetter + " " + thirdLetter);
    /*degreeAbbbreviationsShort.push_back(firstLetter + " " + secondLetter + " " + thirdLetter + ")");
    degreeAbbbreviationsShort.push_back(firstLetter + " " + secondLetter + " " + thirdLetter + ",");
    degreeAbbbreviationsShort.push_back(firstLetter + " " + secondLetter + " " + thirdLetter + ".");
    degreeAbbbreviationsShort.push_back(firstLetter + " " + secondLetter + " " + thirdLetter + "-");*/
    degreeAbbbreviationsShort.push_back(firstLetter + "." + secondLetter + "." + thirdLetter);
    /*degreeAbbbreviationsShort.push_back(firstLetter + "." + secondLetter + "." + thirdLetter + ")");
    degreeAbbbreviationsShort.push_back(firstLetter + "." + secondLetter + "." + thirdLetter + ",");
    degreeAbbbreviationsShort.push_back(firstLetter + "." + secondLetter + "." + thirdLetter + ".");
    degreeAbbbreviationsShort.push_back(firstLetter + "." + secondLetter + "." + thirdLetter + "-");*/
    degreeAbbbreviationsShort.push_back(firstLetter + ". " + secondLetter + ". " + thirdLetter);
    /*degreeAbbbreviationsShort.push_back(firstLetter + ". " + secondLetter + ". " + thirdLetter + ")");
    degreeAbbbreviationsShort.push_back(firstLetter + ". " + secondLetter + ". " + thirdLetter + ",");
    degreeAbbbreviationsShort.push_back(firstLetter + ". " + secondLetter + ". " + thirdLetter + ".");
    degreeAbbbreviationsShort.push_back(firstLetter + ". " + secondLetter + ". " + thirdLetter + "-");*/
}
