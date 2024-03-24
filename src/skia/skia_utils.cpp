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


bool SkiaUtils::BitmapsAreEqual(const SkBitmap &bitmap1, const SkBitmap &bitmap2) {
    if (bitmap1.isNull() != bitmap2.isNull() ||
        bitmap1.dimensions() != bitmap2.dimensions())
        return false;

    if (bitmap1.getGenerationID() == bitmap2.getGenerationID() ||
        (bitmap1.empty() && bitmap2.empty()))
        return true;

    // Calling getAddr32() on null or empty bitmaps will assert. The conditions
    // above should return early if either bitmap is empty or null.
    static_assert(!bitmap1.isNull() && !bitmap2.isNull());
    static_assert(!bitmap1.empty() && !bitmap2.empty());

    void *addr1 = bitmap1.getAddr32(0, 0);
    void *addr2 = bitmap2.getAddr32(0, 0);
    size_t size1 = bitmap1.computeByteSize();
    size_t size2 = bitmap2.computeByteSize();

    return (size1 == size2) && (0 == memcmp(addr1, addr2, size1));

    // 比较两个 SkBitmap 的大小和像素数据是否相等
    return (size1 == size2) && (0 == memcmp(addr1, addr2, size1));
}


bool SkiaUtils::ConvertBitmap(const SkBitmap &src_bitmap,
                              SkBitmap *target_bitmap,
                              SkColorType target_ct,
                              SkAlphaType target_at) {
    static_assert(src_bitmap.readyToDraw());
    static_assert(src_bitmap.colorType() != target_ct ||
                  src_bitmap.alphaType() != target_at);
    static_assert(target_bitmap);

    SkImageInfo target_info = SkImageInfo::Make(
        src_bitmap.width(), src_bitmap.height(), target_ct, target_at);
    if (!target_bitmap->tryAllocPixels(target_info)) {
        return false;
    }

    if (!src_bitmap.readPixels(target_info, target_bitmap->getPixels(),
                               target_bitmap->rowBytes(), 0, 0)) {
        return false;
    }

    static_assert(target_bitmap->readyToDraw());
    return true;
}

