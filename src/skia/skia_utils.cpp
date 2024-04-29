//
// Created by centforever on 2024/3/23.
//
#include "skia_utils.h"
#include "include/codec/SkBmpDecoder.h"
#include "include/codec/SkCodec.h"
#include "include/codec/SkGifDecoder.h"
#include "include/codec/SkIcoDecoder.h"
#include "include/codec/SkJpegDecoder.h"
#include "include/codec/SkJpegxlDecoder.h"
#include "include/codec/SkPngDecoder.h"
#include "include/codec/SkWbmpDecoder.h"
#include "include/codec/SkWebpDecoder.h"
#include "include/core/SkData.h"
#include "include/core/SkImageGenerator.h"
#include "include/core/SkImageInfo.h"
#include "include/core/SkStream.h"
#include <cassert>
#include <format>
#include <include/core/SkPath.h>
#include <include/core/SkStream.h>
#include <include/encode/SkPngEncoder.h>
#include <type_traits>

SkRect SkiaUtils::GetBounds(SkCanvas *canvas) {
  SkRect rect;
  rect.set(canvas->getDeviceClipBounds());
  return rect;
}

sk_sp<SkImage> SkiaUtils::LoadImageFromFile(const SkString &filePath) {
  sk_sp<SkData> skData = SkData::MakeFromFileName(filePath.c_str());
  if (!skData)
    return nullptr;

  return SkImages::DeferredFromEncodedData(std::move(skData));
}

bool SkiaUtils::IsXInRect(SkScalar x, const SkRect &rect) {
  return rect.left() <= x && x <= rect.right();
}

bool SkiaUtils::IsPointInRect(SkScalar x, SkScalar y, const SkRect &rect) {
  return rect.left() <= x && x <= rect.right() && rect.top() <= y &&
         y <= rect.bottom();
}

bool SkiaUtils::IsPathAPoint(const SkPath &path) {
  if (path.isEmpty())
    return false;

  if (path.countPoints() == 1)
    return true;

  for (int i = 0; i < path.countPoints() - 1; ++i) {
    if (path.getPoint(i) != path.getPoint(i + 1))
      return false;
  }
  return true;
}

SkRuntimeEffect::Result SkiaUtils::LoadFromFile(const SkString &filePath) {
  sk_sp<SkData> sk_data = SkData::MakeFromFileName(filePath.c_str());
  if (!sk_data)
    return {nullptr, SkString{"Can't load the shader"}};
  SkString str{static_cast<const char *>(sk_data->data()), sk_data->size()};
  return SkRuntimeEffect::MakeForShader(std::move(str));
}

sk_sp<SkData> SkiaUtils::encode_to_data(const SkPixmap &pm) {
  SkDynamicMemoryWStream stream;
  if (SkPngEncoder::Encode(&stream, pm, SkPngEncoder::Options())) {
    return stream.detachAsData();
  }
  return nullptr;
}

void SkiaUtils::DebugShowSkiaPaint(const SkPaint &paint) {
  if (SkPaint::kFill_Style == paint.getStyle()) {
    printf("fill 0x%08x\n", paint.getColor());
  } else {
    printf("stroke 0x%08x width %g\n", paint.getColor(),
           paint.getStrokeWidth());
  }
}

void SkiaUtils::DebugShowSkiaPath(const SkPath &path) {
  printf(" **\n");
  SkDynamicMemoryWStream stream;
  path.dump(&stream, false);
  size_t numBytesWritten = stream.bytesWritten();

  std::vector<char> storage(numBytesWritten);
  stream.copyTo(storage.data());

  printf("%.*s", static_cast<int>(storage.size()), storage.data());
}

void SkiaUtils::DebugShowCanvasClip(const SkCanvas *canvas) {
  SkMatrix matrix = canvas->getTotalMatrix();
  SkScalar m[9];
  matrix.get9(m);
  printf("matrix (%g,%g,%g) (%g,%g,%g) (%g,%g,%g)\n", m[0], m[1], m[2], m[3],
         m[4], m[5], m[6], m[7], m[8]);
  SkRect local = canvas->getLocalClipBounds();
  SkIRect device = canvas->getDeviceClipBounds();

  printf("local bounds %g %g %g %g\n", local.fLeft, local.fTop, local.fRight,
         local.fBottom);
  printf("device bounds %d %d %d %d\n", device.fLeft, device.fTop,
         device.fRight, device.fBottom);
}

void SkiaUtils::DebugShowSkiaDrawPath(const SkCanvas *canvas,
                                      const SkPaint &paint,
                                      const SkPath &path) {
  DebugShowSkiaPaint(paint);
  DebugShowCanvasClip(canvas);
  DebugShowSkiaPath(path);
  printf("\n");
}

void SkiaUtils::DebugShowSkiaDrawRect(const SkCanvas *canvas,
                                      const SkPaint &paint,
                                      const SkRect &rect) {
  DebugShowSkiaPaint(paint);
  DebugShowCanvasClip(canvas);
  printf("rect %g %g %g %g\n", rect.fLeft, rect.fTop, rect.fRight,
         rect.fBottom);
}

