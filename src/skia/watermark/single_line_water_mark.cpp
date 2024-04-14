#include <include/core/SkCanvas.h>
#include <include/core/SkBitmap.h>
#include <include/core/SkPaint.h>
#include <include/core/SkPath.h>
#include <include/core/SkSurface.h>
#include <include/core/SkFont.h>
#include <include/codec/SkCodec.h>
#include <include/core/SkStream.h>
#include <include/core/SkTextBlob.h>
#include <include/core/SkFontMetrics.h>
#include <include/encode/SkPngEncoder.h>
#include "include/gpu/GrDirectContext.h"
#include "include/core/SkData.h"
#include "include/core/SkImage.h"
#include "include/core/SkTileMode.h"
#include "include/core/SkShader.h"
#include <iostream>
#include <string>
#include <filesystem>
#include "skia/file.h"

void draw(SkCanvas *canvas) {
    canvas->clear(SK_ColorWHITE);

    // 创建SkPaint对象并设置文本样式
    SkFont font(nullptr, 30.0f); // 创建字体对象
    font.setEdging(SkFont::Edging::kAntiAlias);

    // 计算文本的宽度
    const char* text = "centforever test 187******** \n 2024-04-14";
    size_t length = strlen(text);
    SkScalar textWidth = font.measureText(text, length, SkTextEncoding::kUTF8);

    SkMatrix matrix;
    matrix.setScale(0.75f, 0.75f);
    matrix.preRotate(45.0f);

    // 创建SkPaint对象并设置文本样式
    SkPaint paint;
    paint.setAntiAlias(true);
    paint.setColor(SK_ColorBLACK); // 设置文本颜色
    canvas->rotate(-45, canvas->imageInfo().width()/2, canvas->imageInfo().height()/2);

    // 计算文本的绘制位置
    float x = (canvas->imageInfo().width() - textWidth) / 2; // 水平居中
    SkFontMetrics fontMetrics{};
    font.getMetrics(&fontMetrics);
    float y = (canvas->imageInfo().height() + fontMetrics.fDescent - fontMetrics.fAscent) / 2; // 垂直居中

    // 将文本绘制到画布上
    canvas->drawSimpleText(text, length, SkTextEncoding::kUTF8, x, y, font, paint);
}

int main() {
    //加载图片资源
    SkBitmap canvasBitmap;
    SkImageInfo imageInfo = SkImageInfo::Make(640, 640, kBGRA_8888_SkColorType, kPremul_SkAlphaType);  //设置位图信息
    canvasBitmap.allocPixels(imageInfo, imageInfo.minRowBytes());  //为位图设备绑定信息和分配内存

    SkCanvas canvas(canvasBitmap);
    draw(&canvas);
    //将绘制结果保存到图片文件中
    std::string current_directory = File::get_current_directory();
    std::filesystem::path current_path = std::filesystem::current_path();
    current_directory = current_path.parent_path().parent_path();
    std::cout << "当前目录是: " << current_directory << std::endl;
    SkFILEWStream save_file((current_directory + PATH_SEPARATOR + "single_line_water_mark.png").c_str()); //创建文件输出流
    if (!save_file.isValid()) {
        return 1;
    }
    SkPngEncoder::Encode(&save_file, canvasBitmap.pixmap(), {});
}
