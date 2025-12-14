#include "skia/file.h"
#include <algorithm>
#include <include/codec/SkCodec.h>
#include <include/core/SkBitmap.h>
#include <include/core/SkCanvas.h>
#include <include/core/SkFont.h>
#include <include/core/SkFontMetrics.h>
#include <include/core/SkPaint.h>
#include <include/core/SkPath.h>
#include <include/core/SkStream.h>
#include <include/core/SkSurface.h>
#include <include/core/SkTextBlob.h>
#include <include/core/SkTypeface.h>
#include <include/core/SkPathBuilder.h>
#include <include/encode/SkPngEncoder.h>
#include <iostream>
#include <string>

#include <algorithm>
#include <cmath>
#include <string>
#include <sstream>

#include <algorithm>
#include <cmath>
#include <string>
#include <sstream>

// 辅助函数：将 SkScalar 转换为特定精度的字符串
std::string ScalarToString(SkScalar s, int precision = 2) {
  std::stringstream ss;
  ss.precision(precision);
  ss << std::fixed << s;
  return ss.str();
}

// 1. 定义 MakeBrushHeader 函数构建路径 (保持不变)
SkPath MakeBrushHeader() {
  SkPathBuilder builder;

  // 1. 移动到起点 (M命令)
  builder.moveTo(17.2941f, 0.852911f);

  // 2. 绘制7段三次贝塞尔曲线 (C命令 -> cubicTo)
  builder.cubicTo(9.36389f, -1.1742f, 4.90393f, 0.852925f, 3.41697f, 2.37323f);
  builder.cubicTo(-3.14972f, 9.08718f, 0.245049f, 16.2588f, 9.36433f, 37.3405f);
  builder.cubicTo(18.4836f, 58.4223f, 36.1271f, 85.484f, 44.5529f, 95.6194f);
  builder.cubicTo(46.8657f, 98.6601f, 54.366f, 104.336f, 65.8642f, 102.714f);
  builder.cubicTo(80.237f, 100.687f, 99.0703f, 73.3214f, 101.548f, 64.1995f);
  builder.cubicTo(103.889f, 55.5844f, 104.522f, 52.5437f, 94.6098f, 42.4083f);
  builder.cubicTo(84.1903f, 31.7538f, 26.7407f, 3.26762f, 17.2941f, 0.852911f);

  // 3. 闭合路径
  builder.close();

  return builder.detach();
}

void draw(SkCanvas* canvas) {
  SkPath brushPath = MakeBrushHeader();

  // 容器和偏移量
  const SkScalar containerSize = 103.0f;
  // 容器高度设置为 103.0f * 2 = 206.0f
  const SkRect containerRect = SkRect::MakeWH(containerSize, containerSize * 2);
  SkScalar offsetX = 0.0f;
  SkScalar offsetY = 0.0f;

  // 初始化画布状态
  canvas->drawColor(SK_ColorWHITE);
  canvas->save();
  canvas->translate(offsetX, offsetY);

  // --- 路径绘制 (蓝色填充和黑色轮廓) ---
  SkPaint fillPaint;
  fillPaint.setAntiAlias(true);
  fillPaint.setStyle(SkPaint::kFill_Style);
  fillPaint.setColor(SkColorSetRGB(66, 133, 244));
  canvas->drawPath(brushPath, fillPaint);

  SkPaint strokePaint;
  strokePaint.setAntiAlias(true);
  strokePaint.setStyle(SkPaint::kStroke_Style);
  strokePaint.setColor(SK_ColorBLACK);
  strokePaint.setStrokeWidth(1.5f);
  // canvas->drawPath(brushPath, strokePaint);

  // =========================================================
  // --- 【参数定义】 ---
  // =========================================================


  const SkScalar centerX = 65.5f;
  const SkScalar centerY = 65.0f;
  const SkScalar MAX_INNER_RADIUS = 38.5f; // 线宽值
  const SkScalar LINE_LENGTH = 80.0f; // 直线长度
  // 3. 设置画笔：内圆 (使用灰色)
  SkPaint innerCirclePaint;
  innerCirclePaint.setAntiAlias(true);
  innerCirclePaint.setStyle(SkPaint::kFill_Style);
  innerCirclePaint.setColor(SK_ColorGRAY); // 按照要求使用灰色
  // 4. 绘制圆
  canvas->drawCircle(centerX, centerY, MAX_INNER_RADIUS, innerCirclePaint);

  // =========================================================
  // --- 【替换为圆角直线】 ---
  // =========================================================

  // 1. 定义直线端点 (长度为 80，中心在 centerX, centerY)
  const SkScalar startX = centerX + MAX_INNER_RADIUS; // 63.0 - 40.0 = 23.0f
  const SkScalar endX   = centerX + LINE_LENGTH; // 63.0 + 40.0 = 103.0f
  const SkScalar lineY  = centerY;

  // 2. 创建用于绘制直线的画笔
  SkPaint linePaint;
  linePaint.setAntiAlias(true);
  linePaint.setColor(SK_ColorRED);
  linePaint.setStyle(SkPaint::kStroke_Style);

  // 设置线宽为 MAX_INNER_RADIUS
  linePaint.setStrokeWidth(MAX_INNER_RADIUS * 2);

  // 设置圆角端点 (实现 "圆角直线")
  linePaint.setStrokeCap(SkPaint::kRound_Cap);

  // 3. 绘制直线
  canvas->drawLine(startX, lineY, endX, lineY, linePaint);

  // 4. 绘制绿色点标记中心位置
  SkPaint centroidDotPaint;
  centroidDotPaint.setAntiAlias(true);
  centroidDotPaint.setColor(SK_ColorGREEN);
  // canvas->drawCircle(centerX, centerY, 5.0f, centroidDotPaint);
  canvas->restore();
}

