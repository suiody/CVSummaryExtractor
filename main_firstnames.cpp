#include <iostream>
#include <vector>
#include <algorithm>
#include <execinfo.h>
#include "ResumeProcessor/Helpers/CDateHelper.h"
#include "ResumeProcessor/Helpers/CResourceLoader.h"
#include "ResumeProcessor/CResumeProcessor.h"
#include "Common/CTextUtils.h"
#include "Common/CFileUtils.h"
#include "Common/CDebugUtils.h"
#include <boost/filesystem.hpp>
#include <boost/exception/info.hpp>
#include <boost/exception/all.hpp>
#include "Easylogging/easylogging++.h"
#include "Stacktrace/stacktrace.h"
#include <boost/lexical_cast.hpp>



#include <iostream>
#include <fstream>
#include <sstream>

using namespace boost::filesystem;
using namespace Summary_Common;
using namespace Summary_Json;
using namespace std;
using namespace Summary_ResumeProcessor_Helpers;
using namespace Summary_ResumeProcessor;
using namespace boost;

typedef boost::error_info<struct tag_stack_str,std::string> stack_info;

INITIALIZE_EASYLOGGINGPP

int main(int argc, char* argv[])
{
    CResourceLoader resourceLoader;
    SLowerCaseUnorderedSet firstNameLookUp = resourceLoader.LoadIntoLowerCaseHashSet(
            CFileUtils::GetRunPath() + "ResumeProcessor/Data/FirstName.txt", ',');
    vector<string> nameResults;
    string nameResultsText;

    vector<string> addresses;
    /*addresses.push_back("https://en.wiktionary.org/wiki/Category:Catalan_male_given_names");
    addresses.push_back("https://en.wiktionary.org/wiki/Category:Czech_male_given_names"); //L
    addresses.push_back("https://en.wiktionary.org/wiki/Category:Danish_male_given_names"); //L
    addresses.push_back("https://en.wiktionary.org/wiki/Category:Dutch_male_given_names");
    addresses.push_back("https://en.wiktionary.org/wiki/Category:Egyptian_male_given_names");
    addresses.push_back("https://en.wiktionary.org/wiki/Category:Ewe_male_given_names");
    addresses.push_back("https://en.wiktionary.org/wiki/Category:Faroese_male_given_names");
    addresses.push_back("https://en.wiktionary.org/wiki/Category:Finnish_male_given_names"); //L
    addresses.push_back("https://en.wiktionary.org/wiki/Category:French_male_given_names"); //L
    addresses.push_back("https://en.wiktionary.org/wiki/Category:German_male_given_names"); //L
    addresses.push_back("https://en.wiktionary.org/wiki/Category:Greenlandic_male_given_names");
    addresses.push_back("https://en.wiktionary.org/wiki/Category:Hawaiian_male_given_names"); //L
    addresses.push_back("https://en.wiktionary.org/wiki/Category:Hungarian_male_given_names");
    addresses.push_back("https://en.wiktionary.org/wiki/Category:Icelandic_male_given_names");
    addresses.push_back("https://en.wiktionary.org/wiki/Category:Italian_male_given_names"); //L
    addresses.push_back("https://en.wiktionary.org/wiki/Category:Kurdish_male_given_names");
    addresses.push_back("https://en.wiktionary.org/wiki/Category:Latvian_male_given_names"); //L
    addresses.push_back("https://en.wiktionary.org/wiki/Category:Lithuanian_male_given_names");
    addresses.push_back("https://en.wiktionary.org/wiki/Category:Macedonian_male_given_names");
    addresses.push_back("https://en.wiktionary.org/wiki/Category:Malay_male_given_names_from_Hebrew");
    addresses.push_back("https://en.wiktionary.org/wiki/Category:Manx_male_given_names");
    addresses.push_back("https://en.wiktionary.org/wiki/Category:Middle_French_male_given_names");
    addresses.push_back("https://en.wiktionary.org/wiki/Category:Norman_male_given_names");
    addresses.push_back("https://en.wiktionary.org/wiki/Category:Northern_Sami_male_given_names");
    addresses.push_back("https://en.wiktionary.org/wiki/Category:Norwegian_male_given_names"); //L
    addresses.push_back("https://en.wiktionary.org/wiki/Category:Polish_male_given_names");
    addresses.push_back("https://en.wiktionary.org/wiki/Category:Portuguese_male_given_names"); //L
    addresses.push_back("https://en.wiktionary.org/wiki/Category:Romanian_male_given_names");
    addresses.push_back("https://en.wiktionary.org/wiki/Category:Romanian_male_given_names_from_Ancient_Greek");
    addresses.push_back("https://en.wiktionary.org/wiki/Category:Russian_male_given_names");
    addresses.push_back("https://en.wiktionary.org/wiki/Category:Scots_male_given_names");
    addresses.push_back("https://en.wiktionary.org/wiki/Category:Scottish_Gaelic_male_given_names");
    addresses.push_back("https://en.wiktionary.org/wiki/Category:Serbo-Croatian_male_given_names");
    addresses.push_back("https://en.wiktionary.org/wiki/Category:Slovak_male_given_names");
    addresses.push_back("https://en.wiktionary.org/wiki/Category:Slovene_male_given_names");
    addresses.push_back("https://en.wiktionary.org/wiki/Category:Somali_male_given_names");
    addresses.push_back("https://en.wiktionary.org/wiki/Category:Spanish_male_given_names"); //L
    addresses.push_back("https://en.wiktionary.org/wiki/Category:Swahili_male_given_names");
    addresses.push_back("https://en.wiktionary.org/wiki/Category:Swedish_male_given_names"); //L
    addresses.push_back("https://en.wiktionary.org/wiki/Category:Turkish_male_given_names"); //L
    addresses.push_back("https://en.wiktionary.org/wiki/Category:Ukrainian_male_given_names"); //L
    addresses.push_back("https://en.wiktionary.org/wiki/Category:Vilamovian_male_given_names");
    addresses.push_back("https://en.wiktionary.org/wiki/Category:Welsh_male_given_names");
    addresses.push_back("https://en.wiktionary.org/wiki/Category:West_Frisian_male_given_names");
    addresses.push_back("https://en.wiktionary.org/wiki/Category:Zazaki_male_given_names");
    addresses.push_back("https://en.wiktionary.org/wiki/Category:Bashkir_male_given_names");*/
    addresses.push_back("https://en.wiktionary.org/wiki/Category:Albanian_female_given_names");
    addresses.push_back("https://en.wiktionary.org/wiki/Category:Asturian_female_given_names");
    addresses.push_back("https://en.wiktionary.org/wiki/Category:Bashkir_female_given_names_from_Arabic");
    addresses.push_back("https://en.wiktionary.org/wiki/Category:Bashkir_female_given_names_from_Persian");
    addresses.push_back("https://en.wiktionary.org/wiki/Category:Bashkir_female_given_names_from_Turkic");
    addresses.push_back("https://en.wiktionary.org/wiki/Category:Basque_female_given_names");
    addresses.push_back("https://en.wiktionary.org/wiki/Category:Belarusian_female_given_names");
    addresses.push_back("https://en.wiktionary.org/wiki/Category:Catalan_female_given_names");
    addresses.push_back("https://en.wiktionary.org/wiki/Category:Czech_female_given_names");
    addresses.push_back("https://en.wiktionary.org/w/index.php?title=Category:Czech_female_given_names&from=T");
    addresses.push_back("https://en.wiktionary.org/wiki/Category:Danish_female_given_names");
    addresses.push_back("https://en.wiktionary.org/w/index.php?title=Category:Danish_female_given_names&from=J");
    addresses.push_back("https://en.wiktionary.org/w/index.php?title=Category:Danish_female_given_names&from=S");
    addresses.push_back("https://en.wiktionary.org/wiki/Category:Dutch_female_given_names");
    addresses.push_back("https://en.wiktionary.org/wiki/Category:English_female_given_names");
    addresses.push_back("https://en.wiktionary.org/wiki/Category:English_female_given_names_from_Ancient_Greek");
    addresses.push_back("https://en.wiktionary.org/wiki/Category:English_female_given_names_from_Arabic");
    addresses.push_back("https://en.wiktionary.org/wiki/Category:English_female_given_names_from_Aramaic");
    addresses.push_back("https://en.wiktionary.org/wiki/Category:English_female_given_names_from_Celtic");
    addresses.push_back("https://en.wiktionary.org/wiki/Category:English_female_given_names_from_Irish");
    addresses.push_back("https://en.wiktionary.org/wiki/Category:English_female_given_names_from_Scottish_Gaelic");
    addresses.push_back("https://en.wiktionary.org/wiki/Category:English_female_given_names_from_Welsh");
    addresses.push_back("https://en.wiktionary.org/wiki/Category:English_female_given_names_from_coinages");
    addresses.push_back("https://en.wiktionary.org/wiki/Category:English_female_given_names_from_English");
    addresses.push_back("https://en.wiktionary.org/wiki/Category:English_female_given_names_from_French");
    addresses.push_back("https://en.wiktionary.org/wiki/Category:English_female_given_names_from_Germanic");
    addresses.push_back("https://en.wiktionary.org/wiki/Category:English_female_given_names_from_Hebrew");
    addresses.push_back("https://en.wiktionary.org/wiki/Category:English_female_given_names_from_Hindi");
    addresses.push_back("https://en.wiktionary.org/wiki/Category:English_female_given_names_from_India");
    addresses.push_back("https://en.wiktionary.org/wiki/Category:English_female_given_names_from_Sanskrit");
    addresses.push_back("https://en.wiktionary.org/wiki/Category:English_female_given_names_from_Italian");
    addresses.push_back("https://en.wiktionary.org/wiki/Category:English_female_given_names_from_Japanese");
    addresses.push_back("https://en.wiktionary.org/wiki/Category:English_female_given_names_from_Latin");
    addresses.push_back("https://en.wiktionary.org/wiki/Category:English_female_given_names_from_Old_English");
    addresses.push_back("https://en.wiktionary.org/wiki/Category:English_female_given_names_from_Persian");
    addresses.push_back("https://en.wiktionary.org/wiki/Category:English_female_given_names_from_place_names");
    addresses.push_back("https://en.wiktionary.org/wiki/Category:English_female_given_names_from_Russian");
    addresses.push_back("https://en.wiktionary.org/wiki/Category:English_female_given_names_from_Slavic");
    addresses.push_back("https://en.wiktionary.org/wiki/Category:English_female_given_names_from_Spanish");
    addresses.push_back("https://en.wiktionary.org/wiki/Category:English_female_given_names_from_surnames");
    addresses.push_back("https://en.wiktionary.org/wiki/Category:lv:English_female_given_names");
    addresses.push_back("https://en.wiktionary.org/wiki/Category:Faroese_female_given_names");
    addresses.push_back("https://en.wiktionary.org/wiki/Category:Finnish_female_given_names");
    addresses.push_back("https://en.wiktionary.org/w/index.php?title=Category:Finnish_female_given_names&from=L");
    addresses.push_back("https://en.wiktionary.org/w/index.php?title=Category:Finnish_female_given_names&from=T");
    addresses.push_back("https://en.wiktionary.org/wiki/Category:French_female_given_names");
    addresses.push_back("https://en.wiktionary.org/w/index.php?title=Category:French_female_given_names&from=J");
    addresses.push_back("https://en.wiktionary.org/w/index.php?title=Category:French_female_given_names&from=V");
    addresses.push_back("https://en.wiktionary.org/wiki/Category:French_female_given_names_from_Arabic");
    addresses.push_back("https://en.wiktionary.org/wiki/Category:Galician_female_given_names");
    addresses.push_back("https://en.wiktionary.org/wiki/Category:German_female_given_names");
    addresses.push_back("https://en.wiktionary.org/w/index.php?title=Category:German_female_given_names&from=H");
    addresses.push_back("https://en.wiktionary.org/w/index.php?title=Category:German_female_given_names&from=R");
    addresses.push_back("https://en.wiktionary.org/wiki/Category:Greenlandic_female_given_names");
    addresses.push_back("https://en.wiktionary.org/w/index.php?title=Category:Greenlandic_female_given_names&from=U");
    addresses.push_back("https://en.wiktionary.org/wiki/Category:Hawaiian_female_given_names");
    addresses.push_back("https://en.wiktionary.org/w/index.php?title=Category:Hawaiian_female_given_names&from=W");
    addresses.push_back("https://en.wiktionary.org/wiki/Category:Hawaiian_female_given_names_from_English");
    addresses.push_back("https://en.wiktionary.org/wiki/Category:Hawaiian_female_given_names_from_the_Bible");
    addresses.push_back("https://en.wiktionary.org/wiki/Category:Hungarian_female_given_names");
    addresses.push_back("https://en.wiktionary.org/wiki/Category:Icelandic_female_given_names");
    addresses.push_back("https://en.wiktionary.org/wiki/Category:Irish_female_given_names");
    addresses.push_back("https://en.wiktionary.org/wiki/Category:Italian_female_given_names");
    addresses.push_back("https://en.wiktionary.org/w/index.php?title=Category:Italian_female_given_names&from=S");
    addresses.push_back("https://en.wiktionary.org/wiki/Category:Kom_%28Cameroon%29_female_given_names");
    addresses.push_back("https://en.wiktionary.org/wiki/Category:Kurdish_female_given_names");
    addresses.push_back("https://en.wiktionary.org/wiki/Category:Latin_female_given_names");
    addresses.push_back("https://en.wiktionary.org/wiki/Category:Latvian_female_given_names");
    addresses.push_back("https://en.wiktionary.org/w/index.php?title=Category:Latvian_female_given_names&from=M");
    addresses.push_back("https://en.wiktionary.org/wiki/Category:Lithuanian_female_given_names");
    addresses.push_back("https://en.wiktionary.org/wiki/Category:Manx_female_given_names");
    addresses.push_back("https://en.wiktionary.org/wiki/Category:Middle_French_female_given_names");
    addresses.push_back("https://en.wiktionary.org/wiki/Category:Navajo_female_given_names");
    addresses.push_back("https://en.wiktionary.org/wiki/Category:Norman_female_given_names");
    addresses.push_back("https://en.wiktionary.org/wiki/Category:Northern_Sami_female_given_names");
    addresses.push_back("https://en.wiktionary.org/wiki/Category:Norwegian_female_given_names");
    addresses.push_back("https://en.wiktionary.org/w/index.php?title=Category:Norwegian_female_given_names&from=J");
    addresses.push_back("https://en.wiktionary.org/w/index.php?title=Category:Norwegian_female_given_names&from=N");
    addresses.push_back("https://en.wiktionary.org/w/index.php?title=Category:Norwegian_female_given_names&from=U");
    addresses.push_back("https://en.wiktionary.org/wiki/Category:Polish_female_given_names");
    addresses.push_back("https://en.wiktionary.org/wiki/Category:Portuguese_female_given_names");
    addresses.push_back("https://en.wiktionary.org/w/index.php?title=Category:Portuguese_female_given_names&from=S");
    addresses.push_back("https://en.wiktionary.org/wiki/Category:Portuguese_female_given_names_from_Hebrew");
    addresses.push_back("https://en.wiktionary.org/wiki/Category:Romanian_female_given_names");
    addresses.push_back("https://en.wiktionary.org/wiki/Category:Russian_female_given_names");
    addresses.push_back("https://en.wiktionary.org/wiki/Category:de:Russian_female_given_names");
    addresses.push_back("https://en.wiktionary.org/wiki/Category:en:Russian_female_given_names");
    addresses.push_back("https://en.wiktionary.org/wiki/Category:et:Russian_female_given_names");
    addresses.push_back("https://en.wiktionary.org/wiki/Category:fi:Russian_female_given_names");
    addresses.push_back("https://en.wiktionary.org/wiki/Category:fr:Russian_female_given_names");
    addresses.push_back("https://en.wiktionary.org/wiki/Category:lv:Russian_female_given_names");
    addresses.push_back("https://en.wiktionary.org/wiki/Category:Scottish_Gaelic_female_given_names");
    addresses.push_back("https://en.wiktionary.org/wiki/Category:Serbo-Croatian_female_given_names");
    addresses.push_back("https://en.wiktionary.org/wiki/Category:Slovak_female_given_names");
    addresses.push_back("https://en.wiktionary.org/wiki/Category:Slovene_female_given_names");
    addresses.push_back("https://en.wiktionary.org/wiki/Category:Spanish_female_given_names");
    addresses.push_back("https://en.wiktionary.org/w/index.php?title=Category:Spanish_female_given_names&from=O");
    addresses.push_back("https://en.wiktionary.org/wiki/Category:Swedish_female_given_names");
    addresses.push_back("https://en.wiktionary.org/w/index.php?title=Category:Swedish_female_given_names&from=J");
    addresses.push_back("https://en.wiktionary.org/w/index.php?title=Category:Swedish_female_given_names&from=T");
    addresses.push_back("https://en.wiktionary.org/wiki/Category:Turkish_female_given_names");
    addresses.push_back("https://en.wiktionary.org/w/index.php?title=Category:Turkish_female_given_names&pagefrom=H");
    addresses.push_back("https://en.wiktionary.org/w/index.php?title=Category:Turkish_female_given_names&pagefrom=O");
    addresses.push_back("https://en.wiktionary.org/wiki/Category:Turkish_female_given_names_from_Old_Turkic");
    addresses.push_back("https://en.wiktionary.org/wiki/Category:Turkish_female_given_names_from_Arabic");
    addresses.push_back("https://en.wiktionary.org/wiki/Category:Turkish_female_given_names_from_Persian");
    addresses.push_back("https://en.wiktionary.org/wiki/Category:Ukrainian_female_given_names");
    addresses.push_back("https://en.wiktionary.org/wiki/Category:Vietnamese_female_given_names");
    addresses.push_back("https://en.wiktionary.org/wiki/Category:Vietnamese_female_given_names_from_Chinese");
    addresses.push_back("https://en.wiktionary.org/wiki/Category:Welsh_female_given_names");
    addresses.push_back("https://en.wiktionary.org/wiki/Category:West_Frisian_female_given_names");
    addresses.push_back("https://en.wiktionary.org/wiki/Category:Zazaki_female_given_names");

    vector<string> existingNames;

    for (string downloadableFile : addresses)
    {
        for (int i = 1; i < 2; i++)
        {
            bool findingFirstWord = true;
            string currentFile = downloadableFile;

            if (downloadableFile.find("baby_names") != string::npos)
            {
                currentFile += boost::lexical_cast<std::string>(i) + ".html";
            }

            if (CFileUtils::FileExists("./" + currentFile))
            {
                //cout << currentFile << " exists" << "\n";
                CFileUtils::RemoveFile("./" + currentFile);
            }

            if (CFileUtils::FileExists("./" + currentFile))
            {
                cout << currentFile << " exists" << "\n";
            }
            else
            {
                cout << currentFile << " does not exist" << "\n";

                if (downloadableFile.find("baby_names") != string::npos)
                {
                    CFileUtils::ExecuteCommand("wget \"http://www.babynamescountry.com/origins/" + currentFile + "\"");
                }
                else if (downloadableFile.find("en.wiktionary.org/w") != string::npos)
                {
                    //currentFile = "P";
                    CFileUtils::ExecuteCommand("wget \"" + currentFile + "\" -O M.txt");
                    currentFile = "M.txt";
                    findingFirstWord = false;
                }
                else
                {
                    CFileUtils::ExecuteCommand("wget \"https://en.wiktionary.org/wiki/" + currentFile + "\"");
                    findingFirstWord = false;
                }
                currentFile = "./" + currentFile;

                if (CFileUtils::FileExists(currentFile))
                {
                    cout << currentFile << " exists" << "\n";



                    ifstream inputFile;
                    stringstream buffer;
                    string dataString;

                    try
                    {
                        //ifstream myfile (location);
                        inputFile.open(currentFile, ifstream::in);

                        if (inputFile.is_open())
                        {
                            buffer << inputFile.rdbuf();
                            inputFile.close();
                        }
                    }
                    catch (std::exception &ex)
                    {
                        cout << currentFile << " reading error " << ex.what();
                        //inputFile.close();
                    }

                    dataString = buffer.str();
                    buffer.clear();

                    remove(currentFile.c_str());

                    size_t nPos = string::npos;
                    if (downloadableFile.find("baby_names") != string::npos)
                    {
                        nPos = dataString.find("href=\"/meanings/", 0); // fist occurrence
                    }
                    else
                    {
                        nPos = dataString.find("href=\"/wiki/", 0); // fist occurrence
                        cout << nPos << endl;
                    }

                    while (nPos != string::npos)
                    {
                        findingFirstWord = false;
                        //cout << dataString.substr(nPos + 1, 20) << "\n";
                        string word = "";
                        size_t nPos1 = dataString.find(">", nPos + 1);
                        size_t nPos2 = dataString.find("</a>", nPos1 + 1);

                        if (nPos1 != string::npos && nPos2 != string::npos)
                        {
                            word = dataString.substr(nPos1 + 1, nPos2 - nPos1 - 1);
                            word = CTextUtils::Trim(word);
                            CLowerCaseString lowerWord(word);

                            SLowerCaseUnorderedSet::const_iterator gotWord = firstNameLookUp.find(lowerWord);

                            if (gotWord != firstNameLookUp.end())
                            {
                                //cout << "Found" << "\n";
                            }
                            else
                            {
                                //cout << "Not found" << "\n";
                                if ( find(existingNames.begin(), existingNames.end(), word)==existingNames.end() )
                                {
                                    cout << word << endl;
                                    firstNameLookUp.insert(lowerWord);
                                    existingNames.push_back(word);
                                }
                            }

                            //cout << CTextUtils::FirstLetterUpCase(word) << "\n";
                        }

                        //nPos = dataString.find("href=\"/meanings/", nPos + 1);
                        if (downloadableFile.find("baby_names") != string::npos)
                        {
                            nPos = dataString.find("href=\"/meanings/", nPos + 1); // fist occurrence
                        }
                        else
                        {
                            nPos = dataString.find("href=\"/wiki/", nPos + 1); // fist occurrence
                        }
                    }

                    if (findingFirstWord)
                    {
                        break;
                    }
                }
                else
                {
                    cout << currentFile << " does not exist" << "\n";
                }
            }
        }
    }

    for (auto i = firstNameLookUp.begin(); i != firstNameLookUp.end(); ++i)
    {
        string output = ((CLowerCaseString) (*i));
        nameResults.push_back(CTextUtils::FirstLetterUpCase(output));
    }

    sort(nameResults.begin(), nameResults.end());

    bool started = false;

    for (auto i = nameResults.begin(); i != nameResults.end(); ++i)
    {
        if ((*i).length() > 2)
        {
            nameResultsText += (started ? "," : "") + *i;
            started = true;
            //std::cout << *i << std::endl;
        }
    }

    ofstream namesFile;
    namesFile.open("./FirstName.txt");
    namesFile << nameResultsText;
    namesFile.close();

    return 0;
}