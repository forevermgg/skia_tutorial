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
#include "include/gpu/gl/GrGLInterface.h"
#include "include/core/SkData.h"
#include "include/core/SkImage.h"
#include "include/gpu/ganesh/SkSurfaceGanesh.h"
#include "../file.h"

void example(SkCanvas* canvas) {
    const SkScalar scale = 256.0f;
    const SkScalar R = 0.45f * scale;
    const SkScalar TAU = 6.2831853f;
    SkPath path;
    for (int i = 0; i < 5; ++i) {
        SkScalar theta = 2 * i * TAU / 5;
        if (i == 0) {
            path.moveTo(R * cos(theta), R * sin(theta));
        } else {
            path.lineTo(R * cos(theta), R * sin(theta));
        }
    }
    path.close();
    SkPaint p;
    p.setAntiAlias(true);
    canvas->clear(SK_ColorWHITE);
    canvas->translate(0.5f * scale, 0.5f * scale);
    canvas->drawPath(path, p);
}

void out_cpu_bitmap(int width, int height,
            void (*draw)(SkCanvas*),
            const char* path) {
    // 创建一个位图设备
    SkBitmap bitmap;
    SkImageInfo imageInfo = SkImageInfo::Make(width, height, kBGRA_8888_SkColorType, kPremul_SkAlphaType);  //设置位图信息
    bitmap.allocPixels(imageInfo, imageInfo.minRowBytes());  //为位图设备绑定信息和分配内存

    // 创建画布
    SkCanvas canvas(bitmap);
    draw(&canvas);
    SkFILEWStream file(path);  //创建文件输出流
    if (!file.isValid()) {
        return ;
    }
    SkPngEncoder::Encode(&file, bitmap.pixmap(), {});
}

int main(int argc, char *const argv[]) {
    std::string current_directory = File::get_current_directory();
    std::filesystem::path current_path = std::filesystem::current_path();
    current_directory = current_path.parent_path().parent_path();
    std::cout << "当前目录是: " << current_directory << std::endl;

    out_cpu_bitmap(256, 256, example, (current_directory + PATH_SEPARATOR + "out_cpu_bitmap.png").c_str());
    return 1;
}