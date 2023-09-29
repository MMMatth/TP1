#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include <cstdlib>
#include "fonctions_SDL.hpp"

int main(int argc, char *argv[])
{
    // Initialisation de la SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) 
    {
        printf("Erreur d’initialisation de la SDL: %s", SDL_GetError());
        SDL_Quit();
        return EXIT_FAILURE;
    }

    // Création de la fenêtre
    SDL_Window *fenetre = SDL_CreateWindow("Fenetre SDL", SDL_WINDOWPOS_CENTERED,
    SDL_WINDOWPOS_CENTERED, 600, 600, SDL_WINDOW_RESIZABLE);
    if (fenetre == NULL) 
    {
        printf("Erreur de la creation d’une fenetre: %s", SDL_GetError());
        SDL_Quit();
        return EXIT_FAILURE;
    }

    // Création du renderer
    SDL_Renderer * ecran = SDL_CreateRenderer(fenetre, -1, SDL_RENDERER_ACCELERATED);

    // Chargement de l'image de fond
    SDL_Texture * fond = charger_image("../src/img/fond.bmp", ecran);

    // Chargement de l'image de l'objet
    SDL_Texture * obj = charger_image_transparente("../src/img/obj.bmp", ecran, 255, 255, 255);
    int objetW, objetH;
    SDL_QueryTexture(obj, NULL, NULL, &objetW, &objetH);
    SDL_Rect SrcR, DestR;
    SrcR.x = 0;
    SrcR.y = 0;
    SrcR.w = objetW;
    SrcR.h = objetH;
    DestR.x = 350;
    DestR.y = 350;
    DestR.w = objetW / 3;
    DestR.h = objetH / 3;

    // Chargement de la feuille de sprites
    SDL_Texture* sprite = charger_image_transparente("../src/img/sprites.bmp", ecran, 0,255,255);
    SDL_Rect DestR_sprite[6], SrcR_sprite[6];
    int tailleW, tailleH;
    SDL_QueryTexture(sprite, NULL, NULL, &tailleW, &tailleH);
    tailleH /= 2;
    tailleW /= 3;
    for(int i=0; i<6; i++)
    {
        DestR_sprite[i].x = i > 2 ? 60*(i+1)+100 : 60*(i+1);
        DestR_sprite[i].y = i > 2 ? 60 : 120;
        DestR_sprite[i].w = tailleW;
        DestR_sprite[i].h = tailleH;
        SrcR_sprite[i].x = tailleW * (i % 3);
        SrcR_sprite[i].y = tailleH * (i / 3);
        SrcR_sprite[i].w = tailleW;
        SrcR_sprite[i].h = tailleH;
    }

    // Chargement du texte
    TTF_Init();
    TTF_Font *font = TTF_OpenFont("../src/font/arial.ttf",28);
    SDL_Color color = {0,0,0,0};
    char msg[] = "TP sur Makefile et SDL";
    SDL_Texture* texte = charger_texte(msg,ecran,font,color);
    SDL_Rect text_pos;
    int texteW, texteH;
    SDL_QueryTexture(texte, NULL, NULL, &texteW, &texteH);
    text_pos.x = 10;
    text_pos.y = 100;
    text_pos.w = texteW;
    text_pos.h = texteH;

    // Boucle principale
    bool terminer = false;
    while (!terminer)
    {
        SDL_RenderClear(ecran);

        SDL_RenderCopy(ecran, fond, NULL, NULL);

        SDL_RenderCopy(ecran, obj, &SrcR, &DestR);

        SDL_RenderCopy(ecran, texte, NULL, &text_pos);

        for (int i = 0; i < 6; i++)
        {
            SDL_RenderCopy(ecran, sprite, &SrcR_sprite[i], &DestR_sprite[i]);
        }

        SDL_Event evenements;
        SDL_PollEvent(&evenements);
        switch (evenements.type)
        {
        case SDL_QUIT:
            terminer = true;
            break;
        case SDL_KEYDOWN:
            switch (evenements.key.keysym.sym)
            {
            case SDLK_ESCAPE:
            case SDLK_q:
                terminer = true;
                break;
            }
        }

        SDL_RenderPresent(ecran);
    }
    // Quitter SDL
    TTF_CloseFont(font);
    TTF_Quit();
    SDL_DestroyTexture(fond);
    SDL_DestroyRenderer(ecran);
    SDL_DestroyWindow(fenetre);
    SDL_Quit();
    return 0;
}