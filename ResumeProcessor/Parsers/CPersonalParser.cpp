//
// File CPersonalParser.cpp
//
//
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file Boost/LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <boost/xpressive/regex_compiler.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/regex.hpp>
#include <boost/locale.hpp>
#include "CPersonalParser.h"
#include "DictionaryLoaderSingleton.h"
#include "../../Common/CTextUtils.h"
#include "../../Common/CDebugUtils.h"
#include "../../Common/CFileUtils.h"
#include "tld/libtld/tld.h"

using namespace Summary_Common;
using namespace Summary_ResumeProcessor_Helpers;

const boost::xpressive::sregex Summary_ResumeProcessor_Parsers::CPersonalParser::EmailRegex =
        boost::xpressive::sregex::compile
        ("[a-z0-9!#$%&'*+/=?^_`{|}~-]+(?:\\.[a-z0-9!#$%&'*+/=?^_`{|}~-]+)*@(?:[a-z0-9](?:[a-z0-9-]*[a-z0-9])?\\.)"
        "+[a-z0-9](?:[a-z0-9-]*[a-z0-9])?",
        boost::xpressive::icase | boost::xpressive::regex_constants::optimize);
const boost::xpressive::sregex Summary_ResumeProcessor_Parsers::CPersonalParser::PhoneRegex =
        boost::xpressive::sregex::compile
        ("(|\\+)(|\\s+)(|\\d{1,3})((\\d{1,30})|((|\\s+)(|\\()(|\\+)(|\\-|\\.)(|\\s+)\\d{1,30}(\\))(|\\s+)))(|\\s+)(|\\-|\\.)(|\\s+)(|\\-|\\.)(|\\s+)(\\d{1,30})(|\\s+)(|\\-|\\.)(|\\s+)"
        "(|\\-|\\.)(|\\s+)(\\d{1,30})(|\\s+)(|\\-|\\.)(|\\s+)(|\\-|\\.)(|\\s+)(\\d{1,30})(|\\s+)(|\\-|\\.)(|\\s+)(|\\-|\\.)(|\\s+)(\\d{1,30})",
         boost::xpressive::regex_constants::optimize);

const boost::xpressive::sregex Summary_ResumeProcessor_Parsers::CPersonalParser::SocialProfileRegex =
        boost::xpressive::sregex::compile
        ("(|(ftp|http|https):\\/\\/)?(\\w+\\.)?(www\\.|(\\w+\\.))?(([a-z]+)\\.)?(linkedin\\.com|facebook\\.com|github\\.com"
        "|stackoverflow\\.com|bitbucket\\.org|sourceforge\\.net|codeplex\\.com|google\\.com|delicious\\.com"
        "|deviantart\\.com|codeplex\\.com|instagram\\.com|myspace\\.com|pinterest\\.com"
        "|stumbleupon\\.com|tumblr\\.com|twitter\\.com|youtube\\.com|cloob\\.com|academia\\.edu|epernicus\\.com"
        "|hi5\\.com|hr\\.com|lafango\\.com|pingsta\\.com|poolwo\\.com|reverbnation\\.com|ryze\\.com|sciencestage\\.com"
        "|twoo\\.com|tribe\\.net|vk\\.com|zooppa\\.com)(:[0-9]+)?((\\/([~0-9a-zA-Z\\s+\\#\\+\\%@\\.\\/_-]+))"
         "?(\\?[0-9a-zA-Z\\s+\\+\\%@\\.\\?\\#\\/&\\[\\];=_-]+)?)?",
        boost::xpressive::icase | boost::xpressive::regex_constants::optimize);

const boost::xpressive::sregex Summary_ResumeProcessor_Parsers::CPersonalParser::SplitByWhiteSpaceRegex =
        boost::xpressive::sregex::compile ("\\s+|\\,|\\:|\\.|\\;|\u2028|\\(|\\)|\\/", boost::xpressive::regex_constants::optimize);

const boost::xpressive::sregex Summary_ResumeProcessor_Parsers::CPersonalParser::BirthDayRegex =
        boost::xpressive::sregex::compile
        ("((([012][1-9]|[1-2]0|3[01])|([1-9]))(|th|st|nd|rd)(|\\,|\\.)(|\\s+)(|of)(|\\s+)(Jan(|.)|January|Feb(|.)|February|Mar(|.)|March"
        "|Apr(|.)|April|May|Jun(|.)|June|Jul(|.)|July|Aug(|.)|August|Sep(|.)|Sept(|.)|September|Oct(|.)|October|Nov(|.)"
        "|November|Dec(|.)|December)(|\\,)(|\\s+)(19|20)([0-9]{2}))"

        "|((([012][1-9]|[1-2]0|3[01])|([1-9]))(|th|st|nd|rd)(\\-|\\/|\\.)(|\\s+)(Jan(|.)|January|Feb(|.)|February|Mar(|.)"
        "|March|Apr(|.)|April|May|Jun(|.)|June|Jul(|.)|July|Aug(|.)|August|Sep(|.)|Sept(|.)|September|Oct(|.)|October"
        "|Nov(|.)|November|Dec(|.)|December)(\\-|\\/|\\.)(19|20)([0-9]{2}))"

        "|((19|20)([0-9]{2})(|\\s+)(|\\-|\\/|\\.)(|\\s+)(Jan(|.)|January|Feb(|.)|February|Mar(|.)|March|Apr(|.)"
        "|April|May|Jun(|.)|June|Jul(|.)|July|Aug(|.)|August|Sep(|.)|Sept(|.)|September|Oct(|.)|October|Nov(|.)|November"
        "|Dec(|.)|December)(|\\s+)(|\\-|\\/|\\.)(|\\s+)(([012][1-9]|[1-2]0|3[01])|([1-9]))(|th|st|nd|rd))"

        "|((([012][1-9]|[1-2]0|3[01])|([1-9]))(|th|st|nd|rd)(\\-|\\/|\\.)(Jan(|.)|January|Feb(|.)|February|Mar(|.)|March|Apr(|.)"
        "|April|May|Jun(|.)|June|Jul(|.)|July|Aug(|.)|August|Sep(|.)|Sept(|.)|September|Oct(|.)|October|Nov(|.)|November"
        "|Dec(|.)|December)(\\-|\\/|\\.)([0-9]{2}))"

        "|((Jan(|.)|January|Feb(|.)|February|Mar(|.)|March|Apr(|.)|April|May|Jun(|.)|June|Jul(|.)|July|Aug(|.)|August"
        "|Sep(|.)|Sept(|.)|September|Oct(|.)|October|Nov(|.)|November|Dec(|.)|December)(|\\s+)(|\\-|\\/|\\.|\\,)(|\\s+)"
        "([012]?[1-9]|[12]0|3[01])(|th|st|nd|rd)(|\\s+)(|\\-|\\/|\\.|\\-|\\,)(|\\s+)(19|20)([0-9]{2}))"

        "|((Jan(|.)|January|Feb(|.)|February|Mar(|.)|March|Apr(|.)|April|May|Jun(|.)|June|Jul(|.)|July|Aug(|.)|August"
        "|Sep(|.)|Sept(|.)|September|Oct(|.)|October|Nov(|.)|November|Dec(|.)|December)\\s+(19|20)([0-9]{2}))"

        "|(([012]?[1-9]|[12]0|3[01])(|th|st|nd|rd)\\-(Jan(|.)|January|Feb(|.)|February|Mar(|.)|March|Apr(|.)|April|May"
        "|Jun(|.)|June|Jul(|.)|July|Aug(|.)|August|Sep(|.)|Sept(|.)|September|Oct(|.)|October|Nov(|.)|November|Dec(|.)"
        "|December)\\-([0-9]{2}))"

        "|((([012][1-9]|[1-2]0|3[01])|([1-9]))(|\\s+)(\\.|\\-|\\/)(|\\s+)((0[1-9]|1[012])|([1-9]))(|\\s+)(\\.|\\-|\\/"
        "|\\s)(|\\s+)(19|20)([0-9]{2}))"

        "|(((0[1-9]|1[012])|([1-9]))(|\\s+)(\\.|\\-|\\/)(|\\s+)(([012][1-9]|[1-2]0|3[01])|([1-9]))(|\\s+)(\\.|\\-|\\/"
        "|\\s)(|\\s+)(19|20)([0-9]{2}))"

        "|((19|20)([0-9]{2})(|\\s+)(\\.|\\-|\\/)(|\\s+)(([012][1-9]|[1-2]0|3[01])|([1-9]))(|\\s+)(\\.|\\-|\\/)(|\\s+)"
        "([012][1-9]|[1-2]0|3[01]))"
                 //yyyy dd mm, yyyy d m, yyyy m d and yyyy mm dd in variations

        "|((([012][1-9]|[1-2]0|3[01])|([1-9]))(|\\s+)(\\.|\\-|\\/)(|\\s+)((0[1-9]|1[012])|([1-9]))(|\\s+)(\\.|\\-|\\/"
        "|\\s)(|\\s+)([0-9]{2}))"

        "|(((0[1-9]|1[012])|([1-9]))(|\\s+)(\\.|\\-|\\/)(|\\s+)(([012][1-9]|[1-2]0|3[01])|([1-9]))(|\\s+)(\\.|\\-|\\/"
        "|\\s)(|\\s+)([0-9]{2}))"

        "|((([012][1-9]|[1-2]0|3[01])|([1-9]))(|\\s+)\\/(|\\s+)((0[1-9]|1[012])|([1-9]))(|\\s+)\\/(|\\s+)(19|20)([0-9]{2}))"

        "|(((0[1-9]|1[012])|([1-9]))(|\\s+)\\/(|\\s+)(([012][1-9]|[1-2]0|3[01])|([1-9]))(|\\s+)\\/(|\\s+)(19|20)([0-9]{2}))"

        //"|((([012][1-9]|[1-2]0|3[01])|([1-9]))\\/((0[1-9]|1[012])|([1-9]))\\-([0-9]{2}))"

        //"|((([012][1-9]|[1-2]0|3[01])|([1-9]))\\/((0[1-9]|1[012])|([1-9]))\\s(19|20)([0-9]{2}))"

        "|(([0-9]{2})\\/((0[1-9]|1[012])|([1-9]))\\/(([012][1-9]|[1-2]0|3[01])|([1-9])))"  //yy/mm/dd

        "|((([012][1-9]|[1-2]0|3[01])|([1-9]))\\s+(19|20)([0-9]{2}))"

        "|((19|20)([0-9]{2}))$",
        boost::xpressive::regex_constants::optimize
        |boost::xpressive::regex_constants::ECMAScript
        |boost::xpressive::regex_constants::icase);

const boost::xpressive::sregex Summary_ResumeProcessor_Parsers::CPersonalParser::SkypeRegex =
        boost::xpressive::sregex::compile
        ("[a-z][a-z0-9\\.,\\-_@]{5,31}",
        boost::xpressive::regex_constants::optimize
        |boost::xpressive::regex_constants::ECMAScript
        |boost::xpressive::regex_constants::icase);

const boost::xpressive::sregex Summary_ResumeProcessor_Parsers::CPersonalParser::UrlRegex =
        boost::xpressive::sregex::compile
                //("(http|https)://([^/ :]+):?([^/ ]*)(/?[^ #?]*)\\x3f?([^ #]*)#?([^ ]*)",
                //("(((ftp|http|https):\\/\\/)?(\\w+\\.)?(www\\.|(\\w+\\.))(\\w+\\.)?(\\w*)(:[0-9]+)?((\\/([~0-9a-zA-Z\\s+\\#\\+\\%@\\.\\/_-]+))?(\\?[0-9a-zA-Z\\s+\\#\\+\\%@\\/&\\[\\];=_-]+)?)?)|",
        ("(((ftp|http|https):\\/\\/)?((www\\.)|([a-z0-9_-]+\\.))(|([a-z0-9_-]+\\.))([a-z0-9_-]+\\.)?([a-z0-9_-]+\\.)?"
        "([a-z0-9_-]+\\.)?([a-z0-9_-]+\\.)?([a-z0-9_-]+\\.)?([a-z0-9_-]+\\.)?([a-z0-9_-]+\\.)?(\\w+)(:[0-9]+)?"
        "((\\/([~0-9a-zA-Z\\s+\\#\\+\\%@\\.\\:\\?\\/&\\/;=_-]+))?(\\?[0-9a-zA-Z\\s+\\#\\+\\%@\\.\\:\\?\\/&\\[\\];=_-]+)?)?)",
                 //"(\\?[0-9a-zA-Z\\s+\\#\\+\\%@\\.\\:\\?\\/&\\[\\];=_-]+)?)?)",
        boost::xpressive::regex_constants::optimize
        |boost::xpressive::regex_constants::ECMAScript
        |boost::xpressive::regex_constants::icase);

