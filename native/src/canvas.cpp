#include "export.h"

#include <iostream>
#include "include/core/SkImage.h"
#include "include/core/SkBitmap.h"
#include "include/core/SkCanvas.h"
#include "include/core/SkCanvas.h"
#include "include/core/SkData.h"
#include "include/core/SkAnnotation.h"
#include "include/core/SkPaint.h"
#include "include/core/SkPath.h"
#include "include/core/SkPathBuilder.h"
#include "include/core/SkMatrix.h"
#include "include/core/SkPathEffect.h"
#include "include/codec/SkCodec.h"
#include "include/core/SkPicture.h"
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

QUEST_API void canvas_draw_image(SkCanvas *canvas, SkImage *image, float width, float height) {
    constexpr SkSamplingOptions samplingOptions(SkFilterMode::kLinear, SkMipmapMode::kLinear);
    canvas->drawImageRect(image, SkRect::MakeWH(width, height), samplingOptions);
}

QUEST_API void canvas_draw_filled_rectangle(SkCanvas *canvas, SkRect rect, SkColor color) {
    SkPaint paint;
    paint.setColor(color);
    paint.setAntiAlias(true);

    canvas->drawRect(rect, paint);
}

QUEST_API void canvas_draw_stroke_rectangle(SkCanvas *canvas, SkRect rect, float strokeWidth, SkColor color) {
    SkPaint paint;

    paint.setColor(color);
    paint.setAntiAlias(true);

    paint.setStroke(true);
    paint.setStrokeWidth(strokeWidth);

    canvas->drawRect(rect, paint);
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
    canvas->clipRect(clipArea);
}

struct SkCanvasMatrix
{
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
