#ifdef _WIN32

#include "export.h"

#include "include/core/SkBitmap.h"
#include "include/core/SkCanvas.h"
#include "include/core/SkStream.h"
#include "include/core/SkDocument.h"
#include "include/docs/SkXPSDocument.h"

#include <xpsobjectmodel.h>

extern "C" {

QUEST_API SkDocument *xps_document_create(SkWStream *stream, SkScalar dpi) {
    IXpsOMObjectFactory *factory = nullptr;

    auto hr = CoCreateInstance(CLSID_XpsOMObjectFactory, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&factory));

    if (!SUCCEEDED(hr))
        return nullptr;

    SkXPS::Options ;
    xpsOptions.dpi = dpi;

    xpsOptions.pngEncoder = [](SkWStream* dst, const SkPixmap& src) {
        return SkPngEncoder::Encode(dst, src, {});
    };

    return SkXPS::MakeDocument(stream, factory, xpsOptions).release();
}

}

#endif