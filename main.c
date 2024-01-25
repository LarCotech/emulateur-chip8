#include <stdio.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include "proc.h"

/*
int automate (int index, char* ch);

int main () {
    automate(0, "alEzz3_eei");
    return 1;
}

int automate (int index, char* ch) {
    int etat = 0;
    while (etat != 3 && ch[index] == '\0') {
        printf("%c", ch[index]);
        if (etat == 1) {
            if (!(ch[index] >= 'a' && ch[index] <= 'z')
            || !(ch[index] >= 'A' && ch[index] <= 'Z')
            || ch[index] != '_') {
                etat = 3;
            }
        }
        else if (etat == 2){
            if (ch[index] >= '0' && ch[index] <= '9') etat = 2;
            else if ((ch[index] >= 'a' && ch[index] <= 'z')
            || (ch[index] >= 'A' && ch[index] <= 'Z')
            || ch[index] == '_') {
                etat = 1;
            }
            else printf("Erreur");
        } else {
            if (ch[index] == 0) {
                printf("Erreur 0 au début de la chaine\n");
            }
            else if (ch[index]) etat = 2;
            else if (ch[index]) etat = 1;
            else printf("Erreur");
        }
        index++;
    }
    if (etat == 3) {
        printf("Chaine reconnu !");
        return 0;
    }
    else {
        printf("Chaine non reconnu");
        return 1;
    }
}
 */

#include <stdio.h>
#include <stdbool.h>
#include <SDL.h>
#define WIDTH 640
#define HEIGHT 480
#define SIZE 200
#define SPEED 600
#define GRAVITY 60
#define FPS 60
#define JUMP -1200

int main(int argc, char* argv[])
{
    printf ("Essai compilation avec SDL2\n");
    /* Initializes the timer, audio, video, joystick,
    haptic, gamecontroller and events subsystems */
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        printf("Error initializing SDL: %s\n", SDL_GetError());
        return 0;
    }
    /* Create a window */
    SDL_Window* wind = SDL_CreateWindow("Hello Platformer!",
                                        SDL_WINDOWPOS_CENTERED,
                                        SDL_WINDOWPOS_CENTERED,
                                        WIDTH, HEIGHT, 0);
    if (!wind)
    {
        printf("Error creating window: %s\n", SDL_GetError());
        SDL_Quit();
        return 0;
    }
    /* Create a renderer */
    Uint32 render_flags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;
    SDL_Renderer* rend = SDL_CreateRenderer(wind, -1, render_flags);
    if (!rend)
    {
        printf("Error creating renderer: %s\n", SDL_GetError());
        SDL_DestroyWindow(wind);
        SDL_Quit();
        return 0;
    }
    /* Main loop */
    bool running = true, jump_pressed = false, can_jump = true,
            left_pressed = false, right_pressed = false;
    float x_pos = (WIDTH-SIZE)/2, y_pos = (HEIGHT-SIZE)/2, x_vel = 0, y_vel = 0;
    SDL_Rect rect = {(int) x_pos, (int) y_pos, SIZE, SIZE};
    SDL_Event event;
    while (running)
    {
        /* Process events */
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
                case SDL_QUIT:
                    running = false;
                    break;
                case SDL_KEYDOWN:
                    switch (event.key.keysym.scancode)
                    {
                        case SDL_SCANCODE_SPACE:
                            jump_pressed = true;
                            break;
                        case SDL_SCANCODE_A:
                        case SDL_SCANCODE_LEFT:
                            left_pressed = true;
                            break;
                        case SDL_SCANCODE_D:
                        case SDL_SCANCODE_RIGHT:
                            right_pressed = true;
                            break;
                        default:
                            break;
                    }
                    break;
                case SDL_KEYUP:
                    switch (event.key.keysym.scancode)
                    {
                        case SDL_SCANCODE_SPACE:
                            jump_pressed = false;
                            break;
                        case SDL_SCANCODE_A:
                        case SDL_SCANCODE_LEFT:
                            left_pressed = false;
                            break;
                        case SDL_SCANCODE_D:
                        case SDL_SCANCODE_RIGHT:
                            right_pressed = false;
                            break;
                        default:
                            break;
                    }
                    break;
                default:
                    break;
            }
        }
        /* Clear screen */
        SDL_SetRenderDrawColor(rend, 0, 0, 0, 255);
        SDL_RenderClear(rend);
        /* Move the rectangle */
        x_vel = (right_pressed - left_pressed)*SPEED;
        y_vel += GRAVITY;
        if (jump_pressed && can_jump)
        {
            can_jump = false;
            y_vel = JUMP;
        }
        x_pos += x_vel / 60;
        y_pos += y_vel / 60;
        if (x_pos <= 0)
            x_pos = 0;
        if (x_pos >= WIDTH - rect.w)
            x_pos = WIDTH - rect.w;
        if (y_pos <= 0)
            y_pos = 0;
        if (y_pos >= HEIGHT - rect.h)
        {
            y_vel = 0;
            y_pos = HEIGHT - rect.h;
            if (!jump_pressed)
                can_jump = true;
        }
        rect.x = (int) x_pos;
        rect.y = (int) y_pos;
        /* Draw the rectangle */
        SDL_SetRenderDrawColor(rend, 255, 0, 0, 255);
        SDL_RenderFillRect(rend, &rect);
        /* Draw to window and loop */
        SDL_RenderPresent(rend);
        SDL_Delay(1000/FPS);
    }
    /* Release resources */
    SDL_DestroyRenderer(rend);
    SDL_DestroyWindow(wind);
    SDL_Quit();
    return 0;
}

/*
   if (argc != 2) {
       fprintf(stderr, "Unexpected number of arguments.\n");
       fprintf(stderr, "Usage: emulator <ROM.ch8>\n");
       return 1;
   }
   if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER|SDL_INIT_AUDIO)) {
       errcode = SDL;
   }
   else {
       printf("The SDL library has been initialized successfully.\n");
       struct Machine machine;
       if (!Machine_init(&machine)) {
           printf("The CHIP 8 emulator is ready.\n");
           if (!Machine_load(&machine,argv[1])) {
               printf("The ROM <%s> has been loaded.\n",argv[1]);
               printf("Run...\n");
               while (
                       !Machine_loop(&machine)
                       );
           }
           Machine_destroy(&machine);
       }
       SDL_Quit();
   }
   if (errcode != QUIT) {
       fprintf(stderr,"The program has terminated abnormally (errcode=%s)\n",errorstr());
       return 1;
   }*/