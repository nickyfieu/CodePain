#pragma once
#include "BaseComponent.h"
#include <SDL.h>

namespace cp
{
	class RigidBody;
	class CollisionBox final : public BaseComponent
	{
	public:

		// CollisionSide has 4 bits when multiple of them are set that means there is collision on those
		// sides of the collision box.
		// Note! the collision box component should never get a collision side with more than 1 bit set
		// The reason for only 1 bit should be set is that then you can ask for all collision boxes
		// With that specific bit set.
		enum CollisionSide : unsigned char
		{
			// collides with all sides 
			all		= 0b1111,
			// the collision side is on the Right side of the block/blocks
			// collides with right level collisions
			right	= 0b0001,
			// the collision side is on the Up side of the block/blocks
			// collides with up level collisions
			up		= 0b0010,
			// the collision side is on the Left side of the block/blocks
			// collides with left level collisions
			left	= 0b0100,
			// the collision side is on the Down side of the block/blocks
			// collides with down level collisions
			down	= 0b1000
		};

		// thanks tom :)

		enum CollisionType
		{
			_static,
			_dynamic
		};

		CollisionBox(int x, int y, int width, int height, CollisionSide side, CollisionType type);
		virtual ~CollisionBox() = default;

		virtual void Update(float elapsedSec) override;
		virtual void FixedUpdate(float elapsedSec) override;
		virtual void Draw() const override;
		virtual void DebugDraw() const override;

		CollisionBox(const CollisionBox& other) = delete;
		CollisionBox(CollisionBox&& other) = delete;
		CollisionBox& operator=(const CollisionBox& other) = delete;
		CollisionBox& operator=(CollisionBox&& other) = delete;

		inline const SDL_Rect& GetCollisionBox() const { return m_CollisionBox; }
		inline const SDL_Rect& GetCurrentWorldBox() const { return m_CurrentWorldBox; }
		inline CollisionSide GetCollisionSide() const { return m_CollisionSide; }
		inline CollisionType GetCollisionType() const { return m_CollisionType; }


	private:
		bool IsOnGround = false;

		void CheckCollision(const float elapsedSec);
		void HandleCollision(GameObject* other, RigidBody* rigidBody, float entryTime);
		bool PreCollisionCheck(const CollisionBox* collision, const RigidBody* rigid);
		SDL_Rect GetWorldCollision(const GameObject* obj, const CollisionBox* collision) const;

		void CalculateInverseEE(float& entry, float& exit, float vel, float v1, float w1, float v2, float w2) const;
		void CalculateEE(float& entry, float& exit, float vel, float invEntry, float invExit) const;
		bool RectCollisionAABB(const SDL_Rect& self, const SDL_Rect& other) const;
		bool IsColliding(const SDL_Rect& self, const SDL_Rect& other) const;
		float SweptAABB(SDL_Rect b1, glm::vec2 velocity, SDL_Rect b2, glm::vec2& normal);
		SDL_Rect CalculateBroadBox(const SDL_Rect& box, glm::vec2 vel) const;

		SDL_Rect m_CurrentWorldBox{};
		const CollisionType m_CollisionType;
		const CollisionSide m_CollisionSide;
		const SDL_Rect m_CollisionBox;
	};
}