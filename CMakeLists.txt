cmake_minimum_required(VERSION 2.8)
project(CVSummaryExtractor)

#set(CMAKE_FIND_LIBRARY_SUFFIXES ".a")
#set(BUILD_SHARED_LIBRARIES OFF)
#set(USE_STATIC_BOOST OFF)

set(Boost_USE_STATIC_LIBS ${USE_STATIC_BOOST})
#set(Boost_USE_STATIC_RUNTIME ${USE_STATIC_BOOST})
set(Boost_USE_MULTITHREADED ON)
#unset(Boost_INCLUDE_DIR CACHE)
#unset(Boost_LIBRARY_DIRS CACHE)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fstrict-aliasing")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=native")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -finline-functions")
#SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
#SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -pthread")


SET(CLD_INCLUDE_DIR "/usr/local/include/cld/")
SET(CLD_LIBRARIES "/usr/local/lib/cld/")

IF (CLD_INCLUDE_DIR AND CLD_LIBRARIES)
    # Already in cache, be silent
    SET(CLD_FIND_QUIETLY TRUE)
ENDIF (CLD_INCLUDE_DIR AND CLD_LIBRARIES)

set(CMAKE_REQUIRED_INCLUDES "${CLD_INCLUDE_DIR} ${CMAKE_REQUIRED_INCLUDES}")
set(CMAKE_REQUIRED_LIBRARIES "${CLD_LIBRARIES} ${CMAKE_REQUIRED_LIBRARIES}")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DCLD_WINDOWS")

include_directories(/usr/local/include/cld/)

FIND_PATH(CLD_INCLUDE_DIR cldutil.h)

FIND_LIBRARY(CLD_LIBRARIES libcld.a)

IF(CLD_INCLUDE_DIR AND CLD_LIBRARIES)
    SET(CLD_FOUND TRUE)
ENDIF(CLD_INCLUDE_DIR AND CLD_LIBRARIES)

IF(CLD_FOUND)
    IF(NOT CLD_FIND_QUIETLY)
        MESSAGE(STATUS "Found Cld: ${CLD_LIBRARIES}")
    ENDIF(NOT CLD_FIND_QUIETLY)
ELSE(CLD_FOUND)
    IF(CLD_FIND_REQUIRED)
        MESSAGE(FATAL_ERROR "Could not find Cld")
    ENDIF(CLD_FIND_REQUIRED)
ENDIF(CLD_FOUND)



#MARK_AS_ADVANCED(
#        CLD_INCLUDE_DIR
#        CLD_LIBRARIES
#        CLD_SECOND_ARGUMENT_IS_CONST
#)
# - Try to find Iconv
# Once done this will define
#
# ICONV_FOUND - system has Iconv
# ICONV_INCLUDE_DIR - the Iconv include directory
# ICONV_LIBRARIES - Link these to use Iconv
# ICONV_SECOND_ARGUMENT_IS_CONST - the second argument for iconv() is const
#

SET(ICONV_INCLUDE_DIR, "/usr/local/opt/libiconv/include/")
SET(ICONV_LIBRARIES, "/usr/local/opt/libiconv/lib/")

IF (ICONV_INCLUDE_DIR AND ICONV_LIBRARIES)
    # Already in cache, be silent
    SET(ICONV_FIND_QUIETLY TRUE)
ENDIF (ICONV_INCLUDE_DIR AND ICONV_LIBRARIES)

FIND_PATH(ICONV_INCLUDE_DIR iconv.h)

FIND_LIBRARY(ICONV_LIBRARIES NAMES iconv libiconv libiconv-2 c)

IF(ICONV_INCLUDE_DIR AND ICONV_LIBRARIES)
    SET(ICONV_FOUND TRUE)
ENDIF(ICONV_INCLUDE_DIR AND ICONV_LIBRARIES)

