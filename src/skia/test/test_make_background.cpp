//
// Created by centforever on 2024/3/11.
//
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
#include "skia/file.h"
#include <iostream>
#include <string>
#include <filesystem>

SkBitmap make_background() {
    SkBitmap background;
    SkBitmap bitmap;
    bitmap.allocN32Pixels(32, 32);
    bitmap.eraseColor(SK_ColorWHITE);
    SkCanvas tmp(bitmap);
    SkPaint gray;
    gray.setColor(SkColorSetARGB(0xFF, 0xEE, 0xEE, 0xEE));
    tmp.drawRect({0, 0, 16, 16}, gray);
    tmp.drawRect({16, 16, 32, 32}, gray);
    SkPaint shader;
    shader.setShader(bitmap.makeShader(SkTileMode::kRepeat, SkTileMode::kRepeat, SkSamplingOptions()));
    background.allocN32Pixels(612, 792);
    SkCanvas tmp2(background);
    tmp2.drawPaint(shader);
    return background;
}

int main(int argc, char *const argv[]) {
    std::filesystem::path current_path = std::filesystem::current_path();
    std::string current_directory = current_path.parent_path().parent_path();
    std::cout << "当前目录是: " << current_directory << std::endl;

    SkBitmap bitmap = make_background();

    // 保存到文件
    SkFILEWStream file((current_directory + PATH_SEPARATOR + "test_sk_data.png").c_str());  //创建文件输出流
    if (!file.isValid()) {
        return 1;
    }
    return SkPngEncoder::Encode(&file, bitmap.pixmap(), {});
}