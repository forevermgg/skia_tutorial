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
#include "include/core/SkPixelRef.h"
#include <iostream>
#include <string>
#include <filesystem>
#include "skia/file.h"

SkBitmap source;
sk_sp<SkImage> image;

class TinyAllocator : public SkBitmap::Allocator {
public:
    bool allocPixelRef(SkBitmap *bitmap) override {
        const SkImageInfo &info = bitmap->info();
        if (info.height() * info.minRowBytes() > sizeof(storage)) {
            return false;
        }
        sk_sp<SkPixelRef> pr = sk_sp<SkPixelRef>(
                new SkPixelRef(info.width(), info.height(), storage, info.minRowBytes()));
        bitmap->setPixelRef(std::move(pr), 0, 0);
        return true;
    }

    char storage[16];
};

void draw(SkCanvas *canvas) {
    TinyAllocator tinyAllocator;
    SkBitmap bitmap;
    bitmap.setInfo(SkImageInfo::MakeN32(2, 2, kOpaque_SkAlphaType));
    if (bitmap.tryAllocPixels(&tinyAllocator)) {
        bitmap.eraseColor(0xff55aa33);
        bitmap.erase(0xffaa3355, SkIRect::MakeXYWH(1, 1, 1, 1));
        canvas->scale(16, 16);
        canvas->drawImage(bitmap.asImage(), 0, 0);
        // canvas->drawBitmap(bitmap, 0, 0);
    }
}

int main(int argc, char *const argv[]) {
    SkString path = SkStringPrintf("resources/example_3.png");
    image = SkImages::DeferredFromEncodedData(SkData::MakeFromFileName(path.c_str()));
    SkAssertResult(image && image->asLegacyBitmap(&source));
    SkBitmap bmp;
    bmp.allocN32Pixels(256, 256);
    bmp.eraseColor(SK_ColorWHITE);
    SkCanvas canvas(bmp);
    draw(&canvas);

    std::string current_directory = File::get_current_directory();
    std::filesystem::path current_path = std::filesystem::current_path();
    current_directory = current_path.parent_path().parent_path();
    std::cout << "当前目录是: " << current_directory << std::endl;
    SkFILEWStream save_file((current_directory + PATH_SEPARATOR + "Bitmap_allocPixels_4.png").c_str()); //创建文件输出流
    if (!save_file.isValid()) {
        return 1;
    }
    SkPngEncoder::Encode(&save_file, bmp.pixmap(), {});
}