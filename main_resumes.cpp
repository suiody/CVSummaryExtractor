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
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/foreach.hpp>


#include <iostream>
#include <fstream>
#include <sstream>
#include <strstream>

#include <locale>
#include <codecvt>

using namespace boost::filesystem;
using namespace Summary_Common;
using namespace Summary_Json;
using namespace std;
using namespace Summary_ResumeProcessor_Helpers;
using namespace Summary_ResumeProcessor;
using namespace boost;

typedef boost::error_info<struct tag_stack_str,std::string> stack_info;

INITIALIZE_EASYLOGGINGPP

int main_resumes(int argc, char* argv[])
{
    CResourceLoader resourceLoader;
    SLowerCaseUnorderedSet firstNameLookUp = resourceLoader.LoadIntoLowerCaseHashSet(
            CFileUtils::GetRunPath() + "ResumeProcessor/Data/FirstName.txt", ',');
    vector<string> nameResults;
    string nameResultsText;

    vector<string> addresses;

    for (int i = 0; i < 2000; i++)
    {
        string addressUrl = "http://api.indeed.com/ads/apisearch?publisher=3662876754714213&v=2&start=";
        addressUrl += boost::lexical_cast<std::string>(i * 25);
        addressUrl += "&radius=1000&limit=10000&st=&jt=&fromage=&filter=&latlong=1&co=us&chnl=&userip=1.2.3.4";
        addresses.push_back(addressUrl);
    }



    for (string downloadableFile : addresses)
    {
        string resumesFile = "resumes.txt";
        CFileUtils::ExecuteCommand("wget \"" + downloadableFile + "\" -O " + resumesFile);
        resumesFile = "./" + resumesFile;

        boost::property_tree::ptree pt;
        read_xml(resumesFile, pt);

        BOOST_FOREACH(boost::property_tree::ptree::value_type &v, pt.get_child("response.results"))
        {
            //cout << v.second.get_child("url").data() << endl;
            string currentFile = "resume.txt";
            CFileUtils::ExecuteCommand("wget \"" + v.second.get_child("url").data() + "\" -O " + currentFile);
            //std::cout << v.second.get_child("<xmlattr>.Reference").data() << std::endl;



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

                size_t nPos = dataString.find("<span id=\"job_summary\" class=\"summary\">", 0); // fist occurrence

                if (nPos != string::npos)
                {
                    size_t nPos1 = dataString.find(">", nPos + 1);
                    size_t nPos2 = dataString.find("</span>", nPos1 + 1);

                    if (nPos1 != string::npos && nPos2 != string::npos)
                    {
                        boost::property_tree::ptree summaryTree;
                        string summary = dataString.substr(nPos1 + 1, nPos2 - nPos1 - 1);
                        //cout << summary << endl;

                        //summary = "<?xml version=\"1.0\"?><response><results><result>" + summary + "</result></results></response>\n";

                        ofstream xmlFile;
                        xmlFile.open("./xmlOutput.txt");
                        xmlFile << summary;
                        xmlFile.close();

                        if (CFileUtils::FileExists("./xmlOutput.txt"))
                        {
                            cout << currentFile << "./xmlOutput.txt" << "\n";
                        }

                        //istrstream summaryStream(summary.c_str());
                        //summaryStream << summary;

                        //boost::property_tree::xml_parser::read_xml("./xmlOutput.txt", summaryTree);
                        //read_xml(summaryStream, summaryTree);

                        /*BOOST_FOREACH(boost::property_tree::ptree::value_type &tv, summaryTree.get_child("response.results"))
                        {


                        }*/
                        std::vector<std::string>    tags;
                        std::vector<std::string>    text;
                        string html = summary;

                        for(;;)
                        {
                            std::string::size_type  startpos;

                            startpos = html.find('<');
                            if(startpos == std::string::npos)
                            {
                                // no tags left only text!
                                text.push_back(html);
                                break;
                            }

                            // handle the text before the tag
                            if(0 != startpos)
                            {
                                text.push_back(html.substr(0, startpos));
                                html = html.substr(startpos, html.size() - startpos);
                                startpos = 0;
                            }

                            //  skip all the text in the html tag
                            std::string::size_type endpos;
                            for(endpos = startpos;
                                endpos < html.size() && html[endpos] != '>';
                                ++endpos)
                            {
                                // since '>' can appear inside of an attribute string we need
                                // to make sure we process it properly.
                                if(html[endpos] == '"')
                                {
                                    endpos++;
                                    while(endpos < html.size() && html[endpos] != '"')
                                    {
                                        endpos++;
                                    }
                                }
                            }

                            //  Handle text and end of html that has beginning of tag but not the end
                            if(endpos == html.size())
                            {
                                html = html.substr(endpos, html.size() - endpos);
                                break;
                            }
                            else
                            {
                                //  handle the entire tag
                                endpos++;
                                tags.push_back(html.substr(startpos, endpos - startpos));
                                html = html.substr(endpos, html.size() - endpos);
                            }
                        }

                        std::cout << "tags:\n-----------------" << std::endl;

                        // auto, iterators or range based for loop would probably be better but
                        // this makes it a bit easier to read.
                        for(size_t i = 0; i < tags.size(); i++)
                        {
                            //std::cout << tags[i] << std::endl;
                        }

                        std::wofstream wof("file.txt", ios::out | ios::app);
                        wof.imbue(std::locale("en_US.UTF-8"));

                        std::cout << "\ntext:\n-----------------" << std::endl;
                        for(size_t i = 0; i < text.size(); i++)
                        {
                            string lineText = text[i];
                            lineText = CTextUtils::Remove(lineText, '"');
                            lineText = CTextUtils::Remove(lineText, '|');
                            lineText = CTextUtils::Trim(lineText);
                            if (lineText.length() > 0)
                            {
                                std::cout << lineText << std::endl;
                                wof << lineText.c_str() << endl;
                            }
                        }

                        //wof.open(L"file.txt");
                        //wof << L"This is a test.";
                        wof.close();

                        //exit(0);
                    }
                }
            }
            else
            {
                cout << currentFile << " does not exist" << "\n";
            }
        }
    }

    return 0;
}