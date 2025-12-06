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
#include <include/encode/SkPngEncoder.h>
#include <iostream>
#include <string>

// 定义圆角半径与高度的比率。
// 设置为 0.5f 使进度条的两端形成半圆形封口。
constexpr float kCornerRadiusRatio = 0.5f;

// --- 1. 形状生成函数：生成横向圆角矩形路径 ---
// 圈复杂度 (CC) = 1
SkPath make_rounded_rect_path(const SkRect &rect, float cornerRadius) {
  SkPath path;
  // 使用 SkPath::addRoundRect 快速生成圆角矩形路径
  path.addRoundRect(rect, cornerRadius, cornerRadius);
  return path;
}

// --- 2. 进度条绘制函数 (横向填充和文本逻辑) ---
// 圈复杂度 (CC) = 3 (包含 1 个 std::max/min 和 1 个 if-else)
void draw_horizontal_progressbar(SkCanvas *canvas, const SkRect &rect,
                                 float progress) {
  // 规范化进度值：确保进度在 0.0 到 100.0 之间
  progress = std::max(0.0f, std::min(100.0f, progress)); // CC +1

  // 计算圆角半径：取高度的一半
  float cornerRadius = rect.height() * kCornerRadiusRatio;

  // 获取圆角矩形轮廓 (Track/Border 的形状)
  SkPath trackPath = make_rounded_rect_path(rect, cornerRadius);

  // 定义统一的深蓝色，用于边框和未被覆盖的文本
  const SkColor kDarkBlue = SkColorSetARGB(255, 40, 75, 120);

  // --- A. 绘制背景轨道 (Track) ---
  SkPaint trackPaint;
  trackPaint.setColor(SkColorSetARGB(255, 230, 230, 230)); // 浅灰背景
  trackPaint.setStyle(SkPaint::kFill_Style);
  canvas->drawPath(trackPath, trackPaint);

  // --- B. 准备裁剪区域 (Clipping) ---
  // 保存画布状态，以便后续恢复裁剪区域
  canvas->save();
  // 将绘图限制在 trackPath (圆角矩形) 内部
  canvas->clipPath(trackPath, true);

  // --- C. 绘制进度填充 (Fill) ---
  SkPaint fillPaint;
  fillPaint.setColor(SkColorSetARGB(255, 0, 160, 255)); // 蓝色填充
  fillPaint.setStyle(SkPaint::kFill_Style);

  // 计算进度填充的宽度
  float fillWidth = rect.width() * (progress / 100.0f);

  // 创建填充矩形：从左侧开始，宽度为 fillWidth
  SkRect fillRect =
      SkRect::MakeXYWH(rect.fLeft, rect.fTop, fillWidth, rect.height());
  canvas->drawRect(fillRect, fillPaint);

  // 恢复画布状态 (移除裁剪)
  canvas->restore();

  // --- D. 绘制边框 ---
  SkPaint borderPaint;
  borderPaint.setColor(kDarkBlue);
  borderPaint.setStyle(SkPaint::kStroke_Style);
  borderPaint.setStrokeWidth(2.0f);
  canvas->drawPath(trackPath, borderPaint);

  // --- E. 绘制文本百分比 ---

  sk_sp<SkTypeface> typeface = SkTypeface::MakeDefault();
  // 字体大小与进度条高度相关
  SkFont font(typeface, rect.height() * 0.7f);
  font.setEdging(SkFont::Edging::kAntiAlias);

  SkPaint textPaint;
  std::string progressText = std::to_string(static_cast<int>(progress)) + "%";

  // 测量文本宽度，用于水平居中
  float textWidth = font.measureText(
      progressText.c_str(), progressText.length(), SkTextEncoding::kUTF8);
  SkFontMetrics fontMetrics{};
  font.getMetrics(&fontMetrics);

  // 1. 水平居中计算
  float x = rect.fLeft + (rect.width() - textWidth) / 2.0f;

  // 2. 垂直居中计算 (基于文本的 ascent/descent)
  float textCenterY = rect.fTop + rect.height() / 2.0f;
  float y = textCenterY - (fontMetrics.fAscent + fontMetrics.fDescent) / 2.0f;

  // --- 文本颜色切换逻辑：根据文本中心是否被蓝色填充覆盖来切换颜色 ---
  // 计算文本的水平中心点
  float textCenterX = rect.fLeft + rect.width() / 2.0f;

  // 如果文本中心点在蓝色填充区域内 (即 x 坐标小于填充宽度)
  if (textCenterX <= rect.fLeft + fillWidth) { // CC +1
    // 在蓝色填充上使用白色文本
    textPaint.setColor(SK_ColorWHITE);
  } else {
    // 在灰色背景上使用深蓝色文本
    textPaint.setColor(kDarkBlue);
  }

  // 绘制文本
  canvas->drawSimpleText(progressText.c_str(), progressText.length(),
                         SkTextEncoding::kUTF8, x, y, font, textPaint);
}

// --- 3. 主绘图函数 (调用绘制逻辑并控制动画) ---
// 圈复杂度 (CC) = 2 (包含 1 个 if)
void draw_frame(SkCanvas *canvas) {
  // 静态变量保存当前进度，用于演示动画效果
  static float currentProgress = 75.0f;

  canvas->drawColor(SK_ColorWHITE); // 设置背景为白色

  // 进度条一：75% 进度 (横向，扁平)
  // 边界定义：(x=50, y=100, width=300, height=50)
  SkRect bounds1 = SkRect::MakeXYWH(50.0f, 100.0f, 300.0f, 50.0f);
  draw_horizontal_progressbar(canvas, bounds1, currentProgress);

  // 进度条二：30% 进度
  SkRect bounds2 = bounds1;
  bounds2.offset(0.0f, 100.0f); // 垂直向下偏移 100.0f
  draw_horizontal_progressbar(canvas, bounds2, 30.0f);

  // 动画逻辑：每次重绘增加进度，达到 100% 后重置
  currentProgress += 0.5f;
  if (currentProgress > 100.0f) { // CC +1
    currentProgress = 0.0f;
  }
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
  draw_frame(&canvas);

  // 3. 缩放 (保持 1.0f 不缩放)
  SkBitmap scaleBitmap = ScaleBitmap(canvasBitmap, 1.0f, 1.0f);

  // 4. 保存到文件
  std::string fileName = "horizontal_rounded_progressbar.png";
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