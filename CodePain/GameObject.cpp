#include "CodePainPCH.h"
#include "GameObject.h"
#include "Components.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include <iomanip>

void cp::GameObject::Update(const float elapsedSec)
{
	for (BaseComponent* components : m_pComponents)
	{
		components->Update(elapsedSec);
	}
}

void cp::GameObject::Render() const
{
	Renderer& rendererRef = Renderer::GetInstance();
	Transform* pTransform = nullptr;
	Texture2D* pTexture2D = nullptr;
	Text* pText = nullptr;
	FrameRate* pFrameRate = nullptr;
	CollisionBox* pCollisionBox = nullptr;
	
	for (BaseComponent* component : m_pComponents)
	{
		switch (component->GetComponentType())
		{
#pragma region Transform
		case cp::ComponentType::_Transform:
			pTransform = static_cast<Transform*>(component);
			break;
#pragma endregion
#pragma region Texture2D
		case cp::ComponentType::_Texture2D:
			pTexture2D = static_cast<Texture2D*>(component);

			if (IS_VALID(pTexture2D))
			{
				SDL_FRect dst = pTexture2D->GetDstRect();
				SDL_Rect src = pTexture2D->GetSrcRect();
				// if for whatever reason ptrans == nullptr we have this check
				// it should never be nullptr as the transform component is the first
				// component to be added to the components container
				if (IS_VALID(pTransform))
				{
					dst.x += pTransform->GetPosition().x;
					dst.y += pTransform->GetPosition().y;
				}

				dst.x += pTexture2D->GetLocalOffset().x;
				dst.y += pTexture2D->GetLocalOffset().y;

				rendererRef.RenderTexture(*pTexture2D, src, dst);
			}

			break;
#pragma endregion
#pragma region Text
		case cp::ComponentType::_Text:
			pText = static_cast<Text*>(component);

			if (IS_VALID(pText))
			{
				Texture2D* pTextTexture = pText->GetTexture2D();
				SDL_FRect dst = pTextTexture->GetDstRect();
				SDL_Rect src = pTextTexture->GetSrcRect();
				// if for whatever reason ptrans == nullptr we have this check
				// it should never be nullptr as the transform component is the first
				// component to be added to the components container
				if (IS_VALID(pTransform))
				{
					dst.x += pTransform->GetPosition().x;
					dst.y += pTransform->GetPosition().y;
				}

				dst.x += pText->GetLocalOffset().x;
				dst.y += pText->GetLocalOffset().y;

				rendererRef.RenderTexture(*pTextTexture, src, dst);
			}
			break;
#pragma endregion
#pragma region FPS
		case cp::ComponentType::_FrameRate:
			pFrameRate = static_cast<FrameRate*>(component);
			break;
#pragma endregion
#pragma region CollisionBox
		case ComponentType::_CollisionBox:
			pCollisionBox = static_cast<CollisionBox*>(component);
#if _DEBUG
			{
				if (rendererRef.gd_RenderCollisionBoxes)
				{
					SDL_Rect box = pCollisionBox->GetCollisionBox();
					CollisionSide side = pCollisionBox->GetCollisionSide();
					// if for whatever reason ptrans == nullptr we have this check
					// it should never be nullptr as the transform component is the first
					// component to be added to the components container
					if (IS_VALID(pTransform))
					{
						box.x += (int)pTransform->GetPosition().x;
						box.y += (int)pTransform->GetPosition().y;
					}

					Uint8 red = 0;
					Uint8 green = 0;
					Uint8 blue = 0;

					(side & CollisionSide::right) ? blue = 255 : blue;
					(side & CollisionSide::up) ? green = 255 : green;
					(side & CollisionSide::left) ? red = 255 : red;

					rendererRef.RenderCollorRect(box, red, green, blue);
				}
			}
#endif
			break;
#pragma endregion
		default:
			break;
		}
	}

	if (IS_VALID(pFrameRate) && IS_VALID(pText))
	{
		float fps = 0.f;
		if (pFrameRate->GetFrameRate(fps))
		{
			pText->SetText(std::to_string(int(fps)) + " FPS");
		}
	}
}

void cp::GameObject::AddComponent(BaseComponent* pToAdd)
{
	if ((pToAdd->GetComponentType() == ComponentType::_Transform) && HasComponent<Transform>(ComponentType::_Transform))
		return;

	m_pComponents.push_back(pToAdd);
}

cp::GameObject::GameObject()
	:m_Type{GameObjectType::none}
{
	// Forces Transform Component to be the first component so it will always be found before anything else
	// There will also only be 1 transform component per gameobject never more ( unless there are local transform components in other components )
	AddComponent(new Transform());
}

cp::GameObject::GameObject(GameObjectType type)
	: m_Type{type}
{
	// Forces Transform Component to be the first component so it will always be found before anything else
	// There will also only be 1 transform component per gameobject never more ( unless there are local transform components in other components )
	AddComponent(new Transform());
}

cp::GameObject::~GameObject()
{
	for (BaseComponent* components : m_pComponents)
	{
		SAFE_DELETE(components);
	}
}

void cp::GameObject::SetActive(bool active)
{
	m_IsActive = active;
}
