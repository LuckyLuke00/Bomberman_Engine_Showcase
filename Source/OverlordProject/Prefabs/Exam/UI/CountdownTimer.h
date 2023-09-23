#pragma once
class CountdownTimer final : public GameObject
{
public:
	explicit CountdownTimer(const std::wstring& font, const DirectX::XMFLOAT2& position, const DirectX::XMFLOAT4& color = { 1.f, 1.f, 1.f, 1.f });
	~CountdownTimer() override = default;

	CountdownTimer(const CountdownTimer& other) = delete;
	CountdownTimer(CountdownTimer&& other) noexcept = delete;
	CountdownTimer& operator=(const CountdownTimer& other) = delete;
	CountdownTimer& operator=(CountdownTimer&& other) noexcept = delete;

	void SetCountdownTime(float time) { m_Time = time; }
	void SetColor(const DirectX::XMFLOAT4& color) { m_Color = color; }

	void PauseTimer() { m_TimerPaused = true; }
	void ResumeTimer() { m_TimerPaused = false; }
	void StartTimer() { m_TimerPaused = false; m_CountdownEnded = false; m_Timer = m_Time; }
	void StopTimer() { m_TimerPaused = true; m_CountdownEnded = false;  m_Timer = .0f; }

	int GetTimeInSeconds() const { return static_cast<int>(m_Time + .5f); }
	bool HasCountdownEnded() const { return m_CountdownEnded; }
	bool IsTimerPaused() const { return m_TimerPaused; }

protected:
	void Initialize(const SceneContext& sceneContext) override;
	void Update(const SceneContext& sceneContext) override;

private:
	bool m_TimerPaused{ false };
	bool m_CountdownEnded{ false };

	float m_Timer{ .0f };
	float m_Time{ .0f };

	DirectX::XMFLOAT4 m_Color{ 1.f, 1.f, 1.f, 1.f };
	DirectX::XMFLOAT2 m_Position{ 0.f, 0.f };

	std::wstring m_FontPath{ L"" };
	SpriteFont* m_pFont{ nullptr };
	TextRenderer* m_pTextRenderer{ nullptr };
};
