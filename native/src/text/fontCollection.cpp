#include "../export.h"

#include "modules/skparagraph/include/ParagraphBuilder.h"
#include "modules/skparagraph/include/TypefaceFontProvider.h"

extern "C" {

struct CreateFontCollectionCommand {
    SkFontMgr *fontManager;
    skia::textlayout::TypefaceFontProvider *typefaceFontProvider;
};

QUEST_API skia::textlayout::FontCollection *font_collection_create(CreateFontCollectionCommand command) {
    auto fontCollection = new skia::textlayout::FontCollection();

    fontCollection->setAssetFontManager(sk_ref_sp(command.typefaceFontProvider));
    fontCollection->setDefaultFontManager(sk_ref_sp(command.fontManager));
    fontCollection->enableFontFallback();
    fontCollection->getParagraphCache()->turnOn(false);

    return fontCollection;
}

QUEST_API void font_collection_unref(skia::textlayout::FontCollection *fontCollection) {
    fontCollection->unref();
}

}
