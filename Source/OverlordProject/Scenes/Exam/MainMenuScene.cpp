#include "stdafx.h"
#include "MainMenuScene.h"
#include "Prefabs/Exam/UI/Buttons/UIButton.h"
#include "Prefabs/Exam/UI/UIManager.h"
#include "Exam/ExamAssets.h"

void MainMenuScene::Initialize()
{
	GetSceneSettings().drawGrid = false;
	GetSceneSettings().drawPhysXDebug = false;
	GetSceneSettings().showInfoOverlay = false;
	GetSceneSettings().enableOnGUI = false;

	// Set-up a fixed camera
	auto pCameraObject{ new FixedCamera{} };
	AddChild(pCameraObject);
	SetActiveCamera(pCameraObject->GetComponent<CameraComponent>());

	m_pUIManager = new UIManager{};
	AddChild(m_pUIManager);

	GameObject* pBackground{ new GameObject{} };
	pBackground->AddComponent(new SpriteComponent{ ExamAssets::MainMenuBackground });
	AddChild(pBackground);

	// Create a button
	auto pFont{ ContentManager::Load<SpriteFont>(ExamAssets::SmallFont) };

	const XMFLOAT2 buttonPos{ m_SceneContext.windowWidth * .155f, m_SceneContext.windowHeight * .3f };
	XMFLOAT2 size{ SpriteFont::MeasureString(L"Start Game", pFont) };

	auto pButtonOne{ new UIButton{ pFont, L"Start Game", { buttonPos.x - size.x * .5f, buttonPos.y } } };

	size = SpriteFont::MeasureString(L"Quit", pFont);
	auto pButtonTwo{ new UIButton{ pFont, L"Quit", { buttonPos.x - size.x * .5f, buttonPos.y + 35.f } } };

	pButtonOne->SetOnClickFunction(std::bind_front(&MainMenuScene::StartGame, this));
	pButtonTwo->SetOnClickFunction(std::bind_front(&MainMenuScene::ExitGame, this));

	pButtonOne->SetNavigationButtons(pButtonTwo, pButtonTwo, nullptr, nullptr);
	pButtonTwo->SetNavigationButtons(pButtonOne, pButtonOne, nullptr, nullptr);

	AddChild(pButtonOne);
	AddChild(pButtonTwo);

	m_pUIManager->AddButton(pButtonOne);
	m_pUIManager->AddButton(pButtonTwo);

	m_pUIManager->SetFirstSelectedButton(pButtonOne);

	m_pUIManager->EnableInput();

	SoundManager::Get()->GetSystem()->createSound(ExamAssets::MainMenuMusic.c_str(), FMOD_LOOP_NORMAL, nullptr, &m_pMenuMusic);
	SoundManager::Get()->GetSystem()->createChannelGroup("Music", &m_pMusicChannelGroup);
	m_pMusicChannelGroup->setVolume(.5f);
}

void MainMenuScene::OnSceneActivated()
{
	SoundManager::Get()->GetSystem()->playSound(m_pMenuMusic, m_pMusicChannelGroup, false, nullptr);
}

void MainMenuScene::ExitGame()
{
	PostQuitMessage(0);
}

void MainMenuScene::StartGame()
{
	m_pMusicChannelGroup->stop();
	SceneManager::Get()->SetActiveGameScene(L"LevelScene");
}
