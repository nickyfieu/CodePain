#pragma once
#include "BaseComponent.h"
#include <SDL.h>
namespace cp
{
	// CollisionSide has 4 bits when multiple of them are set that means there is collision on those
	// sides of the collision box.
	// Note! the collision box component should never get a collision side with more than 1 bit set
	// The reason for only 1 bit should be set is that then you can ask for all collision boxes
	// With that specific bit set.
	enum CollisionSide : unsigned char
	{
		// the collision side is on the Right side of the block/blocks
		right = 0b0001,
		// the collision side is on the Up side of the block/blocks
		up = 0b0010,
		// the collision side is on the Left side of the block/blocks
		left = 0b0100,
		// the collision side is on the Down side of the block/blocks
		down = 0b1000
	};

	class CollisionBox final : public BaseComponent
	{
	public:
		CollisionBox(int x, int y, int width, int height, CollisionSide side);
		virtual ~CollisionBox() = default;

		virtual void Update(float elapsedSec) override;

		CollisionBox(const CollisionBox& other) = delete;
		CollisionBox(CollisionBox&& other) = delete;
		CollisionBox& operator=(const CollisionBox& other) = delete;
		CollisionBox& operator=(CollisionBox&& other) = delete;

		inline const SDL_Rect& GetCollisionBox() const { return m_CollisionBox; }
		inline CollisionSide GetCollisionSide() const { return m_CollisionSide; }
	private:
		const CollisionSide m_CollisionSide;
		const SDL_Rect m_CollisionBox;
	};
}