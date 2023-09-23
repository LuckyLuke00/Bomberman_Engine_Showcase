#include "stdafx.h"
#include "EndScene.h"
#include "Prefabs/Exam/UI/Buttons/UIButton.h"
#include "Prefabs/Exam/UI/UIManager.h"
#include "Exam/ExamAssets.h"

void EndScene::Initialize()
{
	GetSceneSettings().drawGrid = false;
	GetSceneSettings().drawPhysXDebug = false;
	GetSceneSettings().showInfoOverlay = false;
	GetSceneSettings().enableOnGUI = false;

	// Set-up a fixed camera
	auto pCameraObject{ new FixedCamera{} };
	AddChild(pCameraObject);
	SetActiveCamera(pCameraObject->GetComponent<CameraComponent>());

	UIManager* pUIManager{ new UIManager{} };
	AddChild(pUIManager);

	GameObject* pBackground{ new GameObject{} };
	pBackground->AddComponent(new SpriteComponent{ ExamAssets::EndScreenBackground });
	AddChild(pBackground);

	// Create a button
	auto pFont{ ContentManager::Load<SpriteFont>(ExamAssets::SmallFont) };

	XMFLOAT2 buttonPos{ m_SceneContext.windowWidth * .155f, m_SceneContext.windowHeight * .3f };
	XMFLOAT2 size{ SpriteFont::MeasureString(L"Main Menu", pFont) };

	auto pButtonOne{ new UIButton{ pFont, L"Main Menu", { buttonPos.x - size.x * .5f, buttonPos.y } } };
	buttonPos.y += 35.f;

	size = SpriteFont::MeasureString(L"Restart", pFont);
	auto pButtonTwo{ new UIButton{ pFont, L"Restart", { buttonPos.x - size.x * .5f, buttonPos.y } } };
	buttonPos.y += 35.f;

	size = SpriteFont::MeasureString(L"Exit Game", pFont);
	auto pButtonThree{ new UIButton{ pFont, L"Exit Game", { buttonPos.x - size.x * .5f, buttonPos.y } } };

	pButtonOne->SetOnClickFunction(std::bind_front(&EndScene::MainMenu, this));
	pButtonTwo->SetOnClickFunction(std::bind_front(&EndScene::RestartGame, this));
	pButtonThree->SetOnClickFunction(std::bind_front(&EndScene::ExitGame, this));

	pButtonOne->SetNavigationButtons(pButtonThree, pButtonTwo, nullptr, nullptr);
	pButtonTwo->SetNavigationButtons(pButtonOne, pButtonThree, nullptr, nullptr);
	pButtonThree->SetNavigationButtons(pButtonTwo, pButtonOne, nullptr, nullptr);

	AddChild(pButtonOne);
	AddChild(pButtonTwo);
	AddChild(pButtonThree);

	pUIManager->AddButton(pButtonOne);
	pUIManager->AddButton(pButtonTwo);
	pUIManager->AddButton(pButtonThree);

	pUIManager->SetFirstSelectedButton(pButtonOne);

	pUIManager->EnableInput();

	SoundManager::Get()->GetSystem()->createSound(ExamAssets::EndScreenMusic.c_str(), FMOD_LOOP_NORMAL, nullptr, &m_pMenuMusic);
	SoundManager::Get()->GetSystem()->createChannelGroup("Music", &m_pMusicChannelGroup);
	m_pMusicChannelGroup->setVolume(.5f);
}

void EndScene::OnSceneActivated()
{
	SoundManager::Get()->GetSystem()->playSound(m_pMenuMusic, m_pMusicChannelGroup, false, nullptr);
}

void EndScene::MainMenu()
{
	m_pMusicChannelGroup->stop();
	SceneManager::Get()->SetActiveGameScene(L"MainMenuScene");
}

void EndScene::RestartGame()
{
	m_pMusicChannelGroup->stop();
	SceneManager::Get()->SetActiveGameScene(L"LevelScene");
}

void EndScene::ExitGame()
{
	PostQuitMessage(0);
}