bool SkiaUtils::BitmapsAreEqual(const SkBitmap &bitmap1,
                                const SkBitmap &bitmap2) {
  if (bitmap1.isNull() != bitmap2.isNull() ||
      bitmap1.dimensions() != bitmap2.dimensions())
    return false;

  if (bitmap1.getGenerationID() == bitmap2.getGenerationID() ||
      (bitmap1.empty() && bitmap2.empty()))
    return true;

  // Calling getAddr32() on null or empty bitmaps will assert. The conditions
  // above should return early if either bitmap is empty or null.
  assert(!bitmap1.isNull() && !bitmap2.isNull());
  assert(!bitmap1.empty() && !bitmap2.empty());

  void *addr1 = bitmap1.getAddr32(0, 0);
  void *addr2 = bitmap2.getAddr32(0, 0);
  size_t size1 = bitmap1.computeByteSize();
  size_t size2 = bitmap2.computeByteSize();

  return (size1 == size2) && (0 == memcmp(addr1, addr2, size1));

  // 比较两个 SkBitmap 的大小和像素数据是否相等
  return (size1 == size2) && (0 == memcmp(addr1, addr2, size1));
}

bool SkiaUtils::ConvertBitmap(const SkBitmap &src_bitmap,
                              SkBitmap *target_bitmap, SkColorType target_ct,
                              SkAlphaType target_at) {
  assert(src_bitmap.readyToDraw());
  assert(src_bitmap.colorType() != target_ct ||
         src_bitmap.alphaType() != target_at);
  assert(target_bitmap);

  SkImageInfo target_info = SkImageInfo::Make(
      src_bitmap.width(), src_bitmap.height(), target_ct, target_at);
  if (!target_bitmap->tryAllocPixels(target_info)) {
    return false;
  }

  if (!src_bitmap.readPixels(target_info, target_bitmap->getPixels(),
                             target_bitmap->rowBytes(), 0, 0)) {
    return false;
  }

  assert(target_bitmap->readyToDraw());
  return true;
}

bool SkiaUtils::EncodeImageToPngFile(const char *path, const SkBitmap &src) {
  SkFILEWStream file(path);
  return file.isValid() && SkPngEncoder::Encode(&file, src.pixmap(), {});
}

bool SkiaUtils::EncodeImageToPngFile(const char *path, const SkPixmap &src) {
  SkFILEWStream file(path);
  return file.isValid() && SkPngEncoder::Encode(&file, src, {});
}

bool SkiaUtils::BitmapToBase64DataURI(const SkBitmap &bitmap, SkString *dst) {
  SkPixmap pm;
  if (!bitmap.peekPixels(&pm)) {
    dst->set("peekPixels failed");
    return false;
  }

  // We're going to embed this PNG in a data URI, so make it as small as
  // possible
  SkPngEncoder::Options options;
  options.fFilterFlags = SkPngEncoder::FilterFlag::kAll;
  options.fZLibLevel = 9;

  SkDynamicMemoryWStream wStream;
  if (!SkPngEncoder::Encode(&wStream, pm, options)) {
    dst->set("SkPngEncoder::Encode failed");
    return false;
  }

  sk_sp<SkData> pngData = wStream.detachAsData();
  size_t len = EncodedSize(pngData->size());

  // The PNG can be almost arbitrarily large. We don't want to fill our logs
  // with enormous URLs. Infra says these can be pretty big, as long as we're
  // only outputting them on failure.
  static const size_t kMaxBase64Length = 1024 * 1024;
  if (len > kMaxBase64Length) {
    dst->printf("Encoded image too large (%u bytes)",
                static_cast<uint32_t>(len));
    return false;
  }

  dst->resize(len);
  SkBase64::Encode(pngData->data(), pngData->size(), dst->data());
  dst->prepend("data:image/png;base64,");
  return true;
}

bool SkiaUtils::decode_everything(const char *path) {
  std::unique_ptr<SkFILEStream> input = SkFILEStream::Make(path);
  if (!input || !input->isValid()) {
    printf("Cannot open file %s\n", path);
    return false;
  }

  sk_sp<SkData> data = SkData::MakeFromStream(input.get(), input->getLength());

  std::unique_ptr<SkCodec> codec = nullptr;
  if (SkBmpDecoder::IsBmp(data->bytes(), data->size())) {
    codec = SkBmpDecoder::Decode(data, nullptr);
  } else if (SkGifDecoder::IsGif(data->bytes(), data->size())) {
    codec = SkGifDecoder::Decode(data, nullptr);
  } else if (SkIcoDecoder::IsIco(data->bytes(), data->size())) {
    codec = SkIcoDecoder::Decode(data, nullptr);
  } else if (SkJpegDecoder::IsJpeg(data->bytes(), data->size())) {
    codec = SkJpegDecoder::Decode(data, nullptr);
  } /*else if (SkJpegxlDecoder::IsJpegxl(data->bytes(), data->size())) {
      codec = SkJpegxlDecoder::Decode(data, nullptr);
  } */
  else if (SkPngDecoder::IsPng(data->bytes(), data->size())) {
    codec = SkPngDecoder::Decode(data, nullptr);
  } else if (SkWbmpDecoder::IsWbmp(data->bytes(), data->size())) {
    codec = SkWbmpDecoder::Decode(data, nullptr);
  } else if (SkWebpDecoder::IsWebp(data->bytes(), data->size())) {
    codec = SkWebpDecoder::Decode(data, nullptr);
  } else {
    printf("Unsupported file format\n");
    return false;
  }

  SkImageInfo info = codec->getInfo();
  printf("Image is %d by %d pixels.\n", info.width(), info.height());

  return true;
}
