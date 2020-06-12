#include "CodePainPCH.h"
#include "Observer.h"
#include "GameObject.h"
#include "Observers.h"
#include "Logger.h"

void Game::IdleEvent::OnNotify(const cp::GameObject* entity, cp::Event event)
{
	if (event != cp::Event::EVENT_IDLE)
		return;

	if (!(entity->GetType() == cp::GameObjectType::Player1 ||
		entity->GetType() == cp::GameObjectType::Player2))
		return;
}
