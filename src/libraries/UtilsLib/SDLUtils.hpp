// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#ifndef __SDL_UTILS_HPP__
#define __SDL_UTILS_HPP__

#include "ImageManager.hpp"
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

namespace rd {

SDL_Surface * RdImage2SDLImage(const Image & image);
bool initSDL();
bool cleanupSDL();

}

#endif  // __SDL_UTILS_HPP__
