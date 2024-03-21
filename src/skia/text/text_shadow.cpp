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
#include "include/core/SkFontMgr.h"
#include "include/core/SkMaskFilter.h"
#include "include/core/SkBlurTypes.h"
#include <iostream>
#include <string>
#include <filesystem>
#include "skia/file.h"

SkBitmap source;
sk_sp<SkImage> image;

void draw(SkCanvas* canvas) {
    const SkScalar sigma = 1.65f;
    const SkScalar xDrop = 5.0f;
    const SkScalar yDrop = 5.0f;
    const SkScalar x = 8.0f;
    const SkScalar y = 52.0f;
    const SkScalar textSize = 48.0f;
    const uint8_t blurAlpha = 127;
    auto blob = SkTextBlob::MakeFromString("Skia", SkFont(nullptr, textSize));
    SkPaint paint;
    paint.setAntiAlias(true);
    SkPaint blur(paint);
    blur.setAlpha(blurAlpha);
    blur.setMaskFilter(SkMaskFilter::MakeBlur(kNormal_SkBlurStyle, sigma, 0));
    canvas->drawColor(SK_ColorWHITE);
    canvas->drawTextBlob(blob.get(), x + xDrop, y + yDrop, blur);
    canvas->drawTextBlob(blob.get(), x, y, paint);
}

int main(int argc, char *const argv[]) {
    SkBitmap bmp;
    bmp.allocN32Pixels(256, 256);
    SkCanvas canvas(bmp);
    draw(&canvas);

    std::string current_directory = File::get_current_directory();
    std::filesystem::path current_path = std::filesystem::current_path();
    current_directory = current_path.parent_path().parent_path();
    std::cout << "当前目录是: " << current_directory << std::endl;
    SkFILEWStream save_file((current_directory + PATH_SEPARATOR + "text_shadow.png").c_str()); //创建文件输出流
    if (!save_file.isValid()) {
        return 1;
    }
    SkPngEncoder::Encode(&save_file, bmp.pixmap(), {});
}