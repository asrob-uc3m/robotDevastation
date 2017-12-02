// Authors: see AUTHORS.md at project root.
// CopyPolicy: released under the terms of the LGPLv2.1, see LICENSE at project root.
// URL: https://github.com/asrob-uc3m/robotDevastation

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
