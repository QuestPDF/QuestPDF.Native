#include "../export.h"

#include "modules/skparagraph/include/ParagraphBuilder.h"
#include "modules/skparagraph/include/TypefaceFontProvider.h"

#ifdef __APPLE__

#include "include/ports/SkFontMgr_mac_ct.h"

sk_sp<SkFontMgr> font_manager_create_default_internal() {
    return SkFontMgr_New_CoreText(nullptr);
}

#endif

#ifdef _WIN32

#include "include/ports/SkTypeface_win.h"

sk_sp<SkFontMgr> font_manager_create_default_internal() {
    return SkFontMgr_New_DirectWrite();
}

#endif

#ifdef __linux__

#include "include/ports/SkFontMgr_fontconfig.h"

sk_sp<SkFontMgr> font_manager_create_default_internal() {
    return SkFontMgr_New_FontConfig(nullptr);
}

#endif

extern "C" {

QUEST_API SkFontMgr *font_manager_get_empty() {
    return SkFontMgr::RefEmpty().release();
}

QUEST_API SkFontMgr *font_manager_create_default() {
    return font_manager_create_default_internal().release();
}

QUEST_API SkTypeface *font_manager_create_typeface(SkFontMgr* fontManager, SkData* fontData) {
    return fontManager->makeFromData(sk_ref_sp(fontData)).release();
}

}
