#include "CodePainPCH.h"
#include "SpriteComponent.h"
#include "BaseComponent.h"
#include "Texture2D.h"
#include "Logger.h"
#include "GameObject.h"
#include "Observer.h"
#include <string>

cp::SpriteComponent::SpriteComponent(SDL_Texture* spriteSheet)
	: BaseComponent(cp::ComponentType::_SpriteComponent)
	, m_CurrentAnimation{ 0 }
	, m_NrOfFinishedFrames{ 0 }
	, m_NrOfFinishedLoops{ 0 }
	, m_CurrentAnimTime{ 0.f }
	, m_Pause{ true }
	, m_TextureComponent{ new Texture2D{spriteSheet} }
{
	UNREFERENCED_PARAMETER(spriteSheet);
}

cp::SpriteComponent::~SpriteComponent()
{
	SAFE_DELETE(m_TextureComponent);
}

void cp::SpriteComponent::Update(const float elapsedSec)
{
	if (m_TextureComponent)
		m_TextureComponent->Update(elapsedSec);

	if (m_CurrentAnimation == 0)
		return;

	if (m_Pause)
		return;
	

	const Sprite sprite = m_Sprites[m_CurrentAnimation];
	if (sprite.nrOfLoops != 0 && m_NrOfFinishedLoops >= sprite.nrOfLoops)
	{
		m_pOwner->NotifyObservers(cp::Event::EVENT_ANIMATION_FINISHED);
		return;
	}

	float nextFrameTime = sprite.frameTime * (m_NrOfFinishedFrames + 1);

	m_CurrentAnimTime += elapsedSec;
	if (m_CurrentAnimTime >= nextFrameTime)
	{
		m_NrOfFinishedFrames++;
		bool calculateNextFrame = true;
		if ((m_NrOfFinishedFrames % (sprite.spritesX * sprite.spritesY)) == 0)
		{
			m_NrOfFinishedLoops++;
			if (sprite.nrOfLoops != 0)
			{
				if((m_NrOfFinishedLoops % sprite.nrOfLoops) == 0)
				{
					(sprite.endOnFirstFrame) ? calculateNextFrame = true : calculateNextFrame = false;
				}
			}
		}

		if (calculateNextFrame)
		{
			SDL_Rect newSrcRect{};
			newSrcRect.x = int(sprite.xOffset + sprite.spriteWidth * (m_NrOfFinishedFrames % sprite.spritesX));
			newSrcRect.y = int(sprite.yOffset + sprite.spriteHeight * (((m_NrOfFinishedFrames % (sprite.spritesX * sprite.spritesY) ) / sprite.spritesX)));
			newSrcRect.w = int(sprite.spriteWidth);
			newSrcRect.h = int(sprite.spriteHeight);
			m_TextureComponent->SetSrcRect(newSrcRect);
		}
	}
}

void cp::SpriteComponent::FixedUpdate(const float elapsedSec)
{
	if (m_TextureComponent)
		m_TextureComponent->FixedUpdate(elapsedSec);
}

void cp::SpriteComponent::Draw() const
{
	if (m_TextureComponent)
		m_TextureComponent->Draw();
}

void cp::SpriteComponent::DebugDraw() const
{
	if (m_TextureComponent)
		m_TextureComponent->DebugDraw();
}

void cp::SpriteComponent::Pause()
{
	m_Pause = true;
}

void cp::SpriteComponent::UnPause()
{
	if (m_CurrentAnimation == 0)
		return;

	if (m_Sprites[m_CurrentAnimation].nrOfLoops != 0)
		if (m_NrOfFinishedLoops >= m_Sprites[m_CurrentAnimation].nrOfLoops)
			return;

	m_Pause = false;
}

void cp::SpriteComponent::SetAnimation(const std::string& name)
{
	unsigned int strHash = (unsigned int)std::hash<std::string>{}(name);

	if (m_Sprites.find(strHash) != m_Sprites.end())
	{
		m_CurrentAnimation = strHash;
		ResetSpriteTimeData();
		Sprite sprite = m_Sprites[strHash];
		SDL_Rect newSrcRect{};
		newSrcRect.x = int(sprite.xOffset + sprite.spriteWidth * (m_NrOfFinishedFrames % sprite.spritesX));
		newSrcRect.y = int(sprite.yOffset + sprite.spriteHeight * ((m_NrOfFinishedFrames % (sprite.spritesX * sprite.spritesY)) / sprite.spritesY));
		newSrcRect.w = int(sprite.spriteWidth);
		newSrcRect.h = int(sprite.spriteHeight);
		m_TextureComponent->SetSrcRect(newSrcRect);
		m_TextureComponent->FlipTexture(sprite.flipTexture);
	}
#if defined(_DEBUG)
	else
	{
		cp::Logger::GetInstance().Log(cp::LogLevel::Warning, "animation: " + name + " is not a valid anim!");
	}
#endif
}

void cp::SpriteComponent::AddAnimation(const std::string& name, const Sprite& sprite)
{
	unsigned int strHash = (unsigned int)std::hash<std::string>{}(name);
#if defined(_DEBUG)
	if (m_Sprites.find(strHash) != m_Sprites.end())
	{
		cp::Logger::GetInstance().Log(cp::LogLevel::Warning, "SpriteComponent::AddAnimation animation with name: " + name + " already exists!");
		return;
	}
#endif
	m_Sprites.insert(std::make_pair(strHash, sprite));
}

void cp::SpriteComponent::ResetSpriteTimeData()
{
	m_NrOfFinishedFrames = 0;
	m_NrOfFinishedLoops = 0;
	m_CurrentAnimTime = 0.f;
}
