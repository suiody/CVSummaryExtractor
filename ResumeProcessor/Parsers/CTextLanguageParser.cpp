//
// File CTextLanguageParser.cpp
//
//

#include "CTextLanguageParser.h"
#include "../../Common/CDebugUtils.h"
#include "../../Common/CTextUtils.h"
#include "Translator/CBingTranslator.h"

#include "encodings/compact_lang_det/compact_lang_det.h"
#include "encodings/compact_lang_det/ext_lang_enc.h"
#include "encodings/compact_lang_det/unittest_data.h"
#include "encodings/proto/encodings.pb.h"

using namespace Summary_Common;
using namespace Summary_Translator;

/*
 * Summary_ResumeProcessor_Parsers::CTextLanguageParser::Parse
 *
 * Description
 * Method for finding overall text language and saving results into resume object
 *
 * Inputs
 * CSection section - section data
 * CResume &resume - resume object to be filled during parsing process
 *
 * Returns
 * changed resume object through input parameter resume
 */
void Summary_ResumeProcessor_Parsers::CTextLanguageParser::Parse(CSection section, CResume &resume)
{
    CDebugUtils::DebugCout("CTextLanguageParser", "Parse", "");
    int i = 0;
//return;
    bool is_plain_text = true;
    bool do_allow_extended_languages = true;
    bool do_pick_summary_language = false;
    bool do_remove_weak_matches = false;
    bool is_reliable;
    Language plus_one = UNKNOWN_LANGUAGE;
    const char* tld_hint = NULL;
    int encoding_hint = UNKNOWN_ENCODING;
    Language language_hint = UNKNOWN_LANGUAGE;

    double normalized_score3[3];
    Language language3[3];
    int percent3[3];
    int text_bytes;

    string allLines = "";

    while (i < section.GetContent().size())
    {
        string line = section.GetContent()[i];

        i++;
        allLines += line;
    }

    const char* src = allLines.c_str();
    Language lang;


    lang = CompactLangDet::DetectLanguage(0,
                                          src, strlen(src),
                                          is_plain_text,
                                          do_allow_extended_languages,
                                          do_pick_summary_language,
                                          do_remove_weak_matches,
                                          tld_hint,
                                          encoding_hint,
                                          language_hint,
                                          language3,
                                          percent3,
                                          normalized_score3,
                                          &text_bytes,
                                          &is_reliable);

    resume.SetTextLanguageCode(LanguageCode(lang));
    resume.SetTextLanguageName(LanguageName(lang));//resume.SetTextLanguageName("ARABIC");

    //At the moment will translate only arabic texts
    if (resume.GetTextLanguageName().compare("ARABIC") == 0)
    {
        CBingTranslator translator;
        string translatedText = translator.Translate(section.GetContent(), resume.GetTextLanguageCode(), "en");
        resume.SetTranslation(CTextUtils::Split(translatedText, '\n'));
    }
}
