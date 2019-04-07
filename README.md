The following utility does extracting of text content from doc/pdf/odf CV/resume, parses it and outputs most significant
fields from the CV/resume.

I Requirements
1. OpenSSL must be installed along with Iconv package
2. For plain text format there are no more requirements.
3. tika-app-1.9.jar must be available in the same folder where CVSummaryExtractor resides in order to read data
from non plain text sources like .doc, .pdf, .docx, .odf. For full list of Apache Tika supported formats please see
https://tika.apache.org/1.9/formats.html
Although these formats are supported but only doc/docx/pdf are tested. Tool will complain if file has any other
file ending as the following: txt,pdf,doc,docx,html,htm,xml,odt,rtf
4. tesseract command line tool should be available for extracting text from images got from input document (.doc/.pdf/etc.)
If tesseract is not built on the system it will only deal with plain text got from input document and does not read
text on the images or scan.

II Usage: CVSummaryExtractor [FILE_NAME] [-extract_image_path PATH]

For example:
CVSummaryExtractor document.pdf
CVSummaryExtractor document.docx
CVSummaryExtractor document.txt
CVSummaryExtractor document.docx -extract_image_path ./temp

In case of .txt files it will only read plain text and outputs the result, other files need to be converted first to
plain text format using Apache Tika.
Converting document to .txt format usually takes 2-3 seconds.
Some documents may be scanned and could consist of scanned image(s). Apache Tika is used to try to extract those
images from the document as attachments. The other command line tool, tesseract will be used on the images
to produce text output and extract it to the end of the same file with textual information. The overall text result
consist of first text got from the actual document plus text got from all images inside the document.

-extract_image_path ./temp
needs folder temp to be created with write permissions before using the tool. It will contain possible face image
candidates if any image existed in document.

Extractor will try to translate given text in [FILE_NAME] to english if it finds another language. Cld library
detects language codes and using detected language code, bing translator api translates text. Cld covers almost all
bing language codes except for the languages Hmong Daw, Klingon, Klingon (pIqaD), Querétaro Otomi, Yucatec Maya [10.2015]
Bing has codes for languages Serbian (Latin) (sr-Latn) and Serbian (Cyrillic) (sr-Cyrl) but Cld has only Serbian (sr)
and Serbian is not yet supported.
For the complete list if Bing languages see https://msdn.microsoft.com/en-us/library/hh456380.aspx

For changing translator API client in case of generating new account see http://blogs.msdn.com/b/translation/p/gettingstarted1.aspx
on how to register new client with secret. These constants need to be changed manually in code.

API limits:
2 M characters / month for free
500 K characters / hour (2012)
Characters balance can be viewed from https://datamarket.azure.com/account/datasets
Source:
https://social.microsoft.com/Forums/en-US/abf2a48f-d8c7-41db-a1fa-00066e7040f4/limits-in-request-to-bing-translator-api?forum=translator

Characters count will increase only on real url-decoded characters.

III Output format:

{
	"first_name":"",
	"middle_name":"",
	"last_name":"",
	"birthday":"",
	"gender":"",
	"email":"",
	"phone":"",
	"skype":"",
	"website_url":"",
	"facebook_url":"",
	"twitter_url":"",
	"googleplus_url":"",
	"linkedin_url":"",
	"instagram_url":"",
	"social-profiles":[],
	"url_candidates":[],
	"title":"",
	"religion":"",
	"languages":"",
	"text_language_code":"",
	"text_language_name":"",
	"description":"",
	"skills":[],
	"words":[],
	"country":"",
	"location":"",
	"positions":[],
	"projects":[],
	"educations":[],
	"courses":[],
	"awards":[],
	"input":[],
	"translation":[]
}

Everything else except for input array is extracted from the text. input contains plain text of the resume itself.
Fields skills, words, social-profiles, courses, awards and input are text arrays.
Field positions is object array with the structure:
		{
			"title":"",
			"start-date":"",
			"end-date":"",
			"company":"",
			"summary":[
				""
			]
		}
Field projects is object array with the structure:
		{
			"title":"",
			"start-date":"",
			"end-date":"",
			"summary":[
				""
			]
		}
