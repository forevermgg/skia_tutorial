#include "include/core/SkData.h"
#include "include/core/SkImage.h"
#include "include/core/SkShader.h"
#include "include/core/SkTileMode.h"
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

static sk_sp<SkShader> make_bw_dither() {
  // 创建一个宽度为 2，高度为 2 的新 SkSurface
  // 对象。这将创建一个用于绘制的位图画布。
  auto surf = SkSurfaces::Raster(SkImageInfo::MakeN32Premul(2, 2));
  // 将画布的颜色设置为白色。这将清除画布上的任何内容，并将其填充为白色。
  surf->getCanvas()->drawColor(SK_ColorWHITE);
  // 使用默认的 SkPaint
  // 对象（默认为黑色），绘制一个矩形。矩形的左上角位于原点，宽度和高度都是 1。
  surf->getCanvas()->drawRect({0, 0, 1, 1}, SkPaint());

  // 使用默认的 SkPaint 对象（默认为黑色），绘制另一个矩形。矩形的左上角位于 (1,
  // 1)，宽度和高度都是 1。
  surf->getCanvas()->drawRect({1, 1, 2, 2}, SkPaint());

  // 从 SkSurface 对象创建一个 SkImage 快照，然后从 SkImage 创建一个 SkShader
  // 对象。 SkShader 的水平和垂直平铺模式都设置为 kRepeat，采样选项设置为
  // kLinear。
  return surf->makeImageSnapshot()->makeShader(
      SkTileMode::kRepeat, SkTileMode::kRepeat,
      SkSamplingOptions(SkFilterMode::kLinear));
}

void draw(SkCanvas *canvas) {
  // 将画布的颜色设置为白色。这将清除画布上的任何内容，并将其填充为白色。
  canvas->drawColor(SK_ColorWHITE);
  // 创建一个新的 SkFont
  // 对象，用于绘制文本。这个对象使用默认的字体家族和指定的字体大小（在这里是
  // 12）。
  SkFont font(nullptr, 12);

  // BW Dither
  // 在画布上移动原点（0,0）。在这个例子中，我们将原点向右移动 5 单位，向下移动
  // 5 单位。
  canvas->translate(5, 5);
  // 创建一个新的 SkPaint
  // 对象，用于绘制形状。这个对象的着色器被设置为一个黑白抖动模式。
  SkPaint p;
  p.setShader(make_bw_dither());
  // 使用之前创建的 SkPaint
  // 对象，绘制一个矩形。矩形的左上角位于原点，宽度和高度都是 100。
  canvas->drawRect({0, 0, 100, 100}, p);
  // 创建一个新的 SkPaint 对象，用于绘制黑色文本。
  SkPaint black;
  // 使用之前创建的 SkFont 对象和 SkPaint
  // 对象，绘制一行文本。文本的左下角位于原点，向右移动 0 单位，向下移动 125
  // 单位。
  canvas->drawString("BW Dither", 0, 125, font, black);

  // Scaled BW Dither
  // 在画布上移动原点。在这个例子中，我们将原点向右移动 105 单位，向下移动 0
  // 单位。
  canvas->translate(105, 0);
  // 保存当前画布的状态。这将允许我们在稍后恢复画布的状态。
  canvas->save();
  // 缩放画布。在这个例子中，我们将画布的宽度和高度都缩小一半。
  canvas->scale(0.5, 0.5);
  // 使用之前创建的 SkPaint
  // 对象，绘制一个矩形。矩形的左上角位于原点，宽度和高度都是 200。
  // 由于画布被缩放，实际绘制的矩形宽度和高度将分别为 100 和 100。
  canvas->drawRect({0, 0, 200, 200}, p);
  // 恢复画布的状态。这将撤消之前对画布的缩放操作。
  canvas->restore();
  // 使用之前创建的 SkFont 对象和 SkPaint
  // 对象，绘制一行文本。文本的左下角位于原点，向右移动 0 单位，向下移动 125
  // 单位。
  canvas->drawString("Scaled Dither", 0, 125, font, black);

  // Blend black on to white
  // 将画布的原点向右移动 105 个单位。
  canvas->translate(105, 0);
  // 设置 SkPaint 对象的颜色为半透明黑色（0x80000000）。
  p.setColor(0x80000000);
  // 将 SkPaint 对象的着色器设置为 nullptr，以便绘制纯色。
  p.setShader(nullptr);
  // 使用之前设置的半透明黑色 SkPaint
  // 对象，绘制一个矩形。矩形的左上角位于原点，宽度和高度都是 100。
  canvas->drawRect({0, 0, 100, 100}, p);
  // 在新位置绘制一行文本。
  canvas->drawString("Blend", 0, 125, font, black);

  // Opaque color (0xFFBCBCBC)
  // 将画布的原点向右移动 105 个单位。
  canvas->translate(105, 0);
  // 设置 SkPaint 对象的颜色为不透明的灰色（0xFFBCBCBC）。
  p.setColor(0xFFBCBCBC);
  // 使用之前设置的不透明灰色 SkPaint
  // 对象，绘制一个矩形。矩形的左上角位于原点，宽度和高度都是 100。
  canvas->drawRect({0, 0, 100, 100}, p);
  // 在新位置绘制一行文本。
  canvas->drawString("0xFFBCBCBC", 0, 125, font, black);

  // Opaque color (0xFF808080)
  // 将画布的原点向右移动 105 个单位。
  canvas->translate(105, 0);
  // 设置 SkPaint 对象的颜色为不透明的深灰色（0xFF808080）
  p.setColor(0xFF808080);
  // 使用之前设置的不透明深灰色 SkPaint
  // 对象，绘制一个矩形。矩形的左上角位于原点，宽度和高度都是 100。
  canvas->drawRect({0, 0, 100, 100}, p);
  // 在新位置绘制一行文本。
  canvas->drawString("0xFF808080", 0, 125, font, black);
}

int main(int argc, char *const argv[]) {
  SkBitmap bitmap;
  SkImageInfo imageInfo =
      SkImageInfo::Make(1000, 200, kBGRA_8888_SkColorType, kPremul_SkAlphaType);
  bitmap.allocPixels(imageInfo, imageInfo.minRowBytes());

  SkCanvas canvas(bitmap);
  draw(&canvas);
  std::string current_directory = File::get_current_directory();
  std::filesystem::path current_path = std::filesystem::current_path();
  current_directory = current_path.parent_path().parent_path();
  std::cout << "当前目录是: " << current_directory << std::endl;
  SkFILEWStream save_file(
      (current_directory + PATH_SEPARATOR + "50_percent_gray.png")
          .c_str()); // 创建文件输出流
  if (!save_file.isValid()) {
    return 1;
  }
  SkPngEncoder::Encode(&save_file, bitmap.pixmap(), {});
}