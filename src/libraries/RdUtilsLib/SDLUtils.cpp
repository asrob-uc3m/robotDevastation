#include "SDLUtils.hpp"

SDL_Surface * rd::RdImage2SDLImage(const RdImage & image)
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
