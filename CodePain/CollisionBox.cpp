#include "CodePainPCH.h"
#include "CollisionBox.h"
#include "GameObject.h"
#include "Transform.h"

#include "Renderer.h"
#include "GameObject.h"
#include "SceneManager.h"
#include "Scene.h"

#include <algorithm>
#include <cmath>

cp::CollisionBox::CollisionBox(int x, int y, int width, int height, CollisionSide side, CollisionType type)
	: BaseComponent(ComponentType::_CollisionBox)
	, m_CollisionSide{side}
	, m_CollisionType{type}
	, m_CollisionBox{x,y,width,height}
{
	switch (m_CollisionSide)
	{
	case CollisionSide::left:
		m_SurfaceNormal = { -1.f,0.f };
		break;
	case CollisionSide::up:
		m_SurfaceNormal = { 0.f,-1.f };
		break;
	case CollisionSide::right:
		m_SurfaceNormal = { 1.f,0.f };
		break;
	case CollisionSide::down:
		m_SurfaceNormal = { 0.f,1.f };
		break;
	}
}

void cp::CollisionBox::Update(const float)
{
}

void cp::CollisionBox::FixedUpdate(const float elapsedSec)
{
	HandleCollision(elapsedSec);
}

void cp::CollisionBox::DebugDraw() const
{
	Renderer& rendererRef = Renderer::GetInstance();
	if (rendererRef.gd_RenderCollisionBoxes)
	{
		SDL_Rect box = this->GetCollisionBox();
		CollisionSide side = this->GetCollisionSide();
		// if for whatever reason ptrans == nullptr we have this check
		// it should never be nullptr as the transform component is the first
		// component to be added to the components container
		Transform* transform = m_pOwner->GetComponent<Transform>(cp::ComponentType::_Transform);
		if (IS_VALID(transform))
		{
			box.x += (int)transform->GetPosition().x;
			box.y += (int)transform->GetPosition().y;
		}

		Uint8 red = 0;
		Uint8 green = 0;
		Uint8 blue = 0;

		// all will be white
		(side & CollisionSide::right) ? blue = 255 : blue;
		(side & CollisionSide::up) ? green = 255 : green;
		(side & CollisionSide::left) ? red = 255 : red;
		(side & CollisionSide::down) ? red = green = 255 : red;
		rendererRef.RenderCollorRect(box, red, green, blue);

		SDL_Point center = { box.x + int(box.w * 0.5f) , box.y + int(box.h * 0.5f) };
		SDL_Point target = center;
		target.x += int(m_SurfaceNormal.x * 10);
		target.y += int(m_SurfaceNormal.y * 10);
		rendererRef.RenderLine(center, target, red, green, blue);
	}
}

void cp::CollisionBox::Draw() const
{
}

bool cp::CollisionBox::RectCollision(const SDL_Rect& self, const SDL_Rect& other)
{
	if (self.x < other.x + other.w &&
		self.x + self.w > other.x&&
		self.y < other.y + other.h &&
		self.y + self.h > other.y)
	{
		return true;
	}
	return false;
}

bool cp::CollisionBox::PreCollisionCheck(const CollisionBox* collision)
{
	switch (m_CollisionSide)
	{
	case CollisionSide::all:
		return true;
		break;
	case CollisionSide::left:
		return collision->GetCollisionSide()& CollisionSide::right;
		break;
	case CollisionSide::up:
		return collision->GetCollisionSide() & CollisionSide::down;
		break;
	case CollisionSide::right:
		return collision->GetCollisionSide() & CollisionSide::left;
		break;
	case CollisionSide::down:
		return collision->GetCollisionSide() & CollisionSide::up;
		break;
	}
	return false;
}

void cp::CollisionBox::HandleCollision(const float elapsedSec)
{
	GameObject* self = this->m_pOwner;
	m_CurrentWorldBox = GetWorldCollision(self, this);
	// for dynamic collision
	// we check any active gameobject
	// that has a collision component
	if (m_CollisionType == CollisionType::_dynamic) 
	{
		// to be replaced by the force of rigid body
		glm::vec2 velocity = { 0.f, -98.1f * elapsedSec };

		Scene* activeScene = SceneManager::GetInstance().GetActiveScene();
		size_t amountOfObjects = activeScene->GetAmountOfGameObjects();
		for (int i = 0; i < amountOfObjects; i++)
		{
			GameObject* other = activeScene->GetGameObject(i);
			if (!other->GetIsActive() || other == self)
				continue;

			std::vector<CollisionBox*> otherCollisions = other->GetAllComponentsOfType<CollisionBox>(cp::ComponentType::_CollisionBox);
			size_t amountOfCollisionBoxes = otherCollisions.size();
			for (int j = 0; j < amountOfCollisionBoxes; j++)
			{
				CollisionBox* otherCollision = otherCollisions[j];

				if (otherCollision->GetCollisionType() != CollisionType::_static)
					continue;

				if (!PreCollisionCheck(otherCollision))
					continue;

				if (RectCollision(m_CurrentWorldBox, GetWorldCollision(other, otherCollision)))
				{
					self->OnCollisionCallback(self, other, m_CollisionSide);
					return;
				}
			}
		}
		self->GetComponent<Transform>(cp::ComponentType::_Transform)->Translate(velocity.x, velocity.y,0.f);
	}
}


SDL_Rect cp::CollisionBox::GetWorldCollision(const GameObject* obj, const CollisionBox* collision) const
{
	glm::vec3 offset;
	offset = obj->GetComponent<Transform>(ComponentType::_Transform)->GetPosition();
	SDL_Rect rect = collision->GetCollisionBox();
	rect.x += (int)offset.x;
	rect.y += (int)offset.y;
	return rect;
}
