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

void draw(SkCanvas* canvas) {
    SkBitmap bitmap;
    SkImageInfo info = SkImageInfo::Make(256, 64, kGray_8_SkColorType, kOpaque_SkAlphaType);
    bitmap.allocPixels(info, info.width() * info.bytesPerPixel() + 64);
    SkCanvas offscreen(bitmap);
    offscreen.scale(.5f, .5f);
    for (int y : { 0, 64, 128, 192 } ) {
        offscreen.drawImage(source.asImage(), 0, -y);
        canvas->drawImage(bitmap.asImage(), 0, y);
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
    SkFILEWStream save_file((current_directory + PATH_SEPARATOR + "Bitmap_allocPixels.png").c_str()); //创建文件输出流
    if (!save_file.isValid()) {
        return 1;
    }
    SkPngEncoder::Encode(&save_file, bmp.pixmap(), {});
}