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
#include "../file.h"

static void write_png(const char *path, sk_sp<SkImage> img) {
    sk_sp<SkData> png = SkPngEncoder::Encode(nullptr, img.get(), {});
    SkASSERT(png);
    SkFILEWStream(path).write(png->data(), png->size());
}

sk_sp<SkImage> make_raster_image(const char *path) {
    SkFILEStream stream(path);
    if (!stream.isValid()) {
        return nullptr;
    }

    auto data = SkData::MakeFromFileName(path);
    if (!data) {
        return nullptr;
    }
    sk_sp<SkImage> image = SkImages::DeferredFromEncodedData(data);
    if (!image) {
        SkDebugf("Couldn't decode %s as an SkImage or an ICC profile.\n", path);
        return nullptr;
    }
    return nullptr;
}

void draw(SkCanvas *canvas, sk_sp<SkImage> &image) {
    canvas->clear(SK_ColorWHITE);
    SkMatrix matrix;
    matrix.setScale(0.75f, 0.75f);
    matrix.preRotate(45.0f);
    SkPaint paint;
    paint.setShader(image->makeShader(SkTileMode::kRepeat, SkTileMode::kRepeat,
                                      SkSamplingOptions(), matrix));
    canvas->drawPaint(paint);
}

int main() {
    // 创建一个位图设备
    SkBitmap bitmap;
    SkImageInfo imageInfo = SkImageInfo::Make(400, 400, kBGRA_8888_SkColorType, kPremul_SkAlphaType); //设置位图信息
    bitmap.allocPixels(imageInfo, imageInfo.minRowBytes()); //为位图设备绑定信息和分配内存
    // 创建画布
    SkCanvas canvas(bitmap);

    sk_sp<SkImage> image = make_raster_image(
        "/Users/centforever/CLionProjects/skia_tutorial/src/skia/watermark/watermark_test.png");
    if (image) {
        draw(&canvas, image);
    }

    //将绘制结果保存到图片文件中
    std::string current_directory = File::get_current_directory();
    std::filesystem::path current_path = std::filesystem::current_path();
    current_directory = current_path.parent_path().parent_path();
    std::cout << "当前目录是: " << current_directory << std::endl;
    SkFILEWStream file((current_directory + PATH_SEPARATOR + "watermark_test_result.png").c_str()); //创建文件输出流
    if (!file.isValid()) {
        return 1;
    }
    SkPngEncoder::Encode(&file, bitmap.pixmap(), {});
    return 0;
}
