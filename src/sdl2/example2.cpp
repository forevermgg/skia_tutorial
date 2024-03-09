/**
 * https://thenumb.at/cpp-course/sdl2/02/02.html
 */
#include <iostream>
#include <SDL.h>

using namespace std;

bool init();

void kill();

bool load();

// Variables to hold our window and surfaces
SDL_Window *window;
SDL_Surface *winSurface;
SDL_Surface *image1;
SDL_Surface *image2;

int main(int argc, char **args) {

    if (!init()) return 1;

    if (!load()) return 1;

    // Blit image to entire window
    SDL_BlitSurface(image1, NULL, winSurface, NULL);

    // Blit image to scaled portion of window
    SDL_Rect dest;
    dest.x = 160;
    dest.y = 120;
    dest.w = 320;
    dest.h = 240;
    SDL_BlitScaled(image2, NULL, winSurface, &dest);

    // Update window
    SDL_UpdateWindowSurface(window);
    system("read -p 'Press Enter to continue...' var");

    kill();
    return 0;
}

bool load() {
    // Temporary surfaces to load images into
    // This should use only 1 temp surface, but for conciseness we use two
    SDL_Surface *temp1, *temp2;

    // Load images
    temp1 = SDL_LoadBMP("resources/test1.bmp");
    temp2 = SDL_LoadBMP("resources/test2.bmp");

    // Make sure loads succeeded
    if (!temp1 || !temp2) {
        cout << "Error loading image: " << SDL_GetError() << endl;
        system("read -p 'Press Enter to continue...' var");
        return false;
    }

    // Format surfaces
    image1 = SDL_ConvertSurface(temp1, winSurface->format, 0);
    image2 = SDL_ConvertSurface(temp2, winSurface->format, 0);

    // Free temporary surfaces
    SDL_FreeSurface(temp1);
    SDL_FreeSurface(temp2);

    // Make sure format succeeded
    if (!image1 || !image2) {
        cout << "Error converting surface: " << SDL_GetError() << endl;
        system("pause");
        return false;
    }
    return true;
}

bool init() {
    // See last example for comments
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        cout << "Error initializing SDL: " << SDL_GetError() << endl;
        system("pause");
        return false;
    }

    window = SDL_CreateWindow("Example", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, SDL_WINDOW_SHOWN);
    if (!window) {
        cout << "Error creating window: " << SDL_GetError() << endl;
        system("pause");
        return false;
    }

    winSurface = SDL_GetWindowSurface(window);
    if (!winSurface) {
        cout << "Error getting surface: " << SDL_GetError() << endl;
        system("pause");
        return false;
    }
    return true;
}

void kill() {
    // Free images
    SDL_FreeSurface(image1);
    SDL_FreeSurface(image2);

    // Quit
    SDL_DestroyWindow(window);
    SDL_Quit();
}
