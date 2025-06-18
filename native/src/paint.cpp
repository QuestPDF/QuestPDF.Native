#include "include/core/SkPathEffect.h"
#include "include/effects/SkDashPathEffect.h"
#include "include/effects/SkGradientShader.h"

#include "include/core/SkTileMode.h"
#include "include/core/SkPaint.h"

#include "export.h"

extern "C" {

    QUEST_API SkPaint *paint_create() {
        auto paint = new SkPaint();
        paint->setAntiAlias(true);
        return paint;
    }

    QUEST_API void paint_delete(const SkPaint* paint) {
        delete paint;
    }

    QUEST_API void paint_set_solid_color(SkPaint* paint, SkColor color) {
        paint->setColor(color);
    }

    QUEST_API void paint_set_linear_gradient(SkPaint* paint, SkRect rect, SkColor* colors, int colorsCount) {
        const SkPoint points[2] = {
            SkPoint::Make(rect.left(), rect.top()),
            SkPoint::Make(rect.right(), rect.bottom())
        };

        const auto shader = SkGradientShader::MakeLinear(
            points,
            colors,
            nullptr,
            colorsCount,
            SkTileMode::kClamp
        );

        paint->setShader(shader);
    }

    QUEST_API void paint_set_stroke(SkPaint* paint, const float thickness) {
        paint->setStroke(true);
        paint->setStrokeWidth(thickness);
    }

    QUEST_API void paint_set_dashed_path_effect(SkPaint* paint, const int length, const SkScalar* intervals) {
        const auto effect = SkDashPathEffect::Make(intervals, length, 0);
        paint->setPathEffect(effect);
    }
}