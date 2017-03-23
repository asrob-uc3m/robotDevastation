#include "MockImageEventListener.hpp"


rd::MockImageEventListener::MockImageEventListener()
{
    images_arrived = 0;
}

bool rd::MockImageEventListener::onImageArrived(rd::ImageManager *manager)
{
    stored_image = manager->getImage();
    images_arrived++;
    return true;
}

int rd::MockImageEventListener::getImagesArrived() { return images_arrived; }

void rd::MockImageEventListener::resetImagesArrived() { images_arrived = 0; }

rd::Image rd::MockImageEventListener::getStoredImage() { return stored_image; }
