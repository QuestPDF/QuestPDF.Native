#include "../export.h"

#include "modules/skparagraph/include/ParagraphBuilder.h"
#include "modules/skparagraph/include/TypefaceFontProvider.h"

extern "C" {

QUEST_API void typeface_unref(SkTypeface *typeface) {
    typeface->unref();
}

}
