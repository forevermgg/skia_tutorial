//
// Created by centforever on 2024/3/11.
//
#include <include/core/SkCanvas.h>
#include <include/core/SkBitmap.h>
#include <include/core/SkPaint.h>
#include <include/core/SkTypeface.h>
#include <include/core/SkFont.h>
#include <include/codec/SkCodec.h>
#include <include/core/SkStream.h>
#include <include/encode/SkPngEncoder.h>
#include "../file.h"

int main(int argc, char *const argv[]) {
    std::filesystem::path current_path = std::filesystem::current_path();
    std::string current_directory = current_path.parent_path().parent_path();
    std::cout << "当前目录是: " << current_directory << std::endl;
    return 1;
}