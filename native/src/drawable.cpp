#include "export.h"

#include "include/core/SkDrawable.h"


extern "C" {
    QUEST_API void drawable_unref(SkDrawable *drawable) {
        drawable->unref();
    }
}