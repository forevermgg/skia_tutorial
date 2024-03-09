#include <include/core/SkCanvas.h>
#include <include/core/SkBitmap.h>
#include <include/core/SkPaint.h>
#include <include/core/SkTypeface.h>
#include <include/core/SkFont.h>
#include <include/codec/SkCodec.h>
#include <include/core/SkStream.h>
#include <include/encode/SkPngEncoder.h>
#include "file.h"

int main(int argc, char *const argv[]) {
    std::filesystem::path current_path = std::filesystem::current_path();
    std::string current_directory = current_path.parent_path().parent_path();
    std::cout << "当前目录是: " << current_directory << std::endl;

    // 创建一个位图设备
    SkBitmap bitmap;
    SkImageInfo imageInfo = SkImageInfo::Make(480, 320, kBGRA_8888_SkColorType, kPremul_SkAlphaType);  //设置位图信息
    bitmap.allocPixels(imageInfo, imageInfo.minRowBytes());  //为位图设备绑定信息和分配内存

    // 创建画布
    SkCanvas canvas(bitmap);
    // 创建画笔
    SkPaint paint;
    canvas.clear(SkColorSetARGB(0xFF, 0x14, 0x14, 0x14));
    // 将画布清空并填充一种颜色，注意这里是ARGB，Alpha通道值在第一位，同时可以直接用16进制数表示，例如上面这个颜色值可以表示为0xFF141414
    paint.setColor(SK_ColorWHITE);
    // 设置画笔颜色
    // 创建字体设备
    SkFont font;
    // 设置字体尺寸
    font.setSize(64);
    // font.setTypeface(SkTypeface::MakeFromName("Microsoft YaHei", SkFontStyle::Normal()));  //设置字体
    SkString text("Hello, Skia!");
    canvas.drawSimpleText(text.c_str(), text.size(), SkTextEncoding::kUTF8, 0, 64, font, paint);  //在画布上绘制字体

    SkFILEWStream file((current_directory + PATH_SEPARATOR + "hello_skia.png").c_str());  //创建文件输出流
    if (!file.isValid()) {
        return 1;
    }
    return SkPngEncoder::Encode(&file, bitmap.pixmap(), {});
}