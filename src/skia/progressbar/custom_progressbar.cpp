#include "skia/file.h"
#include <algorithm>
#include <cmath>
#include <filesystem> // For std::filesystem::path and current_path()
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
#include <include/encode/SkPngEncoder.h>
#include <iostream>
#include <string>

// --- 1. 形状修改：生成左细右粗的横向路径 (增加参数) ---
// 圈复杂度 (CC) = 1
SkPath make_tapered_progressbar_path(const SkRect &rect, float h_left_ratio,
                                     float h_right_ratio) {
  SkPath path;
  const float x = rect.fLeft;
  const float y = rect.fTop;
  const float w = rect.width();
  const float h_base = rect.height(); // 基准高度 (用于垂直定位)

  // 计算实际的左右两端高度 (可调参数)
  const float h_left = h_base * h_left_ratio;
  const float h_right = h_base * h_right_ratio;

  // 半径：取对应高度的一半
  const float radius_left = h_left / 2.0f;
  const float radius_right = h_right / 2.0f;

  // Y 偏移：将形状垂直居中于 rect 的高度 h_base (主要用于定位)
  const float y_offset_left = (h_base - h_left) / 2.0f;
  const float y_offset_right = (h_base - h_right) / 2.0f;

  // 左端边界 (Top/Bottom)
  const float left_top_y = y + y_offset_left;
  const float left_bottom_y = y + y_offset_left + h_left;

  // 右端边界 (Top/Bottom)
  const float right_top_y = y + y_offset_right;

  // 1. 从左侧半圆的顶部开始
  path.moveTo(x + radius_left, left_top_y);

  // 2. 绘制上侧直线段 (连接左侧顶部和右侧顶部)
  path.lineTo(x + w - radius_right, right_top_y);

  // 3. 绘制右侧半圆弧 (右侧封口)
  // 边界矩形：从 x+w-h_right 开始，宽度和高度都为 h_right
  SkRect right_rect =
      SkRect::MakeXYWH(x + w - h_right, right_top_y, h_right, h_right);
  path.arcTo(right_rect, 270, 180, false); // 270度开始，顺时针 180度

  // 4. 绘制下侧直线段 (连接右侧底部和左侧底部)
  path.lineTo(x + radius_left, left_bottom_y);

  // 5. 绘制左侧半圆弧 (左侧封口)
  // 边界矩形：从 x 开始，宽度和高度都为 h_left
  SkRect left_rect = SkRect::MakeXYWH(x, left_top_y, h_left, h_left);
  path.arcTo(left_rect, 90, 180, false); // 90度开始，顺时针 180度

  path.close();
  return path;
}

// --- 2. 进度条绘制函数 (添加抗锯齿) ---
// 圈复杂度 (CC) = 3
void draw_horizontal_progressbar(SkCanvas *canvas, const SkRect &rect,
                                 float progress, float h_left_ratio,
                                 float h_right_ratio) {
  progress = std::max(0.0f, std::min(100.0f, progress)); // CC +1

  // 获取梯形轮廓 (传递新的高度比例参数)
  SkPath trackPath =
      make_tapered_progressbar_path(rect, h_left_ratio, h_right_ratio);

  // 定义统一的深蓝色
  const SkColor kDarkBlue = SkColorSetARGB(255, 40, 75, 120);

  // --- A. 绘制背景轨道 (Track) ---
  SkPaint trackPaint;
  trackPaint.setColor(SkColorSetARGB(255, 230, 230, 230));
  trackPaint.setStyle(SkPaint::kFill_Style);
  trackPaint.setAntiAlias(true); // 💡 优化：抗锯齿
  canvas->drawPath(trackPath, trackPaint);

  // --- B. 准备裁剪区域 (Clipping) ---
  canvas->save();
  // 裁剪区域为梯形形状
  canvas->clipPath(trackPath, true);

  // --- C. 绘制进度填充 (Fill) ---
  SkPaint fillPaint;
  fillPaint.setColor(SkColorSetARGB(255, 0, 160, 255));
  fillPaint.setStyle(SkPaint::kFill_Style);
  fillPaint.setAntiAlias(true); // 💡 优化：抗锯齿

  // 进度条的填充范围 (从左侧开始，宽度为 fillWidth)
  float fillWidth = rect.width() * (progress / 100.0f);

  // 创建填充矩形：使用一个覆盖整个高度的矩形，让裁剪路径来塑形
  SkRect fillRect =
      SkRect::MakeXYWH(rect.fLeft, rect.fTop, fillWidth, rect.height());
  canvas->drawRect(fillRect, fillPaint);

  canvas->restore();

  // --- D. 绘制边框 ---
  SkPaint borderPaint;
  borderPaint.setColor(kDarkBlue);
  borderPaint.setStyle(SkPaint::kStroke_Style);
  borderPaint.setStrokeWidth(2.0f);
  borderPaint.setAntiAlias(true); // 💡 优化：抗锯齿
  canvas->drawPath(trackPath, borderPaint);

  // --- E. 绘制文本百分比 (保持不变) ---

  sk_sp<SkTypeface> typeface = SkTypeface::MakeDefault();
  SkFont font(typeface, rect.height() * 0.5f);
  font.setEdging(SkFont::Edging::kAntiAlias);

  SkPaint textPaint;
  std::string progressText = std::to_string(static_cast<int>(progress)) + "%";

  float textWidth = font.measureText(
      progressText.c_str(), progressText.length(), SkTextEncoding::kUTF8);
  SkFontMetrics fontMetrics{};
  font.getMetrics(&fontMetrics);

  // 居中计算
  float x = rect.fLeft + (rect.width() - textWidth) / 2.0f;
  float textCenterY = rect.fTop + rect.height() / 2.0f;
  float y = textCenterY - (fontMetrics.fAscent + fontMetrics.fDescent) / 2.0f;

  // 文本颜色切换逻辑
  float textCenterX = rect.fLeft + rect.width() / 2.0f;

  if (textCenterX <= rect.fLeft + fillWidth) { // CC +1
    textPaint.setColor(SK_ColorWHITE);
  } else {
    textPaint.setColor(kDarkBlue);
  }

  canvas->drawSimpleText(progressText.c_str(), progressText.length(),
                         SkTextEncoding::kUTF8, x, y, font, textPaint);
}

