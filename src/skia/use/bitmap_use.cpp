#include "../file.h"
#include <include/codec/SkCodec.h>
#include <include/core/SkBitmap.h>
#include <include/core/SkCanvas.h>
#include <include/core/SkFont.h>
#include <include/core/SkPaint.h>
#include <include/core/SkStream.h>
#include <include/core/SkTypeface.h>
#include <include/encode/SkPngEncoder.h>
#include <iostream>

int main(int argc, char *const argv[]) {
  std::filesystem::path current_path = std::filesystem::current_path();
  std::string current_directory = current_path.parent_path().parent_path();
  std::cout << "当前目录是: " << current_directory << std::endl;

  // 1：第一阶段 创建格外的Canvas
  SkBitmap m_bitmap;
  m_bitmap.rowBytes();  // size_t
  m_bitmap.getPixels(); // void*
  int canvas_width = 100;
  int canvas_height = 100;
  std::string canvas_data;
  size_t nRowBytes = 0;
  bool need_unpremul = false;
  constexpr SkColorType kDefaultColorType = kRGBA_8888_SkColorType;
  size_t length = canvas_width * canvas_height * 4;
  canvas_data.reserve(length);
  std::fill(canvas_data.begin(), canvas_data.end(), '\0');
  SkImageInfo image_info =
      SkImageInfo::Make(canvas_width, canvas_height, kDefaultColorType,
                        need_unpremul ? SkAlphaType::kUnpremul_SkAlphaType
                                      : SkAlphaType::kOpaque_SkAlphaType);
  m_bitmap.setInfo(image_info,
                   nRowBytes != 0 ? nRowBytes : image_info.minRowBytes());
  m_bitmap.setPixels(canvas_data.data());

  // 2：第二阶段 绘制数据
  // 创建画布
  SkCanvas canvas(m_bitmap);
  // 创建画笔
  SkPaint paint;
  canvas.clear(SkColorSetARGB(0xFF, 0x14, 0x14, 0x14));
  // 将画布清空并填充一种颜色，注意这里是ARGB，Alpha通道值在第一位，同时可以直接用16进制数表示，例如上面这个颜色值可以表示为0xFF141414
  paint.setColor(SK_ColorWHITE);
  // 创建字体设备
  SkFont font;
  // 设置字体尺寸
  font.setSize(14);
  SkString text("Hello, Skia!");
  // 在画布上绘制字体
  canvas.drawSimpleText(text.c_str(), text.size(), SkTextEncoding::kUTF8, 0, 64,
                        font, paint);

  // 3：第三阶段 生成回调数据
  void *data = m_bitmap.getPixels();
  int width = canvas.imageInfo().width();
  int height = canvas.imageInfo().height();
  size_t rowBytes = nRowBytes;
  SkColorType colorType = SkColorType::kRGBA_8888_SkColorType;
  SkAlphaType alphaType = need_unpremul ? SkAlphaType::kUnpremul_SkAlphaType
                                        : SkAlphaType::kOpaque_SkAlphaType;

  // 4：第四阶段 获取数据恢复SkBitmap
  SkBitmap bitmap;
  SkImageInfo imageInfo =
      SkImageInfo::Make(width, height, colorType, alphaType);
  bitmap.setInfo(imageInfo, rowBytes);
  bitmap.setPixels(data);
  bitmap.asImage().

      // 5:第五阶段 保存数据，查看结果。
      SkFILEWStream file((current_directory + PATH_SEPARATOR + "bitmap_use.png")
                             .c_str()); // 创建文件输出流
  if (!file.isValid()) {
    return 1;
  }
  return SkPngEncoder::Encode(&file, bitmap.pixmap(), {});
}