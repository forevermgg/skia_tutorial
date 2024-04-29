#include "../file.h"
#include "include/core/SkData.h"
#include "include/core/SkImage.h"
#include "include/gpu/GrDirectContext.h"
#include "include/gpu/ganesh/SkSurfaceGanesh.h"
#include "include/gpu/gl/GrGLInterface.h"
#include <include/codec/SkCodec.h>
#include <include/core/SkBitmap.h>
#include <include/core/SkCanvas.h>
#include <include/core/SkFont.h>
#include <include/core/SkPaint.h>
#include <include/core/SkPath.h>
#include <include/core/SkRRect.h>
#include <include/core/SkRect.h>
#include <include/core/SkStream.h>
#include <include/core/SkSurface.h>
#include <include/core/SkTextBlob.h>
#include <include/encode/SkPngEncoder.h>

int main() {
  // 创建一个位图设备
  SkBitmap bitmap;
  SkImageInfo imageInfo = SkImageInfo::Make(
      400, 400, kBGRA_8888_SkColorType, kPremul_SkAlphaType); // 设置位图信息
  bitmap.allocPixels(imageInfo,
                     imageInfo.minRowBytes()); // 为位图设备绑定信息和分配内存
  // 创建画布
  SkCanvas canvas(bitmap);
  // 创建一个带有透明度的颜色
  // canvas.clear(SkColorSetARGB(0xFF, 0x14, 0x14, 0x14));
  SkColor color = SkColorSetARGB(0xFF, 0x14, 0x14, 0x14);
  // 创建一个圆角矩形
  SkRect rect = SkRect::MakeXYWH(50.0f, 50.0f, 300.0f, 300.0f);
  SkRRect rrect;
  rrect.setRectXY(rect, 20, 20);
  // 创建一个画笔
  SkPaint paint;
  paint.setColor(color);
  // 绘制圆角矩形
  canvas.drawRRect(rrect, paint);
  // 将绘制结果保存到图片文件中
  std::string current_directory = File::get_current_directory();
  std::filesystem::path current_path = std::filesystem::current_path();
  current_directory = current_path.parent_path().parent_path();
  std::cout << "当前目录是: " << current_directory << std::endl;
  SkFILEWStream file(
      (current_directory + PATH_SEPARATOR + "drawable_circle_rect.png")
          .c_str()); // 创建文件输出流
  if (!file.isValid()) {
    return 1;
  }
  SkPngEncoder::Encode(&file, bitmap.pixmap(), {});
  return 0;
}