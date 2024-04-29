#include "skia/file.h"
#include <filesystem>
#include <include/core/SkBitmap.h>
#include <include/core/SkCanvas.h>
#include <include/core/SkColorFilter.h>
#include <include/core/SkGraphics.h>
#include <include/core/SkPath.h>
#include <include/core/SkRegion.h>
#include <include/core/SkShader.h>
#include <include/core/SkStream.h>
#include <include/core/SkTextBlob.h>
#include <include/core/SkTypeface.h>
#include <include/encode/SkPngEncoder.h>
#include <iostream>
#include <istream>
#include <modules/skparagraph/include/FontCollection.h>
#include <modules/skparagraph/include/ParagraphBuilder.h>
#include <modules/skparagraph/include/TextStyle.h>
#include <modules/skparagraph/include/TypefaceFontProvider.h>
#include <src/core/SkOSFile.h>

std::string draw(SkCanvas *canvas) {
  // Clear background
  canvas->clear(SK_ColorWHITE);
  auto path_tf = SkTypeface::MakeFromFile("resources/微软雅黑.ttf");
  sk_sp<skia::textlayout::TypefaceFontProvider> font_provider =
      sk_make_sp<skia::textlayout::TypefaceFontProvider>();
  font_provider->registerTypeface(path_tf);
  // Draw a message with a nice black paint
  SkFont font;
  font.setTypeface(path_tf);
  font.setSubpixel(true);
  font.setSize(16);
  SkPaint paint;
  paint.setColor(SK_ColorBLACK);
  paint.setAntiAlias(true);

  sk_sp<SkFontMgr> pMgr = (SkFontMgr::RefDefault());
  int FamilyCount = pMgr->countFamilies();
  for (int j = 0; j < FamilyCount; j++) {
    SkString familyName;
    pMgr->getFamilyName(j, &familyName);
    sk_sp<SkTypeface> typeface =
        SkTypeface::MakeFromName(familyName.c_str(), SkFontStyle::Normal());
    if (typeface == nullptr) {
      continue;
    }
    font.setTypeface(typeface);
    const char str[] = "\xe4\xbd\xa0\xe5\xa5\xbd"; // 你好
    std::string test_support_font_message = std::string(str);
    test_support_font_message.append(familyName.c_str());
    canvas->drawString(test_support_font_message.c_str(), SkIntToScalar(16),
                       SkIntToScalar(16) * SkIntToScalar(j) + 16, font, paint);
    std::cout << "familyName: " << familyName.c_str() << std::endl;
  }

  return "hello_check_font.png";
}

int main() {
  auto fontProvider = sk_make_sp<skia::textlayout::TypefaceFontProvider>();
  SkOSFile::Iter iter("resources");
  SkString path;
  while (iter.next(&path)) {
    SkString file_path;
    file_path.printf("%s/%s", "resources", path.c_str());
    std::cout << "file_path = " << file_path.c_str() << std::endl;
    fontProvider->registerTypeface(SkTypeface::MakeFromFile(file_path.c_str()));
  }

  SkBitmap canvasBitmap;
  SkImageInfo imageInfo =
      SkImageInfo::Make(1080, 1920 * 3, kBGRA_8888_SkColorType,
                        kPremul_SkAlphaType); // 设置位图信息
  canvasBitmap.allocPixels(
      imageInfo, imageInfo.minRowBytes()); // 为位图设备绑定信息和分配内存

  SkCanvas canvas(canvasBitmap);
  std::string fileName = draw(&canvas);

  // 将绘制结果保存到图片文件中
  std::string current_directory = File::get_current_directory();
  std::filesystem::path current_path = std::filesystem::current_path();
  current_directory = current_path.parent_path().parent_path();
  std::cout << "当前目录是: " << current_directory << std::endl;
  SkFILEWStream save_file((current_directory + PATH_SEPARATOR + fileName)
                              .c_str()); // 创建文件输出流
  if (!save_file.isValid()) {
    return 1;
  }
  SkPngEncoder::Encode(&save_file, canvasBitmap.pixmap(), {});
  return 0;
}