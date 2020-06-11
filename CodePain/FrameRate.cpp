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

void cp::FrameRate::FixedUpdate(float elapsedSec)
{
	m_FixedTimer += elapsedSec;
	m_FixedFrameCounter++;
}

void cp::FrameRate::DebugDraw() const
{
}

void cp::FrameRate::Draw() const
{
}

bool cp::FrameRate::GetFrameRate(float& container)
{
	if (m_Time > 1.f)
	{
		container = float(int(m_FrameCounter / m_Time));
		m_Time = fmodf(m_Time - 1.f, 1.f);
		m_FrameCounter = 0;
		return true;
	}
	return false;
}

bool cp::FrameRate::GetFixedFrameRate(float& container)
{
	if (m_FixedTimer > 1.f)
	{
		container = float(int(m_FixedFrameCounter / m_FixedTimer));
		m_FixedTimer = fmodf(m_FixedTimer - 1.f, 1.f);
		m_FixedFrameCounter = 0;
		return true;
	}
	return false;
}
