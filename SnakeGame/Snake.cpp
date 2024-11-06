#include "Snake.h"
Snake::Snake() : m_body(std::list<sf::Sprite>(4)) , speed(16.f)
{
    m_head = --m_body.end();
    m_tail = m_body.begin();
}

Snake::~Snake()
{
}

void Snake::Init(const sf::Texture& texture)
{
    float x = speed;
    for (auto& piece : m_body)
    {
        piece.setTexture(texture);
        piece.setPosition({ 64.f, 64.f });
        x += speed;
    }
    
}

void Snake::Move(const sf::Vector2f& direction)
{
    m_tail->setPosition(m_head->getPosition() + direction);
    m_head = m_tail;
    ++m_tail;

    if (m_tail == m_body.end())
    {
        m_tail = m_body.begin();
    }
}

bool Snake::IsOn(const sf::Sprite& other) const
{
    return other.getGlobalBounds().intersects(m_head->getGlobalBounds());
}

void Snake::Grow(const sf::Vector2f& direction)
{
    sf::Sprite newPiece;
    newPiece.setTexture(*(m_body.begin()->getTexture()));
    newPiece.setPosition(m_head->getPosition() + direction);

    m_head = m_body.insert(++m_head, newPiece);
}

#pragma region Change Texture (Test1)
//void Snake::ChangeTexture(sf::Texture texture)
//{
//
//    for (auto piece = m_body.begin(); piece != m_body.end(); ++piece)
//    {
//        piece->setTexture(texture, true);
//    }
//}
#pragma endregion

void Snake::ChangeTexture(const sf::Texture& texture)
{
    for (auto& piece : m_body)
    {
        piece.setTexture(texture);
    }
}

bool Snake::IsSelfIntersecting() const
{
    bool flag = false;

    if (snakeMoved)
    {
        for (auto piece = m_body.begin(); piece != m_body.end(); ++piece)
        {
            if (m_head != piece)
            {
                flag = IsOn(*piece);

                if (flag)
                {
                    break;
                }
            }
        }
    }

    return flag;
}

void Snake::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    for (auto& piece : m_body)
    {
        target.draw(piece);
    }
}