// Authors: see AUTHORS.md at project root.
// CopyPolicy: released under the terms of the LGPLv2.1, see LICENSE at project root.
// URL: https://github.com/asrob-uc3m/robotDevastation

#include "SDLUtils.hpp"

#include <SDL_image.h>
#include <SDL_ttf.h>

#include <yarp/os/LogStream.h>

#include "LogComponent.hpp"

SDL_Surface * rd::RdImage2SDLImage(const Image & image)
{
    if (image.width()==0)
        return NULL;

    //-- Image parameters (in the future they could be parameters of this function
    int cameraDepth=8;  // the depth of the surface in bits
    int cameraChannels=3;  // R G B
    int cameraWidthstep = image.getRowSize(); // the length of a row of pixels in bytes 1920 for 640x480
    int cameraWidth = image.width();
    int cameraHeight = image.height();


    // http://stackoverflow.com/1questions/393954/how-to-convert-an-opencv-iplimage-to-an-sdl-surface
    return SDL_CreateRGBSurfaceFrom( (void*)image.getRawImage(),
                                         cameraWidth, cameraHeight,
                                         cameraDepth*cameraChannels,
                                         cameraWidthstep,
                                         0x0000ff, 0x00ff00, 0xff0000, 0
                                       );
}

bool rd::initSDL()
{
    //-- Init SDL
    if(SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        yCError(RD_UTIL) << "SDL could not initialize:" << SDL_GetError();
        return false;
    }

    //-- Init ttf
    if (TTF_Init() == -1)
    {
        yCError(RD_UTIL) << "SDL_ttf could not initialize:" << TTF_GetError();
        cleanupSDL();
        return false;
    }

    //Initialize PNG loading
    if(!(IMG_Init(IMG_INIT_PNG)&IMG_INIT_PNG))
    {
        yCError(RD_UTIL) << "SDL_image could not initialize:" << IMG_GetError();
        cleanupSDL();
        return false;
    }
    return true;
}

bool rd::cleanupSDL()
{
    yCInfo(RD_UTIL) << "Freeing resources...";
    IMG_Quit();
    TTF_Quit();
    SDL_Quit();
    return true;
}
