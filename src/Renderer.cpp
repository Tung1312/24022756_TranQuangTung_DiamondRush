#include "../include/Renderer.h"
#include "../include/Constants.h"
#include "../include/Player.h"
#include "../include/GameTypes.h"
#include <SDL2/SDL_image.h>
#include <iostream>
#include <algorithm>

extern SDL_Renderer* renderer;
extern SDL_Texture* mapTexture;
extern SDL_Texture* leavesTexture;
extern SDL_Texture* boulderTexture;
extern SDL_Texture* diamondTexture;
extern Player player;
extern TileList leavesTiles;
extern BlockList diamonds;
extern BlockList boulderTiles;

SDL_Texture* loadTexture(const char* path) {
    SDL_Texture* texture = IMG_LoadTexture(renderer, path);
    if (!texture) {
        std::cout << "Failed to load texture: " << IMG_GetError() << std::endl;
    }
    return texture;
}

void render() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    //tinh toan vi tri cua camera, nguoi choi luon o chinh giua man hinh
    int offsetX = player.x * TILE_SIZE - SCREEN_WIDTH / 2 + TILE_SIZE / 2;
    int offsetY = player.y * TILE_SIZE - SCREEN_HEIGHT / 2 + TILE_SIZE / 2;

    //gioi han vi tri camera
    offsetX = std::max(0, std::min(offsetX, GRID_WIDTH - SCREEN_WIDTH));
    offsetY = std::max(0, std::min(offsetY, GRID_HEIGHT - SCREEN_HEIGHT));

    //tinh toan padding doc
    int verticalPadding = (SCREEN_HEIGHT - GRID_HEIGHT) / 2;
    verticalPadding = std::max(0, verticalPadding);

    //offset vi tri map
    SDL_Rect mapSrcRect = {
        offsetX, offsetY,
        SCREEN_WIDTH, SCREEN_HEIGHT
    };
    
    SDL_Rect mapDestRect = {
        0, verticalPadding,
        SCREEN_WIDTH, GRID_HEIGHT
    };
    
    SDL_RenderCopy(renderer, mapTexture, &mapSrcRect, &mapDestRect);

    //render leaves
    for (const auto& leaf : leavesTiles) {
        SDL_Rect leafRect = {
            leaf.first * TILE_SIZE - offsetX,
            leaf.second * TILE_SIZE - offsetY + verticalPadding,
            TILE_SIZE, TILE_SIZE
        };
        SDL_RenderCopy(renderer, leavesTexture, NULL, &leafRect);
    }

    //render diamonds
    for (const auto& diamond : diamonds) {
        SDL_Rect diamondRect = {
            diamond.x * TILE_SIZE - offsetX,
            diamond.y * TILE_SIZE - offsetY + verticalPadding,
            TILE_SIZE, TILE_SIZE
        };
        SDL_RenderCopy(renderer, diamondTexture, NULL, &diamondRect);
    }

    //render boulders
    for (const auto& boulder : boulderTiles) {
        SDL_Rect boulderRect = {
            boulder.x * TILE_SIZE - offsetX,
            boulder.y * TILE_SIZE - offsetY + verticalPadding,
            TILE_SIZE, TILE_SIZE
        };
        SDL_RenderCopy(renderer, boulderTexture, NULL, &boulderRect);
    }

    //render nguoi choi
    SDL_Rect playerRect = {
        player.x * TILE_SIZE - offsetX,
        player.y * TILE_SIZE - offsetY + verticalPadding,
        TILE_SIZE, TILE_SIZE
    };
    
    SDL_RenderCopyEx(renderer, player.texture, NULL, &playerRect, 0, NULL, 
                    player.flip ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE);

    SDL_RenderPresent(renderer);
}