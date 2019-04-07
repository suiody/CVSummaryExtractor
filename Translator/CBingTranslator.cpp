//
// File CBingTranslator.cpp
//
//

#define BOOST_NETWORK_ENABLE_HTTPS
#include <boost/network.hpp>
#include <boost/function_output_iterator.hpp>
#include "CBingTranslator.h"
#include "Common/CTextUtils.h"
#include "Common/CFileUtils.h"
#include "Easylogging/easylogging++.h"

//#define TEST_ACCOUNT                        1

#ifdef TEST_ACCOUNT
#define         CLIENT_ID                   "tllnlntrts"
#define         CLIENT_SECRET               "w61EqpDf6uOOqH1R+kCJv8QDYaVlVdFxZMJYa83156o="
#else
#define         CLIENT_ID                   "Mercans"
#define         CLIENT_SECRET               "pxWN61HfqG127z4xqGRZlPvSFc1uu8pXW5c/cFq4oWM="
#endif

#define         CLIENT_BACKUP_ID            "tllnlntrts"
#define         CLIENT_BACKUP_SECRET        "w61EqpDf6uOOqH1R+kCJv8QDYaVlVdFxZMJYa83156o="

//#define         CLIENT_ID                   "tllnlntrts"//"Mercans"//"tllnlntrts"
//#define         CLIENT_SECRET               "w61EqpDf6uOOqH1R+kCJv8QDYaVlVdFxZMJYa83156o="//"pxWN61HfqG127z4xqGRZlPvSFc1uu8pXW5c/cFq4oWM="//"w61EqpDf6uOOqH1R+kCJv8QDYaVlVdFxZMJYa83156o="
#define         BING_API_TEXT_LIMIT         10000

namespace network = boost::network;
namespace http = boost::network::http;
using namespace Summary_Common;

/*
 * Summary_Translator::CBingTranslator::CBingTranslator
 *
 * Description
 * Constructor
 *
 * Inputs
 *
 * Returns
 *
 */
Summary_Translator::CBingTranslator::CBingTranslator()
{
    translationFirstMode = true;
    translationCount = 0;
}

/*
 * Summary_Translator::CBingTranslator::Translate
 *
 * Description
 * Reads data of from the input string vector and translates from language given with input parameter
 * to the language given with other input parameter. Translator is bing online api
 *
 * Inputs
 * const vector<string> &inputText - given input text array
 * const string &fromLanguage - language to translate from, short code (en, ar etc.)
 * const string &toLanguage - language to translate to, short code (en, ar etc.)
 *
 * Returns
 * Translated string. NB! Not thread safe, not to be used concurrently
 */
string Summary_Translator::CBingTranslator::Translate(const vector<string> &inputText,
                                                           const string &fromLanguage,
                                                           const string &toLanguage)
{
    string httpResponseBody;
    string outputText = "";
    string sendText = "";
    int totalLength = 0;
    bool continueThisMode = translationFirstMode;
    translateRequestError = false;
    translatedOutputText = "";
    translationCount ++;

    if (!translationFirstMode && translationCount > 2)
    {
        return "";
    }

    //Get bing api access token within api response string
    httpResponseBody = GetWebAccessToken();

    if (httpResponseBody.length() < 1)
    {
        LOG(ERROR) << CFileUtils::GetSourcePath() << ": " << "Bing response exception to access token request";
        return translatedOutputText;
    }

    //Extract access token itself
    string accessToken = ExtractAccessToken(httpResponseBody);

    if (accessToken.length() < 1)
    {
        LOG(ERROR) << CFileUtils::GetSourcePath() << ": " << "No Bing access token from web response";
        return translatedOutputText;
    }

    //Go through lines and send text in parts
    for (string line: inputText)
    {
        //Test urlencoded line length against maximum length
        if (line.length() * 3 + 6 < BING_API_TEXT_LIMIT)
        {
            totalLength += line.length() * 3 + 6;

            //Bing translate api accepts only 10000 characters
            if (totalLength < BING_API_TEXT_LIMIT)
            {
                sendText += line + "\n";
            }
            else
            {
                //Send translation portions by parts
                AddTranslationPart(accessToken, sendText, fromLanguage, toLanguage);
                if (!translationFirstMode && continueThisMode)
                {
                    return Translate(inputText, fromLanguage, toLanguage);
                }

                sendText = line + "\n";
                totalLength = line.length() * 3 + 6;
            }
        }
        else
        {
            //One line was longer than allowed, send what was in the buffer
            AddTranslationPart(accessToken, sendText, fromLanguage, toLanguage);
            if (!translationFirstMode && continueThisMode)
            {
                return Translate(inputText, fromLanguage, toLanguage);
            }

            int totalWordsLength = 0;
            sendText = "";

            //Split line to words and gather enough words
            //to form text for next translation part
            for (string word: CTextUtils::Split(line, ' '))
            {
                totalWordsLength += word.length() * 3 + 3;

                //Bing translate api accepts only 10000 characters
                if (totalWordsLength < BING_API_TEXT_LIMIT)
                {
                    sendText += word + " ";
                }
                else
                {
                    //Send translation portions by parts
                    AddTranslationPart(accessToken, sendText, fromLanguage, toLanguage);
                    if (!translationFirstMode && continueThisMode)
                    {
                        return Translate(inputText, fromLanguage, toLanguage);
                    }

                    sendText = word + " ";
                    totalWordsLength = word.length() * 3 + 3;
                }
            }

            //Send whats left from the last sent words
            AddTranslationPart(accessToken, sendText, fromLanguage, toLanguage);
            if (!translationFirstMode && continueThisMode)
            {
                return Translate(inputText, fromLanguage, toLanguage);
            }

            translatedOutputText += "\n";

            sendText = "";
            totalLength = 0;
        }
    }

    //Send whats left from the last sent text part
    AddTranslationPart(accessToken, sendText, fromLanguage, toLanguage);
    if (!translationFirstMode && continueThisMode)
    {
        return Translate(inputText, fromLanguage, toLanguage);
    }

    //cout << translatedOutputText << endl;
    return translatedOutputText;
}

