#include "CodePainPCH.h"
#include "Score.h"
#include "GameObject.h"
#include "Observer.h"

cp::Score::Score(int score)
	:cp::BaseComponent(cp::ComponentType::_Score)
	,m_CurrentScore{}
{
	AddScore(score);
}

void cp::Score::Update(float)
{
}

void cp::Score::FixedUpdate(float)
{
}

void cp::Score::Draw() const
{
}

void cp::Score::DebugDraw() const
{
}

void cp::Score::AddScore(int amount)
{
	m_CurrentScore += amount;
	if (m_pOwner)
		m_pOwner->NotifyObservers(cp::Event::EVENT_SCORE_CHANGE);
}