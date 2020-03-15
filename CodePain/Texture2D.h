#pragma once
#include "BaseComponent.h"

struct SDL_Texture;
namespace cp
{
	/**
	 * Simple RAII wrapper for an SDL_Texture
	 */
	class Transform;
	class Texture2D final : public BaseComponent
	{
	public:
		explicit Texture2D(SDL_Texture* texture);
		virtual ~Texture2D();

		virtual void Update(float elapsedSec) override;

		Texture2D(const Texture2D &) = delete;
		Texture2D(Texture2D &&) = delete;
		Texture2D & operator= (const Texture2D &) = delete;
		Texture2D & operator= (const Texture2D &&) = delete;

		void SetLocalOffset(float x, float y, float z);
		glm::vec3 GetLocalOffset() const;
		SDL_Texture* GetSDLTexture() const;

	private:
		SDL_Texture* m_pTexture;
		Transform* m_pLocalOffset;
	};
}
