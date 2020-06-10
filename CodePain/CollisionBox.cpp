#include "CodePainPCH.h"
#include "CollisionBox.h"
#include "GameObject.h"
#include "Transform.h"

#include "Renderer.h"
#include "GameObject.h"
#include "SceneManager.h"
#include "Scene.h"
#include "RigidBody.h"

#include <algorithm>
#include <cmath>

cp::CollisionBox::CollisionBox(int x, int y, int width, int height, CollisionSide side, CollisionType type)
	: BaseComponent(ComponentType::_CollisionBox)
	, m_CollisionSide{side}
	, m_CollisionType{type}
	, m_CollisionBox{x,y,width,height}
{
}

void cp::CollisionBox::Update(const float)
{
}

void cp::CollisionBox::FixedUpdate(const float elapsedSec)
{
	CheckCollision(elapsedSec);
}

void cp::CollisionBox::DebugDraw() const
{
	Renderer& rendererRef = Renderer::GetInstance();
	if (!rendererRef.gd_RenderCollisionBoxes)
		return;

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

void cp::CollisionBox::CheckCollision(const float elapsedSec)
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
		std::vector<GameObject*> levelObjects = activeScene->GetAllGameObjectsOfType(cp::GameObjectType::level);
		size_t amountOfObjects = levelObjects.size();
		for (int i = 0; i < amountOfObjects; i++)
		{
			GameObject* other = levelObjects[i];
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

				if (!RectCollision(m_CurrentWorldBox, GetWorldCollision(other, otherCollision)))
					continue;

				RigidBody* rigidBody = self->GetComponent<RigidBody>(cp::ComponentType::_RigidBody);
				if (rigidBody == nullptr)
					continue;

				HandleCollision(other, rigidBody);
			}
		}
	}
}

void cp::CollisionBox::HandleCollision(GameObject* other, RigidBody* rigidBody)
{
	this->m_pOwner->OnCollisionCallback(this->m_pOwner, other, m_CollisionSide);

	if (m_CollisionSide == CollisionSide::all)
	{
		// not implemented yet
	}
	else if (m_CollisionSide == CollisionSide::right)
		rigidBody->SetIsColRight(true);
	else if (m_CollisionSide == CollisionSide::up)
		rigidBody->SetIsColUp(true);
	else if (m_CollisionSide == CollisionSide::left)
		rigidBody->SetIsColLeft(true);
	else if (m_CollisionSide == CollisionSide::down)
	{
		rigidBody->SetIsColDown(true);
		rigidBody->SetIsOnGround(true);
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