const boost::xpressive::sregex Summary_ResumeProcessor_Parsers::CPersonalParser::ShortUrlRegex =
        boost::xpressive::sregex::compile
        ("/(?:https?:\\/\\/)?(?:(?:0rz\\.tw)|(?:1link\\.in)|(?:1url\\.com)|(?:2\\.gp)|(?:2big\\.at)|(?:2tu\\.us)|"
         "(?:3\\.ly)|(?:307\\.to)|(?:4ms\\.me)|(?:4sq\\.com)|(?:4url\\.cc)|(?:6url\\.com)|(?:7\\.ly)|(?:a\\.gg)|"
         "(?:a\\.nf)|(?:aa\\.cx)|(?:abcurl\\.net)|(?:ad\\.vu)|(?:adf\\.ly)|(?:adjix\\.com)|(?:afx\\.cc)|"
         "(?:all\\.fuseurl.com)|(?:alturl\\.com)|(?:amzn\\.to)|(?:ar\\.gy)|(?:arst\\.ch)|(?:atu\\.ca)|(?:azc\\.cc)|"
         "(?:b23\\.ru)|(?:b2l\\.me)|(?:bacn\\.me)|(?:bcool\\.bz)|(?:binged\\.it)|(?:bit\\.ly)|(?:bizj\\.us)|"
         "(?:bloat\\.me)|(?:bravo\\.ly)|(?:bsa\\.ly)|(?:budurl\\.com)|(?:canurl\\.com)|(?:chilp\\.it)|(?:chzb\\.gr)|"
         "(?:cl\\.lk)|(?:cl\\.ly)|(?:clck\\.ru)|(?:cli\\.gs)|(?:cliccami\\.info)|(?:clickthru\\.ca)|(?:clop\\.in)|"
         "(?:conta\\.cc)|(?:cort\\.as)|(?:cot\\.ag)|(?:crks\\.me)|(?:ctvr\\.us)|(?:cutt\\.us)|(?:dai\\.ly)|"
         "(?:decenturl\\.com)|(?:dfl8\\.me)|(?:digbig\\.com)|(?:digg\\.com)|(?:disq\\.us)|(?:dld\\.bz)|(?:dlvr\\.it)|"
         "(?:do\\.my)|(?:doiop\\.com)|(?:dopen\\.us)|(?:easyuri\\.com)|(?:easyurl\\.net)|(?:eepurl\\.com)|"
         "(?:eweri\\.com)|(?:fa\\.by)|(?:fav\\.me)|(?:fb\\.me)|(?:fbshare\\.me)|(?:ff\\.im)|(?:fff\\.to)|(?:fire\\.to)|"
         "(?:firsturl\\.de)|(?:firsturl\\.net)|(?:flic\\.kr)|(?:flq\\.us)|(?:fly2\\.ws)|(?:fon\\.gs)|(?:freak\\.to)|"
         "(?:fuseurl\\.com)|(?:fuzzy\\.to)|(?:fwd4\\.me)|(?:fwib\\.net)|(?:g\\.ro.lt)|(?:gizmo\\.do)|(?:gl\\.am)|"
         "(?:go\\.9nl.com)|(?:go\\.ign.com)|(?:go\\.usa.gov)|(?:goo\\.gl)|(?:goshrink\\.com)|(?:gurl\\.es)|(?:hex\\.io)|"
         "(?:hiderefer\\.com)|(?:hmm\\.ph)|(?:href\\.in)|(?:hsblinks\\.com)|(?:htxt\\.it)|(?:huff\\.to)|(?:hulu\\.com)|"
         "(?:hurl\\.me)|(?:hurl\\.ws)|(?:icanhaz\\.com)|(?:idek\\.net)|(?:ilix\\.in)|(?:is\\.gd)|(?:its\\.my)|(?:ix\\.lt)|"
         "(?:j\\.mp)|(?:jijr\\.com)|(?:kl\\.am)|(?:klck\\.me)|(?:korta\\.nu)|(?:krunchd\\.com)|(?:l9k\\.net)|(?:lat\\.ms)|"
         "(?:liip\\.to)|(?:liltext\\.com)|(?:linkbee\\.com)|(?:linkbun\\.ch)|(?:liurl\\.cn)|(?:ln-s\\.net)|(?:ln-s\\.ru)|"
         "(?:lnk\\.gd)|(?:lnk\\.ms)|(?:lnkd\\.in)|(?:lnkurl\\.com)|(?:lru\\.jp)|(?:lt\\.tl)|(?:lurl\\.no)|(?:macte\\.ch)|"
         "(?:mash\\.to)|(?:merky\\.de)|(?:migre\\.me)|(?:miniurl\\.com)|(?:minurl\\.fr)|(?:mke\\.me)|(?:moby\\.to)|"
         "(?:moourl\\.com)|(?:mrte\\.ch)|(?:myloc\\.me)|(?:myurl\\.in)|(?:n\\.pr)|(?:nbc\\.co)|(?:nblo\\.gs)|(?:nn\\.nf)|"
         "(?:not\\.my)|(?:notlong\\.com)|(?:nsfw\\.in)|(?:nutshellurl\\.com)|(?:nxy\\.in)|(?:nyti\\.ms)|(?:o-x\\.fr)|"
         "(?:oc1\\.us)|(?:om\\.ly)|(?:omf\\.gd)|(?:omoikane\\.net)|(?:on\\.cnn.com)|(?:on\\.mktw.net)|(?:onforb\\.es)|"
         "(?:orz\\.se)|(?:ow\\.ly)|(?:ping\\.fm)|(?:pli\\.gs)|(?:pnt\\.me)|(?:politi\\.co)|(?:post\\.ly)|(?:pp\\.gg)|"
         "(?:profile\\.to)|(?:ptiturl\\.com)|(?:pub\\.vitrue.com)|(?:qlnk\\.net)|(?:qte\\.me)|(?:qu\\.tc)|(?:qy\\.fi)|"
         "(?:r\\.im)|(?:rb6\\.me)|(?:read\\.bi)|(?:readthis\\.ca)|(?:reallytinyurl\\.com)|(?:redir\\.ec)|(?:redirects\\.ca)|"
         "(?:redirx\\.com)|(?:retwt\\.me)|(?:ri\\.ms)|(?:rickroll\\.it)|(?:riz\\.gd)|(?:rt\\.nu)|(?:ru\\.ly)|"
         "(?:rubyurl\\.com)|(?:rurl\\.org)|(?:rww\\.tw)|(?:s4c\\.in)|(?:s7y\\.us)|(?:safe\\.mn)|(?:sameurl\\.com)|"
         "(?:sdut\\.us)|(?:shar\\.es)|(?:shink\\.de)|(?:shorl\\.com)|(?:short\\.ie)|(?:short\\.to)|(?:shortlinks\\.co.uk)|"
         "(?:shorturl\\.com)|(?:shout\\.to)|(?:show\\.my)|(?:shrinkify\\.com)|(?:shrinkr\\.com)|(?:shrt\\.fr)|(?:shrt\\.st)|"
         "(?:shrten\\.com)|(?:shrunkin\\.com)|(?:simurl\\.com)|(?:slate\\.me)|(?:smallr\\.com)|(?:smsh\\.me)|(?:smurl\\.name)|"
         "(?:sn\\.im)|(?:snipr\\.com)|(?:snipurl\\.com)|(?:snurl\\.com)|(?:sp2\\.ro)|(?:spedr\\.com)|(?:srnk\\.net)|(?:srs\\.li)|"
         "(?:starturl\\.com)|(?:su\\.pr)|(?:surl\\.co.uk)|(?:surl\\.hu)|(?:t\\.cn)|(?:t\\.co)|(?:t\\.lh.com)|(?:ta\\.gd)|"
         "(?:tbd\\.ly)|(?:tcrn\\.ch)|(?:tgr\\.me)|(?:tgr\\.ph)|(?:tighturl\\.com)|(?:tiniuri\\.com)|(?:tiny\\.cc)|"
         "(?:tiny\\.ly)|(?:tiny\\.pl)|(?:tinylink\\.in)|(?:tinyuri\\.ca)|(?:tinyurl\\.com)|(?:tl\\.gd)|(?:tmi\\.me)|"
         "(?:tnij\\.org)|(?:tnw\\.to)|(?:tny\\.com)|(?:to\\.ly)|(?:togoto\\.us)|(?:totc\\.us)|(?:toysr\\.us)|(?:tpm\\.ly)|"
         "(?:tr\\.im)|(?:tra\\.kz)|(?:trunc\\.it)|(?:twhub\\.com)|(?:twirl\\.at)|(?:twitclicks\\.com)|(?:twitterurl\\.net)|"
         "(?:twitterurl\\.org)|(?:twiturl\\.de)|(?:twurl\\.cc)|(?:twurl\\.nl)|(?:u\\.mavrev.com)|(?:u\\.nu)|(?:u76\\.org)|"
         "(?:ub0\\.cc)|(?:ulu\\.lu)|(?:updating\\.me)|(?:ur1\\.ca)|(?:url\\.az)|(?:url\\.co.uk)|(?:url\\.ie)|(?:url360\\.me)|"
         "(?:url4\\.eu)|(?:urlborg\\.com)|(?:urlbrief\\.com)|(?:urlcover\\.com)|(?:urlcut\\.com)|(?:urlenco\\.de)|(?:urli\\.nl)|"
         "(?:urls\\.im)|(?:urlshorteningservicefortwitter\\.com)|(?:urlx\\.ie)|(?:urlzen\\.com)|(?:usat\\.ly)|(?:use\\.my)|"
         "(?:vb\\.ly)|(?:vgn\\.am)|(?:vl\\.am)|(?:vm\\.lc)|(?:w55\\.de)|(?:wapo\\.st)|(?:wapurl\\.co.uk)|(?:wipi\\.es)|"
         "(?:wp\\.me)|(?:x\\.vu)|(?:xr\\.com)|(?:xrl\\.in)|(?:xrl\\.us)|(?:xurl\\.es)|(?:xurl\\.jp)|(?:y\\.ahoo.it)|"
         "(?:yatuc\\.com)|(?:ye\\.pe)|(?:yep\\.it)|(?:yfrog\\.com)|(?:yhoo\\.it)|(?:yiyd\\.com)|(?:youtu\\.be)|(?:yuarel\\.com)|"
         "(?:z0p\\.de)|(?:zi\\.ma)|(?:zi\\.mu)|(?:zipmyurl\\.com)|(?:zud\\.me)|(?:zurl\\.ws)|(?:zz\\.gd)|(?:zzang\\.kr)|"
         "(?:›\\.ws)|(?:✩\\.ws)|(?:✿\\.ws)|(?:❥\\.ws)|(?:➔\\.ws)|(?:➞\\.ws)|(?:➡\\.ws)|(?:➨\\.ws)|(?:➯\\.ws)|(?:➹\\.ws)|"
         "(?:➽\\.ws))\\/[a-z0-9]*/",
        boost::xpressive::regex_constants::optimize
        |boost::xpressive::regex_constants::ECMAScript
        |boost::xpressive::regex_constants::icase);

const boost::xpressive::sregex Summary_ResumeProcessor_Parsers::CPersonalParser::DomainNameRegex =
        boost::xpressive::sregex::compile
        ("\\b((?=[a-z0-9-]{1,63}\\.)(xn--)?[a-z0-9]+(-[a-z0-9]+)*\\.)+[a-z]{2,63}\\b",
        boost::xpressive::regex_constants::optimize
        |boost::xpressive::regex_constants::ECMAScript
        |boost::xpressive::regex_constants::icase);

#define PROTOCOLS                               "http,https,ftp,*\0"
#define EXAMPLE_LINE                            "www.abc.ei"
#define LINE_COUNT_FROM_KEYWORD_LINE            4

/*
 * Summary_ResumeProcessor_Parsers::CPersonalParser::CPersonalParser
 *
 * Description
 * Constructor. Loads county names and indicators that text could contain phone information
 *
 * Inputs
 * CResourceLoader resourceLoader - input resource loader object
 *
 * Returns
 *
 */
Summary_ResumeProcessor_Parsers::CPersonalParser::CPersonalParser(CResourceLoader resourceLoader)
{
    vector<string> lesserStopWords;
    firstNameLookUp = DictionaryLoaderSingleton::GetInstance().GetFirstNames();
    countryLookUp = DictionaryLoaderSingleton::GetInstance().GetCountries();
    countryLookUpLower = DictionaryLoaderSingleton::GetInstance().GetCountriesLower();
    countryMap = DictionaryLoaderSingleton::GetInstance().GetCountriesMap();
    religionLookUp = DictionaryLoaderSingleton::GetInstance().GetReligions();
    stopWordsLookup = DictionaryLoaderSingleton::GetInstance().GetStopWords();
    nonNamesLookup = DictionaryLoaderSingleton::GetInstance().GetNonNames();
    fieldsOfStudyLookUp = DictionaryLoaderSingleton::GetInstance().GetFieldsOfStudy();
    shortTaggedDegreeLookUp = DictionaryLoaderSingleton::GetInstance().GetShortTaggedDegrees();
    vector<string> jobTitles = DictionaryLoaderSingleton::GetInstance().GetJobs();

    phoneIndicators.push_back("tel.");
    phoneIndicators.push_back("tel,");
    phoneIndicators.push_back("tel ");
    phoneIndicators.push_back("tel:");
    phoneIndicators.push_back("telephone");
    phoneIndicators.push_back("telefon ");  //This is not in english, thou
    phoneIndicators.push_back("phone");
    phoneIndicators.push_back("phones");
    phoneIndicators.push_back("cell");
    phoneIndicators.push_back("mobile");
    phoneIndicators.push_back("mobil");
    phoneIndicators.push_back("mob ");
    phoneIndicators.push_back("mob.");
    phoneIndicators.push_back("mob,");
    phoneIndicators.push_back("mob:");
    phoneIndicators.push_back("gsm");
    phoneIndicators.push_back("ph.");
    phoneIndicators.push_back("ph:");
    phoneIndicators.push_back("t:");
    phoneIndicators.push_back("voice");

    exactPhoneIndicators.push_back("tel");
    exactPhoneIndicators.push_back("mob");
    exactPhoneIndicators.push_back("home");

    notAllowedStartings.push_back("email");
    notAllowedStartings.push_back("e-mail");
    copy(phoneIndicators.begin(), phoneIndicators.end(), std::back_inserter(notAllowedStartings));

    for(string stopWord: stopWordsLookup)
    {
        if (stopWord.length() > 1)
        {
            lesserStopWords.push_back(stopWord);
        }
    }

    stopWordsLookup = lesserStopWords;

    stopWordsLookup.push_back("first");
    stopWordsLookup.push_back("last");
    stopWordsLookup.push_back("family");
    stopWordsLookup.push_back("sure");
    stopWordsLookup.push_back("lastname");
    stopWordsLookup.push_back("surname");
    stopWordsLookup.push_back("familyname");
    stopWordsLookup.push_back("byname");
    stopWordsLookup.push_back("cognomen");
    stopWordsLookup.push_back("matronym");
    stopWordsLookup.push_back("metronym");
    stopWordsLookup.push_back("patronym");

    /*for (string jobTitle: jobTitles)
    {
        if (jobTitle.find(" ") == string::npos)
        {
            nonNamesLookup.push_back(CTextUtils::ToLower(jobTitle));
        }
    }*/

    for (string stopWord: stopWordsLookup)
    {
        if (stopWord.find(" ") == string::npos)
        {
            nonNamesLookup.push_back(CTextUtils::ToLower(stopWord));
        }
    }

    nameKeysVector.push_back("lastname");
    nameKeysVector.push_back("last name");
    nameKeysVector.push_back("surname");
    nameKeysVector.push_back("sur name");
    nameKeysVector.push_back("sure name");
    nameKeysVector.push_back("second name");
    nameKeysVector.push_back("familyname");
    nameKeysVector.push_back("family name");
    nameKeysVector.push_back("byname");
    nameKeysVector.push_back("cognomen");
    nameKeysVector.push_back("matronym");
    nameKeysVector.push_back("metronym");
    nameKeysVector.push_back("patronym");

    for (int j = 0; j < countryLookUpLower.size(); j++)
    {
        string country = countryLookUpLower.at(j);

        if (country.find(" ") != string::npos || country.find("-") != string::npos)
        {
            nonNamesLookup.push_back(country);
        }
        else
        {
            /*for (string countryPart: CTextUtils::Split(country, ' '))
            {
                //countryPart = TrimNameCharacters(countryPart);
                if (TrimNameCharacters(countryPart).length() < 4)
                {
                    continue;
                }
                nonNamesLookup.push_back(countryPart);
            }*/
        }
    }

    fullNameKeysVector.push_back("fullname");
    fullNameKeysVector.push_back("full name");
    fullNameKeysVector.push_back("the name");
    fullNameKeysVector.push_back("name of person");
    fullNameKeysVector.push_back("person name");
    fullNameKeysVector.push_back("person full name");
    fullNameKeysVector.push_back("person fullname");
    fullNameKeysVector.push_back("name");

    //English honorifics
    namePrefixes.push_back("dr");
    namePrefixes.push_back("miss");
    namePrefixes.push_back("ms");
    namePrefixes.push_back("sir");
    namePrefixes.push_back("mr");
    namePrefixes.push_back("mrs");
    namePrefixes.push_back("lady");
    namePrefixes.push_back("lord");
    namePrefixes.push_back("mx");
    namePrefixes.push_back("master");
    namePrefixes.push_back("sir");
    namePrefixes.push_back("madam");
    namePrefixes.push_back("dame");
    namePrefixes.push_back("adv");
    namePrefixes.push_back("prof");
    namePrefixes.push_back("br");
    namePrefixes.push_back("sr");
    namePrefixes.push_back("fr");
    namePrefixes.push_back("rev");
    namePrefixes.push_back("elder");
    namePrefixes.push_back("rabbi");
    namePrefixes.push_back("eng");
    namePrefixes.push_back("engr");
    namePrefixes.push_back("eng'r");
    namePrefixes.push_back("esquire");
    namePrefixes.push_back("captain");
    namePrefixes.push_back("coach");
    namePrefixes.push_back("officer");
    namePrefixes.push_back("colonel");
    namePrefixes.push_back("reverend");
    namePrefixes.push_back("phd");
    namePrefixes.push_back("fellow");

    //Persian honorifics
    namePrefixes.push_back("akhoond");
    namePrefixes.push_back("akhund");
    namePrefixes.push_back("akhwand");
    namePrefixes.push_back("khawaja");
    namePrefixes.push_back("khwaja");
    namePrefixes.push_back("ayatollah");
    namePrefixes.push_back("hojatoleslam");
    namePrefixes.push_back("mawlawi");
    namePrefixes.push_back("mawlānā");
    namePrefixes.push_back("mawlana");
    namePrefixes.push_back("maulana");
    namePrefixes.push_back("mullah");
    namePrefixes.push_back("sheikh");
    namePrefixes.push_back("sheik");
    namePrefixes.push_back("shayk");
    namePrefixes.push_back("shaykh");
    //namePrefixes.push_back("shaikh");
    namePrefixes.push_back("shekh");
    /*namePrefixes.push_back("shaik");
    namePrefixes.push_back("cheikh");
    */
    namePrefixes.push_back("allamah");
    namePrefixes.push_back("hadrat");
    namePrefixes.push_back("hadhrat");
    namePrefixes.push_back("hazret");
    namePrefixes.push_back("hazrat");
    namePrefixes.push_back("ulama");
    namePrefixes.push_back("ulema");

    //Islamic & Arabic honorifics
    namePrefixes.push_back("dervish");
    namePrefixes.push_back("darvesh");
    namePrefixes.push_back("murshid");
    namePrefixes.push_back("otin");
    namePrefixes.push_back("pir");
    namePrefixes.push_back("peer");
    namePrefixes.push_back("qalandar");
    namePrefixes.push_back("rahimahullah");
    namePrefixes.push_back("rais");
    namePrefixes.push_back("khoja");
    namePrefixes.push_back("khwāja");
    namePrefixes.push_back("sayyid");
    namePrefixes.push_back("seyd");
    namePrefixes.push_back("sayed");
    namePrefixes.push_back("sayyed");
    namePrefixes.push_back("saiyid");
    namePrefixes.push_back("seyed");
    namePrefixes.push_back("seyyed");
    namePrefixes.push_back("sharif");
    namePrefixes.push_back("asharif");
    namePrefixes.push_back("alsharif");
    namePrefixes.push_back("amier");
    namePrefixes.push_back("hajji");
    namePrefixes.push_back("alhaji");
    namePrefixes.push_back("hadji");
    namePrefixes.push_back("faqīh");
    namePrefixes.push_back("faqih");
    namePrefixes.push_back("qadi");
    namePrefixes.push_back("qaadi");
    namePrefixes.push_back("qaadee");
    namePrefixes.push_back("qazi");
    namePrefixes.push_back("kazi");
    namePrefixes.push_back("quazi");
    namePrefixes.push_back("lalla");
    namePrefixes.push_back("sidi");
    namePrefixes.push_back("mufti");
    namePrefixes.push_back("effendi");
    namePrefixes.push_back("effendy");
    namePrefixes.push_back("efendi");
    //namePrefixes.push_back("effendi");
    //namePrefixes.push_back("effendi");
    //namePrefixes.push_back("effendi");

    //Indian honorifics
    namePrefixes.push_back("sri");
    namePrefixes.push_back("shri");
    namePrefixes.push_back("sriman");

    //namePrefixes.push_back("kum");
    namePrefixes.push_back("kumari");
    namePrefixes.push_back("smt");

    //namePrefixes.push_back("baba");
    namePrefixes.push_back("babaji");
    namePrefixes.push_back("swami");
    namePrefixes.push_back("goswami");
    namePrefixes.push_back("pandit");
    namePrefixes.push_back("thiru");
    namePrefixes.push_back("thiruvalar");
    namePrefixes.push_back("thirumathi");
    namePrefixes.push_back("srimati");
    namePrefixes.push_back("srimati");
    namePrefixes.push_back("srimati");

    //German honorifics
    namePrefixes.push_back("herr");
    namePrefixes.push_back("frau");
    namePrefixes.push_back("doktor");
    namePrefixes.push_back("panie");

    //Javanese honorifics
    namePrefixes.push_back("ibu");
    namePrefixes.push_back("bu");
    namePrefixes.push_back("bapak");
    namePrefixes.push_back("mas");

    //French honorifics
    namePrefixes.push_back("monsieur");
    namePrefixes.push_back("madame");
    namePrefixes.push_back("professeur");
    namePrefixes.push_back("docteur");
    namePrefixes.push_back("eminence");
    namePrefixes.push_back("monseigneur");
    namePrefixes.push_back("dom");
    namePrefixes.push_back("mademoiselle");
    namePrefixes.push_back("mme");
    namePrefixes.push_back("mlle");

    //Italian honorifics
    namePrefixes.push_back("don");
    namePrefixes.push_back("donna");
    namePrefixes.push_back("prevosto");
    namePrefixes.push_back("arciprete");
    namePrefixes.push_back("curato");
    namePrefixes.push_back("canonico");
    namePrefixes.push_back("dottore");
    namePrefixes.push_back("dott");
    namePrefixes.push_back("avvocato");
    namePrefixes.push_back("avv");
    namePrefixes.push_back("ingegnere");
    namePrefixes.push_back("ing");
    namePrefixes.push_back("architetto");
    namePrefixes.push_back("arch");
    namePrefixes.push_back("dottoressa");
    namePrefixes.push_back("maestro");
    namePrefixes.push_back("maestra");
    namePrefixes.push_back("mastro");
    namePrefixes.push_back("professore");
    namePrefixes.push_back("professoressa");
    namePrefixes.push_back("signora");
    namePrefixes.push_back("signorina");
    namePrefixes.push_back("sig");

    //Portuguese honorifics
    namePrefixes.push_back("dom");
    namePrefixes.push_back("dona");
    namePrefixes.push_back("doutor");

    //Spanish honorifics
    namePrefixes.push_back("doñ");
    namePrefixes.push_back("doña");
    namePrefixes.push_back("señor");
    namePrefixes.push_back("señora");
    namePrefixes.push_back("señorina");
    namePrefixes.push_back("senor");
    namePrefixes.push_back("senora");
    namePrefixes.push_back("senorina");

    //Brazilian honorifics
    namePrefixes.push_back("senhor");
    namePrefixes.push_back("senhora");

    //Polish honorifics
    namePrefixes.push_back("pan");
    namePrefixes.push_back("panowie");
    namePrefixes.push_back("pani");
    namePrefixes.push_back("panie");

    //Judaistic honorifics
    namePrefixes.push_back("rabbi");
    namePrefixes.push_back("rav");
    namePrefixes.push_back("rebbe");
    namePrefixes.push_back("hakham");
    namePrefixes.push_back("maskil");

    //Other
    namePrefixes.push_back("comrade");  //Still exists

    imageFormatEndings.push_back(".arw");
    imageFormatEndings.push_back(".bmp");
    imageFormatEndings.push_back(".cr2");
    imageFormatEndings.push_back(".crw");
    imageFormatEndings.push_back(".dcm");
    imageFormatEndings.push_back(".djvu");
    imageFormatEndings.push_back(".dmg");
    imageFormatEndings.push_back(".dng");
    imageFormatEndings.push_back(".fpx");
    imageFormatEndings.push_back(".gif");
    imageFormatEndings.push_back(".ico");
    imageFormatEndings.push_back(".jp2");
    imageFormatEndings.push_back(".jpeg");
    imageFormatEndings.push_back(".jpg");
    imageFormatEndings.push_back(".nef");
    imageFormatEndings.push_back(".orf");
    imageFormatEndings.push_back(".pcd");
    imageFormatEndings.push_back(".pcx");
    imageFormatEndings.push_back(".pict");
    imageFormatEndings.push_back(".png");
    imageFormatEndings.push_back(".psd");
    imageFormatEndings.push_back(".sfw");
    imageFormatEndings.push_back(".tga");
    imageFormatEndings.push_back(".tif");
    imageFormatEndings.push_back(".tiff");
    imageFormatEndings.push_back(".webp");
    imageFormatEndings.push_back(".xcf");

    nonNamesLookup.push_back("eng");

    siteKeywords.push_back("website");
    siteKeywords.push_back("homepage");

    dropNames.push_back("c.v");
    dropNames.push_back("cv");
    dropNames.push_back("curriculum vitae");
    dropNames.push_back("m.sc");
    dropNames.push_back("b.sc");
    dropNames.push_back("u.s");
    dropNames.push_back("u.s.a");

    vector<string> degreeAbbreviations = DictionaryLoaderSingleton::GetInstance().GetDegreeAbbbreviations();

    for (string abbreviation: degreeAbbreviations)
    {
        //cout << abbreviation1 << endl;
        dropNames.push_back(abbreviation);
    }
}

