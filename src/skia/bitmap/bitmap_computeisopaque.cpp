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
    bitmap.setInfo(SkImageInfo::Make(2, 2, kN32_SkColorType, kPremul_SkAlphaType));
    for (int index = 0; index < 2; ++index) {
        bitmap.allocPixels();
        bitmap.eraseColor(0x00000000);
        /*这是一个文档注释，描述了一个函数的功能、参数和返回值。这个函数用于检查一个给定的 SkBitmap 是否所有像素都是不透明的。函数的行为取决于 SkBitmap 的 SkColorType。

        根据 SkColorType 的不同，这个函数的行为如下：

        对于 kRGB_565_SkColorType 和 kGray_8_SkColorType，这些像素格式中没有 alpha 通道，所以函数总是返回 true。
        对于 kAlpha_8_SkColorType、kBGRA_8888_SkColorType 和 kRGBA_8888_SkColorType，函数会检查所有像素的 alpha 值是否都等于 255。如果是，则返回 true。
        对于 kARGB_4444_SkColorType，函数会检查所有像素的 alpha 值是否都等于 15。如果是，则返回 true。
        对于 kRGBA_F16_SkColorType，函数会检查所有像素的 alpha 值是否都大于等于 1.0。如果是，则返回 true。
        对于 kUnknown_SkColorType，函数返回 false。*/
        SkDebugf("computeIsOpaque: %s\n", SkBitmap::ComputeIsOpaque(bitmap) ? "true" : "false");
        bitmap.eraseColor(0xFFFFFFFF);
        SkDebugf("computeIsOpaque: %s\n", SkBitmap::ComputeIsOpaque(bitmap) ? "true" : "false");
        bitmap.setInfo(bitmap.info().makeAlphaType(kOpaque_SkAlphaType));
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