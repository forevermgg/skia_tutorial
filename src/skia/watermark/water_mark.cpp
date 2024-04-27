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
    labels.push_back("centforever test 187******** 2024-04-14");
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
        // 文字宽度
        float textWidth = font.measureText(text.c_str(), length, SkTextEncoding::kUTF8);
        std::cout << "textWidth: " << textWidth << std::endl;
        std::cout << "percentWH: " << percentWH << std::endl;
        while (textWidth > percentWH) {
            isCutOff = true;
            text = text.substr(0,text.length() - 1);
            std::cout << "text.substr text: " << text << std::endl;
            textWidth = font.measureText(text.c_str(), length, SkTextEncoding::kUTF8);
        }
        if (isCutOff) {
            subLabels.push_back(text);
            std::cout << "subLabels text: " << text << std::endl;
        } else {
            subLabels.push_back(text);
        }
    }
    for (int k = 0; k < subLabels.size(); k++) {
        std::cout << "print subLabels text: " << subLabels[k] << std::endl;
    }

    // 文字baseline在y轴方向的位置
    SkFontMetrics fontMetrics{};
    font.getMetrics(&fontMetrics);
    float baseLineY = std::abs(fontMetrics.fDescent + fontMetrics.fAscent) / 2;
    // 文本高度：文本的高度就是实际绘制区域的高度，可以用(fontMetrics.descent - fontMetrics.ascent)获取，因为ascent为负数，所以最终算出来的是两者的和
    float fontHeight = fontMetrics.fDescent - fontMetrics.fAscent;
    // 间距
    auto textPadding = 32;
    auto padding = 0;
    /*auto padding = 0;
    int setSizes = 10 - 1;
    int splitPoint = setSizes / 2.0;
    for (int i = 0; i < setSizes; i++) {
        double gap = i / 2.0;
        padding = i > splitPoint ? (textPadding * (i - splitPoint)) : (textPadding * (-splitPoint + i));
        std::cout <<
        " 除数: " << gap <<
        " 剩数: " << i % 2 <<
        " 分割点: " << splitPoint <<
        " padding: " << padding <<
        std::endl;
    }*/
    for (int i = 0; i < column + 1; i++) {
        for (int j = 0; j < row + 1; j++) {
            canvas->save();
            canvas->translate(startX, startY);
            startX = startW + i * tileW;
            startY = startH + j * tileH;
            canvas->rotate(-45);
            for (int k = 0; k < subLabels.size(); k++) {
                std::string text = subLabels[k];
                // std::cout << "draw subLabels text: " << subLabels[k] << std::endl;
                // 文本长度宽
                size_t length = strlen(text.c_str());
                // 文字宽
                float textWidth = font.measureText(text.c_str(), length, SkTextEncoding::kUTF8);
                canvas->drawSimpleText(text.c_str(),
                                       length,
                                       SkTextEncoding::kUTF8,
                                       SkScalar((percentWH / 2.0 - textWidth / 2.0) + percentWH / 4.0),
                                       SkScalar(fontHeight * k * 1.0 - fontHeight) + padding,
                                       font,
                                       paint);
                /*canvas->drawSimpleText(text.c_str(),
                                       length,
                                       SkTextEncoding::kUTF8,
                                       (percentWH / 2 - textWidth / 2),
                                       (fontHeight * k - percentWH / 2),
                                       font,
                                       paint);*/
            }
            canvas->restore();
        }
    }
    return "multi_line_water_mark.png";
}

