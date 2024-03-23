//
// Created by centforever on 2024/3/23.
//

#include "utils.h"
#include "include/core/SkData.h"

SkRect Utils::GetBounds(SkCanvas *canvas) {
    SkRect rect;
    rect.set(canvas->getDeviceClipBounds());
    return rect;
}

sk_sp<SkImage> Utils::LoadImageFromFile(const SkString &filePath) {
    sk_sp<SkData> skData = SkData::MakeFromFileName(filePath.c_str());
    if (!skData)
        return nullptr;

    return SkImages::DeferredFromEncodedData(std::move(skData));
}

bool Utils::IsXInRect(SkScalar x, const SkRect &rect) {
    return rect.left() <= x && x <= rect.right();
}

bool Utils::IsPointInRect(SkScalar x, SkScalar y, const SkRect &rect) {
    return rect.left() <= x && x <= rect.right() && rect.top() <= y && y <= rect.bottom();
}


SkRuntimeEffect::Result Utils::LoadFromFile(const SkString& filePath)
{
    sk_sp<SkData> sk_data = SkData::MakeFromFileName(filePath.c_str());
    if (!sk_data)
        return {nullptr, SkString{"Can't load the shader"}};
    SkString str{static_cast<const char*>(sk_data->data()), sk_data->size()};
    return SkRuntimeEffect::MakeForShader(std::move(str));
}