#set(CMAKE_REQUIRED_INCLUDES ${ICONV_INCLUDE_DIR} ${CLD_INCLUDE_DIR})
#set(CMAKE_REQUIRED_LIBRARIES ${ICONV_LIBRARIES} ${CLD_LIBRARIES})
#set(CMAKE_REQUIRED_INCLUDES)
#set(CMAKE_REQUIRED_LIBRARIES)
set(CMAKE_REQUIRED_INCLUDES "${ICONV_INCLUDE_DIR} ${CMAKE_REQUIRED_INCLUDES}")
set(CMAKE_REQUIRED_LIBRARIES "${ICONV_LIBRARIES} ${CMAKE_REQUIRED_LIBRARIES}")

IF(ICONV_FOUND)
    IF(NOT ICONV_FIND_QUIETLY)
        MESSAGE(STATUS "Found Iconv: ${ICONV_LIBRARIES}")
    ENDIF(NOT ICONV_FIND_QUIETLY)
ELSE(ICONV_FOUND)
    IF(Iconv_FIND_REQUIRED)
        MESSAGE(FATAL_ERROR "Could not find Iconv")
    ENDIF(Iconv_FIND_REQUIRED)
ENDIF(ICONV_FOUND)

MARK_AS_ADVANCED(
        ICONV_INCLUDE_DIR
        ICONV_LIBRARIES
        ICONV_SECOND_ARGUMENT_IS_CONST
)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

SET_SOURCE_FILES_PROPERTIES( Common/entities.c tld/tld_data.c tld/tld.c PROPERTIES LANGUAGE CXX )
set(SOURCE_FILES main.cpp Stemmers/porter2_stemmer.cpp Stemmers/porter2_stemmer.h ResumeProcessor/Parsers/DictionaryLoaderSingleton.cpp ResumeProcessor/Parsers/DictionaryLoaderSingleton.h ResumeProcessor/Parsers/IDictionaryLoader.cpp ResumeProcessor/Parsers/IDictionaryLoader.h ResumeProcessor/Parsers/CTextLanguageParser.cpp ResumeProcessor/Parsers/CTextLanguageParser.h Translator/ITranslator.cpp Translator/ITranslator.h Translator/CBingTranslator.cpp Translator/CBingTranslator.h OpenSSL/Patch.cpp Common/entities.c tld/libtld/tld.h tld/tld_data.h tld/tld_data.c tld/tld.c ResumeProcessor/Parsers/CEducationDataParser.cpp ResumeProcessor/Parsers/CEducationDataParser.h Model/CDates.cpp Model/CDates.h Model/CEducationData.cpp Model/CEducationData.h Model/CSchoolData.cpp Model/CSchoolData.h)

#
# Find the OpenSSL client includes and library
#

# This module defines
# OPENSSL_INCLUDE_DIRECTORIES, where to find openssl.h
# OPENSSL_LIBRARIES, the libraries to link against
# OPENSSL_FOUND, if false, you cannot build anything that requires it.

# also defined, but not for general use are
# OPENSSL_LIBRARY.

if( OPENSSL_INCLUDE_DIRECTORIES AND OPENSSL_LIBRARIES )
    # in cache already
    set(OPENSSL_FOUND 1)
else( OPENSSL_INCLUDE_DIRECTORIES AND OPENSSL_LIBRARIES )
    set(OPENSSL_FOUND 0)
    find_path(OPENSSL_INCLUDE_DIRECTORIES
            NAMES
            ssl.h
            PATHS
            /usr/local/opt/openssl/include/
            /usr/include
            /usr/include/openssl
            /usr/local/include
            /usr/local/include/openssl
            /usr/local/openssl/include
            DOC
            "Specify the directory containing openssl.h."
    )

    find_library(OPENSSL_LIBRARIES
            NAMES
            ssleay32
            ssl
            PATHS
            /usr/local/opt/openssl/lib/
            /usr/lib64
            /usr/local/lib64
            /usr/lib
            /usr/lib/ssl
            /usr/local/lib
            /usr/local/lib/ssl
            /usr/local/ssl/lib
            DOC "Specify the OpenSSL library here."
    )

    if( OPENSSL_LIBRARIES )
        if( OPENSSL_INCLUDE_DIRECTORIES )
            set( OPENSSL_FOUND 1 )
            message(STATUS "Found OpenSSL library: ${OPENSSL_LIBRARIES}")
            message(STATUS "Found OpenSSL headers: ${OPENSSL_INCLUDE_DIRECTORIES}")
        else ( OPENSSL_INCLUDE_DIRECTORIES )
            message(FATAL_ERROR "Could not find OpenSSL headers! Please install the development-headers")
        endif( OPENSSL_INCLUDE_DIRECTORIES )
    else( OPENSSL_LIBRARIES )
        message(FATAL_ERROR "Could not find OpenSSL libraries! Please install the library before continuing")
    endif( OPENSSL_LIBRARIES )
    mark_as_advanced( OPENSSL_FOUND OPENSSL_LIBRARIES OPENSSL_INCLUDE_DIRECTORIES )