// --- 3. 主绘图函数 (可调节参数演示) ---
// 圈复杂度 (CC) = 2
void draw_frame(SkCanvas *canvas) {
  static float currentProgress = 75.0f;

  canvas->drawColor(SK_ColorWHITE);

  // 进度条一：标准左细右粗 (h_left: 50% 基准高度, h_right: 100% 基准高度)
  // 长度 L=300, 基准高度 H=50. 左高 25, 右高 50.
  SkRect bounds1 = SkRect::MakeXYWH(50.0f, 50.0f, 300.0f, 50.0f);
  draw_horizontal_progressbar(canvas, bounds1, currentProgress, 0.5f, 1.0f);

  // 进度条二：极细到极粗 (h_left: 20%, h_right: 100%)
  // 长度 L=300, 基准高度 H=50. 左高 10, 右高 50.
  SkRect bounds2 = bounds1;
  bounds2.offset(0.0f, 80.0f); // 垂直向下偏移 80.0f
  draw_horizontal_progressbar(canvas, bounds2, 30.0f, 0.2f, 1.0f);

  // 进度条三：反向渐变 (h_left: 100%, h_right: 50%)
  // 长度 L=200, 基准高度 H=40. 左高 40, 右高 20.
  SkRect bounds3 = SkRect::MakeXYWH(50.0f, 200.0f, 200.0f, 40.0f);
  draw_horizontal_progressbar(canvas, bounds3, 90.0f, 1.0f, 0.5f);

  currentProgress += 0.5f;
  if (currentProgress > 100.0f) { // CC +1
    currentProgress = 0.0f;
  }
}

// --- 4. 文件保存和初始化 (保持不变) ---
// CC = 1
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

// CC = 2
int main() {
  SkBitmap canvasBitmap;
  // 调整画布高度以适应三个进度条
  SkImageInfo imageInfo =
      SkImageInfo::Make(540, 280, kRGBA_8888_SkColorType, kPremul_SkAlphaType);
  canvasBitmap.allocPixels(imageInfo, imageInfo.minRowBytes());

  SkCanvas canvas(canvasBitmap);

  draw_frame(&canvas);

  SkBitmap scaleBitmap = ScaleBitmap(canvasBitmap, 1.0f, 1.0f);

  std::string fileName = "horizontal_tapered_progressbar_final.png";

  // 修正目录查找逻辑 (使用当前工作目录，简化路径查找)
  std::string current_directory = File::get_current_directory();
  std::filesystem::path current_path = std::filesystem::current_path();
  current_directory = current_path.parent_path().parent_path();
  std::cout << "当前目录是: " << current_directory << std::endl;

  std::string fullPath = current_directory + PATH_SEPARATOR + fileName;
  SkFILEWStream save_file(fullPath.c_str());

  if (!save_file.isValid()) { // CC +1
    std::cerr << "错误: 无法打开文件进行写入: " << fullPath << std::endl;
    return 1;
  }

  bool success = SkPngEncoder::Encode(&save_file, scaleBitmap.pixmap(), {});

  if (success) {
    std::cout << "成功保存 PNG 文件到: " << fullPath << std::endl;
  } else {
    std::cerr << "错误: PNG 编码失败." << std::endl;
    return 1;
  }

  return 0;
}