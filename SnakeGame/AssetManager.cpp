#include "AssetManager.h"
#include <iostream>

Engine::AssetManager::AssetManager()
{
}

Engine::AssetManager::~AssetManager()
{
}

void Engine::AssetManager::AddTexture(int id, const std::string& filePath, bool wantRepeated)
{
    auto texture = std::make_unique<sf::Texture>();
    if (texture->loadFromFile(filePath))  
    {
        texture->setRepeated(wantRepeated);
        m_textures[id] = std::move(texture);
    }
    else
    {
        std::cerr << "Error: Failed to load texture from " << filePath << std::endl;
    }
}


void Engine::AssetManager::AddFont(int id, const std::string& filePath)
{
	auto font = std::make_unique<sf::Font>();
	if (font->loadFromFile(filePath))
	{
		m_fonts[id] = std::move(font);
	}
}

const sf::Texture& Engine::AssetManager::GetTexture(int id) const
{
	return *(m_textures.at(id).get());
}

const sf::Font& Engine::AssetManager::GetFont(int id) const
{
	return *(m_fonts.at(id).get());

}
