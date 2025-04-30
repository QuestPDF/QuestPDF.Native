#include "../export.h"

#include "modules/skparagraph/include/ParagraphStyle.h"
#include "modules/skparagraph/include/ParagraphBuilder.h"
#include "modules/skparagraph/include/TextStyle.h"

extern "C" {

QUEST_API void paragraph_plan_layout(skia::textlayout::Paragraph *paragraph, float availableWidth) {
    paragraph->layout(availableWidth);
}

QUEST_API void paragraph_get_line_metrics(skia::textlayout::Paragraph *paragraph, SkSize **outputArray, int *outputArrayLength) {
    std::vector<skia::textlayout::LineMetrics> lineMetrics;
    paragraph->getLineMetrics(lineMetrics);

    *outputArrayLength = lineMetrics.size();
    *outputArray = new SkSize[*outputArrayLength];

    for (int i = 0; i < *outputArrayLength; ++i) {
        auto metrics = lineMetrics[i];
        (*outputArray)[i] = SkSize::Make(metrics.fWidth, metrics.fHeight);
    }
}

QUEST_API void paragraph_delete_line_metrics(SkSize *array) {
    delete[] array;
}

QUEST_API void paragraph_get_unresolved_codepoints(skia::textlayout::Paragraph *paragraph, SkUnichar **outputArray, int *outputArrayLength) {
    const auto codepoints = paragraph->unresolvedCodepoints();

    *outputArrayLength = codepoints.size();
    *outputArray = new int[*outputArrayLength];

    int index = 0;

    for (const auto &codepoint: codepoints)
        (*outputArray)[index++] = codepoint;
}

QUEST_API void paragraph_delete_unresolved_codepoints(SkUnichar *array) {
    delete[] array;
}

QUEST_API void paragraph_get_placeholder_positions(skia::textlayout::Paragraph *paragraph, SkRect **outputArray, int *outputArrayLength) {
    const auto placeholders = paragraph->getRectsForPlaceholders();

    *outputArrayLength = placeholders.size();
    *outputArray = new SkRect[*outputArrayLength];

    for (int i = 0; i < *outputArrayLength; ++i)
        (*outputArray)[i] = placeholders[i].rect;
}

QUEST_API void paragraph_delete_positions(SkRect *array) {
    delete[] array;
}

QUEST_API void paragraph_get_text_range_positions(skia::textlayout::Paragraph *paragraph, int rangeStart, int rangeEnd, SkRect **outputArray, int *outputArrayLength) {
    const auto placeholders = paragraph->getRectsForRange(rangeStart, rangeEnd, skia::textlayout::RectHeightStyle::kTight, skia::textlayout::RectWidthStyle::kTight);

    *outputArrayLength = placeholders.size();
    *outputArray = new SkRect[*outputArrayLength];

    for (int i = 0; i < *outputArrayLength; ++i)
        (*outputArray)[i] = placeholders[i].rect;
}

QUEST_API void paragraph_delete(skia::textlayout::Paragraph *paragraph) {
    delete paragraph;
}

}