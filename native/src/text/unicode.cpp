#include "../export.h"

#include "modules/skunicode/include/SkUnicode.h"
#include "modules/skunicode/include/SkUnicode_libgrapheme.h"

extern "C" {

QUEST_API SkUnicode *unicode_create() {
    return SkUnicodes::Libgrapheme::Make().release();
}

QUEST_API void unicode_unref(SkUnicode *unicode) {
    unicode->unref();
}

}  