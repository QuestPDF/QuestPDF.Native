#include "../export.h"

#include "modules/skparagraph/include/TextStyle.h"

extern "C" {

struct FontFeature {
    int tag;
    int value;
};

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

    static const int FONT_FEATURES_LENGTH = 16;
    FontFeature* fontFeatures[FONT_FEATURES_LENGTH];
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

    for (auto & fontFeature : configuration.fontFeatures) {
        if(fontFeature == nullptr)
            continue;

        if(fontFeature->tag == 0)
            continue;

        char decodedTag[4];
        decodedTag[0] = (fontFeature->tag >> 24) & 0xFF;
        decodedTag[1] = (fontFeature->tag >> 16) & 0xFF;
        decodedTag[2] = (fontFeature->tag >> 8) & 0xFF;
        decodedTag[3] = fontFeature->tag & 0xFF;

        textStyle->addFontFeature(SkString(decodedTag), fontFeature->value);
    }

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