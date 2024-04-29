#include "include/core/SkData.h"
#include "include/core/SkImage.h"
#include "include/core/SkShader.h"
#include "include/core/SkTileMode.h"
#include "include/core/SkUnPreMultiply.h"
#include "include/gpu/GrDirectContext.h"
#include "skia/file.h"
#include <filesystem>
#include <include/codec/SkCodec.h>
#include <include/core/SkBitmap.h>
#include <include/core/SkCanvas.h>
#include <include/core/SkFont.h>
#include <include/core/SkPaint.h>
#include <include/core/SkPath.h>
#include <include/core/SkStream.h>
#include <include/core/SkSurface.h>
#include <include/core/SkTextBlob.h>
#include <include/encode/SkPngEncoder.h>
#include <iostream>
#include <string>

SkBitmap source;
sk_sp<SkImage> image;

void draw(SkCanvas *canvas) {
  uint8_t set1[5] = {0xCA, 0xDA, 0xCA, 0xC9, 0xA3};
  uint8_t set2[5] = {0xAC, 0xA8, 0x89, 0x47, 0x87};
  SkBitmap bitmap;
  bitmap.installPixels(
      SkImageInfo::Make(5, 1, kGray_8_SkColorType, kOpaque_SkAlphaType), set1,
      5);
  canvas->scale(10, 50);
  canvas->drawImage(bitmap.asImage(), 0, 0);
  bitmap.allocPixels();
  bitmap.eraseColor(SK_ColorBLACK);
  canvas->drawImage(bitmap.asImage(), 8, 0);
  bitmap.setPixels(set2);
  canvas->drawImage(bitmap.asImage(), 16, 0);
}

int main(int argc, char *const argv[]) {
  SkString path = SkStringPrintf("resources/example_3.png");
  image =
      SkImages::DeferredFromEncodedData(SkData::MakeFromFileName(path.c_str()));
  SkAssertResult(image && image->asLegacyBitmap(&source));
  SkBitmap bmp;
  bmp.allocN32Pixels(256, 256);
  SkCanvas canvas(bmp);
  draw(&canvas);

  std::string current_directory = File::get_current_directory();
  std::filesystem::path current_path = std::filesystem::current_path();
  current_directory = current_path.parent_path().parent_path();
  std::cout << "当前目录是: " << current_directory << std::endl;
  SkFILEWStream save_file(
      (current_directory + PATH_SEPARATOR + "Bitmap_allocPixels_3.png")
          .c_str()); // 创建文件输出流
  if (!save_file.isValid()) {
    return 1;
  }
  SkPngEncoder::Encode(&save_file, bmp.pixmap(), {});
}