/*
 * Summary_ResumeProcessor_Parsers::CPersonalParser::Parse
 *
 * Description
 * Method for parsing personal section and saving results (address, phine, gender, e-mail, birthday and social profiles)
 * into resume object
 *
 * Inputs
 * CSection section - section data
 * CResume &resume - resume object to be filled during parsing process
 *
 * Returns
 * changed resume object through input parameter resume
 */
void Summary_ResumeProcessor_Parsers::CPersonalParser::Parse(CSection section, CResume &resume)
{
    CDebugUtils::DebugCout("CPersonalParser", "Parse", "");
    bool firstNameFound = false;
    bool lastNameFound = false;
    bool addressFound = false;
    bool countryFound = false;
    bool genderFound = false;
    bool emailFound = false;
    bool phoneFound = false;
    bool birthDayFound = false;
    bool religionFound = false;
    bool skypeFound = false;
    bool webSiteFound = false;
    bool extractedHeaderFirstName = false;
    birthDayStringFound = false;
    religionStringFound = false;
    countryStringFound = false;
    religionKeyWordSteps = 10;
    countryKeyWordSteps = 10;
    phoneKeyWordSteps = 10;
    genderKeyWordSteps = 10;
    nameLineCount = 10;
    lastNameKeyWordSteps = 10;
    vector<string> nonKeywords;
    vector<string> sectionRawContentLower;
    vector<string> removedUnknownRawContent;
    //vector<string> removedUnknownRawContentLower;
    long rawContentLength = 0;

    string fileName = CFileUtils::GetDataFileName();

    if (fileName.length() > 0)
    {
        fileName = TrimNameCharacters(fileName);
        ExtractHeaderFirstName(resume, firstNameFound, fileName);

        if (resume.HasFirstName())
        {
            extractedHeaderFirstName = true;
        }
    }

    bool fullNameFound = false;
    for (string line: section.GetRawContent())
    {
        if (line.length() < 1)
        {
            continue;
        }

        fullNameFound = ExtractAndRepairFullName(resume, fullNameFound, line);

        rawContentLength += line.length();
    }

    resume.SetTextLength(rawContentLength);

    for (string line: section.GetRawContent())
    {
        if (line.length() < 1)
        {
            continue;
        }

        string newLine = CTextUtils::RemoveUnknownCharacters(line);
        removedUnknownRawContent.push_back(newLine);

        sectionRawContentLower.push_back(CTextUtils::ToLower(line));
        //removedUnknownRawContentLower.push_back(CTextUtils::ToLower(newLineSecond));
    }

    bool educationFound = false;
    for (string line: sectionRawContentLower)
    {
        phoneFound = ExtractPhone(resume, phoneFound, "", nonKeywords, false, line);
    }

    for (string line: section.GetContent())
    {
        if (line.length() < 1)
        {
            continue;
        }

        string originalLine = line;
        line = CTextUtils::RemoveUnknownCharacters(line);

        //Extract address only from the line where address string resides
        addressFound = ExtractAddress(resume, addressFound, true, originalLine);

        genderFound = ExtractGender(resume, genderFound, line);

        emailFound = ExtractEmail(resume, emailFound, originalLine);

        birthDayFound = ExtractBirthDay(resume, birthDayFound, line, "birth");

        religionFound = ExtractReligion(resume, religionFound, line);

        skypeFound = ExtractSkype(resume, skypeFound, line);

        ExtractSocialProfiles(resume, originalLine);
    }

    countryKeyWordSteps = 10;

    for (string line: section.GetRawContent())
    {
        if (line.length() < 1)
        {
            continue;
        }
        //Extract address only from the line where address string resides
        addressFound = ExtractAddress(resume, addressFound, true, line);

        firstNameFound = ExtractFirstAndLastName(resume, resume.GetFirstName(), firstNameFound, line, false);

        emailFound = ExtractEmail(resume, emailFound, line);

        ExtractSocialProfiles(resume, line);
    }

    for (string line: removedUnknownRawContent)
    {
        genderFound = ExtractGender(resume, genderFound, line);

        birthDayFound = ExtractBirthDay(resume, birthDayFound, line, "birth");

        religionFound = ExtractReligion(resume, religionFound, line);

        skypeFound = ExtractSkype(resume, skypeFound, line);

        webSiteFound = ExtractWebSite(resume, webSiteFound, line);
    }

    //Extract url candidates separately after all social propfiles are known
    for (string line: section.GetRawContent())
    {
        if (line.length() < 1)
        {
            continue;
        }

        ExtractUrlCandidates(resume, line);
    }

    if (!resume.HasCountry())
    {
        countryKeyWordSteps = 10;
        for (string line: section.GetRawContent())
        {
            if (line.length() < 1)
            {
                continue;
            }

            countryFound = ExtractCountry(resume, countryFound, line);
            if (countryFound)
            {
                break;
            }
        }
    }

    if (!addressFound)
    {
        for (string line: section.GetRawContent())
        {
            if (line.length() < 1)
            {
                continue;
            }

            //Search for other lines where address word is not found
            addressFound = ExtractAddress(resume, addressFound, false, line);

            if (addressFound)
            {
                break;
            }
        }
    }

    if (!resume.HasLastName())
    {
        lastNameFound = false;
        nameKeyFound = false;
        nameLineCount = 5;

        for (string line: section.GetRawContent())
        {
            if (line.length() < 1)
            {
                continue;
            }

            lastNameFound = ExtractAndRepairName(resume, lastNameFound, line);
            if (lastNameFound)
            {
                break;
            }
        }
    }

    fullNameFound = false;
    for (string line: section.GetRawContent())
    {
        if (line.length() < 1)
        {
            continue;
        }

        fullNameFound = ExtractAndRepairFullName(resume, fullNameFound, line);

        if (fullNameFound)
        {
            break;
        }
    }

    if (resume.HasFirstName() && !resume.HasMiddleName() && !resume.HasLastName())
    {
        lastNameFound = false;
        string lowerFirstName = CTextUtils::ToLower(resume.GetFirstName());

        for (string line: section.GetRawContent())
        {
            if (line.length() < 1)
            {
                continue;
            }

            lastNameFound = ExtractLastName(resume, lastNameFound, line, lowerFirstName);

            if (lastNameFound)
            {
                break;
            }
        }
    }

    if (extractedHeaderFirstName && !resume.HasMiddleName() && !resume.HasLastName())
    {
        lastNameFound = false;
        string lowerFirstName = CTextUtils::ToLower(resume.GetFirstName());

        for (string line: section.GetRawContent())
        {
            if (line.length() < 1)
            {
                continue;
            }

            lastNameFound = ExtractAndOverWriteName(resume, lastNameFound, line);

            if (lastNameFound)
            {
                break;
            }
        }
    }

    if (!resume.HasMiddleName() && !resume.HasLastName())
    {
        lastNameFound = false;

        for (string line: section.GetRawContent())
        {
            if (line.length() < 1)
            {
                continue;
            }

            lastNameFound = ExtractLastNameAfterKeyword(resume, lastNameFound, line);

            if (lastNameFound)
            {
                break;
            }
        }
    }

    if (!birthDayFound)
    {
        for (string line: removedUnknownRawContent)
        {
            birthDayFound = ExtractBirthDay(resume, birthDayFound, line, "d.o.b");

            if (birthDayFound)
            {
                break;
            }
        }
    }

    if (!birthDayFound)
    {
        for (string line: removedUnknownRawContent)
        {
            birthDayFound = ExtractBirthDay(resume, birthDayFound, line, "dob");

            if (birthDayFound)
            {
                break;
            }
        }
    }

    if (!birthDayFound)
    {
        for (string line: removedUnknownRawContent)
        {
            birthDayFound = ExtractBirthDay(resume, birthDayFound, line, "born");

            if (birthDayFound)
            {
                break;
            }
        }
    }

    phoneFound = BulkExtractPhone(sectionRawContentLower, resume, phoneFound);

    //Make final corrections to names
    PostProcessData(resume);
}

/*
 * Summary_ResumeProcessor_Parsers::CPersonalParser::ExtractAddress
 *
 * Description
 * Extracts address and saves into resume object. If already found then returns immediately
 *
 * Inputs
 * CResume &resume - reference to the resume object
 * bool addressFound - boolean indicating if address is already found
 * bool onlyAddressLines - if true then go through lines with address string in it only, otherwise use other lines
 * string line - input string line
 *
 * Returns
 * changed resume object through input parameter resume
 */
bool Summary_ResumeProcessor_Parsers::CPersonalParser::ExtractAddress(CResume &resume,
                                                                      bool addressFound,
                                                                      bool onlyAddressLines,
                                                                      string line)
{
    if (addressFound) return addressFound;

    string::size_type addressPosition = string::npos;
    string::size_type colonPosition = string::npos;
    string country = "";

    string lowerLine = CTextUtils::ToLower(line);
    line = CTextUtils::Trim(line);
    colonPosition = CTextUtils::FindString(line, ":");
    bool canAddLocation = true;

    if (onlyAddressLines)
    {
        addressPosition = CTextUtils::FindString(lowerLine, "address");

        if (addressPosition != string::npos && (colonPosition != string::npos && colonPosition > addressPosition))
        {
            string lineHalf = line.substr(colonPosition);
            lineHalf = CTextUtils::Trim(lineHalf);

            if (lineHalf.length() > 3 && !CTextUtils::StringContainsIgnoreCase(lineHalf, "address"))
            {
                lineHalf = line.substr(colonPosition + 1);
                lineHalf = CTextUtils::Trim(lineHalf);
                string lowerLineHalf = CTextUtils::ToLower(lineHalf);

                for (string nonStart: notAllowedStartings)
                {
                    if (CTextUtils::StartsWith(lowerLineHalf, nonStart))
                    {
                        canAddLocation = false;
                        break;
                    }
                }

                if (canAddLocation)
                {
                    resume.SetLocation(lineHalf);
                }

                if (!resume.HasCountry())
                {
                    countryKeyWordSteps = 0;  //Extract country information from the same line where address word is
                    country = ExtractCountryFromLine(lowerLine, true, "address");

                    if (!CTextUtils::IsEmptyOrWhiteSpace(country))
                    {
                        resume.SetCountry(country);
                    }
                }

                addressFound = true;
            }
        }

        return addressFound;
    }

    country = ExtractCountryFromLine(lowerLine, false, "");

    if (CTextUtils::IsEmptyOrWhiteSpace(country))
    {
        return addressFound;
    }

    //Assume address is in one line and ending with country name
    //Working backward to the beginning of the line to get the address
    resume.SetLocation(line.substr(0, CTextUtils::FindStringIgnoreCase(line, country) + country.size()));

    if (!resume.HasCountry())
    {
        resume.SetCountry(country);
    }

    addressFound = true;

    return addressFound;
}

/*
 * Summary_ResumeProcessor_Parsers::CPersonalParser::ExtractCountry
 *
 * Description
 * Extracts country and saves into resume object. If already found then returns immediately
 *
 * Inputs
 * CResume &resume - reference to the resume object
 * bool countryFound - boolean indicating if address is already found
 * string line - input string line
 *
 * Returns
 * changed resume object through input parameter resume
 */
bool Summary_ResumeProcessor_Parsers::CPersonalParser::ExtractCountry(CResume &resume, bool countryFound, string line)
{
    if (countryFound) return countryFound;

    string::size_type countryPosition = string::npos;
    string::size_type colonPosition = string::npos;
    string country = "";
    string lowerLine = CTextUtils::ToLower(line);
    line = CTextUtils::Trim(line);
    colonPosition = CTextUtils::FindString(line, ":");

    countryPosition = CTextUtils::FindString(lowerLine, "country");
    if (countryPosition == string::npos)
    {
        countryPosition = CTextUtils::FindString(lowerLine, "persistent location");
    }
    if (countryPosition == string::npos)
    {
        countryPosition = CTextUtils::FindString(lowerLine, "current location");
    }

    if (!countryStringFound && countryPosition != string::npos && (colonPosition != string::npos && colonPosition > countryPosition))
    {
        string lineHalf = line.substr(colonPosition);
        lineHalf = CTextUtils::Trim(lineHalf);

        if (lineHalf.length() > 3 &&
            !CTextUtils::StringContainsIgnoreCase(lineHalf, "country") &&
            !CTextUtils::StringContainsIgnoreCase(lineHalf, "location"))
        {
            countryStringFound = true;
            countryKeyWordSteps = 0;
        }
    }

    if (countryStringFound && countryKeyWordSteps < LINE_COUNT_FROM_KEYWORD_LINE)
    {
        country = ExtractCountryFromLine(lowerLine, false, "");
    }

    if (countryStringFound)
    {
        countryKeyWordSteps++;
    }

    if (CTextUtils::IsEmptyOrWhiteSpace(country))
    {
        return countryFound;
    }

    resume.SetCountry(country);

    countryFound = true;

    return countryFound;
}

/*
 * Summary_ResumeProcessor_Parsers::CPersonalParser::ExtractSocialProfiles
 *
 * Description
 * Extracts social profiles from string line
 *
 * Inputs
 * CResume &resume - reference to the resume object
 * string line - input string line
 *
 * Returns
 * changed resume object through input parameter resume
 */
void Summary_ResumeProcessor_Parsers::CPersonalParser::ExtractSocialProfiles(CResume &resume, string line)
{
    //line = EXAMPLE_LINE;
    boost::xpressive::sregex_iterator it(line.begin(), line.end(), SocialProfileRegex);
    boost::xpressive::sregex_iterator end;
    for (; it != end; ++it)
    {
        bool canAdd = true;
        string replacedProfile = "";
        string lowerNewProfile = CTextUtils::ToLower(it->str());
        allSocialProfiles.push_back(it->str()); //Gather here locally to the class scope all social profiles

        //Profile is a bit longer than traditional www.linkedin.com etc.
        if (1 == 1)
        {
            canAdd = !CTextUtils::VectorContainsMutually(resume.GetSocialProfiles(), lowerNewProfile, false);

            if (canAdd)
            {
                if (CTextUtils::EndsWith(lowerNewProfile, "facebook.com") ||
                        CTextUtils::EndsWith(lowerNewProfile, "twitter.com") ||
                        CTextUtils::EndsWith(lowerNewProfile, "google.com") ||
                        CTextUtils::EndsWith(lowerNewProfile, "linkedin.com") ||
                        CTextUtils::EndsWith(lowerNewProfile, "instagram.com"))
                {
                    canAdd = false;
                }
            }

            if (canAdd)
            {
                if (replacedProfile.length() > 0)
                {
                    vector<string> socialProfiles = resume.GetSocialProfiles();
                    std::replace (socialProfiles.begin(), socialProfiles.end(), replacedProfile, it->str());
                    resume.SetSocialProfiles(socialProfiles);
                }
                else
                {
                    resume.AddSocialProfile(it->str());
                }
            }
        }
    }

    for (string socialProfile: resume.GetSocialProfiles())
    {
        string lowerProfile = CTextUtils::ToLower(socialProfile);

        if (CTextUtils::StringContains(lowerProfile, "facebook.com"))
        {
            resume.SetFacebook(socialProfile);
        }
        else if (CTextUtils::StringContains(lowerProfile, "twitter.com"))
        {
            resume.SetTwitter(socialProfile);
        }
        else if (CTextUtils::StringContains(lowerProfile, "plus.google"))
        {
            resume.SetGooglePlus(socialProfile);
        }
        else if (CTextUtils::StringContains(lowerProfile, "linkedin.com"))
        {
            resume.SetLinkedIn(socialProfile);
        }
        else if (CTextUtils::StringContains(lowerProfile, "instagram.com"))
        {
            resume.SetInstagram(socialProfile);
        }
    }

    //DomainNameRegex
}

