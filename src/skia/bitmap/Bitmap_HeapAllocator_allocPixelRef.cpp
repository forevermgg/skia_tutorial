#include <include/core/SkCanvas.h>
#include <include/core/SkBitmap.h>
#include <include/core/SkPaint.h>
#include <include/core/SkPath.h>
#include <include/core/SkTypeface.h>
#include <include/core/SkFont.h>
#include <include/codec/SkCodec.h>
#include <include/core/SkStream.h>
#include <include/core/SkTextBlob.h>
#include <include/core/SkRRect.h>
#include <include/encode/SkPngEncoder.h>
#include "../file.h"


void draw(SkCanvas *canvas) {
    SkBitmap bitmap;
    bitmap.setInfo(SkImageInfo::MakeN32(16, 16, kPremul_SkAlphaType));
    SkDebugf("pixel address = %p\n", bitmap.getPixels());
    SkBitmap::HeapAllocator stdalloc;
    if (!stdalloc.allocPixelRef(&bitmap)) {
        SkDebugf("pixel allocation failed\n");
    } else {
        SkDebugf("pixel address = %p\n", bitmap.getPixels());
    }
}

int main(int argc, char *const argv[]) {
    SkBitmap bitmap;
    SkImageInfo imageInfo = SkImageInfo::Make(480, 320, kBGRA_8888_SkColorType, kPremul_SkAlphaType);  //设置位图信息
    bitmap.allocPixels(imageInfo, imageInfo.minRowBytes());  //为位图设备绑定信息和分配内存

    // 创建画布
    SkCanvas canvas(bitmap);

    draw(&canvas);
    return 0;
}