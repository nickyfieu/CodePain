#pragma once
#include "BaseComponent.h"

#include <unordered_map>
#include <SDL.h>

namespace cp
{
	struct Sprite
	{
		// offset from 0,0
		size_t xOffset; 
		size_t yOffset;
		// sprite size
		size_t spriteWidth;
		size_t spriteHeight;
		// amount of sprites in x / y
		size_t spritesX;
		size_t spritesY;
		// duration of a frame in seconds;
		float frameTime = 1.0f; 
		// flips the texture 
		bool flipTexture = false;
		// endOnFirstFrame  ( end at the last frame or first frame of the animation )
		bool endOnFirstFrame = false;
		// nr of loops 0 = replaying, otherwise stops after x loops
		size_t nrOfLoops = 0;
	};

	class Texture2D;
	class SpriteComponent final : public BaseComponent
	{
	public:
		explicit SpriteComponent(SDL_Texture* spriteSheet);
		virtual ~SpriteComponent();

		virtual void Update(float elapsedSec) override;
		virtual void FixedUpdate(float elapsedSec) override;
		virtual void Draw() const override;
		virtual void DebugDraw() const override;

		SpriteComponent(const SpriteComponent&) = delete;
		SpriteComponent(SpriteComponent&&) = delete;
		SpriteComponent& operator= (const SpriteComponent&) = delete;
		SpriteComponent& operator= (const SpriteComponent&&) = delete;

		void Pause();
		void UnPause();
		void SetAnimation(const std::string& name);
		void AddAnimation(const std::string& name, const Sprite& sprite);

		bool IsFinished() const { return m_NrOfFinishedLoops >= m_Sprites.at(m_CurrentAnimation).nrOfLoops; }
		Texture2D* GetTextureComponent() const { return m_TextureComponent; }

	protected:
		void ResetSpriteTimeData();

		bool m_Pause;
		// 0 on current animation == not a valid animation
		unsigned int m_CurrentAnimation;
		unsigned int m_NrOfFinishedFrames;
		unsigned int m_NrOfFinishedLoops;
		float m_CurrentAnimTime;

		Texture2D* m_TextureComponent;

		// size_t of unordered map == std::hash<std::string>{}(name);
		std::unordered_map<unsigned int, Sprite> m_Sprites;
	};

}