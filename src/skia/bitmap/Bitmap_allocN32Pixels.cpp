#include "../file.h"
#include "include/core/SkImage.h"
#include <cmath>
#include <include/codec/SkCodec.h>
#include <include/core/SkBitmap.h>
#include <include/core/SkCanvas.h>
#include <include/core/SkFont.h>
#include <include/core/SkPaint.h>
#include <include/core/SkPath.h>
#include <include/core/SkRRect.h>
#include <include/core/SkStream.h>
#include <include/core/SkTextBlob.h>
#include <include/core/SkTypeface.h>
#include <include/encode/SkPngEncoder.h>
#include <iomanip>
#include <iostream>
#include <map>
#include <random>
#include <string>

void draw(SkCanvas *canvas) {
  SkBitmap bitmap;
  bitmap.allocN32Pixels(64, 64);
  bitmap.eraseColor(SK_ColorTRANSPARENT);
  std::default_random_engine rng;
  const auto randUint = [&rng](uint32_t min, uint32_t max) -> uint32_t {
    return std::uniform_int_distribution<uint32_t>(min, max)(rng);
  };

  for (int y = 0; y < 256; y += 64) {
    for (int x = 0; x < 256; x += 64) {
      SkColor color = randUint(0, UINT_MAX);
      uint32_t w = randUint(4, 32);
      uint32_t cx = randUint(0, 64 - w);
      uint32_t h = randUint(4, 32);
      uint32_t cy = randUint(0, 64 - h);
      bitmap.erase(color, SkIRect::MakeXYWH(cx, cy, w, h));
      canvas->drawImage(bitmap.asImage(), x, y);
    }
  }
}

int main(int argc, char *const argv[]) {
  SkBitmap bitmap;
  SkImageInfo imageInfo = SkImageInfo::Make(
      480, 320, kBGRA_8888_SkColorType, kPremul_SkAlphaType); // 设置位图信息
  bitmap.allocPixels(imageInfo,
                     imageInfo.minRowBytes()); // 为位图设备绑定信息和分配内存

  // 创建画布
  SkCanvas canvas(bitmap);

  draw(&canvas);
  std::string current_directory = File::get_current_directory();
  std::filesystem::path current_path = std::filesystem::current_path();
  current_directory = current_path.parent_path().parent_path();
  std::cout << "当前目录是: " << current_directory << std::endl;

  // 保存到文件
  SkFILEWStream file(
      (current_directory + PATH_SEPARATOR + "Bitmap_allocN32Pixels.png")
          .c_str()); // 创建文件输出流
  if (!file.isValid()) {
    return 1;
  }
  return SkPngEncoder::Encode(&file, bitmap.pixmap(), {});
}