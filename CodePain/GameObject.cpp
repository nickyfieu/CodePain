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
				float x = 0.f, y = 0.f;
				// if for whatever reason ptrans == nullptr we have this check
				// it should never be nullptr as the transform component is the first
				// component to be added to the components container
				if (IS_VALID(pTransform))
				{
					x += pTransform->GetPosition().x;
					y += pTransform->GetPosition().y;
				}

				x += pTexture2D->GetLocalOffset().x;
				y += pTexture2D->GetLocalOffset().y;

				rendererRef.RenderTexture(*pTexture2D, x, y);
			}

			break;
#pragma endregion
#pragma region Text
		case cp::ComponentType::_Text:
			pText = static_cast<Text*>(component);

			if (IS_VALID(pText))
			{
				float x = 0.f, y = 0.f;
				// if for whatever reason ptrans == nullptr we have this check
				// it should never be nullptr as the transform component is the first
				// component to be added to the components container
				if (IS_VALID(pTransform))
				{
					x += pTransform->GetPosition().x;
					y += pTransform->GetPosition().y;
				}

				x += pText->GetLocalOffset().x;
				y += pText->GetLocalOffset().y;

				rendererRef.RenderTexture(*pText->GetTexture2D(), x, y);
			}
			break;
#pragma endregion
#pragma region FPS
		case cp::ComponentType::_FrameRate:
			pFrameRate = static_cast<FrameRate*>(component);
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
	if (typeid(*pToAdd) == typeid(Transform) && HasComponent<Transform>())
		return;

	m_pComponents.push_back(pToAdd);
}

cp::GameObject::GameObject()
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
