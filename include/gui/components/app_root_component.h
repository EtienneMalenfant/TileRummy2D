#include <SFML/Graphics.hpp>

namespace gui {

    class AppRootComponent : public sf::Drawable {
    private:
        std::vector<sf::Drawable*>* _components;
    public:
        AppRootComponent(std::vector<sf::Drawable*>* components);
        ~AppRootComponent() override;
        void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    };
}
