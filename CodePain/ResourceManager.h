#pragma once
#include "Singleton.h"
#include <SDL_surface.h>

struct SDL_Texture;
namespace cp
{
	class Font;
	class ResourceManager final : public Singleton<ResourceManager>
	{
	public:
		virtual ~ResourceManager() = default;

		ResourceManager(const ResourceManager& other) = delete;
		ResourceManager(ResourceManager&& other) = delete;
		ResourceManager& operator=(const ResourceManager& other) = delete;
		ResourceManager& operator=(ResourceManager&& other) = delete;

		void Init(const std::string& data);
		SDL_Texture* LoadSDLTexture(const std::string& file) const;
		SDL_Surface* LoadSDLSurface(const std::string& file) const;
		Uint32 GetPixel(const SDL_Surface* pSurface, const int x, const int y) const;
		Font* LoadFont(const std::string& file, unsigned int size) const;

		inline const std::string& GetDataPath() const { return m_DataPath; }
	private:
		friend class Singleton<ResourceManager>;
		ResourceManager() = default;
		std::string m_DataPath;
	};
}
