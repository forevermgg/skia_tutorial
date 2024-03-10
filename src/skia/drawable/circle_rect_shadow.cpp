#include <include/core/SkCanvas.h>
#include <include/core/SkBitmap.h>
#include <include/core/SkPaint.h>
#include <include/core/SkPath.h>
#include <include/core/SkSurface.h>
#include <include/core/SkFont.h>
#include <include/codec/SkCodec.h>
#include <include/core/SkStream.h>
#include <include/core/SkRect.h>
#include <include/core/SkRRect.h>
#include <include/core/SkTextBlob.h>
#include <include/encode/SkPngEncoder.h>
#include "include/gpu/GrDirectContext.h"
#include "include/gpu/gl/GrGLInterface.h"
#include "include/core/SkData.h"
#include "include/core/SkImage.h"
#include "include/gpu/ganesh/SkSurfaceGanesh.h"
#include "include/utils/SkShadowUtils.h"
#include "include/core/SkMatrix.h"
#include "include/core/SkPoint3.h"
#include "../file.h"

namespace {
    // These numbers have been chosen empirically to give a result closest to the
    // material spec.
    // These values are also used by the CanvasKit renderer and the native engine.
    // See:
    //   flutter/display_list/skia/dl_sk_dispatcher.cc
    //   flutter/lib/web_ui/lib/src/engine/canvaskit/util.dart
    constexpr SkScalar kShadowAmbientAlpha = 0.039;
    constexpr SkScalar kShadowSpotAlpha = 0.25;
    constexpr SkScalar kShadowLightRadius = 1.1;
    constexpr SkScalar kShadowLightHeight = 600.0;
    constexpr SkScalar kShadowLightXOffset = 0;
    constexpr SkScalar kShadowLightYOffset = -450;
} // namespace

void canvas_drawShadow(SkCanvas *canvas,
                       SkPath *path,
                       SkScalar elevation,
                       SkScalar devicePixelRatio,
                       SkColor color,
                       bool transparentOccluder) {
    SkColor inAmbient =
            SkColorSetA(color, kShadowAmbientAlpha * SkColorGetA(color));
    SkColor inSpot = SkColorSetA(color, kShadowSpotAlpha * SkColorGetA(color));
    SkColor outAmbient;
    SkColor outSpot;
    SkShadowUtils::ComputeTonalColors(inAmbient, inSpot, &outAmbient, &outSpot);
    uint32_t flags = transparentOccluder
                         ? SkShadowFlags::kTransparentOccluder_ShadowFlag
                         : SkShadowFlags::kNone_ShadowFlag;
    flags |= SkShadowFlags::kDirectionalLight_ShadowFlag;
    SkShadowUtils::DrawShadow(
        canvas, *path, SkPoint3::Make(0.0f, 0.0f, elevation * devicePixelRatio),
        SkPoint3::Make(kShadowLightXOffset, kShadowLightYOffset,
                       kShadowLightHeight * devicePixelRatio),
        devicePixelRatio * kShadowLightRadius, outAmbient, outSpot, flags);
}

SkRect ComputeShadowBounds(const SkPath &path,
                           float elevation,
                           SkScalar dpr,
                           const SkMatrix &ctm) {
    SkRect shadow_bounds(path.getBounds());
    SkShadowUtils::GetLocalBounds(
        ctm, path, SkPoint3::Make(0, 0, dpr * elevation),
        SkPoint3::Make(0, -1, 1), kShadowLightRadius / kShadowLightHeight,
        SkShadowFlags::kDirectionalLight_ShadowFlag, &shadow_bounds);
    return shadow_bounds;
}

const SkScalar kLightHeight = 400;
const SkScalar kLightRadius = 400;

void DrawShadow(SkCanvas *canvas,
                const SkPath &path,
                SkColor color,
                float elevation,
                bool transparentOccluder,
                SkScalar dpr) {
    const SkScalar kAmbientAlpha = 0.039f;
    const SkScalar kSpotAlpha = 0.25f;

    uint32_t flags = transparentOccluder
                         ? SkShadowFlags::kTransparentOccluder_ShadowFlag
                         : SkShadowFlags::kNone_ShadowFlag;
    flags |= SkShadowFlags::kDirectionalLight_ShadowFlag;
    SkColor inAmbient = SkColorSetA(color, kAmbientAlpha * SkColorGetA(color));
    SkColor inSpot = SkColorSetA(color, kSpotAlpha * SkColorGetA(color));
    SkColor ambientColor, spotColor;
    SkShadowUtils::ComputeTonalColors(inAmbient, inSpot, &ambientColor,
                                      &spotColor);
    SkShadowUtils::DrawShadow(canvas,
                              path,
                              SkPoint3::Make(0, 0, dpr * elevation),
                              SkPoint3::Make(0, -1, 1),
                              kLightRadius / kLightHeight,
                              ambientColor,
                              spotColor,
                              flags);
}

void draw(SkCanvas *canvas) {
    SkPath path;
    path.addRoundRect({50, 50, 200, 200}, 20, 20);
    canvas->drawColor(SK_ColorTRANSPARENT);
    DrawShadow(canvas, path, SK_ColorRED, 10, false, 1.0);
}

int main() {
    // 创建一个位图设备
    SkBitmap bitmap;
    SkImageInfo imageInfo = SkImageInfo::Make(400, 400, kBGRA_8888_SkColorType, kPremul_SkAlphaType); //设置位图信息
    bitmap.allocPixels(imageInfo, imageInfo.minRowBytes()); //为位图设备绑定信息和分配内存
    // 创建画布
    SkCanvas canvas(bitmap);

    draw(&canvas);

    //将绘制结果保存到图片文件中
    std::string current_directory = File::get_current_directory();
    std::filesystem::path current_path = std::filesystem::current_path();
    current_directory = current_path.parent_path().parent_path();
    std::cout << "当前目录是: " << current_directory << std::endl;
    SkFILEWStream file((current_directory + PATH_SEPARATOR + "circle_rect_shadow.png").c_str()); //创建文件输出流
    if (!file.isValid()) {
        return 1;
    }
    SkPngEncoder::Encode(&file, bitmap.pixmap(), {});
    return 0;
}
