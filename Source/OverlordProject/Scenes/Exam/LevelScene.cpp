#include "stdafx.h"
#include "LevelScene.h"
#include "Materials/Shadow/DiffuseMaterial_Shadow.h"
#include "Prefabs/Exam/Player/Player.h"
#include "Exam/ExamAssets.h"
#include "Prefabs/Exam/Level/GridMap.h"
#include "Prefabs/Exam/UI/CountdownTimer.h"
#include "Prefabs/Exam/UI/UIManager.h"
#include "Prefabs/Exam/UI/Buttons/UIButton.h"
#include "Materials/Post/PostBloom.h"

bool LevelScene::m_GameStarted{ false };
bool LevelScene::m_GameEnded{ false };

LevelScene::~LevelScene()
{
	RemoveGridObjects();
}

void LevelScene::TogglePause()
{
	if (!m_GameStarted) return;

	m_Paused = !m_Paused;

	if (m_Paused)
	{
		m_pCountdownTimer->PauseTimer();
		GridObject::Pause();
	}
	else
	{
		m_pCountdownTimer->ResumeTimer();
		GridObject::UnPause();
	}
}

void LevelScene::Initialize()
{
	GetSceneSettings().drawGrid = false;
	GetSceneSettings().drawPhysXDebug = false;
	GetSceneSettings().showInfoOverlay = false;
	GetSceneSettings().enableOnGUI = false;

	m_SceneContext.pLights->SetDirectionalLight({ .0f, 50.f, .0f }, { .2f, -.7f, .4f });

	SetUpCamera();
	CreateGroundPlane();

	m_pPostBloom = MaterialManager::Get()->CreateMaterial<PostBloom>();
	AddPostProcessingEffect(m_pPostBloom);

	SoundManager::Get()->GetSystem()->createSound(ExamAssets::BattleStartSound.c_str(), FMOD_DEFAULT, nullptr, &m_pBattleStartSound);
	SoundManager::Get()->GetSystem()->createSound(ExamAssets::BattleWinSound.c_str(), FMOD_DEFAULT, nullptr, &m_pBattleWinSound);
	SoundManager::Get()->GetSystem()->createSound(ExamAssets::BattleMusic.c_str(), FMOD_LOOP_NORMAL, nullptr, &m_pBattleMusic);
	SoundManager::Get()->GetSystem()->createChannelGroup("Music", &m_pMusicChannelGroup);
	m_pMusicChannelGroup->setVolume(.5f);

	m_pSmallFont = ContentManager::Load<SpriteFont>(ExamAssets::SmallFont);
	m_pMediumFont = ContentManager::Load<SpriteFont>(ExamAssets::MediumFont);
}

void LevelScene::Update()
{
	AddGridObjects();
	RemoveGridObjects();

	if (m_Paused)
	{
		if (m_pPauseMenu) return;
		CreatePauseMenu();
	}
	else
	{
		ClearPauseMenu();
	}

	if (!WaitForPlayers())
	{
		XMFLOAT2 pos{ m_SceneContext.windowWidth * .5f, m_SceneContext.windowHeight * .9f };
		pos.x -= SpriteFont::MeasureString(L"Press START to join", m_pSmallFont).x * .5f;

		// Pixel perfect
		pos.x = static_cast<float>(static_cast<int>(pos.x + .5f));
		pos.y = static_cast<float>(static_cast<int>(pos.y + .5f));

		TextRenderer::Get()->DrawText(m_pSmallFont, L"Press START to join", pos);

		return;
	}

	if (!m_GameStarted)
	{
		SetupTimer();
		SoundManager::Get()->GetSystem()->playSound(m_pBattleMusic, m_pMusicChannelGroup, false, nullptr);
	}

	m_GameStarted = true;

	if (HasGameEnded() && !m_GameEnded)
	{
		m_GameEnded = true;
		m_pMusicChannelGroup->stop();
		SoundManager::Get()->GetSystem()->playSound(m_pBattleWinSound, nullptr, false, nullptr);
		DecideWinner();
		CreateWinMessage();
	}

	if (m_GameEnded)
	{
		XMFLOAT2 pos{ m_SceneContext.windowWidth * .5f, m_SceneContext.windowHeight * .5f };
		XMFLOAT2 size{ SpriteFont::MeasureString(m_WinMessage, m_pMediumFont) };
		pos.x -= size.x * .5f;
		pos.y -= size.y * .5f;

		// Pixel perfect
		pos.x = static_cast<float>(static_cast<int>(pos.x + .5f));
		pos.y = static_cast<float>(static_cast<int>(pos.y + .5f));

		TextRenderer::Get()->DrawText(m_pMediumFont, m_WinMessage, pos);

		m_WinMessageTimer += m_SceneContext.pGameTime->GetElapsed();
		if (m_WinMessageTimer >= m_WinMessageTime)
		{
			m_WinMessageTimer = .0f;
			m_GameEnded = false;

			Reset();
			ClearWinMessage();
			SceneManager::Get()->SetActiveGameScene(L"EndScene");
		}
	}
}

