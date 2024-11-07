#pragma once
#include <list>

#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>


static bool snakeMoved = false;
class Snake : public sf::Drawable
{
  private:
	std::list<sf::Sprite> m_body;
	std::list<sf::Sprite>::iterator m_head;
	std::list<sf::Sprite>::iterator m_tail;

	float speed;

	int health;
  public: 
		
		Snake();
		~Snake();
		void Init(const sf::Texture& texture);
		void Move(const sf::Vector2f& direction);
		bool IsOn(const sf::Sprite& other) const;
		void Grow(const sf::Vector2f& direction);

		#pragma region Change Texture (Test1)

				//void ChangeTexture(sf::Texture texture);
		#pragma endregion

		void ChangeTexture(const sf::Texture& texture);


		bool IsSelfIntersecting() const;


		int GetHealth() const
		{
			return health;

		}
		float GetSpeed() const
		{
			return speed;
		}
		
		sf::Sprite GetSnakeHead() const
		{
			return m_body.end() != m_head ? *m_head : sf::Sprite();
		}

		void SetHealth(int health)
		{
			this->health = health;
		}

		void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};