/*
 * Summary_ResumeProcessor_Parsers::CPersonalParser::ExtractUrlCandidates
 *
 * Description
 * Extracts url candidates from string line
 *
 * Inputs
 * CResume &resume - reference to the resume object
 * string line - input string line
 *
 * Returns
 * changed resume object through input parameter resume
 */
void Summary_ResumeProcessor_Parsers::CPersonalParser::ExtractUrlCandidates(CResume &resume, const string &line)
{
    if (resume.GetWebsite().length() > 0)
    {
        return;
    }

    boost::xpressive::sregex_iterator it3(line.begin(), line.end(), ShortUrlRegex);
    boost::xpressive::sregex_iterator end3;
    for (; it3 != end3; ++it3)
    {
        string url = it3->str();

        resume.AddUrlCandidate(url);
    }

    boost::xpressive::sregex_iterator it(line.begin(), line.end(), UrlRegex);
    boost::xpressive::sregex_iterator end;
    for (; it != end; ++it)
    {
        string urlCandidate = it->str();

        if (CTextUtils::StringContains(urlCandidate, " "))
        {
            urlCandidate = CTextUtils::Split(urlCandidate, ' ')[0];
        }

        if (UrlContainsValidDomain(line, urlCandidate) && urlCandidate.length() > 5)
        {
            bool canAdd = !CTextUtils::VectorContainsMutually(resume.GetUrlCandidates(), urlCandidate, false);

            if(canAdd && find(allSocialProfiles.begin(), allSocialProfiles.end(), urlCandidate) == allSocialProfiles.end())
            {
                canAdd = true;

                for (string imageEnding: imageFormatEndings)
                {
                    if (CTextUtils::EndsWith(urlCandidate, imageEnding))
                    {
                        canAdd = false;
                        break;
                    }
                }

                if (canAdd)
                {
                    if (CTextUtils::EndsWith(urlCandidate, "facebook.com") ||
                        CTextUtils::EndsWith(urlCandidate, "twitter.com") ||
                        CTextUtils::EndsWith(urlCandidate, "google.com") ||
                        CTextUtils::EndsWith(urlCandidate, "linkedin.com") ||
                        CTextUtils::EndsWith(urlCandidate, "yahoo.com") ||
                        CTextUtils::EndsWith(urlCandidate, "gmail.com") ||
                        CTextUtils::EndsWith(urlCandidate, "instagram.com"))
                    {
                        canAdd = false;
                    }
                }

                if (canAdd)
                {
                    //TODO: Need to fix this within UrlRegex
                    if (CTextUtils::StringContains(urlCandidate, "www.") ||
                        CTextUtils::StringContains(urlCandidate, "http://") ||
                        CTextUtils::StringContains(urlCandidate, "https://") ||
                        CTextUtils::StringContains(urlCandidate, "ftp://"))
                    {
                        string oldUrlCandidate = urlCandidate;

                        urlCandidate = CTextUtils::LTrimAnyChar(urlCandidate, "0123456789");

                        if (!CTextUtils::StartsWith(urlCandidate, "www.") &&
                            !CTextUtils::StartsWith(urlCandidate, "http://") &&
                            !CTextUtils::StartsWith(urlCandidate, "https://") &&
                            !CTextUtils::StartsWith(urlCandidate, "ftp://"))
                        {
                            urlCandidate = oldUrlCandidate;
                        }
                    }

                    resume.AddUrlCandidate(urlCandidate);
                }
            }
        }
    }
}

/*
 * Summary_ResumeProcessor_Parsers::CPersonalParser::ExtractPhone
 *
 * Description
 * Extracts phone and saves into resume object. If already found then returns immediately
 *
 * Inputs
 * CResume &resume - reference to the resume object
 * bool phoneFound - boolean indicating if phone is already found
 * const string &phoneKeyWord - keyword indicating that phone can be on the line or on next lines.
 * Expected to be in lower case.
 * const vector<string> &nonKeywords - vector of keywords not allowed in the text when digging for phone numbers.
 * Expected to be in lower case.
 * bool keyWordAfterAllowed - if true, keywords to determine phone number existence can ba after number
 * const string &line - input string line. Expected to be in lower case
 *
 * Returns
 * changed resume object through input parameter resume
 */
bool Summary_ResumeProcessor_Parsers::CPersonalParser::ExtractPhone(CResume &resume,
                                                                    bool phoneFound,
                                                                    const string &phoneKeyWord,
                                                                    const vector<string> &nonKeywords,
                                                                    bool keyWordAfterAllowed,
                                                                    const string &line)
{
    if (phoneFound) return phoneFound;

    string halfLine = line;
    string totalMatch = "";
    string prevMatch = "";
    string firstMatch = "";
    string lowerLine = CTextUtils::RemoveUnknownCharacters(line);
    lowerLine = CTextUtils::Replace(lowerLine, ',', '.');
    string lowerPhoneKeyword = phoneKeyWord;
    bool hasPhoneIndicator = false;
    bool hasCurrentPhoneIndicator = false;
    bool firstIteration = true;
    string::size_type phoneIndicatorPosition = string::npos;
    vector<string> linePhoneNumbers;

    if (CTextUtils::StringContains(lowerLine, "home address"))
    {
        return phoneFound;
    }

    boost::xpressive::sregex_iterator it(lowerLine.begin(), lowerLine.end(), PhoneRegex);
    boost::xpressive::sregex_iterator end;

    if (phoneKeyWord.length() > 0)
    {
        for (string nonKeyWord: nonKeywords)
        {
            //If any of the not allowed keywords are in the text then return immediately
            string lowerNonKeyWord = nonKeyWord;//CTextUtils::ToLower(nonKeyWord);

            if (CTextUtils::StringContains(lowerLine, lowerNonKeyWord))
            {
                return phoneFound;
            }
        }

        phoneIndicatorPosition = CTextUtils::FindString(lowerLine, lowerPhoneKeyword);

        //If the keyword is in the text then phone is to be expected right after it or after some lines
        if (phoneIndicatorPosition != string::npos &&
                (
                    CTextUtils::ContainsWithExactStartAndEnding(lowerLine, lowerPhoneKeyword) ||
                    lowerPhoneKeyword.length() == 1
                )
            )
        {
            //Set line count to 0 and expect phone number from 0-th to less than const (4) lines after 0-th line
            phoneKeyWordSteps = 0;
        }
    }
    else
    {
        if (phoneKeyWordSteps != 0)
        {
            for (string indicator: exactPhoneIndicators)
            {
                string lowerIndicator = indicator;

                if (CTextUtils::StartsWith(lowerLine, lowerIndicator) &&
                    CTextUtils::ContainsWithExactEnding(lowerLine, lowerIndicator))
                {
                    if (lowerIndicator.compare("home") == 0)
                    {
                        //cout << "123:" << lowerLine << endl;
                    }
                    //Set line count to 0 and expect phone number from 0-th to const (3) lines after 0-th line
                    phoneKeyWordSteps = 0;
                    break;
                }
            }
        }

        if (phoneKeyWordSteps != 0)
        {
            for (string indicator: phoneIndicators)
            {
                string lowerIndicator = indicator;
                phoneIndicatorPosition = CTextUtils::FindString(lowerLine, lowerIndicator);

                if (
                        phoneIndicatorPosition != string::npos &&
                        CTextUtils::ContainsWithExactStart(lowerLine, lowerIndicator) &&
                        (
                                CTextUtils::EndsWith(indicator, ":") ||
                                CTextUtils::ContainsWithExactEnding(lowerLine, lowerIndicator)
                        )
                    )
                {
                    //Set line count to 0 and expect phone number from 0-th to const (3) lines after 0-th line
                    phoneKeyWordSteps = 0;
                    break;
                }
            }
        }
    }

    //If keyword length is exactly 1 symbol
    if (phoneKeyWord.length() == 1)
    {
        if (phoneKeyWordSteps > 0)
        {
            //Then don't search for the phone number on next line
            return phoneFound;
        }
    }

    for (; it != end; ++it)
    {
        //Iterate through each phone number candidate on the line
        string currentMatch = CTextUtils::Trim(it->str());
        //cout << currentMatch << endl;

        if (CTextUtils::StringContainsAnyOf(lowerLine, {currentMatch + "@", "@" + currentMatch}))
        {
            continue;
        }

        phoneIndicatorPosition = string::npos;

        if (!hasPhoneIndicator)
        {
            firstMatch = currentMatch;
        }

        if (phoneKeyWord.length() > 0)
        {
            phoneIndicatorPosition = CTextUtils::FindString(lowerLine, lowerPhoneKeyword);

            if (firstIteration)
            {
                hasPhoneIndicator = HasPhoneKeyWordIndicator(firstMatch, phoneIndicatorPosition,
                                                             lowerLine, lowerPhoneKeyword, keyWordAfterAllowed);
                hasCurrentPhoneIndicator = hasPhoneIndicator;
            }
            else
            {
                hasCurrentPhoneIndicator = HasPhoneKeyWordIndicator(currentMatch, phoneIndicatorPosition,
                                                                    lowerLine, lowerPhoneKeyword, keyWordAfterAllowed);
            }
        }
        else
        {
            bool foundExactKeyword = false;

            for (string indicator: exactPhoneIndicators)
            {
                string lowerIndicator = indicator;

                if (CTextUtils::StartsWith(lowerLine, lowerIndicator) &&
                    CTextUtils::ContainsWithExactEnding(lowerLine, lowerIndicator))
                {
                    if (lowerIndicator.compare("home") == 0)
                    {
                        //cout << "1234:" << lowerLine << endl;
                    }
                    phoneIndicatorPosition = CTextUtils::FindString(lowerLine, lowerIndicator);

                    if (firstIteration)
                    {
                        hasPhoneIndicator = HasPhoneIndicator(firstMatch, phoneIndicatorPosition,
                                                              lowerLine, lowerIndicator);
                        hasCurrentPhoneIndicator = hasPhoneIndicator;
                    }
                    else
                    {
                        hasCurrentPhoneIndicator = HasPhoneIndicator(currentMatch, phoneIndicatorPosition,
                                                                     lowerLine, lowerIndicator);
                    }
                }

                if (hasCurrentPhoneIndicator && hasPhoneIndicator)
                {
                    foundExactKeyword = true;
                    break;
                }
            }

            if (!foundExactKeyword)
            {
                for (string indicator: phoneIndicators)
                {
                    string lowerIndicator = CTextUtils::ToLower(indicator);
                    phoneIndicatorPosition = CTextUtils::FindString(lowerLine, lowerIndicator);
                    //cout << indicator << ":" << phoneIndicatorPosition << endl;
                    //cout << currentMatch << endl;

                    if (firstIteration)
                    {
                        hasPhoneIndicator = HasPhoneIndicator(firstMatch, phoneIndicatorPosition,
                                                              lowerLine, lowerIndicator);
                        hasCurrentPhoneIndicator = hasPhoneIndicator;
                    }
                    else
                    {
                        hasCurrentPhoneIndicator = HasPhoneIndicator(currentMatch, phoneIndicatorPosition,
                                                                     lowerLine, lowerIndicator);
                    }

                    if (hasCurrentPhoneIndicator && hasPhoneIndicator)
                    {
                        break;
                    }
                }
            }
        }

        if (hasPhoneIndicator)
        {
            if (prevMatch.length() > 0)
            {
                halfLine = halfLine.substr(CTextUtils::FindString(halfLine, prevMatch) + prevMatch.length());
                halfLine = CTextUtils::Trim(halfLine);

                if (halfLine.find(currentMatch) == 0)
                {
                    if (totalMatch.find("+") == string::npos || currentMatch.find("+") == string::npos )
                    {
                        totalMatch = totalMatch + (totalMatch.length() > 0 ? " " : "") + currentMatch;
                    }
                }
            }
            else
            {
                totalMatch = currentMatch;
            }
        }

        if (hasCurrentPhoneIndicator && resume.GetPhoneNumbers().length() < currentMatch.length())
        {
            if ( resume.GetPhoneNumbers().length() < 7 ||
                 find(linePhoneNumbers.begin(), linePhoneNumbers.end(), resume.GetPhoneNumbers())==linePhoneNumbers.end() )
            {
                resume.SetPhoneNumbers(currentMatch);
            }
        }

        if (hasPhoneIndicator && resume.GetPhoneNumbers().length() < totalMatch.length())
        {
            resume.SetPhoneNumbers(totalMatch);
        }

        if (hasPhoneIndicator || hasCurrentPhoneIndicator)
        {
            phoneFound = true;
        }

        prevMatch = currentMatch;

        if (hasPhoneIndicator)
        {
            firstIteration = false;
        }

        if (currentMatch.length() > 5)
        {
            linePhoneNumbers.push_back(currentMatch);
        }
    }

    phoneKeyWordSteps++;

    return phoneFound;
}

/*
 * Summary_ResumeProcessor_Parsers::CPersonalParser::BulkExtractPhone
 *
 * Description
 * Extracts phone by using different clues like contact, address etc. keywords and saves into resume object.
 * If already found then returns immediately
 *
 * Inputs
 * const vector<string> &sectionRawContentLower - raw content data in liwer case
 * CResume &resume - resume object to change
 * bool phoneFound - boolean indicating if phone is already found
 *
 * Returns
 * changed resume object through input parameter resume
 */
bool Summary_ResumeProcessor_Parsers::CPersonalParser::BulkExtractPhone(const vector<string> &sectionRawContentLower,
                                                                        CResume &resume,
                                                                        bool phoneFound)
{
    if (phoneFound) return phoneFound;
//return false;
    vector<string> phoneKeyWords;
    vector<string> nonKeywords;

    phoneKeyWords.push_back("contact");
    phoneKeyWords.push_back("contacts");

    //Search for phone numbers after words "contact", "contacts"
    phoneFound = ExtractMorePhoneData(sectionRawContentLower, resume, phoneFound, phoneKeyWords, nonKeywords);

    //Do not count on lines where the following keywords exist:
    nonKeywords.push_back("e-mail");
    nonKeywords.push_back("email");
    nonKeywords.push_back("skype");
    nonKeywords.push_back("to address");

    phoneKeyWords.clear();
    phoneKeyWords.push_back("address");

    //Search for phone numbers after word "address"
    phoneFound = ExtractMorePhoneData(sectionRawContentLower, resume, phoneFound, phoneKeyWords, nonKeywords);

    nonKeywords.clear();

    if (!phoneFound)
    {
        PreparePhoneClues(resume, sectionRawContentLower);

        phoneFound = ExtractPhoneByClues(sectionRawContentLower, resume, phoneFound);

        if (phoneFound)
        {
            //cout << "nf1" << endl;
        }
    }

    //Did not yet find phone
    for (string line: sectionRawContentLower)
    {
        //Find phone after the "@" keyword in text
        phoneFound = ExtractPhone(resume, phoneFound, "@", nonKeywords, true, line);

        if (phoneFound)
        {
            if (CTextUtils::StringContainsAnyOf(line, {resume.GetPhoneNumbers() + "@", "@" + resume.GetPhoneNumbers()}))
            {
                phoneFound = false;
                resume.SetPhoneNumbers("");
            }
            else
            {
                break;
                //cout << "nf2" << endl;
            }
        }
    }

    //Did not yet find phone but at least one of the names exists
    if (!phoneFound && (resume.HasFirstName() || resume.HasMiddleName() || resume.HasLastName()))
    {
        //Do not count on lines where the following keywords exist:
        nonKeywords.push_back("www");
        nonKeywords.push_back("https");
        nonKeywords.push_back("http");
        nonKeywords.push_back("ftp");
        nonKeywords.push_back("year");
        nonKeywords.push_back("meter");
        nonKeywords.push_back("tonn");
        nonKeywords.push_back("m2");
        nonKeywords.push_back("area");
        nonKeywords.push_back("kilo");
        nonKeywords.push_back("permit");
        nonKeywords.push_back("passport");

        string name = resume.GetFirstName();
        name += (name.length() > 0 || resume.GetMiddleName().length() > 0 ? " " : "") + resume.GetMiddleName();
        name = CTextUtils::Trim(name);
        name += (name.length() > 0 || resume.GetLastName().length() > 0 ? " " : "") + resume.GetLastName();
        name = CTextUtils::Trim(name);
        name = CTextUtils::ToLower(name);
        bool somePhoneFound = false;

        for (string line: sectionRawContentLower)
        {
            //Find phone after the name in text
            phoneFound = ExtractPhone(resume, phoneFound, name, nonKeywords, false, line);

            //If found phone on this line or before it
            if (phoneFound || somePhoneFound)
            {
                somePhoneFound = true;

                //BUT. This time search for the longest phone on 3 lines
                if (phoneKeyWordSteps < LINE_COUNT_FROM_KEYWORD_LINE)
                {
                    phoneFound = false;
                }
                else
                {
                    //Do not allow too short phone numbers near name
                    if (resume.GetPhoneNumbers().length() < 6)
                    {
                        phoneFound = false;
                        resume.SetPhoneNumbers("");
                    }
                    else
                    {
                        phoneFound = true;
                        //cout << "nf3" << endl;
                        break;
                    }
                }
            }
        }
    }

    if (!phoneFound)
    {
        phoneKeyWords.clear();

        phoneKeyWords.push_back("personal information");
        phoneKeyWords.push_back("personal data");

        nonKeywords.clear();
        //Do not count on lines where the following keywords exist:
        nonKeywords.push_back("www");
        nonKeywords.push_back("https");
        nonKeywords.push_back("http");
        nonKeywords.push_back("ftp");

        //Search for phone numbers after words "personal information", "personal data"
        phoneFound = ExtractMorePhoneData(sectionRawContentLower, resume, phoneFound, phoneKeyWords, nonKeywords);
        if (phoneFound)
        {
            //cout << "nf4" << endl;
        }
    }

    /*if (!phoneFound)
    {
        nonKeywords.clear();

        nonKeywords.push_back("address");

        //Did not yet find phone
        for (string line: sectionRawContentLower)
        {
            //Find phone after the "home" keyword in text
            phoneFound = ExtractPhone(resume, phoneFound, "home", nonKeywords, true, line);

            if (phoneFound)
            {
                cout << "nf5" << endl;
                break;
            }
        }
    }*/

    return false;
}

