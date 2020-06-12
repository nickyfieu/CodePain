#include "CodePainPCH.h"
#include "States.h"
#include "GameObject.h"
#include "Observer.h"

bool Game::IdleState::UpdateState(const cp::GameObject* gameObject)
{
	gameObject->NotifyObservers(cp::Event::EVENT_IDLE);
	return false;
}
