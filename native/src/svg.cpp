#include "export.h"

#include "include/core/SkStream.h"
#include "modules/svg/include/SkSVGDOM.h"
#include "modules/svg/include/SkSVGSVG.h"
#include "modules/svg/include/SkSVGRenderContext.h"

extern "C" {

QUEST_API SkSVGDOM *svg_create(SkData *data, SkFontMgr *fontManager) {
    auto svgStream = SkMemoryStream(sk_ref_sp(data));

    auto svg = SkSVGDOM::Builder()
        .setFontManager(sk_ref_sp(fontManager))
        .make(svgStream);

    return svg.release();
}

QUEST_API void svg_unref(SkSVGDOM *svg) {
    svg->unref();
}

QUEST_API void svg_get_viewbox(SkSVGDOM *svg, SkRect *size) {
    auto viewBox = svg->getRoot()->getViewBox().getMaybeNull();

    if (viewBox != nullptr)
        size->setXYWH(viewBox->left(), viewBox->top(), viewBox->right(), viewBox->bottom());

    else
        size->setXYWH(0, 0, svg->getRoot()->getWidth().value(), svg->getRoot()->getHeight().value());
}

}