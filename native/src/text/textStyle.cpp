#include "../export.h"

#include "modules/skparagraph/include/TextStyle.h"

extern "C" {

struct TextStyleConfiguration {
    static const int FONT_FAMILIES_LENGTH = 16;

    SkScalar fontSize;
    SkFontStyle::Weight fontWeight;
    bool isItalic;

    char* fontFamilies[FONT_FAMILIES_LENGTH];

    SkColor foregroundColor;
    SkColor backgroundColor;

    SkColor decorationColor;
    SkScalar decorationThickness;
    skia::textlayout::TextDecoration decorationType;
    skia::textlayout::TextDecorationMode decorationMode;
    skia::textlayout::TextDecorationStyle decorationStyle;

    SkScalar lineHeight;
    SkScalar letterSpacing;
    SkScalar wordSpacing;
    SkScalar baselineOffset;
};

QUEST_API skia::textlayout::TextStyle *text_style_create(TextStyleConfiguration configuration) {
    auto textStyle = new skia::textlayout::TextStyle();

    textStyle->setFontSize(configuration.fontSize);

    SkFontStyle fontStyle(configuration.fontWeight, SkFontStyle::Width::kNormal_Width, configuration.isItalic ? SkFontStyle::Slant::kItalic_Slant : SkFontStyle::Slant::kUpright_Slant);
    textStyle->setFontStyle(fontStyle);

    // set font families
    std::vector<SkString> fontFamilies;

    for (auto & fontFamily : configuration.fontFamilies) {
        if (fontFamily == nullptr)
            continue;

        fontFamilies.emplace_back(fontFamily);
    }

    textStyle->setFontFamilies(fontFamilies);
    // end

    if (configuration.foregroundColor != 0) {
        SkPaint paint;
        paint.setColor(configuration.foregroundColor);
        textStyle->setForegroundPaint(paint);
    }

    if (configuration.backgroundColor != 0) {
        SkPaint paint;
        paint.setColor(configuration.backgroundColor);
        textStyle->setBackgroundColor(paint);
    }

    textStyle->setDecorationColor(configuration.decorationColor);
    textStyle->setDecoration(configuration.decorationType);
    textStyle->setDecorationMode(configuration.decorationMode);
    textStyle->setDecorationStyle(configuration.decorationStyle);
    textStyle->setDecorationThicknessMultiplier(configuration.decorationThickness);

    textStyle->setHeight(configuration.lineHeight);
    textStyle->setHeightOverride(true);
    textStyle->setLetterSpacing(configuration.letterSpacing);
    textStyle->setWordSpacing(configuration.wordSpacing);
    textStyle->setBaselineShift(configuration.baselineOffset);

    return textStyle;
}

QUEST_API void text_style_delete(skia::textlayout::TextStyle *textStyle) {
    delete textStyle;
}

}