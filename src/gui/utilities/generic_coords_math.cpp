#include <gui/utilities/generic_coords_math.h>

using namespace gui;

sf::Vector2f CoordsMath::getOffsetToCenter(const sf::FloatRect& parentContainer, const sf::FloatRect& childContainer) {
    float offsetX = (parentContainer.width - childContainer.width) / 2 - childContainer.left;
    float offsetY = (parentContainer.height - childContainer.height) / 2 - childContainer.top;
    return sf::Vector2f(offsetX, offsetY);
}