#include "CodePainPCH.h"
#include "CollisionBox.h"
#include "GameObject.h"
#include "Transform.h"

#include "Renderer.h"
#include "GameObject.h"
#include "SceneManager.h"
#include "Scene.h"
#include "RigidBody.h"
#include "Observer.h"

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

bool cp::CollisionBox::RectCollisionAABB(const SDL_Rect& self, const SDL_Rect& other) const
{
	if (self.x < other.x + other.w &&
		self.x + self.w > other.x &&
		self.y < other.y + other.h &&
		self.y + self.h > other.y)
	{
		return true;
	}
	return false;
}

bool cp::CollisionBox::IsColliding(const SDL_Rect& self, const SDL_Rect& other) const
{
	float lft = float(other.x - (self.x + self.w));
	float top = float((other.y + other.h) - self.y);
	float rht = float((other.x + other.w) - self.x);
	float bot = float(other.y - (self.y + self.h));

	return !(lft > 0.f || rht < 0.f || top < 0.f || bot > 0.f);
}

bool cp::CollisionBox::StaticPreColCheck(const CollisionBox* collision, const RigidBody* rigid)
{
	switch (m_CollisionSide)
	{
	case CollisionSide::all:
		return false;
		break;
	case CollisionSide::left:
		return (collision->GetCollisionSide()& CollisionSide::right) && (rigid->GetVelocity().x < -FLT_EPSILON);
		break;
	case CollisionSide::up:
		return (collision->GetCollisionSide() & CollisionSide::down) && (rigid->GetVelocity().y > FLT_EPSILON);
		break;
	case CollisionSide::right:
		return (collision->GetCollisionSide() & CollisionSide::left) && (rigid->GetVelocity().x > FLT_EPSILON);
		break;
	case CollisionSide::down:
		return (collision->GetCollisionSide() & CollisionSide::up) && (rigid->GetVelocity().y < -FLT_EPSILON);
		break;
	}
	return false;
}

void cp::CollisionBox::CheckCollision(const float)
{
	GameObject* self = this->m_pOwner;
	RigidBody* rigidBody = self->GetComponent<RigidBody>(cp::ComponentType::_RigidBody);
	if (rigidBody == nullptr)
		return;

	m_CurrentWorldBox = GetWorldCollision(self, this);
	// for dynamic collision
	// we check any active gameobject
	// that has a collision component
	if (m_CollisionType == CollisionType::_dynamic) 
	{
		m_pOverlapObjRef = nullptr;
		Scene* activeScene = SceneManager::GetInstance().GetActiveScene();
		size_t amountOfObjects = activeScene->GetAmountOfGameObjects();
		for (size_t i = 0; i < amountOfObjects; i++)
		{
			GameObject* other = activeScene->GetGameObject(i);
			if (!other->GetIsActive() || other == self)
				continue;

			std::vector<CollisionBox*> otherCollisions = other->GetAllComponentsOfType<CollisionBox>(cp::ComponentType::_CollisionBox);
			size_t amountOfCollisionBoxes = otherCollisions.size();
			for (size_t j = 0; j < amountOfCollisionBoxes; j++)
			{
				CollisionBox* otherCollision = otherCollisions[j];
				if (otherCollision->GetCollisionType() == CollisionType::_static)
				{
					if (!StaticPreColCheck(otherCollision, rigidBody))
						continue;

					glm::vec2 normal;
					float entryTime = SweptAABB(GetWorldCollision(self, this), rigidBody->GetVelocity(), GetWorldCollision(other, otherCollision), normal);
					if (entryTime < 1.f)
					{
						HandleCollision(rigidBody, entryTime);
					}
				}
				else
				{
					if (otherCollision->GetCollisionSide() != CollisionSide::all)
						continue;

					if (m_CollisionSide != CollisionSide::all)
						continue;
					
					if (m_pOwner->GetType() == other->GetType())
						continue;

					if (RectCollisionAABB(m_CurrentWorldBox, GetWorldCollision(other, otherCollision)))
					{
						m_pOverlapObjRef = other;
						this->m_pOwner->NotifyObservers(cp::Event::EVENT_COLLISION_OVERLAP);
					}
				}
			}
		}
	}
}

