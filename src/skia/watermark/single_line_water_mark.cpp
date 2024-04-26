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
#include "include/core/SkImage.h"
#include "include/core/SkTileMode.h"
#include "include/core/SkShader.h"
#include <iostream>
#include <string>
#include <filesystem>
#include "skia/file.h"

std::string drawSingleWaterMark(SkCanvas *canvas) {
    canvas->clear(SK_ColorWHITE);

    // 创建SkPaint对象并设置文本样式
    SkFont font(nullptr, 30.0f); // 创建字体对象
    font.setEdging(SkFont::Edging::kAntiAlias);

    // 计算文本的宽度
    const char *text = "centforever test 187******** \n 2024-04-14";
    size_t length = strlen(text);
    SkScalar textWidth = font.measureText(text, length, SkTextEncoding::kUTF8);

    SkMatrix matrix;
    matrix.setScale(0.75f, 0.75f);
    matrix.preRotate(45.0f);

    // 创建SkPaint对象并设置文本样式
    SkPaint paint;
    paint.setAntiAlias(true);
    paint.setColor(SK_ColorBLACK); // 设置文本颜色
    canvas->rotate(-45, canvas->imageInfo().width() / 2, canvas->imageInfo().height() / 2);

    // 计算文本的绘制位置
    float x = (canvas->imageInfo().width() - textWidth) / 2; // 水平居中
    SkFontMetrics fontMetrics{};
    font.getMetrics(&fontMetrics);
    float y = (canvas->imageInfo().height() + fontMetrics.fDescent - fontMetrics.fAscent) / 2; // 垂直居中

    // 将文本绘制到画布上
    canvas->drawSimpleText(text, length, SkTextEncoding::kUTF8, x, y, font, paint);
    return "single_line_water_mark.png";
}

std::string drawWaterMarkModelOne(SkCanvas *canvas) {
    // 清处背景
    canvas->clear(SK_ColorWHITE);
    // 创建SkFont和文字大小
    // 创建字体对象
    SkFont font(nullptr, 30.0f);
    // 抗锯齿
    font.setEdging(SkFont::Edging::kAntiAlias);

    // 文本
    const char *text = "YangLe'Blog";
    // 文本长度
    size_t length = strlen(text);
    // 文字宽
    float textWidth = font.measureText(text, length, SkTextEncoding::kUTF8);

    int column = 3; //默认3列
    int row = 0;//默认0行

    // view宽
    auto width = canvas->imageInfo().width();
    // view高
    auto height = canvas->imageInfo().height();

    // 更新行列
    row = height / textWidth;
    // row = height / (width / 3);

    // padding left right偏移
    int startW = 16;
    // padding top bottom偏移
    int startH = 16;

    // 创建SkPaint对象并设置文本样式
    SkPaint paint;
    paint.setAntiAlias(true);
    // 设置文本颜色
    paint.setColor(SK_ColorBLACK);

    auto tileW = (width - 2 * startW) / column;//宽
    auto tileH = (height - 2 * startH) / row;//高

    float startX = 0, startY = 0;
    for (int i = 0; i < column + 1; i++) { //横向分隔,线条数=格数+1，画竖线
        startX = startW + i * tileW;
        startY = startH;
        canvas->drawLine(startX, startY, startX, height - startH, paint);//(x,y[min])-->(x,y[max])
    }
    for (int i = 0; i < row + 1; i++) {//纵向分隔,线条数=格数+1，画横线
        startX = startW;
        startY = startH + i * tileH;
        canvas->drawLine(startX, startY, width - startW, startY, paint);//(x[min],y)-->(x[max],y)
    }

    // 文字baseline在y轴方向的位置
    SkFontMetrics fontMetrics{};
    font.getMetrics(&fontMetrics);
    float baseLineY = std::abs(fontMetrics.fDescent + fontMetrics.fAscent) / 2;

    for (int i = 0; i < column + 1; i++) {
        for (int j = 0; j < row + 1; j++) {
            canvas->save();
            canvas->translate(startX, startY);
            canvas->rotate(-45);
            startX = startW + i * tileW;
            startY = startH + j * tileH;
            canvas->drawSimpleText(text, length, SkTextEncoding::kUTF8, textWidth / 2, baseLineY + tileH / 2, font,
                                   paint);
            canvas->restore();
        }
    }
    return "multi_line_water_mark_model_one.png";
}