std::string drawMultiTwolinesWaterMark(SkCanvas *canvas) {
    // 清处背景
    canvas->clear(SK_ColorWHITE);
    // 创建SkFont和文字大小
    // 创建字体对象
    SkFont font(nullptr, 30.0f);
    // 抗锯齿
    font.setEdging(SkFont::Edging::kAntiAlias);

    // 文本
    std::vector<std::string> labels;
    labels.push_back("centforever test 187******** 2024-04-14");
    labels.push_back("2515");
    /*labels.push_back("centforever");
    labels.push_back("2515");*/

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
    std::cout << "column: " << column << std::endl;
    std::cout << "row: " << row << std::endl;
    auto tileW = (width - 2 * startW) / column;//宽
    auto tileH = percentWH;

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
        // 文字宽度
        float textWidth = font.measureText(text.c_str(), length, SkTextEncoding::kUTF8);
        std::cout << "textWidth: " << textWidth << std::endl;
        std::cout << "percentWH: " << percentWH << std::endl;
        while (textWidth > percentWH) {
            isCutOff = true;
            text = text.substr(0,text.length() - 1);
            std::cout << "text.substr text: " << text << std::endl;
            textWidth = font.measureText(text.c_str(), length, SkTextEncoding::kUTF8);
        }
        if (isCutOff) {
            subLabels.push_back(text);
            std::cout << "subLabels text: " << text << std::endl;
        } else {
            subLabels.push_back(text);
        }
    }
    for (const auto & subLabel : subLabels) {
        std::cout << "print subLabels text: " << subLabel << std::endl;
    }

    // 文字baseline在y轴方向的位置
    SkFontMetrics fontMetrics{};
    font.getMetrics(&fontMetrics);
    float baseLineY = std::abs(fontMetrics.fDescent + fontMetrics.fAscent) / 2;
    // 文本高度：文本的高度就是实际绘制区域的高度，可以用(fontMetrics.descent - fontMetrics.ascent)获取，因为ascent为负数，所以最终算出来的是两者的和
    float fontHeight = fontMetrics.fDescent - fontMetrics.fAscent;
    // top绝对值
    float top = abs(fontMetrics.fTop);
    // ascent绝对值
    float ascent = abs(fontMetrics.fAscent);
    // descent，正值
    float descent = fontMetrics.fDescent;
    // bottom，正值
    float bottom = fontMetrics.fBottom;
    // 行数
    int textLines = subLabels.size();
    // 文本高度
    float textHeight = top + bottom;
    // 文本总高度
    float textTotalHeight = textHeight * textLines;
    // 基数
    float basePosition = (textLines - 1) / 2.0f;
    // 间距
    auto textPadding = 16;
    auto padding = 0;
    for (int i = 0; i < column + 1; i++) {
        for (int j = 0; j < row + 1; j++) {
            canvas->save();
            canvas->translate(startX, startY);
            startX = startW + i * tileW;
            startY = startH + j * tileH;
            std::cout << "startX: " << startX << "startY: " << startY << std::endl;
            // 将坐标原点移到控件中心
            canvas->translate(percentWH / 2, percentWH / 2);
            // x轴
            canvas->drawLine(-percentWH / 2, 0, percentWH / 2, 0, paint);
            // y轴
            canvas->drawLine(0, -percentWH / 2, 0, percentWH / 2, paint);
            canvas->rotate(-45);
            for (int k = 0; k < subLabels.size(); k++) {
                std::string text = subLabels[k];
                // 文本长度宽
                size_t length = strlen(text.c_str());
                // 文字宽
                float textWidth = font.measureText(text.c_str(), length, SkTextEncoding::kUTF8);
                // 文本baseline在y轴方向的位置
                float baselineY;
                if (k < basePosition) {
                    // x轴上，值为负
                    // 总高度的/2 - 已绘制的文本高度 - 文本的top值（绝对值）
                    baselineY = -(textTotalHeight / 2 - textHeight * k - top);

                } else if (k > basePosition) {
                    // x轴下，值为正
                    // 总高度的/2 - 未绘制的文本高度 - 文本的bottom值（绝对值）
                    baselineY = textTotalHeight / 2 - textHeight * (textLines - k - 1) - bottom;

                } else {
                    // x轴中，值为正
                    // 计算公式请参考单行文本居中公式
                    baselineY = (ascent - descent) / 2;
                }
                canvas->drawSimpleText(text.c_str(),
                                       length,
                                       SkTextEncoding::kUTF8,
                                       -textWidth / 2,
                                       baselineY,
                                       font,
                                       paint);
            }
            canvas->restore();
        }
    }
    return "multi_two_line_water_mark.png";
}

int main() {
    //加载图片资源
    SkBitmap canvasBitmap;
    SkImageInfo imageInfo = SkImageInfo::Make(1080, 1920, kBGRA_8888_SkColorType, kPremul_SkAlphaType);  //设置位图信息
    canvasBitmap.allocPixels(imageInfo, imageInfo.minRowBytes());  //为位图设备绑定信息和分配内存

    SkCanvas canvas(canvasBitmap);
    std::string fileName = drawMultiTwolinesWaterMark(&canvas);
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