/*
 * Summary_Translator::CBingTranslator::GetWebAccessToken
 *
 * Description
 * Sends a web request to http://api.microsofttranslator.com to obtain
 * access token for the bing translation api
 *
 * Inputs
 *
 * Returns
 * Web request response that contains bing translation service access token
 */
string Summary_Translator::CBingTranslator::GetWebAccessToken()
{
    http::client httpClient;
    string httpResponseBody = "";
    string clientID = CLIENT_ID;
    string clientSecret = CLIENT_SECRET;

    if (!translationFirstMode)
    {
        clientID = CLIENT_BACKUP_ID;
        clientSecret = CLIENT_BACKUP_SECRET;
    }

    // Form parameters
    stringstream apiParams;
    apiParams << "grant_type=client_credentials&client_id=" << network::uri::encoded(clientID)
    << "&client_secret=" << network::uri::encoded(clientSecret)
    << "&scope=" + network::uri::encoded("http://api.microsofttranslator.com");

    try
    {
        http::client::request httpRequest("https://datamarket.accesscontrol.windows.net/v2/OAuth2-13");
        httpRequest << network::header("Content-Type", "application/x-www-form-urlencoded");

        http::client::response httpResponse = httpClient.post(httpRequest, apiParams.str());

        httpResponseBody = http::body(httpResponse);
    }
    catch (exception& e)
    {
        // deal with exceptions here
        LOG(ERROR) << CFileUtils::GetSourcePath() << ": " << boost::diagnostic_information(e);
        return httpResponseBody;
    }

    return httpResponseBody;
}

/*
 * Summary_Translator::CBingTranslator::ExtractAccessToken
 *
 * Description
 * Extract bing translator api access token got from web request response to http://api.microsofttranslator.com
 *
 * Inputs
 * const string &httpResponseBody - web response body
 *
 * Returns
 * Extracted access token string
 */
string Summary_Translator::CBingTranslator::ExtractAccessToken(const string &httpResponseBody)
{
    string pureJsonResponse = httpResponseBody;
    string accessTokenKey = "\"access_token\"";
    string::size_type tokenKeyPosition = pureJsonResponse.find(accessTokenKey);
    string::size_type tokenStartPosition = string::npos;
    string::size_type tokenEndPosition = string::npos;
    string accessToken = "";

    if (tokenKeyPosition == string::npos)
    {
        return "";
    }

    tokenStartPosition = pureJsonResponse.find("\"", tokenKeyPosition + accessTokenKey.length());
    tokenEndPosition = pureJsonResponse.find("\"", tokenStartPosition + 1);

    accessToken = pureJsonResponse.substr(tokenStartPosition + 1, tokenEndPosition - tokenStartPosition - 1);

    return accessToken;
}

/*
 * Summary_Translator::CBingTranslator::TranslatePart
 *
 * Description
 * Reads data of the input string and translates from language given with input parameter
 * to the language given with other input parameter. Translator is bing online api. If
 * api returns error code then sets property translateRequestError to true and returns empty string
 *
 * Inputs
 * const string &accessToken - access token to bing translate api
 * const string &inputText - input text, maximum length must not exceed 10000 characters
 * const string &fromLanguage - language to translate from, short code (en, ar etc.)
 * const string &toLanguage - language to translate to, short code (en, ar etc.)
 *
 * Returns
 * Translated string. NB! Not thread safe, not to be used concurrently.
 */