std::string drawWaterMarkModelTwo(SkCanvas *canvas) {
    // 清处背景
    canvas->clear(SK_ColorWHITE);
    // 创建SkFont和文字大小
    // 创建字体对象
    SkFont font(nullptr, 30.0f);
    // 抗锯齿
    font.setEdging(SkFont::Edging::kAntiAlias);

    // 文本
    const char *text = "YangLe'Blog";
    // 文本长度
    size_t length = strlen(text);
    // 文字宽
    float textWidth = font.measureText(text, length, SkTextEncoding::kUTF8);

    int column = 3; //默认3列
    int row = 0;//默认0行

    // view宽
    auto width = canvas->imageInfo().width();
    // view高
    auto height = canvas->imageInfo().height();

    // 更新行列
    row = height / (width / 3);

    // padding left right偏移
    int startW = 16;
    // padding top bottom偏移
    int startH = 16;

    // 创建SkPaint对象并设置文本样式
    SkPaint paint;
    paint.setAntiAlias(true);
    // 设置文本颜色
    paint.setColor(SK_ColorBLACK);

    auto tileW = (width - 2 * startW) / column;//宽
    auto tileH = (height - 2 * startH) / row;//高

    float startX = 0, startY = 0;
    for (int i = 0; i < column + 1; i++) { //横向分隔,线条数=格数+1，画竖线
        startX = startW + i * tileW;
        startY = startH;
        canvas->drawLine(startX, startY, startX, height - startH, paint);//(x,y[min])-->(x,y[max])
    }
    for (int i = 0; i < row + 1; i++) {//纵向分隔,线条数=格数+1，画横线
        startX = startW;
        startY = startH + i * tileH;
        canvas->drawLine(startX, startY, width - startW, startY, paint);//(x[min],y)-->(x[max],y)
    }

    // 文字baseline在y轴方向的位置
    SkFontMetrics fontMetrics{};
    font.getMetrics(&fontMetrics);
    float baseLineY = std::abs(fontMetrics.fDescent + fontMetrics.fAscent) / 2;

    for (int i = 0; i < column + 1; i++) {
        for (int j = 0; j < row + 1; j++) {
            canvas->save();
            canvas->translate(startX, startY);
            canvas->rotate(-45);
            startX = startW + i * tileW;
            startY = startH + j * tileH;
            canvas->drawSimpleText(text, length, SkTextEncoding::kUTF8, textWidth, baseLineY, font, paint);
            canvas->restore();
        }
    }
    return "multi_line_water_mark_model_two.png";
}

