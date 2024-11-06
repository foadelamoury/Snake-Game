#pragma once

#include <SFML/System/Time.hpp>

namespace Engine
{
	class State
	{

	public:
		State() {}
		/// <summary>
		/// Without virtual destruction won't happen properly for classes inheriting from State class and also it's an abstract class.
		/// </summary>
		virtual ~State() {} 
		virtual void Init() = 0;
		virtual void ProcessInput() = 0;
		virtual void Update(const sf::Time& deltaTime) = 0;;  // Any machine running will have the same FPS
		virtual void Draw() = 0;
		virtual void Pause(){};
		virtual void Start() {};



	};

}