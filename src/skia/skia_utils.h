//
// Created by centforever on 2024/3/23.
//
#pragma once
#ifndef SKIA_UTILS_H
#define SKIA_UTILS_H

#include "include/core/SkImage.h"
#include "include/core/SkString.h"
#include "include/core/SkCanvas.h"
#include "include/core/SkTileMode.h"
#include "include/effects/SkRuntimeEffect.h"

namespace SkiaUtils {
    /**
    * @brief 获取画布的裁剪边界矩形
    *
    * @param canvas SkCanvas 指针
    * @return SkRect 表示裁剪边界的矩形
    */
    SkRect GetBounds(SkCanvas *canvas);

    /**
    * @brief 从文件加载图像
    *
    * @param filePath 图像文件路径
    * @return sk_sp<SkImage> 加载的图像
    */
    sk_sp<SkImage> LoadImageFromFile(const SkString &filePath);

    /**
    * @brief 检查 x 坐标是否在矩形内
    *
    * @param x x 坐标值
    * @param rect 矩形
    * @return true 如果 x 在矩形内，否则返回 false
    */
    bool IsXInRect(SkScalar x, const SkRect &rect);

   /**
   * @brief 检查点 (x, y) 是否在矩形内
   *
   * @param x x 坐标值
   * @param y y 坐标值
   * @param rect 矩形
   * @return true 如果点在矩形内，否则返回 false
   */
    bool IsPointInRect(SkScalar x, SkScalar y, const SkRect &rect);

   /**
   * @brief 检查路径是否为一个点
   *
   * @param path SkPath 对象
   * @return true 如果路径表示一个点，否则返回 false
   */
    bool IsPathAPoint(const SkPath& path);

   /**
   * @brief 从文件加载运行时效果
   *
   * @param filePath 文件路径
   * @return SkRuntimeEffect::Result 加载的运行时效果结果
   */
    SkRuntimeEffect::Result LoadFromFile(const SkString& filePath);

    /**
     * @brief 将 SkPixmap 编码为 SkData
     *
     * @param pm SkPixmap 对象
     * @return sk_sp<SkData> 编码后的数据
     */
    sk_sp<SkData> encode_to_data(const SkPixmap& pm);

    /**
     * @brief 打印 SkPaint 的调试信息
     *
     * @param paint SkPaint 对象
     */
    void DebugShowSkiaPaint(const SkPaint& paint);

    /**
     * @brief 打印 SkPath 的调试信息
     *
     * @param path SkPath 对象
     */
    void DebugShowSkiaPath(const SkPath& path);

    /**
     * @brief 打印画布的裁剪信息
     *
     * @param canvas SkCanvas 指针
     */
    void DebugShowCanvasClip(const SkCanvas* canvas);

   /**
    * @brief 打印绘制路径的调试信息
    *
    * @param canvas SkCanvas 指针
    * @param paint SkPaint 对象
    * @param path SkPath 对象
    */
    void DebugShowSkiaDrawPath(const SkCanvas* canvas,
                           const SkPaint& paint,
                           const SkPath& path);

   /**
    * @brief 打印绘制矩形的调试信息
    *
    * @param canvas SkCanvas 指针
    * @param paint SkPaint 对象
    * @param rect SkRect 对象
    */
    void DebugShowSkiaDrawRect(const SkCanvas* canvas,
                           const SkPaint& paint,
                           const SkRect& rect);
}
#endif // SKIA_UTILS_H
