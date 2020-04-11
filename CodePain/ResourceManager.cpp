#include "CodePainPCH.h"
#include "ResourceManager.h"
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include "Renderer.h"
#include "Texture2D.h"
#include "Font.h"
#include "Logger.h"

void cp::ResourceManager::Init(const std::string& dataPath)
{
	m_DataPath = dataPath;

	// load support for png and jpg, this takes a while!

	if ((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) != IMG_INIT_PNG) 
	{
		Logger::GetInstance().Log(LogLevel::Critical, std::string("Failed to load support for png's: ") + SDL_GetError(), true);
	}

	if ((IMG_Init(IMG_INIT_JPG) & IMG_INIT_JPG) != IMG_INIT_JPG) 
	{
		Logger::GetInstance().Log(LogLevel::Critical, std::string("Failed to load support for jpg's: ") + SDL_GetError(), true);
	}

	if (TTF_Init() != 0) 
	{
		Logger::GetInstance().Log(LogLevel::Critical, std::string("Failed to load support for fonts: ") + SDL_GetError(), true);
	}
}

SDL_Texture* cp::ResourceManager::LoadSDLTexture(const std::string& file) const
{
	const std::string fullPath = m_DataPath + file;
	SDL_Texture* texture = IMG_LoadTexture(Renderer::GetInstance().GetSDLRenderer(), fullPath.c_str());
	if (texture == nullptr) 
	{
		Logger::GetInstance().Log(LogLevel::Critical, std::string("Failed to load texture: ") + SDL_GetError(), true);
	}
	return texture;
}

SDL_Surface* cp::ResourceManager::LoadSDLSurface(const std::string& file) const
{
	const std::string fullPath = m_DataPath + file;
	SDL_Surface* pSurf = IMG_Load(fullPath.c_str());
	if (pSurf == nullptr)
	{
		Logger::GetInstance().Log(LogLevel::Critical,std::string("Failed to load surface: ") + SDL_GetError(),true);
	}
	return pSurf;
}

cp::Font* cp::ResourceManager::LoadFont(const std::string& file, unsigned int size) const
{
	const std::string fullPath = m_DataPath + file;
	return new Font(m_DataPath + file, size);
}

Uint32 cp::ResourceManager::GetPixel(const SDL_Surface* pSurface, const int x, const int y) const
{
	//function gotten from http://sdl.beuc.net/sdl.wiki/Pixel_Access
	int bpp = pSurface->format->BytesPerPixel;
	/* Here p is the address to the pixel we want to retrieve */
	Uint8* p = (Uint8*)pSurface->pixels + y * pSurface->pitch + x * bpp;

	switch (bpp)
	{
	case 1:
		return *p;
		break;
	case 2:
		return *(Uint16*)p;
		break;
	case 3:
#if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
			return p[0] << 16 | p[1] << 8 | p[2];
#else
			return p[0] | p[1] << 8 | p[2] << 16;
#endif
		break;
	case 4:
		return *(Uint32*)p;
		break;
	default:
		return 0;       /* shouldn't happen, but avoids warnings */
	}
}