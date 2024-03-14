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

    // 为 SkBitmap 分配像素内存，设置图像的宽度为 64，高度为 64，颜色类型为灰度，alpha 类型为不透明
    bitmap.allocPixels(SkImageInfo::Make(64, 64, kGray_8_SkColorType, kOpaque_SkAlphaType));

    // 创建一个 SkCanvas 对象，使用 SkBitmap 作为其绘图表面
    SkCanvas offscreen(bitmap);

    // 将 offscreen 的缩放设置为 0.5，这将使得在 offscreen 上绘制的图像被缩小一半
    offscreen.scale(.5f, .5f);

    // 在一个循环中，对每个 y 值（0、64、128、192）执行以下操作：
    for (int y: {0, 64, 128, 192}) {
        // 在 offscreen 上绘制 source 位图，位图的 x 和 y 坐标都被向左和向上移动 y 个单位
        offscreen.drawImage(source.asImage(), -y, -y);

        // 在输入的 canvas 上绘制 bitmap，位图的 x 和 y 坐标为 y
        canvas->drawImage(bitmap.asImage(), y, y);
    }
}

int main(int argc, char *const argv[]) {
    SkString path = SkStringPrintf("resources/example_3.png");
    image = SkImages::DeferredFromEncodedData(SkData::MakeFromFileName(path.c_str()));
    SkAssertResult(image && image->asLegacyBitmap(&source));
    SkBitmap bmp;
    bmp.allocN32Pixels(256, 256);
    SkCanvas canvas(bmp);
    draw(&canvas);

    std::string current_directory = File::get_current_directory();
    std::filesystem::path current_path = std::filesystem::current_path();
    current_directory = current_path.parent_path().parent_path();
    std::cout << "当前目录是: " << current_directory << std::endl;
    SkFILEWStream save_file((current_directory + PATH_SEPARATOR + "Bitmap_allocPixels_2.png").c_str()); //创建文件输出流
    if (!save_file.isValid()) {
        return 1;
    }
    SkPngEncoder::Encode(&save_file, bmp.pixmap(), {});
}