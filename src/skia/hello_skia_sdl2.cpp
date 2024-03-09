#include "include/core/SkBitmap.h"
#include "include/core/SkCanvas.h"
#include "include/core/SkPaint.h"
#include "include/core/SkFont.h"
#include "SDL.h"

//SDL头文件被我放入到了include文件夹中。
SkBitmap draw(int w, int h) {
    //声明
    SkBitmap bitmap;
    //设置Bitmap的配置信息
    bitmap.setInfo(SkImageInfo::Make(w, h, kBGRA_8888_SkColorType, kOpaque_SkAlphaType));
    //转换为像素填充
    bitmap.allocPixels();
    //创建画布
    SkCanvas canvas(bitmap);
    //创建画笔
    SkPaint paint;
    //设置画布颜色
    canvas.clear(SK_ColorWHITE);
    //设置画笔抗锯齿
    paint.setAntiAlias(true);
    //设置画笔颜色(此处为红色)
    paint.setARGB(255, 255, 0, 0);
    //绘制圆形
    canvas.drawCircle(80, 80, 40, paint);
    //绘制线段
    canvas.drawLine(0, 280, w, 280, paint);
    //设置字体大小
    SkFont font;
    font.setSize(24);
    //绘制字体
    canvas.drawString("Hello, SDL!", 100, 200, font, paint);

    //返回SkBitmap对象
    return bitmap;
}

int main() {
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window *window = SDL_CreateWindow("SDL Window", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 600,
                                          SDL_WINDOW_SHOWN);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    SkBitmap bitmap = draw(800, 600);

    SDL_Surface *surface = SDL_CreateRGBSurfaceFrom(bitmap.getPixels(), bitmap.width(), bitmap.height(), 32,
                                                    bitmap.width() * sizeof(uint32_t), 0xFF0000, 0x00FF00, 0x0000FF,
                                                    0xFF000000);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);

    SDL_Rect dstrect = {0, 0, bitmap.width(), bitmap.height()};

    SDL_Event event;
    bool quit = false;

    while (!quit) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                quit = true;
            }
        }

        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, texture, NULL, &dstrect);
        SDL_RenderPresent(renderer);
    }

    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}