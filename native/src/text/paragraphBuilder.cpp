#include "../export.h"

#include "modules/skparagraph/include/ParagraphStyle.h"
#include "modules/skparagraph/include/ParagraphBuilder.h"
#include "modules/skparagraph/include/TextStyle.h"

extern "C" {

struct ParagraphStyleConfiguration {
    skia::textlayout::TextAlign alignment;
    skia::textlayout::TextDirection direction;
    int maxLinesVisible;
};

QUEST_API skia::textlayout::ParagraphBuilder *paragraph_builder_create(ParagraphStyleConfiguration configuration, skia::textlayout::FontCollection *fontCollection) {
    skia::textlayout::ParagraphStyle paragraphStyle = skia::textlayout::ParagraphStyle();
    paragraphStyle.setTextAlign(configuration.alignment);
    paragraphStyle.setTextDirection(configuration.direction);
    paragraphStyle.setReplaceTabCharacters(true);
    paragraphStyle.setTextHeightBehavior(skia::textlayout::kAll);

    if (configuration.maxLinesVisible > 0) {
        paragraphStyle.setEllipsis(SkString("..."));
        paragraphStyle.setMaxLines(configuration.maxLinesVisible);
    } else {
        const int infiniteLines = std::numeric_limits<int>::max();
        paragraphStyle.setMaxLines(infiniteLines);
    }

    auto paragraphBuilder = skia::textlayout::ParagraphBuilder::make(paragraphStyle, sk_ref_sp(fontCollection));
    return paragraphBuilder.release();
}

QUEST_API void paragraph_builder_add_text(skia::textlayout::ParagraphBuilder *paragraphBuilder, char *text, skia::textlayout::TextStyle *style) {
    paragraphBuilder->pushStyle(*style);
    paragraphBuilder->addText(text);
    paragraphBuilder->pop();
}

QUEST_API void paragraph_builder_add_placeholder(skia::textlayout::ParagraphBuilder *paragraphBuilder, skia::textlayout::PlaceholderStyle style) {
    paragraphBuilder->addPlaceholder(style);
}

QUEST_API skia::textlayout::Paragraph *paragraph_builder_create_paragraph(skia::textlayout::ParagraphBuilder *paragraphBuilder) {
    auto paragraph = paragraphBuilder->Build();
    return paragraph.release();
}

QUEST_API void paragraph_builder_reset(skia::textlayout::ParagraphBuilder *paragraphBuilder) {
    paragraphBuilder->Reset();
}

QUEST_API void paragraph_builder_delete(skia::textlayout::ParagraphBuilder *paragraphBuilder) {
    delete paragraphBuilder;
}

}  