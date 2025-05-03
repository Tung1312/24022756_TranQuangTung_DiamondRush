#include "../include/Renderer.h"
#include "../include/Constants.h"
#include "../include/Player.h"
#include "../include/GameTypes.h"
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include <algorithm>
#include <string>

extern SDL_Renderer* renderer;
extern SDL_Texture* mapTexture;
extern SDL_Texture* leavesTexture;
extern SDL_Texture* boulderTexture;
extern SDL_Texture* diamondTexture;
extern SDL_Texture* playerUnderBoulderTexture;
extern Player player;
extern TileList leavesTiles;
extern BlockList diamonds;
extern BlockList boulderTiles;
extern bool isPlayerUnderBoulder;
extern int diamondsCollected;
extern int leavesDestroyed;
extern float currentFPS;
extern TTF_Font* gameFont;

SDL_Texture* loadTexture(const char* path) {
    SDL_Texture* texture = IMG_LoadTexture(renderer, path);
    if (!texture) {
        std::cout << "Failed to load texture: " << IMG_GetError() << std::endl;
    }
    return texture;
}

SDL_Texture* renderText(const std::string& text, SDL_Color color) {
    if (!gameFont) {
        std::cout << "Font not loaded for text rendering" << std::endl;
        return nullptr;
    }
    
    // Render text to a surface
    SDL_Surface* textSurface = TTF_RenderText_Solid(gameFont, text.c_str(), color);
    if (!textSurface) {
        std::cout << "Failed to create text surface: " << TTF_GetError() << std::endl;
        return nullptr;
    }
    
    // Convert surface to texture
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    if (!textTexture) {
        std::cout << "Failed to create text texture: " << SDL_GetError() << std::endl;
    }
    
    // Free the surface as it's no longer needed
    SDL_FreeSurface(textSurface);
    
    return textTexture;
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
        SDL_Rect diamondRect;
        
        if (diamond.isFalling || diamond.needsUpdate) {
            //su dung vi tri pixel khi roi
            diamondRect = {
                static_cast<int>(diamond.pixelX) - offsetX,
                static_cast<int>(diamond.pixelY) - offsetY + verticalPadding,
                TILE_SIZE, TILE_SIZE
            };
        } else {
            //su dung vi tri grid khi khong roi
            diamondRect = {
                diamond.x * TILE_SIZE - offsetX,
                diamond.y * TILE_SIZE - offsetY + verticalPadding,
                TILE_SIZE, TILE_SIZE
            };
        }
        
        SDL_RenderCopy(renderer, diamondTexture, NULL, &diamondRect);
    }

    //render boulders
    for (const auto& boulder : boulderTiles) {
        SDL_Rect boulderRect;
        
        if (boulder.isFalling || boulder.needsUpdate) {
            boulderRect = {
                static_cast<int>(boulder.pixelX) - offsetX,
                static_cast<int>(boulder.pixelY) - offsetY + verticalPadding,
                TILE_SIZE, TILE_SIZE
            };
        } else {
            boulderRect = {
                boulder.x * TILE_SIZE - offsetX,
                boulder.y * TILE_SIZE - offsetY + verticalPadding,
                TILE_SIZE, TILE_SIZE
            };
        }
        
        SDL_RenderCopy(renderer, boulderTexture, NULL, &boulderRect);
    }

    //player render handling
    if (isPlayerUnderBoulder) {
        //render nguoi choi o duoi boulder
        SDL_Rect warningRect = {
            player.x * TILE_SIZE - offsetX,
            player.y * TILE_SIZE - offsetY + verticalPadding,  // Position above player
            TILE_SIZE, TILE_SIZE
        };
        SDL_RenderCopy(renderer, playerUnderBoulderTexture, NULL, &warningRect);
    } else {
    //render nguoi choi binh thuong
    player.render(renderer, offsetX, offsetY, verticalPadding);
    }

    //debug overlay
    if (DEBUG_MODE) {
        //debug info strings
        std::string fpsText = "FPS: " + std::to_string(static_cast<int>(currentFPS));
        
        std::string collectiblesText = "Diamonds: " + std::to_string(diamondsCollected) + 
                                      " | Leaves: " + std::to_string(leavesDestroyed);
        
        //player state
        std::string playerState;
        if (isPlayerUnderBoulder) {
            playerState = "UNDER BOULDER";
        } else if (player.isPushing) {
            playerState = "PUSHING BOULDER";
        } else if (player.isAnimating) {
            playerState = "STANDING";
        }
        
        std::string stateText = "State: " + playerState;
        
        std::string positionText = "Position: (" + std::to_string(player.x) + 
                                    ", " + std::to_string(player.y) + ") Facing: " + 
                                    (player.flip ? "Left" : "Right");
        
        std::string audioText = "Audio: Sound " + std::string(SOUND_ENABLED ? "ON" : "OFF") + 
                                " (" + std::to_string(static_cast<int>(SOUND_VOLUME * 100)) + "%) | Music " + 
                                std::string(MUSIC_ENABLED ? "ON" : "OFF") + 
                                " (" + std::to_string(static_cast<int>(MUSIC_VOLUME * 100)) + "%)";
        
        //background
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 180); // 70% opacity
        
        //tinh toan kich thuoc debug overlay
        int lineHeight = 16;
        int totalLines = 5; // FPS, collectibles, state, position, audio
        int totalHeight = lineHeight * totalLines + 10;
        
        SDL_Rect debugBgRect = {0, 0, SCREEN_WIDTH, totalHeight};
        SDL_RenderFillRect(renderer, &debugBgRect);
        
        //reset blend mode
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
        SDL_Color debugColor = {255, 255, 255, 255};
        
        //FPS counter
        SDL_Texture* fpsTexture = renderText(fpsText, debugColor);
        if (fpsTexture) {
            int textWidth, textHeight;
            SDL_QueryTexture(fpsTexture, NULL, NULL, &textWidth, &textHeight);
            SDL_Rect fpsRect = {5, 5, textWidth, textHeight};
            SDL_RenderCopy(renderer, fpsTexture, NULL, &fpsRect);
            SDL_DestroyTexture(fpsTexture);
        }
        
        //bo dem diamonds va leaves
        SDL_Texture* collectiblesTexture = renderText(collectiblesText, debugColor);
        if (collectiblesTexture) {
            int textWidth, textHeight;
            SDL_QueryTexture(collectiblesTexture, NULL, NULL, &textWidth, &textHeight);
            SDL_Rect collectiblesRect = {5, 5 + lineHeight, textWidth, textHeight};
            SDL_RenderCopy(renderer, collectiblesTexture, NULL, &collectiblesRect);
            SDL_DestroyTexture(collectiblesTexture);
        }
        
        // Player state display
        SDL_Texture* stateTexture = renderText(stateText, debugColor);
        if (stateTexture) {
            int textWidth, textHeight;
            SDL_QueryTexture(stateTexture, NULL, NULL, &textWidth, &textHeight);
            SDL_Rect stateRect = {5, 5 + lineHeight * 2, textWidth, textHeight};
            SDL_RenderCopy(renderer, stateTexture, NULL, &stateRect);
            SDL_DestroyTexture(stateTexture);
        }
        
        //player coords
        SDL_Texture* posTexture = renderText(positionText, debugColor);
        if (posTexture) {
            int textWidth, textHeight;
            SDL_QueryTexture(posTexture, NULL, NULL, &textWidth, &textHeight);
            SDL_Rect posRect = {5, 5 + lineHeight * 3, textWidth, textHeight};
            SDL_RenderCopy(renderer, posTexture, NULL, &posRect);
            SDL_DestroyTexture(posTexture);
        }
        
        //audio stuff br br
        SDL_Texture* audioTexture = renderText(audioText, debugColor);
        if (audioTexture) {
            int textWidth, textHeight;
            SDL_QueryTexture(audioTexture, NULL, NULL, &textWidth, &textHeight);
            SDL_Rect audioRect = {5, 5 + lineHeight * 4, textWidth, textHeight};
            SDL_RenderCopy(renderer, audioTexture, NULL, &audioRect);
            SDL_DestroyTexture(audioTexture);
        }
    }

    SDL_RenderPresent(renderer);
}