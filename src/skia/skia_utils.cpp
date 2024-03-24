//
// Created by centforever on 2024/3/23.
//
#include <include/core/SkStream.h>
#include <include/core/SkPath.h>
#include <include/encode/SkPngEncoder.h>
#include "skia_utils.h"
#include "include/core/SkData.h"

SkRect SkiaUtils::GetBounds(SkCanvas *canvas) {
    SkRect rect;
    rect.set(canvas->getDeviceClipBounds());
    return rect;
}

sk_sp<SkImage> SkiaUtils::LoadImageFromFile(const SkString &filePath) {
    sk_sp<SkData> skData = SkData::MakeFromFileName(filePath.c_str());
    if (!skData)
        return nullptr;

    return SkImages::DeferredFromEncodedData(std::move(skData));
}

bool SkiaUtils::IsXInRect(SkScalar x, const SkRect &rect) {
    return rect.left() <= x && x <= rect.right();
}

bool SkiaUtils::IsPointInRect(SkScalar x, SkScalar y, const SkRect &rect) {
    return rect.left() <= x && x <= rect.right() && rect.top() <= y && y <= rect.bottom();
}

bool SkiaUtils::IsPathAPoint(const SkPath &path) {
    if (path.isEmpty())
        return false;

    if (path.countPoints() == 1)
        return true;

    for (int i = 0; i < path.countPoints() - 1; ++i) {
        if (path.getPoint(i) != path.getPoint(i + 1))
            return false;
    }
    return true;
}


SkRuntimeEffect::Result SkiaUtils::LoadFromFile(const SkString &filePath) {
    sk_sp<SkData> sk_data = SkData::MakeFromFileName(filePath.c_str());
    if (!sk_data)
        return {nullptr, SkString{"Can't load the shader"}};
    SkString str{static_cast<const char *>(sk_data->data()), sk_data->size()};
    return SkRuntimeEffect::MakeForShader(std::move(str));
}

sk_sp<SkData> SkiaUtils::encode_to_data(const SkPixmap &pm) {
    SkDynamicMemoryWStream stream;
    if (SkPngEncoder::Encode(&stream, pm, SkPngEncoder::Options())) {
        return stream.detachAsData();
    }
    return nullptr;
}

void SkiaUtils::DebugShowSkiaPaint(const SkPaint &paint) {
    if (SkPaint::kFill_Style == paint.getStyle()) {
        printf("fill 0x%08x\n", paint.getColor());
    } else {
        printf("stroke 0x%08x width %g\n", paint.getColor(),
               paint.getStrokeWidth());
    }
}

void SkiaUtils::DebugShowSkiaPath(const SkPath &path) {
    printf(" **\n");
    SkDynamicMemoryWStream stream;
    path.dump(&stream, false);
    size_t numBytesWritten = stream.bytesWritten();

    std::vector<char> storage(numBytesWritten);
    stream.copyTo(storage.data());

    printf("%.*s", static_cast<int>(storage.size()), storage.data());
}

void SkiaUtils::DebugShowCanvasClip(const SkCanvas *canvas) {
    SkMatrix matrix = canvas->getTotalMatrix();
    SkScalar m[9];
    matrix.get9(m);
    printf("matrix (%g,%g,%g) (%g,%g,%g) (%g,%g,%g)\n", m[0], m[1], m[2], m[3],
           m[4], m[5], m[6], m[7], m[8]);
    SkRect local = canvas->getLocalClipBounds();
    SkIRect device = canvas->getDeviceClipBounds();

    printf("local bounds %g %g %g %g\n", local.fLeft, local.fTop, local.fRight,
           local.fBottom);
    printf("device bounds %d %d %d %d\n", device.fLeft, device.fTop,
           device.fRight, device.fBottom);
}

void SkiaUtils::DebugShowSkiaDrawPath(const SkCanvas *canvas,
                                      const SkPaint &paint,
                                      const SkPath &path) {
    DebugShowSkiaPaint(paint);
    DebugShowCanvasClip(canvas);
    DebugShowSkiaPath(path);
    printf("\n");
}

void SkiaUtils::DebugShowSkiaDrawRect(const SkCanvas *canvas,
                                      const SkPaint &paint,
                                      const SkRect &rect) {
    DebugShowSkiaPaint(paint);
    DebugShowCanvasClip(canvas);
    printf("rect %g %g %g %g\n", rect.fLeft, rect.fTop, rect.fRight,
           rect.fBottom);
}
