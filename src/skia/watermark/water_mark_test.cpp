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
#include "include/codec/SkPngDecoder.h"
#include <iostream>
#include <string>
#include <filesystem>
#include "skia/file.h"

bool loadImageToBitmap(std::string imagePath, SkBitmap *bitmap) {
    sk_sp<SkData> data = SkData::MakeFromFileName(imagePath.c_str());
    if (!data) {
        std::cerr << "Failed to create SkData from file: " << imagePath << std::endl;
        return false;
    }
    std::unique_ptr<SkCodec> test_codec = SkCodec::MakeFromData(data);
    // std::unique_ptr<SkCodec>  test_codec(SkCodec::MakeFromData(data));
    if (!test_codec) {
        std::cerr << "Failed to create SkCodec from SkData." << std::endl;
        return false;
    }
    SkCodec::Result result;
    std::unique_ptr<SkCodec> codec = SkPngDecoder::Decode(SkMemoryStream::Make(data), &result);
    if (result != SkCodec::Result::kSuccess) {
        std::string errorMsg = SkStringPrintf("Could not create codec for \"%s\": %s.",
                                              imagePath.c_str(),
                                              SkCodec::ResultToString(result))
                .c_str();
        std::cerr << "Failed to decode image: " << imagePath << std::endl;
        return false;
    }

    // 设置SkBitmap的信息
    SkImageInfo info = codec->getInfo().makeColorType(kN32_SkColorType);
    bitmap->allocPixels(info);

    // 解码图片数据到SkBitmap
    if (codec->getPixels(info, bitmap->getPixels(), bitmap->rowBytes()) != SkCodec::kSuccess) {
        std::cerr << "Failed to decode image: " << imagePath << std::endl;
        return false;
    }

    return true;
}

/*static void write_png(const char *path, sk_sp<SkImage> img) {
    sk_sp<SkData> png = SkPngEncoder::Encode(nullptr, img.get(), {});
    SkASSERT(png);
    SkFILEWStream(path).write(png->data(), png->size());
}*/

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

void DrawBackgroundBitmap(const SkBitmap &bitmap, SkCanvas *canvas, const SkRect *prcDst) {
    SkSamplingOptions sampling;
    sk_sp<SkShader> bg_shader = bitmap.makeShader(SkTileMode::kRepeat,
                                                  SkTileMode::kRepeat, sampling, nullptr);
    SkPaint bg_paint;
    bg_paint.setShader(bg_shader);
    canvas->drawRect(*prcDst, bg_paint);
}

void DrawBackgroundBitmap(const SkBitmap &bitmap, SkCanvas *canvas, float l, float t, float r, float b) {
    SkRect rect = SkRect::MakeLTRB(l, t, r, b);
    DrawBackgroundBitmap(bitmap, canvas, &rect);
}

int main() {
    //加载图片资源
    SkBitmap canvasBitmap;
    SkImageInfo imageInfo = SkImageInfo::Make(640, 640, kBGRA_8888_SkColorType, kPremul_SkAlphaType);  //设置位图信息
    canvasBitmap.allocPixels(imageInfo, imageInfo.minRowBytes());  //为位图设备绑定信息和分配内存
    std::string file = "resources/watermark_test.png";
    if (std::filesystem::exists(file)) {
        std::cerr << "File found: " << file << std::endl;
    } else {
        std::cerr << "File not found: " << file << std::endl;
    }
    SkBitmap image_bitmap;
    if (!loadImageToBitmap(file, &image_bitmap)) {
        std::cerr << "Failed to load image to SkBitmap." << std::endl;
        return 1;
    }

    // 定义绘制区域
    float left = 0.0f, top = 0.0f, right = 640.0f, bottom = 640.0f;

    SkCanvas canvas(canvasBitmap);
    // 在SkCanvas上绘制背景图像
    DrawBackgroundBitmap(image_bitmap, &canvas, left, top, right, bottom);


    //将绘制结果保存到图片文件中
    std::string current_directory = File::get_current_directory();
    std::filesystem::path current_path = std::filesystem::current_path();
    current_directory = current_path.parent_path().parent_path();
    std::cout << "当前目录是: " << current_directory << std::endl;
    SkFILEWStream save_file((current_directory + PATH_SEPARATOR + "watermark_test_result.png").c_str()); //创建文件输出流
    if (!save_file.isValid()) {
        return 1;
    }
    SkPngEncoder::Encode(&save_file, canvasBitmap.pixmap(), {});
}