/*
 * Summary_ResumeProcessor_Parsers::CPersonalParser::ExtractMorePhoneData
 *
 * Description
 * Extracts phone by using clues from phoneKeyWords vector and saves into resume object.
 * If already found then returns immediately
 *
 * Inputs
 * const vector<string> &sectionRawContentLower - raw content data in liwer case
 * CResume &resume - resume object to change
 * bool phoneFound - boolean indicating if phone is already found
 * const vector<string> &phoneKeyWords - keywords vector indicating that phone can be on the line or on next lines
 * const vector<string> &nonKeywords - vector of keywords not allowed in the text when digging for phone numbers
 *
 * Returns
 * changed resume object through input parameter resume
 */
bool Summary_ResumeProcessor_Parsers::CPersonalParser::ExtractMorePhoneData(const vector<string> &sectionRawContentLower,
                                                                            CResume &resume,
                                                                            bool phoneFound,
                                                                            const vector<string> &phoneKeyWords,
                                                                            const vector<string> &nonKeywords)
{
    if (phoneKeyWords.size() < 1)
    {
        return phoneFound;
    }

    for (string phoneKeyWord: phoneKeyWords)
    {
        //Did not yet find phone
        if (!phoneFound)
        {
            bool somePhoneFound = false;

            for (string line: sectionRawContentLower)
            {
                //Find phone after the phoneKeyWord keyword in text
                phoneFound = ExtractPhone(resume, phoneFound, phoneKeyWord, nonKeywords, false, line);

                if (phoneFound || somePhoneFound)
                {
                    somePhoneFound = true;

                    //BUT. This time search for the longest phone on 3 lines
                    if (phoneKeyWordSteps < LINE_COUNT_FROM_KEYWORD_LINE)
                    {
                        phoneFound = false;
                    }
                    else
                    {
                        phoneFound = true;
                        break;
                    }
                }
            }
        }
    }

    return phoneFound;
}

void Summary_ResumeProcessor_Parsers::CPersonalParser::PreparePhoneClues(const CResume &resume,
                                                                         const vector<string> &sectionRawContentLower)
{
    if (resume.GetPhoneNumbers().length() > 0 || resume.GetEmailAddress().length() < 1)
    {
        return;
    }
    int lineIndex = -1;
    string emailAddressLower = CTextUtils::ToLower(resume.GetEmailAddress());

    for (string line: sectionRawContentLower)
    {
        lineIndex++;

        if (CTextUtils::StringContains(line, emailAddressLower))
        {
            phoneClueLineNumbers.push_back(lineIndex);
            //cout << lineIndex << endl;
        }
    }
}

bool Summary_ResumeProcessor_Parsers::CPersonalParser::ExtractPhoneByClues(const vector<string> &sectionRawContentLower,
                                                                           CResume &resume,
                                                                           bool phoneFound)
{
    if (phoneFound) return phoneFound;

    int lineIndex = -1;

    for (string line: sectionRawContentLower)
    {
        lineIndex++;

        if (LineDistance(phoneClueLineNumbers, lineIndex, false, 3) == 1)
        {
            string halfLine = line;
            string totalMatch = "";
            string prevMatch = "";
            string firstMatch = "";
            string lowerLine = CTextUtils::RemoveUnknownCharacters(line);
            lowerLine = CTextUtils::Replace(lowerLine, ',', '.');
            bool hasPhoneIndicator = false;
            bool hasCurrentPhoneIndicator = false;
            string::size_type phoneIndicatorPosition = string::npos;
            vector<string> linePhoneNumbers;

            boost::xpressive::sregex_iterator it(lowerLine.begin(), lowerLine.end(), PhoneRegex);
            boost::xpressive::sregex_iterator end;

            for (; it != end; ++it)
            {
                //Iterate through each phone number candidate on the line
                string currentMatch = CTextUtils::Trim(it->str());

                if (CTextUtils::StringContainsAnyOf(lowerLine, {currentMatch + "@", "@" + currentMatch}))
                {
                    continue;
                }
                if (!hasPhoneIndicator)
                {
                    firstMatch = currentMatch;
                }

                hasPhoneIndicator = true;

                if (prevMatch.length() > 0)
                {
                    halfLine = halfLine.substr(CTextUtils::FindString(halfLine, prevMatch) + prevMatch.length());
                    halfLine = CTextUtils::Trim(halfLine);

                    if (halfLine.find(currentMatch) == 0)
                    {
                        if (totalMatch.find("+") == string::npos || currentMatch.find("+") == string::npos )
                        {
                            totalMatch = totalMatch + (totalMatch.length() > 0 ? " " : "") + currentMatch;
                        }
                    }
                }
                else
                {
                    totalMatch = currentMatch;
                }

                if (resume.GetPhoneNumbers().length() < currentMatch.length())
                {
                    if ( resume.GetPhoneNumbers().length() < 7 ||
                         find(linePhoneNumbers.begin(), linePhoneNumbers.end(), resume.GetPhoneNumbers())==linePhoneNumbers.end() )
                    {
                        resume.SetPhoneNumbers(currentMatch);
                        phoneFound = true;
                    }
                    //resume.SetPhoneNumbers(currentMatch);
                }

                if (resume.GetPhoneNumbers().length() < totalMatch.length())
                {
                    resume.SetPhoneNumbers(totalMatch);
                    phoneFound = true;
                }

                prevMatch = currentMatch;

                if (currentMatch.length() > 5)
                {
                    linePhoneNumbers.push_back(currentMatch);
                }
            }
        }
    }

    return phoneFound;
}

/*
 * Summary_ResumeProcessor_Parsers::CPersonalParser::CPersonalParser
 *
 * Description
 * Extracts gender and saves into resume object. If already found then returns immediately
 *
 * Inputs
 * CResume &resume - reference to the resume object
 * bool genderFound - boolean indicating if gender is already found
 * string line - input string line
 *
 * Returns
 * changed resume object through input parameter resume
 */
bool Summary_ResumeProcessor_Parsers::CPersonalParser::ExtractGender(CResume &resume, bool genderFound, const string &line)
{
    if (genderFound) return genderFound;

    string lowerLine = CTextUtils::ToLower(line);

    if (CTextUtils::StringContainsAnyOf(lowerLine, {"sex", "gender"}))
    {
        genderKeyWordSteps = 0;
    }

    if (genderKeyWordSteps < LINE_COUNT_FROM_KEYWORD_LINE)
    {
        if (CTextUtils::ContainsWithExactStartAndEnding(lowerLine, "female") ||
            CTextUtils::ContainsWithExactStartAndEnding(lowerLine, "feminine") ||
            CTextUtils::ContainsWithExactStartAndEnding(lowerLine, "woman"))
        {
            resume.SetGender("female");
            genderFound = true;
        }

        if (!genderFound &&
                (
                    CTextUtils::ContainsWithExactStartAndEnding(lowerLine, "male") ||
                    CTextUtils::ContainsWithExactStartAndEnding(lowerLine, "masculine") ||
                    CTextUtils::ContainsWithExactStartAndEnding(lowerLine, "man")
                )
            )
        {
            resume.SetGender("male");
            genderFound = true;
        }
    }

    genderKeyWordSteps++;

    return genderFound;
}

/*
 * Summary_ResumeProcessor_Parsers::CPersonalParser::ExtractHeaderFirstName
 *
 * Description
 * Extracts name from data file header and saves into resume object. If first name already found then returns immediately
 *
 * Inputs
 * CResume &resume - reference to the resume object
 * bool firstNameFound - boolean indicating if first name is already found
 * string line - input string line
 *
 * Returns
 * changed resume object through input parameter resume
 */
bool Summary_ResumeProcessor_Parsers::CPersonalParser::ExtractHeaderFirstName(CResume &resume,
                                                                               bool firstNameFound,
                                                                              const string &line)
{
    if (firstNameFound) return firstNameFound;

    boost::xpressive::sregex_token_iterator i(line.begin(), line.end(), SplitByWhiteSpaceRegex, -1);
    boost::xpressive::sregex_token_iterator j;
    SLowerCaseUnorderedSet::const_iterator gotWord, gotWordPart;

    while(i != j)
    {
        string word = *i++;

        if (word.length() < 1)
        {
            continue;
        }

        word = TrimNameCharacters(word);
        CLowerCaseString lowerWord(word);
        gotWord = firstNameLookUp.find (lowerWord);

        if (gotWord != firstNameLookUp.end())
        {
            resume.SetFirstName(word);

            firstNameFound = true;
            break;
        }
        else
        {
            if (CTextUtils::StringContains(word, "-"))
            {
                for (string wordPart: CTextUtils::Split(word, '-'))
                {
                    CLowerCaseString lowerWordPart(wordPart);
                    gotWordPart = firstNameLookUp.find (lowerWordPart);

                    if (gotWordPart != firstNameLookUp.end())
                    {
                        resume.SetFirstName(wordPart);

                        firstNameFound = true;
                        break;
                    }
                }

                if (firstNameFound)
                {
                    break;
                }
            }
        }
    }

    return firstNameFound;
}

/*
 * Summary_ResumeProcessor_Parsers::CPersonalParser::ExtractFirstAndLastName
 *
 * Description
 * Extracts name and saves into resume object. If first name already found then returns immediately
 *
 * Inputs
 * CResume &resume - reference to the resume object
 * string nameSuggestion - suggestion for the first name
 * bool firstNameFound - boolean indicating if first name is already found
 * string line - input string line
 * bool requireLastName - if true then return true in case of last name is found
 *
 * Returns
 * changed resume object through input parameter resume
 */
bool Summary_ResumeProcessor_Parsers::CPersonalParser::ExtractFirstAndLastName(CResume &resume,
                                                                               string nameSuggestion,
                                                                               bool firstNameFound,
                                                                               const string &line,
                                                                               bool requireLastName)
{
    if (firstNameFound) return firstNameFound;

    bool firstWord = true;
    bool firstWordIsHonorific = false;
    string lowerFirstWord = "";
    string lowerLine = CTextUtils::ToLower(line);
    string newLine = line;
    string lowerFirstName = CTextUtils::ToLower(resume.GetFirstName());
    string lowerNameSuggestion = CTextUtils::ToLower(nameSuggestion);
    size_t nameSuggestionIndex = string::npos;

    if (CTextUtils::OnlyAlphanumericString(line).length() > 100 /*|| any_of(line.begin(), line.end(), ::isdigit)*/)
    {
        if (!resume.HasFirstName() || !CTextUtils::StringContains(lowerLine, lowerFirstName))
        {
            return firstNameFound;
        }
    }

    if (CTextUtils::StringContains(line, "@"))
    {
        if (!CTextUtils::StringContains(lowerLine, " " + lowerFirstName) &&
            !CTextUtils::StringContains(lowerLine, lowerFirstName + " "))
        {
            return firstNameFound;
        }
    }

    if (nameSuggestion.length() > 0)
    {
        if (!CTextUtils::StringContains(TrimNameCharacters(lowerLine), lowerNameSuggestion))
        {
            return firstNameFound;
        }

        nameSuggestionIndex = lowerLine.find(lowerNameSuggestion, 0);
    }

    bool lastNameFound = false;
    bool firstNameFromSuggestionFound = false;
    string firstNameFromSuggestion = "";
    vector<string> lastNameFromSuggestion;

    for (string dropName: dropNames)
    {
        //cout << dropName << endl;
        if (CTextUtils::ContainsWithExactStartAndEnding(lowerLine, dropName))
        {
            size_t index = lowerLine.find(dropName, 0);

            if (index == std::string::npos)
            {
                continue;
            }

            if (nameSuggestion.length() > 0 && index > nameSuggestionIndex)
            {
                newLine = newLine.substr(0, index);
                lowerLine = lowerLine.substr(0, index);
            }
            else
            {
                newLine = newLine.replace(index, dropName.length(), "");
                lowerLine = lowerLine.replace(index, dropName.length(), "");
            }

            //cout << newLine << endl;
        }
    }

    boost::xpressive::sregex_token_iterator i(newLine.begin(), newLine.end(), SplitByWhiteSpaceRegex, -1);
    boost::xpressive::sregex_token_iterator j;
    SLowerCaseUnorderedSet::const_iterator gotWord = firstNameLookUp.end();

    while(i != j)
    {
        string word = *i++;

        word = TrimNameCharacters(word);
        CLowerCaseString lowerWord(word);

        if (word.length() < 1)
        {
            continue;
        }

        if (firstWord)
        {
            lowerFirstWord = CTextUtils::ToLower(word);
            lowerFirstWord = CTextUtils::RemoveUnknownCharacters(lowerFirstWord);
            firstWordIsHonorific = CTextUtils::StringIsAnyOf(lowerFirstWord, namePrefixes);
        }

        if (!firstNameFound)
        {
            gotWord = firstNameLookUp.find (lowerWord);
        }
        else
        {
            //resume.AddLastName(CTextUtils::OnlyAlphanumericStringWithSpaces(word));
            bool addLastName = ValidateLastName(word);

            if (addLastName)
            {
                resume.AddLastName(word);
                lastNameFound = true;
            }
            else
            {
                break;
            }
        }

        //Name suggestion is one of names
        if (nameSuggestion.length() > 0)
        {
            //And first name suggestion found inside text
            if (firstNameFromSuggestionFound)
            {
                bool addLastName = ValidateLastName(word);

                if (addLastName)
                {
                    lastNameFromSuggestion.push_back(word);
                    lastNameFound = true;
                }
                else
                {
                    break;  //If at least one of names if not qualified then interrupt the cycle
                }
            }

            //First name suggestion yet not found but word is first name
            if (!firstNameFromSuggestionFound && gotWord != firstNameLookUp.end())
            {
                firstNameFromSuggestion = word;
                firstNameFromSuggestionFound = true;
            }

            //First name yest not found but word equals to name suggestion
            if (!firstNameFound && (CTextUtils::ToLower(word).compare(CTextUtils::ToLower(nameSuggestion)) == 0))
            {
                resume.SetFirstName(word);

                firstNameFound = true;
            }
        }
        else
        {
            if (!firstNameFound && !firstWord &&!firstWordIsHonorific)
            {
                break;
            }
            if (!firstNameFound && gotWord != firstNameLookUp.end())
            {
                string existentNameKey = "";
                for (string nameKey: nameKeysVector)
                {
                    if (CTextUtils::StringContains(lowerLine, nameKey))
                    {
                        existentNameKey = nameKey;
                        break;
                    }
                }

                //If have not any keyword that says the line has surname or the keyword is after first name
                if (existentNameKey.length() < 1 ||
                    CTextUtils::FindString(lowerLine, existentNameKey) > CTextUtils::FindStringIgnoreCase(lowerLine, word))
                {
                    resume.SetFirstName(word);

                    if (firstWordIsHonorific)
                    {
                        resume.SetTitle(lowerFirstWord);
                    }

                    firstNameFound = true;
                }
            }
        }

        firstWord = false;
    }

    if (nameSuggestion.length() > 0 &&
        firstNameFromSuggestion.length() > 0 &&
        (CTextUtils::ToLower(resume.GetFirstName()).compare(CTextUtils::ToLower(firstNameFromSuggestion)) != 0))
    {
        //First name was not a suggested first name
        resume.SetFirstName(firstNameFromSuggestion);
        resume.SetLastName("");

        for (string lastName: lastNameFromSuggestion)
        {
            resume.AddLastName(lastName);
        }
    }

    return requireLastName ? lastNameFound : firstNameFound;
}

/*
 * Summary_ResumeProcessor_Parsers::CPersonalParser::ExtractLastName
 *
 * Description
 * Extracts last name based on existing first name
 *
 * Inputs
 * CResume &resume - reference to the resume object
 * bool lastNameFound - boolean indicating if last name is already found
 * const string &line - input string line
 * const string &lowerFirstName - first name in lower case
 *
 * Returns
 * changed resume object through input parameter resume
 */
bool Summary_ResumeProcessor_Parsers::CPersonalParser::ExtractLastName(CResume &resume, bool lastNameFound,
                                                                       const string &line, const string &lowerFirstName)
{
    if (lastNameFound) return lastNameFound;

    if (lowerFirstName.length() < 1)
    {
        return lastNameFound;
    }

    if (resume.HasMiddleName() || resume.HasLastName())
    {
        return lastNameFound;
    }

    //string lowerFirstName = CTextUtils::ToLower(resume.GetFirstName());
    string lowerLine = CTextUtils::ToLower(line);

    if (CTextUtils::StartsWith(lowerLine, lowerFirstName) &&
        CTextUtils::ContainsWithExactEnding(lowerLine, lowerFirstName) &&
        lowerLine.length() > lowerFirstName.length())
    {
        string lastName = line.substr(lowerFirstName.length());

        lastNameFound = AddLastNames(resume, lastName);
    }

    return lastNameFound;
}

/*
 * Summary_ResumeProcessor_Parsers::CPersonalParser::ExtractLastNameAfterKeyword
 *
 * Description
 * Extracts last name after last name keywords ("surname", "last name" etc.)
 *
 * Inputs
 * CResume &resume - reference to the resume object
 * bool lastNameFound - boolean indicating if last name is already found
 * const string &line - input string line
 *
 * Returns
 * changed resume object through input parameter resume
 */
