#include "export.h"

#include <iostream>
#include "include/core/SkImage.h"
#include "include/core/SkBitmap.h"
#include "include/core/SkCanvas.h"
#include "include/core/SkData.h"
#include "include/core/SkRRect.h"
#include "include/core/SkAnnotation.h"
#include "include/core/SkPaint.h"
#include "include/core/SkPath.h"
#include "include/pathops/SkPathOps.h"
#include "include/core/SkPathBuilder.h"
#include "include/core/SkMatrix.h"
#include "include/codec/SkCodec.h"
#include "include/core/SkPathEffect.h"
#include "include/effects/SkBlurMaskFilter.h"
#include "include/effects/Sk2DPathEffect.h"
#include "include/utils/SkParsePath.h"
#include "modules/skparagraph/include/Paragraph.h"
#include "modules/svg/include/SkSVGDOM.h"

extern "C" {

QUEST_API SkCanvas *canvas_create_from_bitmap(SkBitmap *bitmap) {
    auto canvas = new SkCanvas(*bitmap);
    return canvas;
}

QUEST_API void canvas_delete(SkCanvas *canvas) {
    delete canvas;
}

QUEST_API void canvas_save(SkCanvas *canvas) {
    canvas->save();
}

QUEST_API void canvas_restore(SkCanvas *canvas) {
    canvas->restore();
}

QUEST_API void canvas_translate(SkCanvas *canvas, float x, float y) {
    canvas->translate(x, y);
}

QUEST_API void canvas_rotate(SkCanvas *canvas, float angle) {
    canvas->rotate(angle);
}

QUEST_API void canvas_scale(SkCanvas *canvas, float factorX, float factorY) {
    canvas->scale(factorX, factorY);
}

QUEST_API void canvas_annotate_url(SkCanvas *canvas, float width, float height, char *url) {
    auto data = SkData::MakeWithCString(url).release();
    SkAnnotateRectWithURL(canvas, SkRect::MakeWH(width, height), data);
    data->unref();
}

QUEST_API void canvas_annotate_destination(SkCanvas *canvas, char *destinationName) {
    auto data = SkData::MakeWithCString(destinationName).release();
    SkAnnotateNamedDestination(canvas, SkPoint(), data);
    data->unref();
}

QUEST_API void canvas_annotate_destination_link(SkCanvas *canvas, float width, float height, char *destinationName) {
    const auto data = SkData::MakeWithCString(destinationName).release();
    SkAnnotateLinkToDestination(canvas, SkRect::MakeWH(width, height), data);
    data->unref();
}

QUEST_API void canvas_draw_line(SkCanvas *canvas, SkPoint start, SkPoint end, SkPaint* paint) {
    canvas->drawLine(start, end, *paint);
}

QUEST_API void canvas_draw_rectangle(SkCanvas *canvas, SkRect rect, SkPaint* paint) {
    canvas->drawRect(rect, *paint);
}

struct SKRoundedRect {
    SkRect rect;
    SkVector topLeftRadius;
    SkVector topRightRadius;
    SkVector bottomRightRadius;
    SkVector bottomLeftRadius;
};

SkPath createRoundedRectPath(const SKRoundedRect &roundedRect) {
    SkPath path;

    const auto rect = roundedRect.rect;
    const auto left = rect.fLeft;
    const auto top = rect.fTop;
    const auto right = rect.fRight;
    const auto bottom = rect.fBottom;
    const auto width = rect.width();
    const auto height = rect.height();

    // copy radii so we can adjust them if needed
    auto topLeft = roundedRect.topLeftRadius;
    auto topRight = roundedRect.topRightRadius;
    auto bottomRight = roundedRect.bottomRightRadius;
    auto bottomLeft = roundedRect.bottomLeftRadius;

    // clamp negative radii to zero
    topLeft.fX = std::max(0.0f, topLeft.fX);
    topLeft.fY = std::max(0.0f, topLeft.fY);
    topRight.fX = std::max(0.0f, topRight.fX);
    topRight.fY = std::max(0.0f, topRight.fY);

    bottomRight.fX = std::max(0.0f, bottomRight.fX);
    bottomRight.fY = std::max(0.0f, bottomRight.fY);
    bottomLeft.fX = std::max(0.0f, bottomLeft.fX);
    bottomLeft.fY = std::max(0.0f, bottomLeft.fY);

    // calculate scale factors for each edge
    auto scaleX = 1.0f;
    auto scaleY = 1.0f;

    // check horizontal edges
    auto topSum = topLeft.fX + topRight.fX;
    auto bottomSum = bottomLeft.fX + bottomRight.fX;

    if (topSum > width) {
        scaleX = std::min(scaleX, width / topSum);
    }
    if (bottomSum > width) {
        scaleX = std::min(scaleX, width / bottomSum);
    }

    // check vertical edges
    auto leftSum = topLeft.fY + bottomLeft.fY;
    auto rightSum = topRight.fY + bottomRight.fY;

    if (leftSum > height) {
        scaleY = std::min(scaleY, height / leftSum);
    }
    if (rightSum > height) {
        scaleY = std::min(scaleY, height / rightSum);
    }

    // apply scale factors to all radii
    topLeft.fX *= scaleX;
    topRight.fX *= scaleX;
    bottomRight.fX *= scaleX;
    bottomLeft.fX *= scaleX;

    topLeft.fY *= scaleY;
    topRight.fY *= scaleY;
    bottomRight.fY *= scaleY;
    bottomLeft.fY *= scaleY;

    // magic constant for cubic bezier approximation of a quarter circle
    const auto kBezierConstant = 0.552284749831f;

    // start at the top-left corner (after the radius)
    path.moveTo(left + topLeft.fX, top);

    // top edge
    path.lineTo(right - topRight.fX, top);

    // top-right corner
    if (topRight.fX > 0 && topRight.fY > 0) {
        path.cubicTo(
            right - topRight.fX + topRight.fX * kBezierConstant, top,
            right, top + topRight.fY - topRight.fY * kBezierConstant,
            right, top + topRight.fY
        );
    }
    else {
            path.lineTo(right, top + topRight.fY);
    }

    // right edge
    path.lineTo(right, bottom - bottomRight.fY);

    // bottom-right corner
    if (bottomRight.fX > 0 && bottomRight.fY > 0) {
        path.cubicTo(
            right, bottom - bottomRight.fY + bottomRight.fY * kBezierConstant,
            right - bottomRight.fX + bottomRight.fX * kBezierConstant, bottom,
            right - bottomRight.fX, bottom
        );
    }
    else {
        path.lineTo(right - bottomRight.fX, bottom);
    }

    // bottom edge
    path.lineTo(left + bottomLeft.fX, bottom);

    // bottom-left corner
    if (bottomLeft.fX > 0 && bottomLeft.fY > 0) {
        path.cubicTo(
            left + bottomLeft.fX - bottomLeft.fX * kBezierConstant, bottom,
            left, bottom - bottomLeft.fY + bottomLeft.fY * kBezierConstant,
            left, bottom - bottomLeft.fY
        );
    }
    else {
        path.lineTo(left, bottom - bottomLeft.fY);
    }

    // left edge
    path.lineTo(left, top + topLeft.fY);

    // top-left corner
    if (topLeft.fX > 0 && topLeft.fY > 0) {
        path.cubicTo(
            left, top + topLeft.fY - topLeft.fY * kBezierConstant,
            left + topLeft.fX - topLeft.fX * kBezierConstant, top,
            left + topLeft.fX, top
        );
    }
    else {
        path.lineTo(left + topLeft.fX, top);
    }

    // close the path
    path.close();

    return path;
}

void path_optimize(const SkPath& source, SkPath* target) {
    SkPath emptyPath;
    SkPath optimizedPath;
    Simplify(source, target);
}

QUEST_API void canvas_draw_complex_border(SkCanvas *canvas, SKRoundedRect innerRect, SKRoundedRect outerRect, SkPaint* paint) {
    const auto innerPath = createRoundedRectPath(innerRect);
    const auto outerPath = createRoundedRectPath(outerRect);

    SkPath borderPath;
    borderPath.addPath(outerPath);
    borderPath.reverseAddPath(innerPath);

    SkPath optimizedBorderPath;
    path_optimize(borderPath, &optimizedBorderPath);

    canvas->drawPath(optimizedBorderPath, *paint);
}

struct SKBoxShadow {
    float offsetX;
    float offsetY;
    float blur;
    SkColor color;
};

QUEST_API void canvas_draw_shadow(SkCanvas *canvas, SKRoundedRect shadowRect, SKBoxShadow shadow) {
    if (shadow.color == 0)
        return;

    SkPaint shadowPaint;
    shadowPaint.setColor(shadow.color);
    shadowPaint.setAntiAlias(true);

    if (shadow.blur > 0)
        shadowPaint.setMaskFilter(SkMaskFilter::MakeBlur(kNormal_SkBlurStyle, shadow.blur));

    canvas->save();
    canvas->translate(shadow.offsetX, shadow.offsetY);

    const auto shadowPath = createRoundedRectPath(shadowRect);
    canvas->drawPath(shadowPath, shadowPaint);

    canvas->restore();
}

QUEST_API void canvas_draw_image(SkCanvas *canvas, SkImage *image, float width, float height) {
    constexpr SkSamplingOptions samplingOptions(SkFilterMode::kLinear, SkMipmapMode::kLinear);
    canvas->drawImageRect(image, SkRect::MakeWH(width, height), samplingOptions);
}

QUEST_API void canvas_draw_picture(SkCanvas *canvas, SkPicture *picture) {
    canvas->drawPicture(picture);
}

QUEST_API void canvas_draw_paragraph(SkCanvas *canvas, skia::textlayout::Paragraph *paragraph, int lineFrom, int lineTo) {
    paragraph->paintLines(canvas, lineFrom, lineTo);
}

QUEST_API void canvas_draw_svg_path(SkCanvas *canvas, char *svg, SkColor color) {
    SkPath path;
    SkParsePath::FromSVGString(svg, &path);

    SkPaint paint;
    paint.setAntiAlias(true);
    paint.setColor(color);
    canvas->drawPath(path, paint);
}

QUEST_API void canvas_draw_svg(SkCanvas *canvas, SkSVGDOM *svg, float width, float height) {
    svg->setContainerSize(SkSize::Make(width, height));
    svg->render(canvas);
}

QUEST_API void canvas_draw_overflow_area(SkCanvas *canvas, SkRect position) {
    // configuration
    const float stripeScale = 6;
    const float stripeThickness = 1.5;
    const SkColor color = 0xFFf44336; // red medium

    // effect configuration
    const auto pathEffectMatrix = SkMatrix()
            .setScale(stripeScale, stripeScale)
            .postRotate(45);

    auto pathEffect = SkLine2DPathEffect::Make(stripeThickness, pathEffectMatrix);

    SkPaint paint;
    paint.setColor(color);
    paint.setPathEffect(pathEffect);
    paint.setAntiAlias(true);

    // position calculation
    const float inflate = stripeScale * 2;
    auto targetArea = SkRect::MakeLTRB(
            position.left() - inflate,
            position.top() - inflate,
            position.right() + inflate,
            position.bottom() + inflate);

    // draw
    canvas->drawRect(targetArea, paint);
}

QUEST_API void canvas_clip_overflow_area(SkCanvas *canvas, SkRect availableSpace, SkRect requiredSpace) {
    auto removeArea = SkRect::MakeWH(
            std::min(availableSpace.right(), requiredSpace.right()),
            std::min(availableSpace.bottom(), requiredSpace.bottom()));

    SkPath path;
    path.addRect(requiredSpace, SkPathDirection::kCW);
    path.addRect(removeArea, SkPathDirection::kCCW);

    canvas->clipPath(path);
}

QUEST_API void canvas_clip_rectangle(SkCanvas *canvas, SkRect clipArea) {
    canvas->clipRect(clipArea, true);
}

QUEST_API void canvas_clip_rounded_rectangle(SkCanvas *canvas, SKRoundedRect rect) {
    const auto path = createRoundedRectPath(rect);
    canvas->clipPath(path, true);
}

struct SkCanvasMatrix {
    float ScaleX;
    float SkewX;
    float TranslateX;

    float SkewY;
    float ScaleY;
    float TranslateY;

    float Perspective1;
    float Perspective2;
    float Perspective3;
};

QUEST_API SkCanvasMatrix canvas_get_matrix9(SkCanvas *canvas) {
    SkScalar array[9];
    canvas->getLocalToDeviceAs3x3().get9(array);

    SkCanvasMatrix matrix;
    matrix.ScaleX = array[0];
    matrix.SkewX = array[1];
    matrix.TranslateX = array[2];

    matrix.SkewY = array[3];
    matrix.ScaleY = array[4];
    matrix.TranslateY = array[5];

    matrix.Perspective1 = array[6];
    matrix.Perspective2 = array[7];
    matrix.Perspective3 = array[8];

    return matrix;
}

QUEST_API void canvas_set_matrix9(SkCanvas *canvas, SkCanvasMatrix matrix) {
    SkScalar array[9];

    array[0] = matrix.ScaleX;
    array[1] = matrix.SkewX;
    array[2] = matrix.TranslateX;

    array[3] = matrix.SkewY;
    array[4] = matrix.ScaleY;
    array[5] = matrix.TranslateY;

    array[6] = matrix.Perspective1;
    array[7] = matrix.Perspective2;
    array[8] = matrix.Perspective3;

    SkMatrix skMatrix;
    skMatrix.set9(array);

    canvas->setMatrix(SkM44(skMatrix));
}

}