void LevelScene::OnSceneActivated()
{
	SetUpLevel();
}

bool LevelScene::WaitForPlayers()
{
	if (!m_PlayerReady[0] && InputManager::IsGamepadButton(InputState::pressed, XINPUT_GAMEPAD_START, GamepadIndex::playerOne))
	{
		SetupPlayer(0);
		m_PlayerReady[0] = true;
		++m_PlayerCount;
	}

	if (!m_PlayerReady[1] && InputManager::IsGamepadButton(InputState::pressed, XINPUT_GAMEPAD_START, GamepadIndex::playerTwo))
	{
		SetupPlayer(1);
		m_PlayerReady[1] = true;
		++m_PlayerCount;
	}

	if (!m_PlayerReady[2] && InputManager::IsGamepadButton(InputState::pressed, XINPUT_GAMEPAD_START, GamepadIndex::playerThree))
	{
		SetupPlayer(2);
		m_PlayerReady[2] = true;
		++m_PlayerCount;
	}

	if (!m_PlayerReady[3] && InputManager::IsGamepadButton(InputState::pressed, XINPUT_GAMEPAD_START, GamepadIndex::playerFour))
	{
		SetupPlayer(3);
		m_PlayerReady[3] = true;
		++m_PlayerCount;
	}

	return m_PlayerCount >= 2;
}

void LevelScene::SetupPlayer(int playerIndex) const
{
	if (playerIndex < 0 || playerIndex >= m_MaxPlayers) return;

	// Calculate the corner of the grid where the player should spawn
	const int x{ playerIndex % 2 == 0 ? 1 : m_pGridMap->GetRows() - 2 };
	const int y{ playerIndex < 2 ? 1 : m_pGridMap->GetCols() - 2 };

	auto* pPlayer{ new Player{ ExamAssets::PlayerMesh, playerIndex } };

	pPlayer->SetScale(.01f, .01f);
	pPlayer->SetPosition(x, y);
	pPlayer->SetSpawnPoint(x, y);
	pPlayer->SetDimensions(1, 1);
	pPlayer->OffsetPosition(.0f, -.5f, .0f);
	pPlayer->MarkForAdd();
}

void LevelScene::SetUpCamera()
{
	auto pCameraObject{ new FixedCamera{} };
	AddChild(pCameraObject);
	SetActiveCamera(pCameraObject->GetComponent<CameraComponent>());

	pCameraObject->GetTransform()->Translate(10, 20, 1);
	pCameraObject->GetTransform()->Rotate(75, 0, 0);
}

void LevelScene::CreateGroundPlane()
{
	auto* pMaterial{ MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow>() };
	pMaterial->SetDiffuseTexture(ExamAssets::BackgroundTexture);

	for (int i{ 0 }; i < 4; ++i)
	{
		for (int j{ 0 }; j < 4; ++j)
		{
			auto* pGround{ new GameObject{} };
			auto* pModel{ pGround->AddComponent(new ModelComponent{ L"Meshes/UnitPlane.ovm", false }) };
			pModel->SetMaterial(pMaterial);
			pGround->GetTransform()->Translate(-5.f + 10.f * static_cast<float>(i), -.5f, 10.f * static_cast<float>(j));
			AddChild(pGround);
		}
	}
}

