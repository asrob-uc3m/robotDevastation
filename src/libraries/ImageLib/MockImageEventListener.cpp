// Authors: see AUTHORS.md at project root.
// CopyPolicy: released under the terms of the LGPLv2.1, see LICENSE at project root.
// URL: https://github.com/asrob-uc3m/robotDevastation

#include "MockImageEventListener.hpp"

rd::MockImageEventListener::MockImageEventListener()
{
    images_arrived = 0;
}

bool rd::MockImageEventListener::onImageArrived(ImageManager *manager)
{
    stored_image = manager->getImage();
    images_arrived++;
    return true;
}

int rd::MockImageEventListener::getImagesArrived() const { return images_arrived; }

void rd::MockImageEventListener::resetImagesArrived() { images_arrived = 0; }

rd::Image rd::MockImageEventListener::getStoredImage() { return stored_image; }
