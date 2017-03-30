// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#ifndef __RD_SDL_UTILS_HPP__
#define __RD_SDL_UTILS_HPP__

#include <SDL.h>

#include "ImageManager.hpp"

namespace rd {

SDL_Surface * RdImage2SDLImage(const Image & image);
bool initSDL();
bool cleanupSDL();

}

#endif  // __RD_SDL_UTILS_HPP__