endif( OPENSSL_INCLUDE_DIRECTORIES AND OPENSSL_LIBRARIES )

if( CRYPTO_LIBRARIES )
    # in cache already
    set(CRYPTO_FOUND 1)
else( CRYPTO_LIBRARIES )
    set(CRYPTO_FOUND 0)

    find_library(CRYPTO_LIBRARIES
            NAMES
            crypto
            PATHS
            /usr/local/opt/openssl/lib/
            /usr/lib64
            /usr/local/lib64
            /usr/lib
            /usr/lib/ssl
            /usr/local/lib
            /usr/local/lib/ssl
            /usr/local/ssl/lib
            DOC "Specify the Crypto library here."
    )

    if( CRYPTO_LIBRARIES )
        set( CRYPTO_FOUND 1 )
        message(STATUS "Found Crypto library: ${CRYPTO_LIBRARIES}")
    else( CRYPTO_LIBRARIES )
        message(FATAL_ERROR "Could not find Crypto library! Please install the library before continuing")
    endif( CRYPTO_LIBRARIES )
    mark_as_advanced( CRYPTO_FOUND CRYPTO_LIBRARIES )
endif( CRYPTO_LIBRARIES )

find_package(Boost 1.58.0 COMPONENTS regex filesystem system locale thread REQUIRED)
if(USE_STATIC_BOOST)
   message(STATUS "Linking against boost static libraries")
else(USE_STATIC_BOOST)
   message(STATUS "Linking against boost dynamic libraries")
endif(USE_STATIC_BOOST)

if(Boost_FOUND)

    message(STATUS "Boost_INCLUDE_DIRS: ${Boost_INCLUDE_DIRS}")
    message(STATUS "Boost_LIBRARIES: ${Boost_LIBRARIES}")
    message(STATUS "Boost_VERSION: ${Boost_VERSION}")

    include_directories(${Boost_INCLUDE_DIRS})

