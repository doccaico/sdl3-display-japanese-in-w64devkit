#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>

#include <stdio.h>

#define TITLE "App - SDL3"
#define WIDTH 640
#define HEIGHT 480

#define FONT_PATH "resource/NotoSansJP-Regular.ttf"
#define FONT_SIZE 24

#define SDL_FLAGS (SDL_INIT_VIDEO)
#define WINDOW_FLAGS 0
// #define WINDOW_FLAGS (SDL_WINDOW_RESIZABLE | SDL_WINDOW_BORDERLESS)
#define FPS_TARGET 30
#define JA_TEXT "あなたはゴブリンに20のダメージを与えた。"

int main(int argc, char* argv[])
{
    int ret = 0;

    if (!SDL_Init(SDL_FLAGS)) {
        SDL_Log("SDL_Init failed: %s", SDL_GetError());
        return 1;
    }

    SDL_Window* window;
    SDL_Renderer* renderer;

    if (!SDL_CreateWindowAndRenderer(TITLE, WIDTH, HEIGHT, WINDOW_FLAGS, &window, &renderer)) {
        SDL_Log("SDL_CreateWindowAndRenderer failed: %s", SDL_GetError());
        ret = 1;
        goto deinit_1;
    }

    if (!TTF_Init()) {
        SDL_Log("TTF_Init failed: %s", SDL_GetError());
        ret = 1;
        goto deinit_2;
    }

    TTF_Font *font = TTF_OpenFont(FONT_PATH, FONT_SIZE);
    if (font == NULL) {
        SDL_Log("TTF_OpenFont failed: %s", SDL_GetError());
        ret = 1;
        goto deinit_3;
    }

    TTF_TextEngine *text_engine = TTF_CreateRendererTextEngine(renderer);
    if (text_engine == NULL) {
        SDL_Log("TTF_CreateRendererTextEngine failed: %s", SDL_GetError());
        ret = 1;
        goto deinit_4;
    }

    TTF_Text *text = TTF_CreateText(text_engine, font, JA_TEXT, 0);
    if (text == NULL) {
        SDL_Log("TTF_CreateText failed: %s", SDL_GetError());
        ret = 1;
        goto deinit_5;
    }


    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);

    SDL_Event e;
    bool stop_running = false;

    while (!stop_running) {
        // Get counter before whole frame
        Uint64 frame_start = SDL_GetTicksNS();

        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_EVENT_QUIT) {
                stop_running = true;
            } else if (e.type == SDL_EVENT_KEY_DOWN) {
                switch (e.key.key) {
                    case SDLK_ESCAPE:
                        stop_running = true;
                        break;
                    default:
                        break;
                }
            }
        }
        // Draw your things
        SDL_RenderClear(renderer);
        TTF_DrawRendererText(text, 0, 0);
        SDL_RenderPresent(renderer);

        // Get counter after whole frame
        Uint64 frame_end = SDL_GetTicksNS();
        // nanoseconds per frame target
        Uint64 npf_target = 1000000000 / FPS_TARGET;

        if ((frame_end - frame_start) < npf_target) {
            Uint64 sleep_time = npf_target - (frame_end - frame_start);
            SDL_DelayPrecise(sleep_time);
            frame_end = SDL_GetTicksNS(); // Update frame_end counter to include sleep_time for fps calculation
        }

        // update fps tracker
        double current_fps = 1000000000.000 / (double)(frame_end - frame_start);
        printf("%f\n", current_fps);


    }

    TTF_DestroyText(text);
deinit_5:
    TTF_DestroyRendererTextEngine(text_engine);
deinit_4:
    TTF_CloseFont(font);
deinit_3:
    TTF_Quit();
deinit_2:
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
deinit_1:
    SDL_Quit();

    return ret;
}
