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


const SkScalar kLightHeight = 400;
const SkScalar kLightRadius = 400;

void DrawShadow(SkCanvas* canvas,
                const SkPath& path,
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

void draw(SkCanvas* canvas) {
    SkPath path;
    path.addRoundRect({50, 50, 200, 200}, 20, 20);
    canvas->drawColor(SK_ColorTRANSPARENT);
    DrawShadow(canvas, path, SK_ColorRED, 10, false, 1.0);
}

int main(){
    // 创建一个位图设备
    SkBitmap bitmap;
    SkImageInfo imageInfo = SkImageInfo::Make(400, 400, kBGRA_8888_SkColorType, kPremul_SkAlphaType);  //设置位图信息
    bitmap.allocPixels(imageInfo, imageInfo.minRowBytes());  //为位图设备绑定信息和分配内存
    // 创建画布
    SkCanvas canvas(bitmap);

    draw(&canvas);

    //将绘制结果保存到图片文件中
    std::string current_directory = File::get_current_directory();
    std::filesystem::path current_path = std::filesystem::current_path();
    current_directory = current_path.parent_path().parent_path();
    std::cout << "当前目录是: " << current_directory << std::endl;
    SkFILEWStream file((current_directory + PATH_SEPARATOR + "circle_rect_shadow.png").c_str());  //创建文件输出流
    if (!file.isValid()) {
        return 1;
    }
    SkPngEncoder::Encode(&file, bitmap.pixmap(), {});
    return 0;
}