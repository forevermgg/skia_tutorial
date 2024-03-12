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
#include <iostream>
#include <string>
#include <filesystem>
#include "skia/file.h"


static sk_sp<SkShader> make() {
    auto surf = SkSurfaces::Raster(SkImageInfo::MakeN32Premul(2, 2));
    surf->getCanvas()->drawColor(SK_ColorWHITE);
    surf->getCanvas()->drawRect({0, 0, 1, 1}, SkPaint());
    surf->getCanvas()->drawRect({1, 1, 2, 2}, SkPaint());
    return surf->makeImageSnapshot()->makeShader(SkTileMode::kRepeat,
                                                 SkTileMode::kRepeat,
                                                 SkSamplingOptions(SkFilterMode::kLinear));
}

void draw(SkCanvas* canvas) {
    // 将画布的颜色设置为白色。这将清除画布上的任何内容，并将其填充为白色。
    canvas->drawColor(SK_ColorWHITE);

   // 定义一个矩形，左上角位于原点，宽度和高度都是 100。
    const SkRect r = { 0, 0, 100, 100 };

    // 创建一个新的 SkPaint 对象。
    SkPaint p;

    // 假设 make() 函数返回一个 SkShader 对象（例如前面提到的 make_bw_dither() 函数）。
    p.setShader(make());

    // 使用 SkPaint 对象（包含 SkShader 对象）在画布上绘制一个矩形，左上角位于原点，宽度和高度都是 50。
    canvas->drawRect({0, 0, 50, 50}, p);

    // 缩放画布。在这个例子中，我们将画布的宽度和高度都缩小一半。
    canvas->scale(0.5, 0.5);

    // 在画布上移动原点。在这个例子中，我们将原点向右移动 100 个单位。
    canvas->translate(100, 0);

    // 使用之前设置的 SkPaint 对象（包含 SkShader 对象）绘制一个矩形。矩形的左上角位于原点，宽度和高度都是 100。
    canvas->drawRect(r, p);

    // 将 SkPaint 对象的着色器设置为 nullptr，以便绘制纯色。
    p.setShader(nullptr);

    // 设置 SkPaint 对象的颜色为不透明的深灰色（0xFF808080）。
    p.setColor(SK_ColorRED);

    // 在画布上移动原点。在这个例子中，我们将原点向右移动 100 个单位。
    canvas->translate(100, 0);

    // 使用之前设置的不透明深灰色 SkPaint 对象，绘制一个矩形。矩形的左上角位于原点，宽度和高度都是 100。
    canvas->drawRect(r, p);

    // 设置 SkPaint 对象的颜色为半透明黑色（0x80000000）。
    p.setColor(SK_ColorBLUE);

    // 在画布上移动原点。在这个例子中，我们将原点向右移动 100 个单位。
    canvas->translate(100, 0);

    // 使用之前设置的半透明黑色 SkPaint 对象，绘制一个矩形。矩形的左上角位于原点，宽度和高度都是 100。
    canvas->drawRect(r, p);
}

int main(int argc, char *const argv[]) {
    SkBitmap bitmap;
    SkImageInfo imageInfo = SkImageInfo::Make(1000, 200, kBGRA_8888_SkColorType, kPremul_SkAlphaType);
    bitmap.allocPixels(imageInfo, imageInfo.minRowBytes());

    SkCanvas canvas(bitmap);
    draw(&canvas);
    std::string current_directory = File::get_current_directory();
    std::filesystem::path current_path = std::filesystem::current_path();
    current_directory = current_path.parent_path().parent_path();
    std::cout << "当前目录是: " << current_directory << std::endl;
    SkFILEWStream save_file((current_directory + PATH_SEPARATOR + "50_percent_srgb.png").c_str()); //创建文件输出流
    if (!save_file.isValid()) {
        return 1;
    }
    SkPngEncoder::Encode(&save_file, bitmap.pixmap(), {});
}
