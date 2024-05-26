#include <SFML/Graphics.hpp>

class Tile
{
	public:
        Tile(sf::Texture& texture, sf::IntRect textureRect, int type, int zIndex, sf::Vector2f position) :
            sprite(texture, textureRect),
            type(type),
            zIndex(zIndex)
        {
            sprite.setPosition(position);
        }

        void draw(sf::RenderWindow& window) 
        {
            window.draw(sprite);
        }

        int type;
        int zIndex;

        sf::Vector2f getPosition()
        {
            return sprite.getPosition();
        }


	private:
		sf::Sprite sprite;

};