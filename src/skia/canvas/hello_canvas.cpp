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
    // Clear background
    canvas->drawColor(SK_ColorWHITE);
    SkPaint paint;
    // Draw a message with a nice black paint.
    paint.setAntiAlias(true);
    paint.setStyle(SkPaint::kStroke_Style);
    // paint.setStrokeWidth(4);
    paint.setColor(SK_ColorBLACK);
    SkFont font;  //创建字体设备
    font.setSize(20);  //设置字体尺寸

    static const char message[] = "Hello Canvas!";

    // Translate and draw the text:
    canvas->save();
    canvas->translate(SkIntToScalar(300), SkIntToScalar(300));
    canvas->drawSimpleText(message, strlen(message), SkTextEncoding::kUTF8, 0, 0, font, paint);
    canvas->restore();
}

void draw1(SkCanvas *canvas) {
    const SkScalar scale = 256.0f;
    const SkScalar R = 0.45f * scale;
    const SkScalar TAU = 6.2831853f;
    SkPath path;
    path.moveTo(R, 0.0f);
    for (int i = 1; i < 7; ++i) {
        SkScalar theta = 3 * i * TAU / 7;
        path.lineTo(R * cos(theta), R * sin(theta));
    }
    path.close();
    SkPaint p;
    p.setAntiAlias(true);
    // canvas->clear(SK_ColorWHITE);
    canvas->translate(0.5f * scale, 0.5f * scale);
    canvas->drawPath(path, p);
}

void draw2(SkCanvas *canvas) {
    canvas->save();
    canvas->translate(SkIntToScalar(128), SkIntToScalar(128));
    canvas->rotate(SkIntToScalar(45));
    SkRect rect = SkRect::MakeXYWH(-90.5f, -90.5f, 181.0f, 181.0f);
    SkPaint paint;
    paint.setColor(SK_ColorBLUE);
    canvas->drawRect(rect, paint);
    canvas->restore();
}

int main(int argc, char *const argv[]) {
    std::string current_directory = File::get_current_directory();
    std::filesystem::path current_path = std::filesystem::current_path();
    current_directory = current_path.parent_path().parent_path();
    std::cout << "当前目录是: " << current_directory << std::endl;
    // 创建一个位图设备
    SkBitmap bitmap;
    SkImageInfo imageInfo = SkImageInfo::Make(480, 320, kBGRA_8888_SkColorType, kPremul_SkAlphaType);  //设置位图信息
    bitmap.allocPixels(imageInfo, imageInfo.minRowBytes());  //为位图设备绑定信息和分配内存

    // 创建画布
    SkCanvas canvas(bitmap);

    draw(&canvas);
    draw1(&canvas);
    draw2(&canvas);

    // 保存到文件
    SkFILEWStream file((current_directory + PATH_SEPARATOR + "hello_canvas.png").c_str());  //创建文件输出流
    if (!file.isValid()) {
        return 1;
    }
    return SkPngEncoder::Encode(&file, bitmap.pixmap(), {});
}