void LevelScene::SetupTimer()
{
	const XMFLOAT2 timerTextPos{ m_SceneContext.windowWidth * .5f, m_SceneContext.windowHeight * .05f };
	m_pCountdownTimer = new CountdownTimer{ ExamAssets::SmallFont, timerTextPos };
	m_pCountdownTimer->SetCountdownTime(180.f);
	m_pCountdownTimer->StartTimer();
	AddChild(m_pCountdownTimer);
}

void LevelScene::RemoveGridObjects()
{
	const auto& toDestroy{ GridObject::GetObjectsToDestroy() };

	for (auto* pObject : toDestroy)
	{
		if (!pObject) continue;

		Player* pPlayer{ dynamic_cast<Player*>(pObject) };
		if (pPlayer) m_pGridMap->RemovePlayer(pPlayer);
		else m_pGridMap->RemoveGridObject(pObject);

		m_pGridMap->RemoveChild(pObject, true);
	}

	GridObject::GetObjectsToDestroy().clear();
}

void LevelScene::AddGridObjects()
{
	const auto& toAdd{ GridObject::GetObjectsToAdd() };

	for (auto& pObject : toAdd)
	{
		if (!pObject) continue;

		Player* pPlayer{ dynamic_cast<Player*>(pObject) };
		if (pPlayer) m_pGridMap->AddPlayer(pPlayer);
		else m_pGridMap->AddGridObject(pObject);

		m_pGridMap->AddChild(pObject);
	}

	GridObject::GetObjectsToAdd().clear();
}

bool LevelScene::HasGameEnded() const
{
	if (!m_pCountdownTimer || !m_pGridMap) return false;

	// Check if the timer has run out
	if (m_pCountdownTimer->HasCountdownEnded()) return true;

	// Check if the Deadplayers vector is greater than or equal to the amount of players
	if (m_pGridMap->GetDeadPlayers().size() >= m_PlayerCount - 1) return true;

	return false;
}

void LevelScene::CreatePauseMenu()
{
	m_pPauseMenu = new GameObject{};
	m_pPauseMenu->AddComponent(new SpriteComponent{ ExamAssets::PauseMenuBackground, DirectX::XMFLOAT2{ .5f, .5f } });
	m_pPauseMenu->GetTransform()->Translate(m_SceneContext.windowWidth * .5f, m_SceneContext.windowHeight * .5f, .0f);
	AddChild(m_pPauseMenu);

	m_pUIManager = new UIManager{};
	AddChild(m_pUIManager);

	XMFLOAT2 buttonPos{ m_SceneContext.windowWidth * .5f, m_SceneContext.windowHeight * .35f };

	m_pPauseButtons.emplace_back(CreatePauseButtons(L"Main Menu", buttonPos));
	m_pUIManager->AddButton(m_pPauseButtons.back());
	m_pPauseButtons.back()->SetOnClickFunction([this]() { Reset(); SceneManager::Get()->SetActiveGameScene(L"MainMenuScene"); });
	AddChild(m_pPauseButtons.back());
	buttonPos.y += 35.f;

	m_pPauseButtons.emplace_back(CreatePauseButtons(L"Restart", buttonPos));
	m_pUIManager->AddButton(m_pPauseButtons.back());
	m_pPauseButtons.back()->SetOnClickFunction([this]() { Reset(); SetUpLevel(); });
	AddChild(m_pPauseButtons.back());
	buttonPos.y += 35.f;

	m_pPauseButtons.emplace_back(CreatePauseButtons(L"Quit", buttonPos));
	m_pUIManager->AddButton(m_pPauseButtons.back());
	m_pPauseButtons.back()->SetOnClickFunction([]() { PostQuitMessage(0); });
	AddChild(m_pPauseButtons.back());

	m_pPauseButtons[0]->SetNavigationButtons(m_pPauseButtons[2], m_pPauseButtons[1], nullptr, nullptr);
	m_pPauseButtons[1]->SetNavigationButtons(m_pPauseButtons[0], m_pPauseButtons[2], nullptr, nullptr);
	m_pPauseButtons[2]->SetNavigationButtons(m_pPauseButtons[1], m_pPauseButtons[0], nullptr, nullptr);

	m_pUIManager->SetFirstSelectedButton(m_pPauseButtons[0]);
	m_pUIManager->EnableInput();
}

