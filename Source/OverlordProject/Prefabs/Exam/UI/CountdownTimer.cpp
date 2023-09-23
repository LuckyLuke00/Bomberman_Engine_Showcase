#include "stdafx.h"
#include "CountdownTimer.h"

CountdownTimer::CountdownTimer(const std::wstring& font, const DirectX::XMFLOAT2& position, const DirectX::XMFLOAT4& color) :
	m_Color{ color },
	m_Position{ position },
	m_FontPath{ font }
{}

void CountdownTimer::Initialize(const SceneContext&)
{
	m_pFont = ContentManager::Load<SpriteFont>(m_FontPath);
	m_pTextRenderer = TextRenderer::Get();
}

void CountdownTimer::Update(const SceneContext& sceneContext)
{
	if (m_TimerPaused || m_CountdownEnded) return;

	const int seconds{ static_cast<int>(m_Timer + .5f) };
	const int minutes{ seconds / 60 };
	const int remainingSeconds{ seconds % 60 };

	const std::wstring timeString{ std::format(L"{:01d}:{:02d}", minutes, remainingSeconds) };
	const auto size{ SpriteFont::MeasureString(timeString, m_pFont) };
	const auto pos{ DirectX::XMFLOAT2(m_Position.x - size.x * .5f, m_Position.y - size.y * .5f) };
	m_pTextRenderer->DrawText(m_pFont, timeString, pos, m_Color);

	m_Timer -= sceneContext.pGameTime->GetElapsed();

	if (m_Timer <= .0f)
	{
		m_Timer = .0f;
		m_TimerPaused = true;
		m_CountdownEnded = true;
	}
}