Field educations is object array with the structure:
		{
			"school":"",
			"course":"",
			"start-date":"",
			"end-date":""
		}

IV Built versions

Pre-built CVSummaryExtractor tool comes in four versions:
1. Centos 7.1 executable with links to C++ boost library
2. Centos 7.1 executable with C++ boost library statically linked
3. OSX Yosemite executable with links to C++ boost library
4. OSX Yosemite executable with C++ boost library statically linked

V Building
IV.I The following section will explain building it on Centos 7

To build it:
1. Install cmake:
sudo yum install cmake
2. Install gcc and g++:
sudo yum install gcc
sudo yum install gcc-c++
2. Install boost (see below how to install recommended static version instead):
a. sudo yum install boost
b. sudo yum install boost-devel.x86_64
On Centos the latest boost version was 1.53 but CMakeLists.txt makes requirement for 1.58. This can be solved by either
downloading boost by hand and building static image or by editing CMakeLists.txt. Beware that 1.53 may not work
at all with default locales on CentOS. See below how to install and build boost static libraries.
2. cd CVSummaryExtractor
3. mkdir build
4. cd build
5. cmake ..
For Release build
cmake .. -DCMAKE_BUILD_TYPE=Release
6. make
That should build CVSummaryExtractor command line tool in the build directory.
7. copy tika-app-1.9.jar to the target directory
8. install java 6
9. install tesseract (see README from Tesseract folder)
10. install cld (see README from Cld folder)
11. install cpp-netlib (see README from Cpp-netlib folder) & copy boost folder from the cpp-netlib unpacked version
to CVSummaryExtractor path. Use always cpp-netlib developer version

To build boost with static linking enabled, install it manually:
1. cd [YOUR OWN PATH FOR STORING BOOST]
2. wget http://sourceforge.net/projects/boost/files/boost/1.58.0/boost_1_58_0.tar.gz
3. tar -xzf boost_1_58_0.tar.gz
4. cd boost_1_58_0/ (version may vary)
5. ./bootstrap.sh
If some dependencies are missing like python or bziph2 headers then install via
6. sudo yum install python-devel
7. sudo yum install bzip2-devel
etc.
Finally
8. ./b2 link=static
9. sudo ./b2 install
10. Then go back to CVSummaryExtractor/build
11. Clean it
12. cmake .. -DUSE_STATIC_BOOST=YES
For Release build
cmake .. -DUSE_STATIC_BOOST=YES -DCMAKE_BUILD_TYPE=Release
13. make


V.II The following section will explain building it on MAC Yosemite
1. Install XCode development tools
2. Install brew package manager
3. brew install boost
4. cd CVSummaryExtractor
5. mkdir build
6. cd build
7. cmake .. -DCMAKE_C_COMPILER=/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc -DCMAKE_CXX_COMPILER=/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++
or if you want to link against static boost libraries, do
cmake .. -DCMAKE_C_COMPILER=/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc -DCMAKE_CXX_COMPILER=/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ -DUSE_STATIC_BOOST=YES
8. make
9. copy tika-app-1.9.jar to the target directory
10. install java 6
11. If some xcode tools, for example xcrun are not working after mac build then do:
sudo xcode-select -switch /
to restore the previous situation
12. install tesseract (see README from Tesseract folder)
13. install cld (see README from Cld folder)
14. install cpp-netlib (see README from Cpp-netlib folder) & copy boost folder from the cpp-netlib unpacked version
to CVSummaryExtractor path. Use always cpp-netlib developer version

VI Running
1. CVSummaryExtractor needs folder ResumeProcessor/Data to be copied to the destination path
2. tika-app-1.9.jar must be available in the same folder where CVSummaryExtractor resides in order to read data
   from non plain text sources like .doc, .pdf, .docx, .odf
3. In order to reduce document conversion time, copy nailgun server nailgun-server-0.9.2.jar along with client executable
   ng to the destination path, run and configure Nailgun server (see README from Nailgun folder).
   CVSummaryExtractor will produce correct output with or without Nailgun server but the server gives faster
   conversions and is better when running many conversions in parallel.