string Summary_Translator::CBingTranslator::TranslatePart(const string &accessToken, const string &inputText,
                                                          const string &fromLanguage, const string &toLanguage)
{
    string translatedText = "";
    http::client httpClient;

    if (accessToken.length() < 1)
    {
        LOG(ERROR) << CFileUtils::GetSourcePath() << ": " << "No Bing access token";
        return translatedText;
    }

    //Encodes input UTF-8 text
    string text = CTextUtils::UrlEncode(inputText);
    string token = accessToken;
    //Sets request header
    string header = "Bearer " + token;
    //Sets request url
    string url = "http://api.microsofttranslator.com/V2/Http.svc/Translate?";
    string params = "to=";
    params += CTextUtils::ToLower(toLanguage) + "&from=";
    params += CldCodeToBingCode(CTextUtils::ToLower(fromLanguage)) + "&contentType=text/plain" + "&text="+ text;
    url += params;

    try
    {
        http::client::request httpRequest(url);
        httpRequest << boost::network::header("Content-Type","application/x-www-form-urlencoded; charset=utf-8")
        << boost::network::header("Authorization", header);
        //Makes the request and gets response
        http::client::response response = httpClient.get(httpRequest);

        //Reads response and removes carriage return symbols
        translatedText = CTextUtils::RemoveSubstring(body(response), "&#xD;");

        if (CTextUtils::StringContains(translatedText, "<body>") &&
            CTextUtils::StringContains(translatedText, "Translate()") &&
            CTextUtils::StringContains(translatedText, "<code>"))
        {
            //Got error code as a response
            LOG(ERROR) << CFileUtils::GetSourcePath() << ": " << translatedText <<
                          (translationFirstMode ? ": first account" : ": backup account");

            if (CTextUtils::StringContains(translatedText, "The Azure Market Place Translator Subscription "
                    "associated with the request credentials has zero balance"))
            {
                translationFirstMode = false;
            }

            translateRequestError = true;
            translatedText = "";
        }
        else
        {
            string::size_type packageStartPosition = translatedText.find("<string");
            string::size_type packageStartEndPosition = translatedText.find("/\">", packageStartPosition);
            string::size_type packageEndPosition = translatedText.find("</string>");

            //Extracts request response from <string>...</string> tags
            if (packageStartPosition != string::npos &&
                packageEndPosition != string::npos &&
                packageStartEndPosition != string::npos)
            {
                packageStartEndPosition += 3;
                translatedText = translatedText.substr(packageStartEndPosition, packageEndPosition - packageStartEndPosition);
                translatedText = CTextUtils::HtmlDecode(translatedText);
                translatedText = CTextUtils::HtmlDecode(translatedText);
            }
        }
    }
    catch (exception& e)
    {
        // deal with exceptions here
        LOG(ERROR) << CFileUtils::GetSourcePath() << ": " << boost::diagnostic_information(e);
        translateRequestError = true;
        return translatedText;
    }


    return translatedText;
}

/*
 * Summary_Translator::CBingTranslator::AddTranslationPart
 *
 * Description
 * Reads data from the input string and translates from language given with input parameter
 * to the language given with other input parameter. Translator is bing online api. Output is
 * copied to property translatedOutputText.
 *
 * Inputs
 * const string &accessToken - access token to bing translate api
 * const string &sendText - input text, maximum length must not exceed 10000 characters
 * const string &fromLanguage - language to translate from, short code (en, ar etc.)
 * const string &toLanguage - language to translate to, short code (en, ar etc.)
 *
 * Returns
 * Changed string translatedOutputText
 */
void Summary_Translator::CBingTranslator::AddTranslationPart(const string &accessToken, const string &sendText,
                                                             const string &fromLanguage, const string &toLanguage)
{
    if (sendText.length() > 0)
    {
        string translatedText = TranslatePart(accessToken, sendText, fromLanguage, toLanguage);
        //cout << sendText << endl;
        if (translateRequestError)
        {
            return;
        }

        translatedOutputText += translatedText;
    }
}

/*
 * Summary_Translator::CBingTranslator::CldCodeToBingCode
 *
 * Description
 * Translate Cld language code to bing language code
 *
 * Inputs
 * const string &cldCode - language code from Cld tool
 *
 * Returns
 * Bing language code equivalent
 */
string Summary_Translator::CBingTranslator::CldCodeToBingCode(const string &cldCode)
{
    //Bing translator language codes are given at https://msdn.microsoft.com/en-us/library/hh456380.aspx
    //These bing languages are not suppported: Hmong Daw, Klingon, Klingon (pIqaD), Querétaro Otomi, Yucatec Maya
    //Bing has codes for languages Serbian (Latin) (sr-Latn) and Serbian (Cyrillic) (sr-Cyrl) but Cld has only
    //one: Serbian (sr)
    //Norwegian code
    if (cldCode.compare("nb") == 0)
    {
        return "no";
    }
    //Bosnian code
    else if (cldCode.compare("bs") == 0)
    {
        return "bs-Latn";
    }
    //Simplified Chinese code
    else if (cldCode.compare("zh") == 0)
    {
        return "zh-CHS";
    }
    //Simplified Chinese code
    else if (cldCode.compare("zh-cn") == 0)
    {
        return "zh-CHS";
    }
    //Traditional Chinese code
    else if (cldCode.compare("zh-tw") == 0)
    {
        return "zh-CHT";
    }//
    else
    {
        return cldCode;
    }
}

