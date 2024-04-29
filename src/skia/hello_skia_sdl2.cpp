#include "SDL.h"
#include "include/core/SkBitmap.h"
#include "include/core/SkCanvas.h"
#include "include/core/SkFont.h"
#include "include/core/SkPaint.h"

// SDL头文件被我放入到了include文件夹中。
SkBitmap draw(int w, int h) {
  // 声明
  SkBitmap bitmap;
  // 设置Bitmap的配置信息
  bitmap.setInfo(
      SkImageInfo::Make(w, h, kBGRA_8888_SkColorType, kOpaque_SkAlphaType));
  // 转换为像素填充
  bitmap.allocPixels();
  // 创建画布
  SkCanvas canvas(bitmap);
  // 创建画笔
  SkPaint paint;
  // 设置画布颜色
  canvas.clear(SK_ColorWHITE);
  // 设置画笔抗锯齿
  paint.setAntiAlias(true);
  // 设置画笔颜色(此处为红色)
  paint.setARGB(255, 255, 0, 0);
  // 绘制圆形
  canvas.drawCircle(80, 80, 40, paint);
  // 绘制线段
  canvas.drawLine(0, 280, w, 280, paint);
  // 设置字体大小
  SkFont font;
  font.setSize(24);
  // 绘制字体
  canvas.drawString("Hello, SDL!", 100, 200, font, paint);
  // 返回SkBitmap对象
  return bitmap;
}

// 程序的入口点
int main() {
  // 初始化SDL为视频显示
  SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS);
  // 创建窗口
  SDL_Window *window =
      SDL_CreateWindow("SDL Window", SDL_WINDOWPOS_UNDEFINED,
                       SDL_WINDOWPOS_UNDEFINED, 800, 600, SDL_WINDOW_SHOWN);
  // SDL_Renderer *renderer = SDL_CreateRenderer(window, -1,
  // SDL_RENDERER_ACCELERATED);
  SDL_Renderer *renderer = SDL_CreateRenderer(
      window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  // 获取绘制后的Bitmap
  SkBitmap bitmap = draw(800, 600);

  // 通过Bitmap的像素数据创建表面
  SDL_Surface *surface = SDL_CreateRGBSurfaceFrom(
      bitmap.getPixels(), bitmap.width(), bitmap.height(), 32,
      bitmap.width() * sizeof(uint32_t), 0xFF0000, 0x00FF00, 0x0000FF,
      0xFF000000);
  // 创建纹理
  SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);

  // 通过bitmap创建矩形
  SDL_Rect dstrect = {0, 0, bitmap.width(), bitmap.height()};

  SDL_Event event;
  bool quit = false;

  while (!quit) {
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT) {
        quit = true;
      }
      if (event.type == SDL_FINGERUP) {
        quit = true;
      }
    }
    // 清理渲染器
    SDL_RenderClear(renderer);
    // 辅助纹理到渲染器
    SDL_RenderCopy(renderer, texture, NULL, &dstrect);
    // 显示到窗口
    SDL_RenderPresent(renderer);
  }
  // 释放表面
  SDL_FreeSurface(surface);
  // 释放纹理
  SDL_DestroyTexture(texture);
  // 释放渲染器
  SDL_DestroyRenderer(renderer);
  // 释放窗口
  SDL_DestroyWindow(window);
  // 结束SDL
  SDL_Quit();
  // 程序退出
  return 0;
}