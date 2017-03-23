#include "MockupImageEventListener.hpp"


rd::MockupImageEventListener::MockupImageEventListener()
{
    images_arrived = 0;
}

bool rd::MockupImageEventListener::onImageArrived(rd::ImageManager *manager)
{
    stored_image = manager->getImage();
    images_arrived++;
    return true;
}

int rd::MockupImageEventListener::getImagesArrived() { return images_arrived; }

void rd::MockupImageEventListener::resetImagesArrived() { images_arrived = 0; }

rd::Image rd::MockupImageEventListener::getStoredImage() { return stored_image; }
