#include "include/core/SkSpan.h"
#include "include/core/SkPathEffect.h"
#include "include/effects/SkDashPathEffect.h"
#include "include/effects/SkGradientShader.h"

#include "include/core/SkTileMode.h"
#include "include/core/SkPaint.h"

#include "export.h"

extern "C" {

    QUEST_API SkPaint *paint_create() {
        const auto paint = new SkPaint();
        paint->setAntiAlias(true);
        return paint;
    }

    QUEST_API void paint_delete(const SkPaint* paint) {
        delete paint;
    }

    QUEST_API void paint_set_solid_color(SkPaint* paint, SkColor color) {
        paint->setColor(color);
    }

    QUEST_API void paint_set_linear_gradient(SkPaint* paint, SkPoint start, SkPoint end, int colorsLength, SkColor* colors) {
        const SkPoint points[2] = {
            start, end
        };

        const auto shader = SkGradientShader::MakeLinear(
            points,
            colors,
            nullptr,
            colorsLength,
            SkTileMode::kClamp
        );

        paint->setShader(shader);
    }

    QUEST_API void paint_set_stroke(SkPaint* paint, const float thickness) {
        paint->setStroke(true);
        paint->setStrokeWidth(thickness);
    }

    QUEST_API void paint_set_dashed_path_effect(SkPaint* paint, const int length, const SkScalar* intervals) {
        const auto intervalsSpan = SkSpan(intervals, length);
        const auto effect = SkDashPathEffect::Make(intervalsSpan, 0);
        paint->setPathEffect(effect);
    }
}