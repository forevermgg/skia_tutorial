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

void draw(SkCanvas* canvas) {
    std::vector<int32_t> srcPixels;
    srcPixels.resize(source.height() * source.rowBytes());
    SkPixmap pixmap(SkImageInfo::MakeN32Premul(source.width(), source.height()),
                    &srcPixels.front(), source.rowBytes());
    source.readPixels(pixmap, 0, 0);
    for (int y = 0; y < source.height(); ++y) {
        for (int x = 0; x < source.width(); ++x) {
            SkPMColor pixel = srcPixels[y * source.width() + x];
            const SkColor color = SkUnPreMultiply::PMColorToColor(pixel);
            if (SkColorGetA(color) == SK_AlphaOPAQUE) {
                srcPixels[y * source.width() + x] = SK_ColorGREEN;
            }
        }
    }
    canvas->drawImage(SkImages::RasterFromPixmapCopy(pixmap), 0, 0);
}


int main(int argc, char *const argv[]) {
    SkImageInfo imageInfo = SkImageInfo::Make(256, 128, kBGRA_8888_SkColorType, kPremul_SkAlphaType);
    source.allocPixels(imageInfo, imageInfo.minRowBytes());

    SkCanvas canvas(source);
    draw(&canvas);
    std::string current_directory = File::get_current_directory();
    std::filesystem::path current_path = std::filesystem::current_path();
    current_directory = current_path.parent_path().parent_path();
    std::cout << "当前目录是: " << current_directory << std::endl;
    SkFILEWStream save_file((current_directory + PATH_SEPARATOR + "Alpha_Constants_b.png").c_str()); //创建文件输出流
    if (!save_file.isValid()) {
        return 1;
    }
    SkPngEncoder::Encode(&save_file, source.pixmap(), {});
}
