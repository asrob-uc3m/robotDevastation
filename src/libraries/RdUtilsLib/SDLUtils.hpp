// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#ifndef __SDL_UTILS_HPP__
#define __SDL_UTILS_HPP__

#include <RdImageManager.hpp>
#include <SDL/SDL_image.h>

namespace rd {

SDL_Surface * RdImage2SDLImage(RdImage image);

}

#endif  // __SDL_UTILS_HPP__