std::string drawMultilinesWaterMark(SkCanvas *canvas) {
    // 清处背景
    canvas->clear(SK_ColorWHITE);
    // 创建SkFont和文字大小
    // 创建字体对象
    SkFont font(nullptr, 30.0f);
    // 抗锯齿
    font.setEdging(SkFont::Edging::kAntiAlias);

    // 文本
    std::vector<std::string> labels;
    labels.push_back("centforever ddddddddddddddddddd");
    labels.push_back("2515");

    int column = 3; //默认3列
    int row = 0;//默认0行

    // view宽
    auto width = canvas->imageInfo().width();
    // view高
    auto height = canvas->imageInfo().height();

    // 更新行列
    row = height / (width / 3);

    // padding left right偏移
    int startW = 16;
    // padding top bottom偏移
    int startH = 16;

    auto percentWH = (width - startW * 2) / 3;

    // 创建SkPaint对象并设置文本样式
    SkPaint paint;
    paint.setAntiAlias(true);
    // 设置文本颜色
    paint.setColor(SK_ColorBLACK);

    auto tileW = (width - 2 * startW) / column;//宽
    auto tileH = (height - 2 * startH) / row;//高

    float startX = 0, startY = 0;
    for (int i = 0; i < column + 1; i++) { //横向分隔,线条数=格数+1，画竖线
        startX = startW + i * tileW;
        startY = startH;
        canvas->drawLine(startX, startY, startX, height - startH, paint);//(x,y[min])-->(x,y[max])
    }
    for (int i = 0; i < row + 1; i++) {//纵向分隔,线条数=格数+1，画横线
        startX = startW;
        startY = startH + i * tileH;
        canvas->drawLine(startX, startY, width - startW, startY, paint);//(x[min],y)-->(x[max],y)
    }

    std::vector<std::string> subLabels;
    auto isCutOff = false;
    for (int k = 0; k < labels.size(); k++) {
        std::string text = labels[k];
        // 文本长度
        size_t length = strlen(text.c_str());
        // 文字宽
        float textWidth = font.measureText(text.c_str(), length, SkTextEncoding::kUTF8);
        std::cout << "textWidth: " << textWidth << std::endl;
        std::cout << "percentWH: " << percentWH << std::endl;
        while (textWidth > percentWH) {
            isCutOff = true;
            text = text.substr(0,text.length() - 1);
            textWidth = font.measureText(text.c_str(), length, SkTextEncoding::kUTF8);
        }
        if (isCutOff) {
            subLabels.push_back(text);
            std::cout << "subLabels text: " << text << std::endl;
        }
    }

    // 文字baseline在y轴方向的位置
    SkFontMetrics fontMetrics{};
    font.getMetrics(&fontMetrics);
    float baseLineY = std::abs(fontMetrics.fDescent + fontMetrics.fAscent) / 2;
    float fontHeight = fontMetrics.fDescent - fontMetrics.fAscent;

    for (int i = 0; i < column + 1; i++) {
        for (int j = 0; j < row + 1; j++) {
            canvas->save();
            canvas->translate(startX, startY);
            startX = startW + i * tileW;
            startY = startH + j * tileH;
            canvas->rotate(-45);
            for (int k = 0; k < labels.size(); k++) {
                std::string text = labels[k];
                // 文本长度
                size_t length = strlen(text.c_str());
                // 文字宽
                float textWidth = font.measureText(text.c_str(), length, SkTextEncoding::kUTF8);
                canvas->drawSimpleText(text.c_str(),
                                       length,
                                       SkTextEncoding::kUTF8,
                                       (percentWH / 2 - textWidth / 2) + percentWH / 4,
                                       fontHeight * k - fontHeight,
                                       font,
                                       paint);

                /*canvas->drawSimpleText(text.c_str(),
                                       length,
                                       SkTextEncoding::kUTF8,
                                       (percentWH - textWidth / 2 - percentWH / 2),
                                       (fontHeight * k - percentWH / 2),
                                       font,
                                       paint);*/
            }
            canvas->restore();
        }
    }
    return "multi_line_water_mark.png";
}

int main() {
    //加载图片资源
    SkBitmap canvasBitmap;
    SkImageInfo imageInfo = SkImageInfo::Make(1080, 1920, kBGRA_8888_SkColorType, kPremul_SkAlphaType);  //设置位图信息
    canvasBitmap.allocPixels(imageInfo, imageInfo.minRowBytes());  //为位图设备绑定信息和分配内存

    SkCanvas canvas(canvasBitmap);
    std::string fileName = drawMultilinesWaterMark(&canvas);
    //将绘制结果保存到图片文件中
    std::string current_directory = File::get_current_directory();
    std::filesystem::path current_path = std::filesystem::current_path();
    current_directory = current_path.parent_path().parent_path();
    std::cout << "当前目录是: " << current_directory << std::endl;
    SkFILEWStream save_file((current_directory + PATH_SEPARATOR + fileName).c_str()); //创建文件输出流
    if (!save_file.isValid()) {
        return 1;
    }
    SkPngEncoder::Encode(&save_file, canvasBitmap.pixmap(), {});
}