bool Summary_ResumeProcessor_Parsers::CPersonalParser::ExtractLastNameAfterKeyword(CResume &resume,
                                                                                   bool lastNameFound,
                                                                                   const string &line)
{
    if (lastNameFound) return lastNameFound;

    if (resume.HasMiddleName() || resume.HasLastName())
    {
        return lastNameFound;
    }

    //string lowerFirstName = CTextUtils::ToLower(resume.GetFirstName());
    string lowerLine = CTextUtils::ToLower(line);
    lowerLine = TrimNameCharacters(CTextUtils::RemoveUnknownCharacters(TrimNameCharacters(lowerLine)));

    for (string lastNameKey: nameKeysVector)
    {
        if (CTextUtils::StartsWith(lowerLine, lastNameKey) &&
            CTextUtils::ContainsWithExactEnding(lowerLine, lastNameKey))
        {
            lastNameKeyWordSteps = 0;
            string lastName = line.substr(lastNameKey.length());

            lastNameFound = AddLastNames(resume, lastName);
        }
    }

    if (!lastNameFound && lastNameKeyWordSteps > 0 && lastNameKeyWordSteps < 2 && line.length() > 0)
    {
        lastNameFound = AddLastNames(resume, line);
    }


    if (line.length() > 0)
    {
        lastNameKeyWordSteps++;
    }

    return lastNameFound;
}

/*
 * Summary_ResumeProcessor_Parsers::CPersonalParser::ExtractAndOverWriteName
 *
 * Description
 * Extracts first & last name and overwrites all names
 *
 * Inputs
 * CResume &resume - reference to the resume object
 * bool lastNameFound - boolean indicating if last name is already found
 * const string &line - input string line
 *
 * Returns
 * changed resume object through input parameter resume
 */
bool Summary_ResumeProcessor_Parsers::CPersonalParser::ExtractAndOverWriteName(CResume &resume,
                                                                               bool lastNameFound,
                                                                               const string &line)
{
    if (lastNameFound) return lastNameFound;

    //Will not overwrite first name with existing middle or last name
    if (resume.HasMiddleName() || resume.HasLastName())
    {
        return lastNameFound;
    }

    bool firstWordIsHonorific = false;
    bool isFirstWord = true;
    string::size_type commaPosition = string::npos;
    int wordsAfterComma = 0;
    string lowerFirstWord = "";
    string firstNameSaved = "";
    string lastNameSaved = "";

    string lowerLine = CTextUtils::ToLower(line);
    lowerLine = TrimNameCharacters(lowerLine);

    for (string fullNameKey: fullNameKeysVector)
    {
        if (CTextUtils::StartsWith(lowerLine, fullNameKey) &&
            CTextUtils::ContainsWithExactEnding(lowerLine, fullNameKey) &&
            lowerLine.length() > fullNameKey.length())
        {
            string localLine = CTextUtils::RemoveUnusedSpaces(line);
            string localLowerLine = CTextUtils::ToLower(localLine);

            for (string dropName: dropNames)
            {
                //cout << dropName << endl;
                if (CTextUtils::ContainsWithExactStartAndEnding(localLowerLine, dropName))
                {
                    size_t index = localLowerLine.find(dropName, 0);

                    if (index == std::string::npos)
                    {
                        continue;
                    }

                    if (fullNameKey.compare(dropName) != 0 && index > fullNameKey.length() + 1)
                    {
                        localLine = localLine.substr(0, index);
                        localLowerLine = localLowerLine.substr(0, index);
                    }
                }
            }

            string lastName = localLine.substr(fullNameKey.length());
            commaPosition = lastName.find(",");
            wordsAfterComma = 0;

            //Now it continues with all possible last names
            lastName = TrimNameCharacters(lastName);

            if (lastName.length() > 0)
            {
                //Read last name word by word to avoid too many spaces
                if (lastName.find(" ") != string::npos)
                {
                    for (string lastNamePart: CTextUtils::Split(lastName, ' '))
                    {
                        lastNamePart = TrimNameCharacters(lastNamePart);
                        string lowerLastNamePart = CTextUtils::ToLower(lastNamePart);

                        if (lastNamePart.length() < 1)
                        {
                            continue;
                        }

                        if (commaPosition != string::npos && lastName.find(lastNamePart) > commaPosition)
                        {
                            wordsAfterComma++;
                        }

                        if (isFirstWord)
                        {
                            firstWordIsHonorific = CTextUtils::StringIsAnyOf(lowerLastNamePart, namePrefixes);
                            lowerFirstWord = lowerLastNamePart;
                        }

                        if (!isFirstWord || !firstWordIsHonorific)
                        {
                            if (!ValidateLastName(lastNamePart))
                            {
                                break;
                            }
                        }

                        if (firstNameSaved.length() < 1)
                        {
                            if (!firstWordIsHonorific || !isFirstWord)
                            {
                                firstNameSaved = lastNamePart;

                                if (firstWordIsHonorific)
                                {
                                    resume.SetTitle(lowerFirstWord);
                                }
                            }
                        }
                        else
                        {
                            if (lastNameSaved.length() < 30)
                            {
                                lastNameSaved = lastNameSaved + (lastNameSaved.length() > 0 ? " " : "") + lastNamePart;
                            }
                        }

                        lastNameFound = true;
                        isFirstWord = false;
                        //resume.AddLastName(lastNamePart);
                    }
                }
            }

            if (lastNameFound)
            {
                break;
            }
        }
    }

    if (firstNameSaved.length() > 0 && lastNameSaved.length() > 0)
    {
        resume.SetFirstName(firstNameSaved);

        resume.SetLastName(lastNameSaved);

        if (wordsAfterComma > 0 && wordsAfterComma < 2)
        {
            RearrangeAllNames(resume);
            ExchangeFirstAndLastName(resume);
        }
    }

    return lastNameFound;
}

/*
 * Summary_ResumeProcessor_Parsers::CPersonalParser::ExtractAndRepairName
 *
 * Description
 * Extracts full name and saves into resume object. If last name already found then returns immediately.
 * Tries to correct already found names if in text there are possibilities for that
 *
 * Inputs
 * CResume &resume - reference to the resume object
 * bool firstNameFound - boolean indicating if first name is already found
 * string line - input string line
 *
 * Returns
 * changed resume object through input parameter resume
 */
