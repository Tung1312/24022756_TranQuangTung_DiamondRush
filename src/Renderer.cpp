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
extern SDL_Texture* skeletonTexture;
extern SDL_Texture* guiTexture;
extern SDL_Texture* menuBackgroundTexture;
extern SDL_Texture* pressSpaceTexture;
extern Player player;
extern TileList leavesTiles;
extern BlockList diamonds;
extern BlockList boulderTiles;
extern bool isPlayerUnderBoulder;
extern bool isPlayerDead;
extern bool isPlayingDeathSequence;
extern int diamondsCollected;
extern int leavesDestroyed;
extern float currentFPS;
extern TTF_Font* gameFont;
extern bool showDebugOverlay;
extern Uint32 playerUnderBoulderStartTime;
extern Uint32 BOULDER_CRUSH_TIME;
extern bool transitionToSkeletonDone;
extern bool darkenElementsDone;

SDL_Texture* loadTexture(const char* path) {
    SDL_Texture* texture = IMG_LoadTexture(renderer, path);
    if (!texture) {
        printf("Failed to load texture: %s\n", IMG_GetError());
    }
    return texture;
}

SDL_Texture* renderText(const std::string& text, SDL_Color color) {
    if (!gameFont) {
        printf("Font not loaded for text rendering\n");
        return nullptr;
    }
    
    SDL_Surface* textSurface = TTF_RenderText_Solid(gameFont, text.c_str(), color);
    if (!textSurface) {
        printf("Failed to create text surface: %s\n", TTF_GetError());
        return nullptr;
    }
    
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    if (!textTexture) {
        printf("Failed to create text texture: %s\n", SDL_GetError());
    }
    
    SDL_FreeSurface(textSurface);
    
    return textTexture;
}

void render() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    if (isPlayerDead) {
        SDL_SetTextureColorMod(mapTexture, 0, 0, 0);
        SDL_SetTextureColorMod(leavesTexture, 0, 0, 0);
        SDL_SetTextureColorMod(diamondTexture, 0, 0, 0);
        SDL_SetTextureColorMod(boulderTexture, 0, 0, 0);
    } else {
        SDL_SetTextureColorMod(mapTexture, 255, 255, 255);
        SDL_SetTextureColorMod(leavesTexture, 255, 255, 255);
        SDL_SetTextureColorMod(diamondTexture, 255, 255, 255);
        SDL_SetTextureColorMod(boulderTexture, 255, 255, 255);
    }

    //tinh toan vi tri camera
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
        SCREEN_WIDTH, SCREEN_HEIGHT - 2 * verticalPadding
    };
    
    SDL_Rect mapDestRect = {
        0, verticalPadding,
        SCREEN_WIDTH, SCREEN_HEIGHT - 2 * verticalPadding
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

    //player rendering
    if (isPlayerDead) {
        //skeleton
        SDL_Rect skeletonRect = {
            player.x * TILE_SIZE - offsetX,
            player.y * TILE_SIZE - offsetY + verticalPadding,
            TILE_SIZE, TILE_SIZE
        };
        
        SDL_RenderCopy(renderer, skeletonTexture, NULL, &skeletonRect);
    } else if (isPlayerUnderBoulder) {
        //under boulder
        SDL_Rect playerRect = {
            player.x * TILE_SIZE - offsetX,
            player.y * TILE_SIZE - offsetY + verticalPadding,
            TILE_SIZE, TILE_SIZE
        };
        SDL_RenderCopy(renderer, playerUnderBoulderTexture, NULL, &playerRect);
    } else {
        //normal player
        player.render(renderer, offsetX, offsetY, verticalPadding);
    }

    //render GUI overlay
    SDL_Rect guiRect = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
    SDL_RenderCopy(renderer, guiTexture, NULL, &guiRect);

    //render the debug display if enabled
    if (DEBUG_MODE && showDebugOverlay) {
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
        
        //collectibles counter
        SDL_Texture* collectiblesTexture = renderText(collectiblesText, debugColor);
        if (collectiblesTexture) {
            int textWidth, textHeight;
            SDL_QueryTexture(collectiblesTexture, NULL, NULL, &textWidth, &textHeight);
            SDL_Rect collectiblesRect = {5, 5 + lineHeight, textWidth, textHeight};
            SDL_RenderCopy(renderer, collectiblesTexture, NULL, &collectiblesRect);
            SDL_DestroyTexture(collectiblesTexture);
        }
        
        //player state display
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
        
        //audio
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

void renderMenu(bool transitioning) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    
    //render background
    SDL_Rect menuRect = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
    SDL_RenderCopy(renderer, menuBackgroundTexture, NULL, &menuRect);
     
    //kich thuoc'press space'
    int spaceWidth = 131;
    int spaceHeight = 12;
    SDL_SetTextureAlphaMod(pressSpaceTexture, 255);
    
    //render 'press space'
    if (!transitioning) {
        SDL_Rect spaceRect = {
            (SCREEN_WIDTH - spaceWidth) / 2,
            240,
            spaceWidth, 
            spaceHeight
        };
        SDL_RenderCopy(renderer, pressSpaceTexture, NULL, &spaceRect);
    }
    
    SDL_RenderPresent(renderer);
}