endif()
message(STATUS ${CMAKE_SYSTEM_NAME})
message(STATUS ${OPENSSL_LIBRARIES})
message(STATUS ${CRYPTO_LIBRARIES})
add_executable(CVSummaryExtractor ${SOURCE_FILES} Model/CEducation.cpp Model/CEducation.h Model/CPeriod.cpp Model/CPeriod.h Model/CPosition.cpp Model/CPosition.h Model/CProject.cpp Model/CProject.h Model/CResume.cpp Model/CResume.h Model/CSection.cpp Model/CSection.h Model/ESectionType.h Model/IInputReader.cpp Model/IInputReader.h Model/CInputReaderBase.cpp Model/CInputReaderBase.h Model/IInputReader.cpp Model/IInputReader.h Model/IOutputFormatter.cpp Model/IOutputFormatter.h Model/IInputReaderFactory.cpp Model/IInputReaderFactory.h PlainReader/CPlainTextInputReader.cpp PlainReader/CPlainTextInputReader.h ResumeProcessor/Parsers/IParser.cpp ResumeProcessor/Parsers/IParser.h ResumeProcessor/Parsers/CAwardsParser.cpp ResumeProcessor/Parsers/CAwardsParser.h ResumeProcessor/Parsers/CCoursesParser.cpp ResumeProcessor/Parsers/CCoursesParser.h ResumeProcessor/Parsers/CEducationParser.cpp ResumeProcessor/Parsers/CEducationParser.h ResumeProcessor/Parsers/CPersonalParser.cpp ResumeProcessor/Parsers/CPersonalParser.h ResumeProcessor/Parsers/CProjectsParser.cpp ResumeProcessor/Parsers/CProjectsParser.h ResumeProcessor/Parsers/CSkillsParser.cpp ResumeProcessor/Parsers/CSkillsParser.h ResumeProcessor/Parsers/CSummaryParser.cpp ResumeProcessor/Parsers/CSummaryParser.h ResumeProcessor/Parsers/CWorkExperienceParser.cpp ResumeProcessor/Parsers/CWorkExperienceParser.h ResumeProcessor/Helpers/IResourceLoader.cpp ResumeProcessor/Helpers/IResourceLoader.h ResumeProcessor/Helpers/CResourceLoader.cpp ResumeProcessor/Helpers/CResourceLoader.h ResumeProcessor/Helpers/CDateHelper.cpp ResumeProcessor/Helpers/CDateHelper.h ResumeProcessor/CInputReaderFactory.cpp ResumeProcessor/CInputReaderFactory.h ResumeProcessor/CResumeProcessor.cpp ResumeProcessor/CResumeProcessor.h ResumeProcessor/CSectionExtractor.cpp ResumeProcessor/CSectionExtractor.h ResumeProcessor/CSectionMatchingService.cpp ResumeProcessor/CSectionMatchingService.h Common/CTextUtils.cpp Common/CTextUtils.h ResumeProcessor/Parsers/CResumeBuilder.cpp ResumeProcessor/Parsers/CResumeBuilder.h Common/CLowerCaseString.cpp Common/CLowerCaseString.h Common/SLowerCaseUnorderedSet.h Json/JsonOutputFormatter.cpp Json/JsonOutputFormatter.h ResumeProcessor/Parsers/CRawInputParser.cpp ResumeProcessor/Parsers/CRawInputParser.h Common/CFileUtils.cpp Common/CFileUtils.h Common/CDebugUtils.cpp Common/CDebugUtils.h Easylogging/easylogging++.h Stacktrace/Stacktrace.h ResumeProcessor/Parsers/CPopularWordsParser.cpp ResumeProcessor/Parsers/CPopularWordsParser.h OpenSSL/Patch.cpp)

if(Boost_FOUND)
    #find_library (LIBSTDCXX NAMES stdc++)

    #find_library(LIBSTDCXX
    #        NAMES
    #        stdc++
    #        PATHS
    #        /usr/lib/gcc/x86_64-redhat-linux/4.8.2   #For CentOS build machine
    #        /usr/lib/
    #        DOC "Specify the std c++ library here."
    #)


    #add_compile_options(-stdlib=libstdc++)

    target_link_libraries(CVSummaryExtractor
            #${LIBSTDCXX}
            ${OPENSSL_LIBRARIES}
            ${CRYPTO_LIBRARIES}
            ${Boost_REGEX_LIBRARY}
            ${Boost_FILESYSTEM_LIBRARY}
            ${Boost_SYSTEM_LIBRARY}
            ${Boost_THREAD_LIBRARY}
            ${ICONV_LIBRARIES}
            ${CLD_LIBRARIES}/libcld.a
            ${CMAKE_CURRENT_SOURCE_DIR}/libs/${CMAKE_SYSTEM_NAME}/libcppnetlib-client-connections.a
            ${CMAKE_CURRENT_SOURCE_DIR}/libs/${CMAKE_SYSTEM_NAME}/libcppnetlib-server-parsers.a
            ${CMAKE_CURRENT_SOURCE_DIR}/libs/${CMAKE_SYSTEM_NAME}/libcppnetlib-uri.a
            ${Boost_LOCALE_LIBRARY} )
    #target_link_libraries(CVSummaryExtractor ${Boost_REGEX_LIBRARY} ${Boost_FILESYSTEM_LIBRARY} ${Boost_SYSTEM_LIBRARY})

endif()

add_custom_command(TARGET CVSummaryExtractor POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_directory
                       ${CMAKE_SOURCE_DIR}/ResumeProcessor/Data $<TARGET_FILE_DIR:CVSummaryExtractor>/ResumeProcessor/Data)

