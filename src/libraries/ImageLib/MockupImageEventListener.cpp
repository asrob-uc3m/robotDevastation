#include "MockupImageEventListener.hpp"


rd::RdMockupImageEventListener::RdMockupImageEventListener()
{
    images_arrived = 0;
}

bool rd::RdMockupImageEventListener::onImageArrived(rd::RdImageManager *manager)
{
    stored_image = manager->getImage();
    images_arrived++;
    return true;
}

int rd::RdMockupImageEventListener::getImagesArrived() { return images_arrived; }

void rd::RdMockupImageEventListener::resetImagesArrived() { images_arrived = 0; }

rd::RdImage rd::RdMockupImageEventListener::getStoredImage() { return stored_image; }
