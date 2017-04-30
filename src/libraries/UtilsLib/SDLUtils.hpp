// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#ifndef __RD_SDL_UTILS_HPP__
#define __RD_SDL_UTILS_HPP__

#include <SDL.h>

#include "ImageManager.hpp"

namespace rd {

//! @ingroup UtilsLib
SDL_Surface * RdImage2SDLImage(const Image & image);

//! @ingroup UtilsLib
bool initSDL();

//! @ingroup UtilsLib
bool cleanupSDL();

}

#endif  // __RD_SDL_UTILS_HPP__
