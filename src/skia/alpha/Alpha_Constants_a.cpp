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
  std::vector<int32_t> srcPixels;
  srcPixels.resize(source.height() * source.rowBytes());
  SkPixmap pixmap(SkImageInfo::MakeN32Premul(source.width(), source.height()),
                  &srcPixels.front(), source.rowBytes());
  source.readPixels(pixmap, 0, 0);
  for (int y = 0; y < 16; ++y) {
    for (int x = 0; x < 16; ++x) {
      int32_t *blockStart =
          &srcPixels.front() + y * source.width() * 16 + x * 16;
      size_t transparentCount = 0;
      for (int fillY = 0; fillY < source.height() / 16; ++fillY) {
        for (int fillX = 0; fillX < source.width() / 16; ++fillX) {
          const SkColor color =
              SkUnPreMultiply::PMColorToColor(blockStart[fillX]);
          transparentCount += SkColorGetA(color) == SK_AlphaTRANSPARENT;
        }
        blockStart += source.width();
      }
      std::cout << "transparentCount :" << transparentCount << std::endl;
      if (transparentCount > 200) {
        blockStart = &srcPixels.front() + y * source.width() * 16 + x * 16;
        for (int fillY = 0; fillY < source.height() / 16; ++fillY) {
          for (int fillX = 0; fillX < source.width() / 16; ++fillX) {
            blockStart[fillX] = SK_ColorRED;
          }
          blockStart += source.width();
        }
      }
    }
  }
  canvas->drawImage(SkImages::RasterFromPixmapCopy(pixmap), 0, 0);
}

int main(int argc, char *const argv[]) {
  SkString path = SkStringPrintf("resources/example_1.png");
  image =
      SkImages::DeferredFromEncodedData(SkData::MakeFromFileName(path.c_str()));
  SkAssertResult(image && image->asLegacyBitmap(&source));
  SkBitmap bmp;
  bmp.allocN32Pixels(256, 128);
  bmp.eraseColor(SK_ColorWHITE);
  SkCanvas canvas(bmp);
  draw(&canvas);

  std::string current_directory = File::get_current_directory();
  std::filesystem::path current_path = std::filesystem::current_path();
  current_directory = current_path.parent_path().parent_path();
  std::cout << "当前目录是: " << current_directory << std::endl;
  SkFILEWStream save_file(
      (current_directory + PATH_SEPARATOR + "Alpha_Constants_a.png")
          .c_str()); // 创建文件输出流
  if (!save_file.isValid()) {
    return 1;
  }
  SkPngEncoder::Encode(&save_file, bmp.pixmap(), {});
}
