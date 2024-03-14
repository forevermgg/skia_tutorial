#include <include/core/SkCanvas.h>
#include <include/core/SkBitmap.h>
#include <include/core/SkPaint.h>
#include <include/core/SkPath.h>
#include <include/core/SkSurface.h>
#include <include/core/SkFont.h>
#include <include/codec/SkCodec.h>
#include <include/core/SkStream.h>
#include <include/core/SkTextBlob.h>
#include <include/encode/SkPngEncoder.h>
#include "include/gpu/GrDirectContext.h"
#include "include/core/SkData.h"
#include "include/core/SkImage.h"
#include "include/core/SkTileMode.h"
#include "include/core/SkShader.h"
#include "include/core/SkUnPreMultiply.h"
#include <iostream>
#include <string>
#include <filesystem>
#include "skia/file.h"

SkBitmap source;
sk_sp<SkImage> image;

void draw(SkCanvas *canvas) {
    // 创建一个空的 SkBitmap 对象
    SkBitmap bitmap;

    // 为 SkBitmap 分配像素内存，并将像素值初始化为零
    bitmap.allocPixelsFlags(SkImageInfo::MakeN32(44, 16, kPremul_SkAlphaType),
                            SkBitmap::kZeroPixels_AllocFlag);

    // 创建一个 SkCanvas 对象，使用 SkBitmap 作为其绘图表面
    SkCanvas offscreen(bitmap);

    // 创建一个 SkPaint 对象，用于绘制文本
    SkPaint paint;

    // 创建一个 SkFont 对象，用于设置文本的字体
    SkFont font;

    // 在 offscreen 上绘制文本 "!@#$%"，文本的起始位置为 (0, 12)
    offscreen.drawString("!@#$%", 0, 12, font, paint);

    // 将输入的 canvas 的缩放设置为 6，这将使得在 canvas 上绘制的图像被放大 6 倍
    canvas->scale(6, 6);

    // 在输入的 canvas 上绘制 bitmap，图像的位置为 (0, 0)
    canvas->drawImage(bitmap.asImage(), 0, 0);

    // 再次在输入的 canvas 上绘制 bitmap，图像的位置为 (8, 8)
    canvas->drawImage(bitmap.asImage(), 8, 8);
}

int main(int argc, char *const argv[]) {
    SkString path = SkStringPrintf("resources/example_3.png");
    image = SkImages::DeferredFromEncodedData(SkData::MakeFromFileName(path.c_str()));
    SkAssertResult(image && image->asLegacyBitmap(&source));
    SkBitmap bmp;
    bmp.allocN32Pixels(256, 256);
    bmp.eraseColor(SK_ColorWHITE);
    SkCanvas canvas(bmp);
    draw(&canvas);

    std::string current_directory = File::get_current_directory();
    std::filesystem::path current_path = std::filesystem::current_path();
    current_directory = current_path.parent_path().parent_path();
    std::cout << "当前目录是: " << current_directory << std::endl;
    SkFILEWStream save_file((current_directory + PATH_SEPARATOR + "Bitmap_allocPixelsFlags.png").c_str()); //创建文件输出流
    if (!save_file.isValid()) {
        return 1;
    }
    SkPngEncoder::Encode(&save_file, bmp.pixmap(), {});
}