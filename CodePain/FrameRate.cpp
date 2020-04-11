#include "CodePainPCH.h"
#include "FrameRate.h"



cp::FrameRate::FrameRate()
	: BaseComponent(ComponentType::_FrameRate)
	, m_Time{0.f}
	, m_FrameCounter{0}
{

}

void cp::FrameRate::Update(float elapsedSec)
{
	m_Time += elapsedSec;
	m_FrameCounter++;
}

bool cp::FrameRate::GetFrameRate(float& container)
{
	if (m_Time > 1.f)
	{
		m_Time -= 1.f;
		container = (float)m_FrameCounter;
		m_FrameCounter = 0;
		return true;
	}
	return false;
}
