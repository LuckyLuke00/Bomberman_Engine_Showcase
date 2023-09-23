#pragma once

class CountdownTimer;
class GridMap;
class PostBloom;
class UIButton;
class UIManager;

class LevelScene final : public GameScene
{
public:
	LevelScene() : GameScene(L"LevelScene") {}
	~LevelScene() override;

	LevelScene(const LevelScene& other) = delete;
	LevelScene(LevelScene&& other) noexcept = delete;
	LevelScene& operator=(const LevelScene& other) = delete;
	LevelScene& operator=(LevelScene&& other) noexcept = delete;

	void TogglePause();
	static bool HasGameStarted() { return m_GameStarted; }
	static bool GameEnd() { return m_GameEnded; }

protected:
	void Initialize() override;
	void Update() override;
	void OnSceneActivated() override;

private:
	CountdownTimer* m_pCountdownTimer{ nullptr };

	FMOD::ChannelGroup* m_pMusicChannelGroup{ nullptr };
	FMOD::Sound* m_pBattleMusic{ nullptr };
	FMOD::Sound* m_pBattleStartSound{ nullptr };
	FMOD::Sound* m_pBattleWinSound{ nullptr };

	GameObject* m_pPauseMenu{ nullptr };
	GameObject* m_pWinMessage{ nullptr };

	GridMap* m_pGridMap{ nullptr };
	PostBloom* m_pPostBloom{ nullptr };
	SpriteFont* m_pSmallFont{ nullptr };
	SpriteFont* m_pMediumFont{ nullptr };

	std::wstring m_WinMessage{};
	std::wstring m_WinnerName{};

	std::vector<UIButton*> m_pPauseButtons{};

	UIManager* m_pUIManager{};

	static bool m_GameStarted;
	static bool m_GameEnded;
	bool m_Paused{ false };
	int m_MaxPlayers{ 4 };
	int m_PlayerCount{ 0 };
	bool m_PlayerReady[4]{ false, false, false, false };

	float m_WinMessageTimer{ .0f };
	float m_WinMessageTime{ 5.f };

	bool HasGameEnded() const;
	bool WaitForPlayers();
	void AddGridObjects();
	void ClearPauseMenu();
	void ClearWinMessage();
	void CreateGroundPlane();
	void CreatePauseMenu();
	void CreateWinMessage();
	void DecideWinner();
	void RemoveGridObjects();
	void SetUpCamera();
	void SetUpLevel();
	void SetupPlayer(int playerIndex) const;
	void SetupTimer();

	void Reset();
	UIButton* CreatePauseButtons(const std::wstring& text, const XMFLOAT2& pos) const;
};