void LevelScene::SetUpLevel()
{
	SoundManager::Get()->GetSystem()->playSound(m_pBattleStartSound, nullptr, false, nullptr);
	m_pGridMap = AddChild(new GridMap{ 19, 13 });
}

void LevelScene::ClearPauseMenu()
{
	if (m_pPauseMenu)
	{
		RemoveChild(m_pPauseMenu, true);
		m_pPauseMenu = nullptr;

		RemoveChild(m_pUIManager, true);
		m_pUIManager = nullptr;

		for (UIButton* pButton : m_pPauseButtons)
		{
			RemoveChild(pButton, true);
		}

		m_pPauseButtons.clear();
	}
}

void LevelScene::DecideWinner()
{
	if (!m_pGridMap) return;

	const auto& players{ m_pGridMap->GetPlayers() };
	m_WinnerName = L"";
	int highestScore{ 0 };
	int highestLives{ 0 };
	int tieCount{ 0 };

	for (const Player* pPlayer : players)
	{
		if (!pPlayer) continue;

		if (pPlayer->GetLives() >= highestLives)
		{
			if (pPlayer->GetLives() > highestLives || pPlayer->GetScore() > highestScore)
			{
				highestLives = pPlayer->GetLives();
				highestScore = pPlayer->GetScore();
				m_WinnerName = pPlayer->GetName();
				tieCount = 0;
			}
			else if (pPlayer->GetLives() == highestLives && pPlayer->GetScore() == highestScore)
			{
				++tieCount;
			}
		}
	}

	if (tieCount > 0)
	{
		m_WinMessage = L"It's a tie!";
	}
	else
	{
		m_WinMessage = m_WinnerName + L" wins!";
	}
}

void LevelScene::CreateWinMessage()
{
	m_pWinMessage = new GameObject{};
	m_pWinMessage->AddComponent(new SpriteComponent{ ExamAssets::TextBackground, DirectX::XMFLOAT2{ .5f, .5f } });
	m_pWinMessage->GetTransform()->Translate(m_SceneContext.windowWidth * .5f, m_SceneContext.windowHeight * .5f, .0f);
	m_pWinMessage->GetTransform()->Scale(.75f, .75f, 1.f);
	AddChild(m_pWinMessage);
}

void LevelScene::ClearWinMessage()
{
	if (!m_pWinMessage) return;

	RemoveChild(m_pWinMessage, true);
	m_pWinMessage = nullptr;
}

void LevelScene::Reset()
{
	m_Paused = false;
	m_GameEnded = false;
	m_pCountdownTimer->ResumeTimer();
	GridObject::UnPause();

	m_pCountdownTimer->StopTimer();
	m_pGridMap->Reset();

	AddGridObjects();
	RemoveGridObjects();

	RemoveChild(m_pGridMap, true);
	m_pGridMap = nullptr;

	RemoveChild(m_pCountdownTimer, true);
	m_pCountdownTimer = nullptr;

	ClearWinMessage();

	m_GameStarted = false;
	m_PlayerCount = 0;

	m_PlayerReady[0] = false;
	m_PlayerReady[1] = false;
	m_PlayerReady[2] = false;
	m_PlayerReady[3] = false;

	m_pMusicChannelGroup->stop();
}

UIButton* LevelScene::CreatePauseButtons(const std::wstring& text, const XMFLOAT2& pos) const
{
	// Measure the size of the buttons
	XMFLOAT2 buttonSize{ SpriteFont::MeasureString(text, m_pSmallFont) };

	// Pixel perfect button position
	const int x{ static_cast<int>(pos.x - buttonSize.x * .5f + .5f) };

	XMFLOAT2 buttonPos{ static_cast<float>(x), pos.y };

	return new UIButton{ m_pSmallFont, text, buttonPos };
}