bool Summary_ResumeProcessor_Parsers::CPersonalParser::ExtractAndRepairName(CResume &resume,
                                                                            bool lastNameFound,
                                                                            const string &line)
{
    if (lastNameFound) return lastNameFound;

    bool foundAnyFullNameKey = false;
    string originalLine = line;
    //line = CTextUtils::RemoveUnknownCharacters(line);
    string lowerOriginalLine = CTextUtils::ToLower(originalLine);

    //Text contains a word that ends with "name"
    if (CTextUtils::ContainsWithExactEnding(lowerOriginalLine, "name"))
    {
        string cleanedOriginalLine = CTextUtils::LTrimNonText(originalLine);
        string lowerCleanedOriginalLine = CTextUtils::ToLower(cleanedOriginalLine);

        for (string fullNameKey: fullNameKeysVector)
        {
            //String starts with name keyword
            if (CTextUtils::FindString(lowerCleanedOriginalLine, fullNameKey) == 0)
            {
                nameKeyFound = true;
                foundAnyFullNameKey = true;
                nameLineCount = 0;

                //We have more text besides first name letters
                if (cleanedOriginalLine.length() > fullNameKey.length())
                {
                    cleanedOriginalLine = cleanedOriginalLine.substr(fullNameKey.length());

                    cleanedOriginalLine = TrimNameCharacters(cleanedOriginalLine);

                    if (
                            cleanedOriginalLine.length() > 0 &&
                            (
                                    !resume.HasFirstName() ||
                                    //first name is in start or end of text or not a part of other words
                                    CTextUtils::ContainsWithExactStartAndEnding(lowerOriginalLine,
                                                                                CTextUtils::ToLower(resume.GetFirstName()))
                            )
                        )
                    {
                        lastNameFound = ExtractAndRepairNameHelper(resume, lastNameFound, cleanedOriginalLine);
                    }
                }

                break;
            }
        }

        //No last name
        if (!foundAnyFullNameKey && !resume.HasLastName())
        {
            //Go through possible words or word combinations that indicate the presence of last name
            for (string lastNameKey: nameKeysVector)
            {
                string::size_type lastNameKeyPosition = CTextUtils::FindString(CTextUtils::LTrimNonText(lowerOriginalLine),
                                                                               lastNameKey);
                //That word combination was in the beginnging of line with some unneeded characters removed
                if (lastNameKeyPosition == 0)
                {
                    string::size_type lastNameKeyPosition = CTextUtils::FindString(lowerOriginalLine, lastNameKey);

                    //There's room for other words after key word(s)
                    if (lastNameKeyPosition + lastNameKey.length() < originalLine.length())
                    {
                        //Read last name text
                        string lastName = originalLine.substr(lastNameKeyPosition + lastNameKey.length());

                        //Now it continues with all possible last names
                        lastName = TrimNameCharacters(lastName);
                        string lowerLastName = CTextUtils::ToLower(lastName);

                        if (lastName.length() > 0)
                        {
                            if (
                                    CTextUtils::FindString(lowerLastName, "name") == string::npos &&
                                    CTextUtils::FindString(lowerLastName, "first") == string::npos)
                            {
                                lastNameFound = AddLastNames(resume, lastName);

                                if (lastNameFound)
                                {
                                    nameLineCount = 5;
                                    break;
                                }
                            }
                            else
                            {
                                //Read last name word by word to avoid too many spaces
                                if (lastName.find(" ") != string::npos)
                                {
                                    bool validNameFound = false;
                                    for (string lastNamePart: CTextUtils::Split(lastName, ' '))
                                    {
                                        lastNamePart = TrimNameCharacters(lastNamePart);

                                        if (lastNamePart.length() < 1)
                                        {
                                            continue;
                                        }

                                        string lowerLastNamePart = CTextUtils::ToLower(lastNamePart);

                                        if (lowerLastNamePart.compare("name") == 0 ||
                                            lowerLastNamePart.compare("first") == 0)
                                        {
                                            break;
                                        }

                                        resume.AddLastName(lastNamePart);
                                        validNameFound = true;
                                    }

                                    if (validNameFound)
                                    {
                                        nameLineCount = 5;

                                        lastNameFound = true;
                                        break;
                                    }
                                }
                                else
                                {
                                    //Only one word that is of those forbidden ones
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    else
    {
        //Word "name" was found in previous lines and don't have last name
        if (nameKeyFound && !resume.HasLastName() && nameLineCount < LINE_COUNT_FROM_KEYWORD_LINE)
        {
            nameLineCount++;

            if (
                    //first name is in start or end of text or not a part of other words
                    resume.HasFirstName() &&
                    CTextUtils::ContainsWithExactStartAndEnding(lowerOriginalLine,
                                                                CTextUtils::ToLower(resume.GetFirstName()))
               )
            {
                lastNameFound = ExtractAndRepairNameHelper(resume, lastNameFound, originalLine);
            }
        }
    }

    return lastNameFound;
}

/*
 * Summary_ResumeProcessor_Parsers::CPersonalParser::ExtractAndRepairFullName
 *
 * Description
 * Extracts full name and saves into resume object. If full name already found then returns immediately.
 * Tries to correct already found names if in text there are possibilities for that
 *
 * Inputs
 * CResume &resume - reference to the resume object
 * bool fullNameFound - boolean indicating if full name is already found
 * string line - input string line
 *
 * Returns
 * changed resume object through input parameter resume. Returns true if full name was found
 */
bool Summary_ResumeProcessor_Parsers::CPersonalParser::ExtractAndRepairFullName(CResume &resume,
                                                                                bool fullNameFound,
                                                                                const string &line)
{
    if (fullNameFound) return fullNameFound;

    string originalLine = line;
    string lowerOriginalLine = CTextUtils::ToLower(originalLine);

    //Go through possible words or word combinations that indicate the presence of full name
    for (string fullNameKey: fullNameKeysVector)
    {
        string::size_type lastNameKeyPosition = CTextUtils::FindString(CTextUtils::LTrimNonText(lowerOriginalLine),
                                                                       fullNameKey);
        bool isOnlyName = fullNameKey.compare("name") == 0;

        if ((lastNameKeyPosition != string::npos && !isOnlyName) || (lastNameKeyPosition == 0 && isOnlyName))
        {
            string::size_type lastNameKeyPosition = CTextUtils::FindString(lowerOriginalLine, fullNameKey);

            if (lastNameKeyPosition + fullNameKey.length() < originalLine.length())
            {
                string fullName = originalLine.substr(lastNameKeyPosition + fullNameKey.length());

                fullName = TrimNameCharacters(fullName);
                string fullNameLower = CTextUtils::ToLower(fullName);

                if (!CTextUtils::StringContains(fullNameLower, "name") &&
                    !CTextUtils::StringContains(fullNameLower, "first") &&
                    !CTextUtils::StringContains(fullNameLower, "last"))
                {
                    if (
                            (
                                resume.HasFirstName() &&
                                resume.HasLastName() &&
                                CTextUtils::StringContains(fullNameLower, CTextUtils::ToLower(resume.GetFirstName())) &&
                                CTextUtils::StringContains(fullNameLower, CTextUtils::ToLower(resume.GetLastName()))
                            ) ||
                            !resume.HasFirstName() ||
                            CTextUtils::StringContains(fullNameLower, CTextUtils::ToLower(resume.GetFirstName()))
                        )
                    {
                        resume.SetLastName("");
                        fullNameFound = ExtractAndRepairNameHelper(resume, fullNameFound, fullName);

                        break;
                    }
                }
            }
        }
    }

    return fullNameFound;
}

/*
 * Summary_ResumeProcessor_Parsers::CPersonalParser::ExtractAndRepairNameHelper
 *
 * Description
 * Extracts full name and saves into resume object. If last name already found then returns immediately.
 * Tries to correct read names from given input
 *
 * Inputs
 * CResume &resume - reference to the resume object
 * bool lastNameFound - boolean indicating if last name is already found
 * string cleanedOriginalLine - input string line, cleaned from unwanted text from the beginning
 *
 * Returns
 * changed resume first and last name. Returns true if last name was found
 */
bool Summary_ResumeProcessor_Parsers::CPersonalParser::ExtractAndRepairNameHelper(CResume &resume,
                                                                                  bool lastNameFound,
                                                                                  string cleanedOriginalLine)
{
    if (lastNameFound) return lastNameFound;

    bool firstWordIsHonorific;
    string firstNameSaved = "";
    string lastNameSaved = "";
    string lowerFirstWord = "";
    string originalCleanedLine = cleanedOriginalLine;
    cleanedOriginalLine = TrimNameCharacters(cleanedOriginalLine);

    //Split line into name words if length > 0
    if (cleanedOriginalLine.length() > 0)
    {
        vector<string> personNames = CTextUtils::Split(cleanedOriginalLine, ' ');
        string::size_type commaPosition = originalCleanedLine.find(",");
        bool isFirstName = true;
        bool willAddName = true;
        int wordsAfterComma = 0;

        for (string personName: personNames)
        {
            personName = TrimNameCharacters(personName);
            string lowerPersonName = CTextUtils::ToLower(personName);

            if (personName.length() > 0)
            {
                if (isFirstName)
                {
                    firstWordIsHonorific = CTextUtils::StringIsAnyOf(lowerPersonName, namePrefixes);
                    lowerFirstWord = lowerPersonName;

                    if (firstWordIsHonorific)
                    {
                        resume.SetTitle(lowerFirstWord);
                    }
                }

                if (!isFirstName || !firstWordIsHonorific)
                {
                    if (!ValidateLastName(personName))
                    {
                        break;
                    }
                }

                if (commaPosition != string::npos && originalCleanedLine.find(personName) > commaPosition)
                {
                    wordsAfterComma++;
                }

                /*if (isFirstName)
                {
                    resume.SetFirstName(personName);
                    nameLineCount = 5;
                }*/
                if (firstNameSaved.length() < 1)
                {
                    if (!firstWordIsHonorific || !isFirstName)
                    {
                        firstNameSaved = personName;
                        resume.SetFirstName(personName);
                        nameLineCount = 5;
                        if (firstWordIsHonorific)
                        {
                            resume.SetTitle(lowerFirstWord);
                        }
                    }
                }
                else
                {
                    resume.AddLastName(personName);
                    lastNameFound = true;
                    lastNameSaved = personName;
                }

                isFirstName = false;
            }
        }
        //Last name was after comma and only one last name
        if (wordsAfterComma > 0 && wordsAfterComma < 2)
        {
            RearrangeAllNames(resume);
            ExchangeFirstAndLastName(resume);
        }

        return lastNameFound;
    }

    return false;
}

/*
 * Summary_ResumeProcessor_Parsers::CPersonalParser::ExtractEmail
 *
 * Description
 * Extracts e-mail and saves into resume object. If already found then returns immediately
 *
 * Inputs
 * CResume &resume - reference to the resume object
 * bool emailFound - boolean indicating if e-mail is already found
 * string line - input string line
 *
 * Returns
 * changed resume object through input parameter resume
 */
bool Summary_ResumeProcessor_Parsers::CPersonalParser::ExtractEmail(CResume &resume, bool emailFound, const string &line)
{
    if (emailFound) return emailFound;

    boost::xpressive::sregex_iterator it(line.begin(), line.end(), EmailRegex);
    boost::xpressive::sregex_iterator end;
    for (; it != end; ++it)
    {
        resume.SetEmailAddress(it->str());
        emailFound = true;
        break;
    }

    return emailFound;
}

/*
 * Summary_ResumeProcessor_Parsers::CPersonalParser::ExtractBirthDay
 *
 * Description
 * Extracts birthday and saves into resume object. If already found then returns immediately
 *
 * Inputs
 * CResume &resume - reference to the resume object
 * bool birthDayFound - boolean indicating if birthday is already found
 * string line - input string line
 * string controlSentence - the word or words after which the birthday search begins
 *
 * Returns
 * changed resume object through input parameter resume
 */
bool Summary_ResumeProcessor_Parsers::CPersonalParser::ExtractBirthDay(CResume &resume,
                                                                       bool birthDayFound,
                                                                       const string &line,
                                                                       string controlSentence)
{
    if (birthDayFound) return birthDayFound;

    if (!birthDayStringFound)
    {
        birthDayStringFound = CTextUtils::StringContainsIgnoreCase(line, controlSentence);
    }

    if (birthDayStringFound &&
        find_if(line.begin(), line.end(), ::isdigit) != line.end())
    {
        if (!birthDayFound)
        {
            boost::xpressive::sregex_iterator it(line.begin(), line.end(), BirthDayRegex);
            boost::xpressive::sregex_iterator end;
            for (; it != end; ++it)
            {
                resume.SetBirthDay(it->str());

                birthDayFound = true;
                break;
            }
        }
    }
    return birthDayFound;
}

/*
 * Summary_ResumeProcessor_Parsers::CPersonalParser::ExtractReligion
 *
 * Description
 * Extracts religion and saves into resume object. If already found then returns immediately
 *
 * Inputs
 * CResume &resume - reference to the resume object
 * bool religionFound - boolean indicating if religion is already found
 * string line - input string line
 *
 * Returns
 * changed resume object through input parameter resume
 */
bool Summary_ResumeProcessor_Parsers::CPersonalParser::ExtractReligion(CResume &resume,
                                                                       bool religionFound,
                                                                       const string &line)
{
    if (religionFound) return religionFound;

    string lowerLine = CTextUtils::ToLower(line);

    if (!religionStringFound)
    {
        religionStringFound = CTextUtils::StringContainsAnyOf(lowerLine, {"religion", "faith"});
    }

    if (religionStringFound)
    {
        religionKeyWordSteps++;
    }

    if (CTextUtils::StringContainsAnyOf(lowerLine, {"religion", "faith"}))
    {
        religionKeyWordSteps = 0;
    }

    if (religionKeyWordSteps > LINE_COUNT_FROM_KEYWORD_LINE)
    {
        religionStringFound = CTextUtils::StringContainsAnyOf(lowerLine, {"religion", "faith"});
    }

    string religion = "";
    int maxCycles = 0;

    if (religionStringFound)
    {
        for (string s : religionLookUp )
        {
            if (CTextUtils::ContainsWithExactStart(lowerLine, CTextUtils::ToLower(s)))
            {
                religion = s;
                break;
            }

            maxCycles++;

            if (maxCycles > 64)
            {
                //break;
            }
        }
    }

    if (religion.length() > 0)
    {
        resume.SetReligion(religion);
        religionFound = true;
    }

    return religionFound;
}

/*
 * Summary_ResumeProcessor_Parsers::CPersonalParser::ExtractSkype
 *
 * Description
 * Extracts skype and saves into resume object. If already found then returns immediately
 *
 * Inputs
 * CResume &resume - reference to the resume object
 * bool skypeFound - boolean indicating if skype is already found
 * string line - input string line
 *
 * Returns
 * changed resume object through input parameter resume
 */
bool Summary_ResumeProcessor_Parsers::CPersonalParser::ExtractSkype(CResume &resume, bool skypeFound, const string &line)
{
    if (skypeFound) return skypeFound;

    string skype = "";
    string localLine = line;

    string::size_type skypeIndicatorPosition = CTextUtils::FindStringIgnoreCase(line, "skype");

    if (skypeIndicatorPosition != string::npos)
    {
        if (skypeIndicatorPosition + 5 < line.length())
        {
            localLine = localLine.substr(skypeIndicatorPosition + 5);
        }

        boost::xpressive::sregex_iterator it(localLine.begin(), localLine.end(), SkypeRegex);
        boost::xpressive::sregex_iterator end;
        for (; it != end; ++it)
        {
            skype = it->str();
        }
    }

    if (skype.length() > 0)
    {
        resume.SetSkype(skype);
        skypeFound = true;
    }

    return false;
}

/*
 * Summary_ResumeProcessor_Parsers::CPersonalParser::ExtractWebSite
 *
 * Description
 * Extracts website and saves into resume object. If already found then returns immediately
 *
 * Inputs
 * CResume &resume - reference to the resume object
 * bool skypeFound - boolean indicating if skype is already found
 * string line - input string line
 *
 * Returns
 * changed resume object through input parameter resume
 */
bool Summary_ResumeProcessor_Parsers::CPersonalParser::ExtractWebSite(CResume &resume, bool webSiteFound, const string &line)
{
    if (webSiteFound) return webSiteFound;
    //line = EXAMPLE_LINE;

    boost::xpressive::sregex_iterator it(line.begin(), line.end(), UrlRegex);
    boost::xpressive::sregex_iterator end;

    for (; it != end; ++it)
    {
        string url = it->str();

        if (UrlContainsValidDomain(line, url))
        {
            if (CTextUtils::StringContainsAnyOf(line, siteKeywords))
            {
                webSiteFound = true;

                resume.SetWebsite(url);
                return webSiteFound;
            }
        }
    }

    return false;
}

/*
 * Summary_ResumeProcessor_Parsers::CPersonalParser::TrimNameCharacters
 *
 * Description
 * Removes unwanted characters from the text and trims from space, -, nonb space and macron
 *
 * Inputs
 * const string &name - input name text
 *
 * Returns
 * changed text string
 */
string Summary_ResumeProcessor_Parsers::CPersonalParser::TrimNameCharacters(const string &input)
{
    return Summary_Common::CTextUtils::TrimNameCharacters(input);
}

/*
 * Summary_ResumeProcessor_Parsers::CPersonalParser::ValidateLastName
 *
 * Description
 * Validates if given name is not in unwanted names list
 *
 * Inputs
 * const string &name - input name text
 *
 * Returns
 * true if name is valid
 */
bool Summary_ResumeProcessor_Parsers::CPersonalParser::ValidateLastName(const string &name)
{
    bool validateLastName = true;

    if (CTextUtils::StringContains(name, "@"))
    {
        return false;
    }

    string pureName = CTextUtils::TrimAnyChar(name, "0123456789 &:+;./()[]{}\\\t\"'^?>=<+*&%$#!_-");

    if (pureName.length() == 0)
    {
        return false;
    }

    for (string nonName: nonNamesLookup)
    {
        if (CTextUtils::ToLower(name).compare(nonName) == 0 || CTextUtils::ToLower(pureName).compare(nonName) == 0)
        {
            validateLastName = false;
            break;
        }
    }
    return validateLastName;
}

/*
 * Summary_ResumeProcessor_Parsers::CPersonalParser::UrlContainsValidDomain
 *
 * Description
 * Validates if url contains valid domain
 *
 * Inputs
 * const string &line - line where url was extracted from
 * const string &url - url
 *
 * Returns
 * true if url is valid
 */
bool Summary_ResumeProcessor_Parsers::CPersonalParser::UrlContainsValidDomain(const string &line, const string &url)
{
    if (CTextUtils::StringContains(url, "@") ||
        CTextUtils::StringContainsAnyOf(line, {url + "@", "@" + url}))
    {
        return false;
    }
    boost::xpressive::sregex_iterator it(url.begin(), url.end(), DomainNameRegex);
    boost::xpressive::sregex_iterator end;
    struct tld_info info;
    enum tld_result result;

    if (it != end)
    {
        result = tld_check_uri(url.c_str(), &info, PROTOCOLS, 0);

        if (result != TLD_RESULT_SUCCESS)
        {
            return false;
        }

        return true;
    }

    return false;
}

/*
 * Summary_ResumeProcessor_Parsers::CPersonalParser::PostProcessData
 *
 * Description
 * Makes final modifications to names
 *
 * Inputs
 * CResume &resume - reference to the resume object
 *
 * Returns
 * changed resume object through input parameter resume
 */
void Summary_ResumeProcessor_Parsers::CPersonalParser::PostProcessData(CResume &resume)
{
    resume.SetFirstName(CTextUtils::OnlyFirstLetterUpCase(TrimNameCharacters(resume.GetFirstName())));
    resume.SetLastName(CTextUtils::OnlyFirstLetterUpCase(TrimNameCharacters(resume.GetLastName())));

    if (resume.HasFirstName())
    {
        string lowerFirstName = CTextUtils::ToLower(resume.GetFirstName());

        for (string namePrefix: namePrefixes)
        {
            if (lowerFirstName.compare(namePrefix) == 0)
            {
                resume.SetFirstName("");
                break;
            }
        }
    }

    RearrangeAllNames(resume);

    if (resume.HasFirstName() && !resume.HasMiddleName() && !resume.HasLastName() && resume.GetTitle().length() > 0)
    {
        resume.SetLastName(resume.GetFirstName());
        resume.SetFirstName("");
    }

    if (resume.GetPhoneNumbers().length() > 0)
    {
        string phoneNumbers = resume.GetPhoneNumbers();
        phoneNumbers = CTextUtils::ReplaceAnyChar(phoneNumbers, "-", ' ');
        phoneNumbers = CTextUtils::ReplaceAnyChar(phoneNumbers, ".", ' ');
        resume.SetPhoneNumbers(CTextUtils::RemoveUnusedSpaces(phoneNumbers));
    }

    if (resume.GetBirthDay().length() > 0)
    {
        int value = atoi(resume.GetBirthDay().c_str());

        if (value > 0)
        {
            boost::posix_time::ptime date_time = boost::posix_time::microsec_clock::universal_time();
            int currentYear = date_time.date().year();

            if (value <= currentYear && currentYear - value < 12)
            {
                resume.SetBirthDay("");
            }
        }
    }
}

/*
 * Summary_ResumeProcessor_Parsers::CPersonalParser::ExtractCountryFromLine
 *
 * Description
 * Extracts country name from the line
 *
 * Inputs
 * const string &lowerLine - line in lower case
 * bool useCode - if true then extracts country name from found country code
 * string keyword - keyword used to determine if country can be found on line
 *
 * Returns
 * changed resume object through input parameter resume
 */
string Summary_ResumeProcessor_Parsers::CPersonalParser::ExtractCountryFromLine(const string &lowerLine,
                                                                                bool useCode,
                                                                                string keyword)
{
    string country = "";

    for (int j = 0; j < countryLookUpLower.size(); j++)
    {
        if (CTextUtils::ContainsWithExactEnding(lowerLine, countryLookUpLower.at(j)))
        {
            country = countryLookUp.at(j);
            break;
        }
    }

    if (useCode && CTextUtils::IsEmptyOrWhiteSpace(country) && !CTextUtils::StringContains(lowerLine, "@"))
    {
        for(auto const &s : countryMap)
        {
            string countryCodeWithDots = CTextUtils::ToLower(CountryCodeWithDots(s.first));
            string countryCode = CTextUtils::ToLower(s.first);

            if (ContainsCountryCode(lowerLine, countryCode) || ContainsCountryCode(lowerLine, countryCodeWithDots))
            {
                bool canAdd = true;
                if (countryKeyWordSteps < 1 && keyword.length() > 0)
                {
                    string::size_type keywordPosition = CTextUtils::FindStringIgnoreCase(lowerLine, CTextUtils::ToLower(keyword));

                    if (keywordPosition != string::npos)
                    {
                        string::size_type countryCodePosition = CTextUtils::FindStringIgnoreCase(lowerLine, countryCode);

                        if (countryCodePosition != string::npos)
                        {
                            if (countryCodePosition < keywordPosition)
                            {
                                canAdd = false;
                            }
                        }
                        else
                        {
                            countryCodePosition = CTextUtils::FindStringIgnoreCase(lowerLine, countryCodeWithDots);

                            if (countryCodePosition != string::npos && countryCodePosition < keywordPosition)
                            {
                                canAdd = false;
                            }
                        }
                    }
                }

                if (canAdd)
                {
                    country = s.second;
                    //countryLongName = s.second;
                    break;
                }
            }
        }
    }

    return country;
}

/*
 * Summary_ResumeProcessor_Parsers::CPersonalParser::ContainsCountryCode
 *
 * Description
 * Validates of line contains country code
 *
 * Inputs
 * const string &lowerLine - line in lower case
 * const string &countryCode - country code
 *
 * Returns
 * true if line contains country code
 */
bool Summary_ResumeProcessor_Parsers::CPersonalParser::ContainsCountryCode(const string &lowerLine, const string &countryCode)
{
    if (
            (
                //If country code with preceding comma exists
                CTextUtils::StringContainsAnyOf(lowerLine, {", " + countryCode, "," + countryCode}) ||
                //or country code exists on the same line with country word
                (
                    countryKeyWordSteps < 1 &&
                    CTextUtils::ContainsWithExactStart(lowerLine, countryCode)
                )
            )
            &&
            CTextUtils::ContainsWithExactEnding(lowerLine, countryCode)
      )
    {
        return true;
    }

    return false;
}

/*
 * Summary_ResumeProcessor_Parsers::CPersonalParser::CountryCodeWithDots
 *
 * Description
 * Gets country three-letter code separated by dots
 *
 * Inputs
 * const string &code - country code
 *
 * Returns
 * changed country code with dots as separators between letters
 */
string Summary_ResumeProcessor_Parsers::CPersonalParser::CountryCodeWithDots(const string &code)
{
    string returnCode = "";

    for (int i = 0; i < code.length(); i++)
    {
        returnCode += (returnCode.length() > 0 ? "." : "");
        returnCode += code[i];
    }

    return returnCode;
}

/*
 * Summary_ResumeProcessor_Parsers::CPersonalParser::HasPhoneKeyWordIndicator
 *
 * Description
 * Validates if phone number is preceded with the right keyword on this line or on lines before number
 *
 * Inputs
 * const string &match - already extracted phone number
 * const string::size_type &phoneIndicatorPosition - a position of the keyword
 * const string &lowerLine - line in lower case
 * const string &lowerPhoneKeyword - keyword that normally precedes phone number, in lower case
 * bool keyWordAfterAllowed - if true then phone indicator keyword can be after the phone number
 *
 * Returns
 * true if match can be phone number
 */
bool Summary_ResumeProcessor_Parsers::CPersonalParser::HasPhoneKeyWordIndicator(const string &match,
                                                                         const string::size_type &phoneIndicatorPosition,
                                                                         const string &lowerLine,
                                                                         const string &lowerPhoneKeyword,
                                                                         bool keyWordAfterAllowed)
{
    bool hasCurrentPhoneIndicator = false;
    if (
            match.length() > 0 &&
            (
                (phoneKeyWordSteps > 0 && phoneKeyWordSteps < LINE_COUNT_FROM_KEYWORD_LINE) ||
                (
                    phoneIndicatorPosition != string::npos &&
                    (
                        lowerPhoneKeyword.length() == 1 ||
                        CTextUtils::ContainsWithExactStartAndEnding(lowerLine, lowerPhoneKeyword)
                    ) &&
                    (
                        keyWordAfterAllowed ||
                        phoneIndicatorPosition < CTextUtils::FindString(lowerLine, match)
                    )
                )
            )
        )
    {
        hasCurrentPhoneIndicator = true;
    }

    return hasCurrentPhoneIndicator;
}

/*
 * Summary_ResumeProcessor_Parsers::CPersonalParser::HasPhoneIndicator
 *
 * Description
 * Validates if phone number is preceded with the right keyword on this line or on lines before number. Used
 * in cycle when going through multple keywords
 *
 * Inputs
 * const string &match - already extracted phone number
 * const string::size_type &phoneIndicatorPosition - a position of the keyword
 * const string &lowerLine - line in lower case
 * const string &lowerIndicator - keyword that normally precedes phone number, in lower case
 *
 * Returns
 * true if match can be phone number
 */
bool Summary_ResumeProcessor_Parsers::CPersonalParser::HasPhoneIndicator(const string &match,
                                                                         const string::size_type &phoneIndicatorPosition,
                                                                         const string &lowerLine,
                                                                         const string &lowerIndicator)
{
    bool hasPhoneIndicator = false;

    if (
            match.length() > 0 &&
            (
                (phoneKeyWordSteps > 0 && phoneKeyWordSteps < LINE_COUNT_FROM_KEYWORD_LINE) ||
                (
                    phoneIndicatorPosition != string::npos &&
                    CTextUtils::ContainsWithExactStartAndEnding(lowerLine, lowerIndicator) &&
                    phoneIndicatorPosition < CTextUtils::FindString(lowerLine, match)
                )
            )
        )
    {
        hasPhoneIndicator = true;
    }

    return hasPhoneIndicator;
}

/*
 * Summary_ResumeProcessor_Parsers::CPersonalParser::AddLastNames
 *
 * Description
 * Adds last name or last names from space separated string until cannot validate the name
 *
 * Inputs
 * CResume &resume - resume object to be changed
 * string lastName - last name we search names in
 *
 * Returns
 * Changed resume object through input parameter
 */
bool Summary_ResumeProcessor_Parsers::CPersonalParser::AddLastNames(CResume &resume, string lastName)
{
    bool lastNameFound = false;

    //Now it continues with all possible last names
    lastName = TrimNameCharacters(lastName);

    if (lastName.length() > 0)
    {
        //Read last name word by word to avoid too many spaces
        if (lastName.find(" ") != string::npos)
        {
            for (string lastNamePart: CTextUtils::Split(lastName, ' '))
            {
                lastNamePart = TrimNameCharacters(lastNamePart);

                if (lastNamePart.length() < 1)
                {
                    continue;
                }

                if (!ValidateLastName(lastNamePart))
                {
                    break;
                }

                resume.AddLastName(lastNamePart);
                lastNameFound = true;
            }
        }
        else
        {
            if (ValidateLastName(lastName))
            {
                resume.AddLastName(lastName);
                lastNameFound = true;
            }
        }
    }

    return lastNameFound;
}

/*
 * Summary_ResumeProcessor_Parsers::CPersonalParser::ExchangeFirstAndLastName
 *
 * Description
 * If resume has first and last name and cannot find first name inside first names list
 * but the last name can be found inside last names list then exchanges first and last name.
 * Can be used when extracts name in format [LAST_NAME], [FIRST NAME] or [LAST_NAME] [MIDDLE NAME], [FIRST NAME]
 *
 * Inputs
 * CResume &resume - resume object to be changed
 *
 * Returns
 * Changed resume object through input parameter
 */
void Summary_ResumeProcessor_Parsers::CPersonalParser::ExchangeFirstAndLastName(CResume &resume)
{
    //Do final check if first name is in first names list
    if (resume.HasFirstName() && resume.HasLastName())
    {
        CLowerCaseString lowerFirstName(resume.GetFirstName());
        CLowerCaseString lowerLastName(resume.GetLastName());
        SLowerCaseUnorderedSet::const_iterator gotName = firstNameLookUp.end();

        gotName = firstNameLookUp.find (lowerFirstName);

        //Cannot use first name as first name
        if (gotName == firstNameLookUp.end())
        {
            //Now change first name with last name if last name was in first names list
            gotName = firstNameLookUp.find (lowerLastName);

            if (gotName != firstNameLookUp.end())
            {
                string tempFirstName = resume.GetFirstName();
                resume.SetFirstName(resume.GetLastName());
                resume.SetLastName(tempFirstName);
            }
        }
    }
}

/*
 * Summary_ResumeProcessor_Parsers::CPersonalParser::RearrangeAllNames
 *
 * Description
 * Moves some of names from last name field to the first name field in case its emptyness and to the
 * middle name field. Works only on space separated last names so that there are more than
 * one last name
 *
 * Inputs
 * CResume &resume - resume object to be changed
 *
 * Returns
 * Changed resume object through input parameter
 */
void Summary_ResumeProcessor_Parsers::CPersonalParser::RearrangeAllNames(CResume &resume)
{
    if (resume.GetLastName().find(" ") != string::npos)
    {
        vector<string> lastNames = CTextUtils::Split(resume.GetLastName(), ' ');
        string middleNames = "";
        string lastName = "";

        for (int i = 0; i < lastNames.size(); i++)
        {
            if (i < lastNames.size() - 1)
            {
                if (i == 0 && resume.GetFirstName().length() == 0)
                {
                    resume.SetFirstName(lastNames.at(i));
                }
                else
                {
                    middleNames += (middleNames.length() > 0 ? " " : "") + lastNames.at(i);
                }
            }
            else
            {
                lastName = lastNames.at(i);
            }
        }

        resume.SetMiddleName(TrimNameCharacters(middleNames));
        resume.SetLastName(TrimNameCharacters(lastName));
    }
}

void Summary_ResumeProcessor_Parsers::CPersonalParser::PushDegreeAbbreviation(const string &firstLetter,
                                                                              const string &secondLetter,
                                                                              bool concatenateSimply)
{
    if (concatenateSimply)
    {
        degreeAbbbreviations.push_back(firstLetter + secondLetter + " ");
    }
    //
    degreeAbbbreviations.push_back(firstLetter + secondLetter + ")");
    degreeAbbbreviations.push_back(firstLetter + secondLetter + "(");
    degreeAbbbreviations.push_back(firstLetter + secondLetter + ",");
    degreeAbbbreviations.push_back(firstLetter + secondLetter + ".");
    degreeAbbbreviations.push_back(firstLetter + secondLetter + "-");
    degreeAbbbreviations.push_back(firstLetter + secondLetter + ":");

    degreeAbbbreviations.push_back(firstLetter + " " + secondLetter + " ");
    degreeAbbbreviations.push_back(firstLetter + " " + secondLetter + ")");
    degreeAbbbreviations.push_back(firstLetter + " " + secondLetter + "(");
    degreeAbbbreviations.push_back(firstLetter + " " + secondLetter + ",");
    degreeAbbbreviations.push_back(firstLetter + " " + secondLetter + ".");
    degreeAbbbreviations.push_back(firstLetter + " " + secondLetter + "-");
    degreeAbbbreviations.push_back(firstLetter + " " + secondLetter + ":");

    degreeAbbbreviations.push_back(firstLetter + "-" + secondLetter + " ");
    degreeAbbbreviations.push_back(firstLetter + "-" + secondLetter + ")");
    degreeAbbbreviations.push_back(firstLetter + "-" + secondLetter + "(");
    degreeAbbbreviations.push_back(firstLetter + "-" + secondLetter + ",");
    degreeAbbbreviations.push_back(firstLetter + "-" + secondLetter + ".");
    degreeAbbbreviations.push_back(firstLetter + "-" + secondLetter + "-");
    degreeAbbbreviations.push_back(firstLetter + "-" + secondLetter + ":");

    degreeAbbbreviations.push_back(firstLetter + "." + secondLetter + " ");
    degreeAbbbreviations.push_back(firstLetter + "." + secondLetter + ")");
    degreeAbbbreviations.push_back(firstLetter + "." + secondLetter + "(");
    degreeAbbbreviations.push_back(firstLetter + "." + secondLetter + ",");
    degreeAbbbreviations.push_back(firstLetter + "." + secondLetter + ".");
    degreeAbbbreviations.push_back(firstLetter + "." + secondLetter + "-");
    degreeAbbbreviations.push_back(firstLetter + "." + secondLetter + ":");

    degreeAbbbreviations.push_back(firstLetter + ". " + secondLetter + " ");
    degreeAbbbreviations.push_back(firstLetter + ". " + secondLetter + ")");
    degreeAbbbreviations.push_back(firstLetter + ". " + secondLetter + "(");
    degreeAbbbreviations.push_back(firstLetter + ". " + secondLetter + ",");
    degreeAbbbreviations.push_back(firstLetter + ". " + secondLetter + ".");
    degreeAbbbreviations.push_back(firstLetter + ". " + secondLetter + "-");
    degreeAbbbreviations.push_back(firstLetter + ". " + secondLetter + ":");
}

void Summary_ResumeProcessor_Parsers::CPersonalParser::PushDegreeAbbreviation(const string &firstLetter,
                                                                              const string &secondLetter,
                                                                              const string &thirdLetter,
                                                                              bool concatenateSimply)
{
    if (concatenateSimply)
    {
        degreeAbbbreviations.push_back(firstLetter + secondLetter + thirdLetter + " ");
    }
    //
    degreeAbbbreviations.push_back(firstLetter + secondLetter + thirdLetter + ")");
    degreeAbbbreviations.push_back(firstLetter + secondLetter + thirdLetter + ",");
    degreeAbbbreviations.push_back(firstLetter + secondLetter + thirdLetter + ".");
    degreeAbbbreviations.push_back(firstLetter + secondLetter + thirdLetter + "-");
    degreeAbbbreviations.push_back(firstLetter + " " + secondLetter + " " + thirdLetter + " ");
    degreeAbbbreviations.push_back(firstLetter + " " + secondLetter + " " + thirdLetter + ")");
    degreeAbbbreviations.push_back(firstLetter + " " + secondLetter + " " + thirdLetter + ",");
    degreeAbbbreviations.push_back(firstLetter + " " + secondLetter + " " + thirdLetter + ".");
    degreeAbbbreviations.push_back(firstLetter + " " + secondLetter + " " + thirdLetter + "-");
    degreeAbbbreviations.push_back(firstLetter + "." + secondLetter + "." + thirdLetter + " ");
    degreeAbbbreviations.push_back(firstLetter + "." + secondLetter + "." + thirdLetter + ")");
    degreeAbbbreviations.push_back(firstLetter + "." + secondLetter + "." + thirdLetter + ",");
    degreeAbbbreviations.push_back(firstLetter + "." + secondLetter + "." + thirdLetter + ".");
    degreeAbbbreviations.push_back(firstLetter + "." + secondLetter + "." + thirdLetter + "-");
    degreeAbbbreviations.push_back(firstLetter + ". " + secondLetter + ". " + thirdLetter + " ");
    degreeAbbbreviations.push_back(firstLetter + ". " + secondLetter + ". " + thirdLetter + ")");
    degreeAbbbreviations.push_back(firstLetter + ". " + secondLetter + ". " + thirdLetter + ",");
    degreeAbbbreviations.push_back(firstLetter + ". " + secondLetter + ". " + thirdLetter + ".");
    degreeAbbbreviations.push_back(firstLetter + ". " + secondLetter + ". " + thirdLetter + "-");
}

bool Summary_ResumeProcessor_Parsers::CPersonalParser::ExtractEducation(CResume &resume,
                                                                        vector<string> &sectionRawContentLower,
                                                                        const string &educationKeyWord,
                                                                        bool useKeyWord)
{
    bool educationFound = false;
    string educationKeyWordLower = CTextUtils::ToLower(educationKeyWord);
    int lineCount = -1;

    for (string line: sectionRawContentLower)
    {
        bool lineAdded = false;
        string newLine = line;
        lineCount ++;
        line = CTextUtils::TrimAnyChar(line, " &:+;./()[]{}\\\t\"'^?>=<+*&%$#!_-");
        line = line + "   ";

        //If keyword needs to be found then search from the first positions of line
        if (useKeyWord && (CTextUtils::StartsWith(line, educationKeyWordLower) ||
                            CTextUtils::StartsWith(line.substr(1), educationKeyWordLower) ||
                            CTextUtils::StartsWith(line.substr(2), educationKeyWordLower)||
                            CTextUtils::StartsWith(line.substr(3), educationKeyWordLower)))
        {
            educationFound = true;
        }

        if (useKeyWord && !educationFound)
        {
            continue;
        }

        if (LineDistance(degreeLineNumbers, lineCount, true, 2) == 0)
        {
            continue;
        }


        for (string lookDegreeFromBeginning: shortTaggedDegreeLookUpBeginning)
        {
            //This keyword was found in the beginning of the text and serves as a clue that we have further aducational data
            if ((CTextUtils::StartsWith(line, lookDegreeFromBeginning) ||
                 CTextUtils::StartsWith(line.substr(1), lookDegreeFromBeginning) ||
                 CTextUtils::StartsWith(line.substr(2), lookDegreeFromBeginning)||
                 CTextUtils::StartsWith(line.substr(3), lookDegreeFromBeginning)))
            {
                degreeLineNumbers.push_back(lineCount);
            }

            //Further educational data is available right after the keyword itself
            if (((newLine.length() > lookDegreeFromBeginning.length() && CTextUtils::StartsWith(line, lookDegreeFromBeginning)) ||
                 (newLine.length() - 1 > lookDegreeFromBeginning.length() && CTextUtils::StartsWith(line.substr(1), lookDegreeFromBeginning) ) ||
                 (newLine.length() - 2 > lookDegreeFromBeginning.length() && CTextUtils::StartsWith(line.substr(2), lookDegreeFromBeginning))||
                 (newLine.length() - 3 > lookDegreeFromBeginning.length() && CTextUtils::StartsWith(line.substr(3), lookDegreeFromBeginning))))
            {
                resume.AddEducationDegree(lineCount, line);
                lineAdded = true;
                educationFound = true;
                break;
            }
        }


        for (string lookDegreeFromBeginning: shortTaggedDegreeLookUpBeginning)
        {
            //cout << lookDegreeFromBeginning << endl;
            if (((newLine.length() > lookDegreeFromBeginning.length() && CTextUtils::StartsWith(line, lookDegreeFromBeginning)) ||
                        (newLine.length() - 1 > lookDegreeFromBeginning.length() && CTextUtils::StartsWith(line.substr(1), lookDegreeFromBeginning) ) ||
                        (newLine.length() - 2 > lookDegreeFromBeginning.length() && CTextUtils::StartsWith(line.substr(2), lookDegreeFromBeginning))||
                        (newLine.length() - 3 > lookDegreeFromBeginning.length() && CTextUtils::StartsWith(line.substr(3), lookDegreeFromBeginning))))
            {
                resume.AddEducationDegree(lineCount, line);
                degreeLineNumbers.push_back(lineCount);
                lineAdded = true;
                educationFound = true;
                break;
            }
        }

        if (lineAdded)
        {
            continue;
        }

        /*for (string school: schoolLookUp)
        {
            if (CTextUtils::StringContains(newLine, school))
            {
                resume.AddEducationDegree(line);
                degreeLineNumbers.push_back(lineCount);
                lineAdded = true;
                educationFound = true;
                break;
            }
        }

        if (lineAdded)
        {
            continue;
        }*/

        if (line.length() > 0 && line.find(" ") != string::npos)
        {
            newLine = CTextUtils::RemoveUnusedSpaces(line) + " ";
        }

        for (string shortTaggedDegree: shortTaggedDegreeLookUpLower)
        {
            //string lowerDegree = CTextUtils::ToLower(shortTaggedDegree);

            if (CTextUtils::StringContains(newLine, shortTaggedDegree))
            {
                resume.AddEducationDegree(lineCount, line);
                degreeLineNumbers.push_back(lineCount);
                lineAdded = true;
                educationFound = true;
                break;
            }
        }

        if (lineAdded)
        {
            continue;
        }

        for (string fieldOfStudy: fieldsOfStudyLookUpLower)
        {
            //string lowerFieldOfStudy = CTextUtils::ToLower(fieldOfStudy);

            if (CTextUtils::StringContains(newLine, fieldOfStudy))
            {
                for (string degreeAbbreviation: degreeAbbbreviations)
                {
                    if (CTextUtils::ContainsWithExactStart(newLine, degreeAbbreviation))
                    {
                        //string originalLine = line;
                        /*string::size_type firstPosition = newLine.find(fieldOfStudy);
                        string::size_type secondPosition = newLine.find(degreeAbbreviation);
                        string::size_type oldPosition;
                        string::size_type lastPosition;
                        string::size_type lastDotPosition;

                        if (firstPosition > secondPosition)
                        {
                            lastPosition = firstPosition + fieldOfStudy.length();
                            firstPosition = secondPosition;
                        }
                        else
                        {
                            lastPosition = secondPosition + degreeAbbreviation.length();
                        }

                        //cout << lastPosition << endl;
                        oldPosition = firstPosition;

                        secondPosition = newLine.find_last_of(".", firstPosition);
                        lastDotPosition = newLine.find(".", lastPosition);

                        if (secondPosition != string::npos)
                        {
                            firstPosition = secondPosition;
                            secondPosition = newLine.find(" ", firstPosition);

                            if (secondPosition < oldPosition)
                            {
                                firstPosition = secondPosition;
                            }
                        }
                        else
                        {
                            firstPosition = 0;
                        }

                        if (lastDotPosition != string::npos && lastDotPosition > firstPosition)
                        {
                            newLine = CTextUtils::TrimAnyChar(newLine.substr(firstPosition, lastDotPosition - firstPosition), " ;-_:/\\");
                        }
                        else
                        {
                            newLine = CTextUtils::TrimAnyChar(newLine.substr(firstPosition), " ;-_:/\\");
                        }*/


                        resume.AddEducationDegree(lineCount, newLine);
                        degreeLineNumbers.push_back(lineCount);
                        lineAdded = true;
                        educationFound = true;

                        //cout << degreeAbbreviation << endl;

                        break;
                    }
                }

                //Have already this line, check for any other fields of study or degrees inside that line later
                if (lineAdded)
                {
                    break;
                }
            }
        }

        if (lineAdded)
        {
            continue;
        }

        if (CTextUtils::StringContains(newLine, "degree"))
        {
            for (string degreeAbbreviation: degreeAbbbreviations)
            {
                if (CTextUtils::ContainsWithExactStart(newLine, degreeAbbreviation + "degree") ||
                    CTextUtils::ContainsWithExactStart(newLine, degreeAbbreviation + " degree"))
                {
                    resume.AddEducationDegree(lineCount, line);
                    degreeLineNumbers.push_back(lineCount);
                    educationFound = true;
                    lineAdded = true;
                    //cout << degreeAbbreviation << endl;
                    //cout << "========" << endl;
                    break;
                }
            }
        }
    }

    return educationFound;
}

bool Summary_ResumeProcessor_Parsers::CPersonalParser::ExtractSchool(CResume &resume,
                                                                     vector<string> &sectionRawContentLower,
                                                                     const string &educationKeyWord, bool useKeyWord)
{
    bool educationFound = false;
    string educationKeyWordLower = CTextUtils::ToLower(educationKeyWord);
    int lineCount = -1;

    for (string line: sectionRawContentLower)
    {
        bool lineAdded = false;
        line = CTextUtils::TrimAnyChar(line, " &:+;./()[]{}\\\t\"'^?>=<+*&%$#!_-");
        string newLine = line;
        lineCount ++;

        line = line + "   ";
        //If keyword needs to be found then search from the first positions of line
        if (useKeyWord && (CTextUtils::StartsWith(line, educationKeyWordLower) ||
                           CTextUtils::StartsWith(line.substr(1), educationKeyWordLower) ||
                           CTextUtils::StartsWith(line.substr(2), educationKeyWordLower)||
                           CTextUtils::StartsWith(line.substr(3), educationKeyWordLower)))
        {
            educationFound = true;
        }

        if (useKeyWord && !educationFound)
        {
            continue;
        }

        if (LineDistance(degreeLineNumbers, lineCount, false, 2) == 0)
        {
            for (int index: degreeLineNumbers)
            {
                //cout << "+" << index << endl;
            }

            continue;
        }

        for (string school: schoolLookUp)
        {
            if (CTextUtils::StringContains(newLine, school))
            {
                resume.AddEducationDegree(lineCount, line);
                degreeLineNumbers.push_back(lineCount);
                lineAdded = true;
                educationFound = true;
                //cout << school << endl;
                //cout << lineCount << endl;
                break;
            }
        }
    }

    return educationFound;
}

int Summary_ResumeProcessor_Parsers::CPersonalParser::LineDistance(const vector<int> &lineNumbers,
                                                                   int lineIndex,
                                                                   bool positiveOnly,
                                                                   int distance)
{
    int returnValue = 0;
//degreeLineNumbers
    if (lineNumbers.size() < 1)
    {
        return -1;
    }

    for (int index: lineNumbers)
    {
        if (positiveOnly)
        {
            if (lineIndex - index <= distance && lineIndex >= index)
            {
                returnValue = 1;
                break;
            }
        }
        else
        {
            if (abs(lineIndex - index) <= distance)
            {
                returnValue = 1;
                break;
            }
        }
    }

    return returnValue;
}

