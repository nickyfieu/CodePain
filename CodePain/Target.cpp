#include "CodePainPCH.h"
#include "Target.h"
#include "Renderer.h"
#include "GameObject.h"
#include "Components.h"

cp::Target::Target()
	:BaseComponent(cp::ComponentType::_Target)
{
}

void cp::Target::Update(float)
{
}

void cp::Target::FixedUpdate(float)
{
}

void cp::Target::Draw() const
{
}

void cp::Target::DebugDraw() const
{
	Renderer& rendererRef = Renderer::GetInstance();
	if (!rendererRef.gd_RenderCollisionBoxes)
		return;
	Transform* pTransform = m_pOwner->GetComponent<Transform>(cp::ComponentType::_Transform);
	glm::vec3 pos = pTransform->GetPosition();
	for (GameObject* ref : m_Targets)
	{
		glm::vec3 otherPos = ref->GetComponent<Transform>(cp::ComponentType::_Transform)->GetPosition();
		rendererRef.RenderLine({ (int)pos.x, (int)pos.y }, { (int)otherPos.x, (int)otherPos.y }, 0, 0, 255);
	}
}

void cp::Target::AddTarget(GameObject* ref)
{
	m_Targets.push_back(ref);
}
