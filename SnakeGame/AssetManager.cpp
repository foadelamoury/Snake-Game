#include "AssetManager.h"

Engine::AssetManager::AssetManager()
{
}

Engine::AssetManager::~AssetManager()
{
}

void Engine::AssetManager::AddTexture(int id, const std::string& filePath, bool wantRepeated)
{
	auto Texture = std::make_unique<sf::Texture>();
	if (!Texture->loadFromFile(filePath))
	{
		Texture->setRepeated(wantRepeated);
		m_textures[id] = std::move(Texture);
	}
}

void Engine::AssetManager::AddFont(int id, const std::string& filePath)
{
}

const sf::Texture& Engine::AssetManager::GetTexture(int id) const
{
	return *(m_textures.at(id).get());
}

const sf::Font& Engine::AssetManager::GetFont(int id) const
{
	return *(m_fonts.at(id).get());

}