void cp::CollisionBox::HandleCollision(RigidBody* rigidBody, float entryTime)
{
	if (m_CollisionSide == CollisionSide::right)
	{
		rigidBody->SetIsColRight(true);
		rigidBody->SetEntryLRTime(entryTime);
	}
	else if (m_CollisionSide == CollisionSide::up)
	{
		rigidBody->SetIsColUp(true);
		rigidBody->SetEntryUDTime(entryTime);
	}
	else if (m_CollisionSide == CollisionSide::left)
	{
		rigidBody->SetIsColLeft(true);
		rigidBody->SetEntryLRTime(entryTime);
	}
	else if (m_CollisionSide == CollisionSide::down)
	{
		rigidBody->SetIsColDown(true);
		rigidBody->SetIsOnGround(true);
		rigidBody->SetEntryUDTime(entryTime);
	}
	else
	{
		return;
	}

	this->m_pOwner->NotifyObservers(cp::Event::EVENT_COLLISION_COLLIDE);
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

void cp::CollisionBox::CalculateInverseEE(float& entry, float& exit, float vel, float v1, float w1, float v2, float w2) const
{
	if (vel > 0.0f)
	{
		entry = v2 - (v1 + w1);
		exit = (v2 + w2) - v1;
	}
	else
	{
		entry = (v2 + w2) - v1;
		exit = v2 - (v1 + w1);
	}
}

void cp::CollisionBox::CalculateEE(float& entry, float& exit, float vel, float invEntry, float invExit) const
{
	if (vel == 0.f)
	{
		entry = -std::numeric_limits<float>::infinity();
		exit = std::numeric_limits<float>::infinity();
	}
	else
	{
		entry = invEntry / vel;
		exit = invExit / vel;
	}
}

SDL_Rect cp::CollisionBox::CalculateBroadBox(const SDL_Rect& box, glm::vec2 vel) const
{
	float x = (vel.x > 0.f) ? box.x : box.x + vel.x;
	float y = (vel.y > 0.f) ? box.y : box.y + vel.y;
	float w = box.w + abs(vel.x);
	float h = box.h + abs(vel.y);

	return SDL_Rect{ (int)x, (int)y, (int)w, (int)h };
}

// based of
// https://www.gamedev.net/tutorials/programming/general-and-gameplay-programming/swept-aabb-collision-detection-and-response-r3084/
// and
// https://amanotes.com/using-swept-aabb-to-detect-and-process-collision/
float cp::CollisionBox::SweptAABB(SDL_Rect b1, glm::vec2 velocity, SDL_Rect b2, glm::vec2&)
{
	glm::vec2 vel = velocity;
	vel.y *= -1.f;
	SDL_Rect broadBox = CalculateBroadBox(b1, vel);
	
	if (!IsColliding(broadBox, b2))
	{
		return 1.f;
	}

	glm::vec2 inverseEntry{}, inverseExit{};

	// find the distance between the objects on the near and far sides for both x and y 
	CalculateInverseEE(inverseEntry.x, inverseExit.x, vel.x, (float)b1.x, (float)b1.w, (float)b2.x, (float)b2.w);
	CalculateInverseEE(inverseEntry.y, inverseExit.y, vel.y, (float)b1.y, (float)b1.h, (float)b2.y, (float)b2.h);

	glm::vec2 entry{}, exit{};
	
	CalculateEE(entry.x, exit.x, vel.x, inverseEntry.x, inverseExit.x);
	CalculateEE(entry.y, exit.y, vel.y, inverseEntry.y, inverseExit.y);

	float entryTime = std::max<float>(entry.x, entry.y);
	float exitTime = std::min<float>(exit.x, exit.y);

	if (entryTime > exitTime || (entry.x < 0.f && entry.y < 0.f) || entry.x > 1.f || entry.y > 1.f)
		return 1.f;

	return entryTime;
}
