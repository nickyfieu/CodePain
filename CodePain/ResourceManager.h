#pragma once
#include "Singleton.h"

struct SDL_Texture;
namespace cp
{
	class Font;
	class ResourceManager final : public Singleton<ResourceManager>
	{
	public:
		void Init(const std::string& data);
		SDL_Texture* LoadSDLTexture(const std::string& file) const;
		Font* LoadFont(const std::string& file, unsigned int size) const;
	private:
		friend class Singleton<ResourceManager>;
		ResourceManager() = default;
		std::string m_DataPath;
	};
}