// --- 4. 文件保存和初始化 (文件 I/O 逻辑) ---
// SkBitmap ScaleBitmap 函数 CC = 1
SkBitmap ScaleBitmap(const SkBitmap &src, float sx, float sy) {
  int width = (int)round(src.width() * sx);
  int height = (int)round(src.height() * sy);
  SkImageInfo image_info = SkImageInfo::Make(
      width, height, kRGBA_8888_SkColorType, kOpaque_SkAlphaType);
  SkBitmap dst;
  dst.setInfo(image_info, image_info.minRowBytes());
  dst.allocPixels(image_info, image_info.minRowBytes());
  SkCanvas canvas(dst);
  canvas.scale(sx, sy);
  canvas.drawImage(src.asImage(), 0, 0);
  return dst;
}

// main 函数 CC = 2 (包含 1 个 if)
int main() {
  // 1. 初始化 SkBitmap 和 SkCanvas
  SkBitmap canvasBitmap;
  // 设置画布尺寸 (540x300)，适应横向进度条
  SkImageInfo imageInfo =
      SkImageInfo::Make(600, 300, kRGBA_8888_SkColorType, kPremul_SkAlphaType);
  canvasBitmap.allocPixels(imageInfo, imageInfo.minRowBytes());

  SkCanvas canvas(canvasBitmap);

  // 2. 调用绘图函数
  draw(&canvas);

  // 3. 缩放 (保持 1.0f 不缩放)
  SkBitmap scaleBitmap = ScaleBitmap(canvasBitmap, 1.0f, 1.0f);

  // 4. 保存到文件
  std::string fileName = "path_test2.2.png";
  std::string current_directory = File::get_current_directory();
  std::filesystem::path current_path = std::filesystem::current_path();
  current_directory = current_path.parent_path().parent_path();
  std::cout << "当前目录是: " << current_directory << std::endl;

  std::string fullPath = current_directory + PATH_SEPARATOR + fileName;
  SkFILEWStream save_file(fullPath.c_str());

  // 检查文件是否成功打开
  if (!save_file.isValid()) { // CC +1
    std::cerr << "错误: 无法打开文件进行写入: " << fullPath << std::endl;
    return 1;
  }

  // 编码并保存 PNG 文件
  bool success = SkPngEncoder::Encode(&save_file, scaleBitmap.pixmap(), {});

  if (success) {
    std::cout << "成功保存 PNG 文件到: " << fullPath << std::endl;
  } else {
    std::cerr << "错误: PNG 编码失败." << std::endl;
    return 1;
  }
  return 0;
}