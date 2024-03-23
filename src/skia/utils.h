//
// Created by centforever on 2024/3/23.
//
#pragma once
#ifndef UTILS_H
#define UTILS_H

#include "include/core/SkImage.h"
#include "include/core/SkString.h"
#include "include/core/SkCanvas.h"
#include "include/core/SkTileMode.h"
#include "include/effects/SkRuntimeEffect.h"

namespace Utils {
    SkRect GetBounds(SkCanvas *canvas);

    sk_sp<SkImage> LoadImageFromFile(const SkString &filePath);

    bool IsXInRect(SkScalar x, const SkRect &rect);

    bool IsPointInRect(SkScalar x, SkScalar y, const SkRect &rect);

    SkRuntimeEffect::Result LoadFromFile(const SkString& filePath);
}
#endif //UTILS_H
