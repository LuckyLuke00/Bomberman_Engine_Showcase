#pragma once

class UIManager;

class EndScene final : public GameScene
{
public:
	EndScene() : GameScene(L"EndScene") {}
	~EndScene() override = default;

	EndScene(const EndScene& other) = delete;
	EndScene(EndScene&& other) noexcept = delete;
	EndScene& operator=(const EndScene& other) = delete;
	EndScene& operator=(EndScene&& other) noexcept = delete;

protected:
	void Initialize() override;
	void OnSceneActivated() override;

private:
	FMOD::ChannelGroup* m_pMusicChannelGroup{ nullptr };
	FMOD::Sound* m_pMenuMusic{ nullptr };

	// OnClick functions for the buttons
	void MainMenu();
	void RestartGame();
	void ExitGame();
};
