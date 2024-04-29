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
  for (int y = 0; y < source.height(); ++y) {
    for (int x = 0; x < source.width(); ++x) {
      SkPMColor pixel = srcPixels[y * source.width() + x];
      const SkColor color = SkUnPreMultiply::PMColorToColor(pixel);
      if (SkColorGetA(color) == SK_AlphaOPAQUE) {
        srcPixels[y * source.width() + x] = SK_ColorGREEN;
      }
    }
  }
  canvas->drawImage(SkImages::RasterFromPixmapCopy(pixmap), 0, 0);
}

int main(int argc, char *const argv[]) {
  SkString path = SkStringPrintf("resources/example_2.png");
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
      (current_directory + PATH_SEPARATOR + "Alpha_Constants_b.png")
          .c_str()); // 创建文件输出流
  if (!save_file.isValid()) {
    return 1;
  }
  SkPngEncoder::Encode(&save_file, bmp.pixmap(), {});
}
