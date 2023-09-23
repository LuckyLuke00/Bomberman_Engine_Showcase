//Precompiled Header [ALWAYS ON TOP IN CPP]
#include "stdafx.h"
#include "MinionScene.h"

#include "Prefabs/TorusPrefab.h"

MinionScene::MinionScene() :
	GameScene(L"MinionScene") {}

void MinionScene::Initialize()
{
	//m_SceneContext.settings.showInfoOverlay = true;
	//m_SceneContext.settings.drawPhysXDebug = true;
	//m_SceneContext.settings.drawGrid = true;
	m_SceneContext.settings.enableOnGUI = true;

	Logger::LogInfo(L"Welcome, humble Minion!");

	//Torus Big
	m_pTorusBig = new TorusPrefab(10, 50, 1, 20, XMFLOAT4(Colors::Orange));
	AddChild(m_pTorusBig);

	//Torus Medium
	m_pTorusMedium = new TorusPrefab(7, 50, 1, 20, XMFLOAT4(Colors::Blue));
	m_pTorusBig->AddChild(m_pTorusMedium);

	//Torus Small
	m_pTorusSmall = new TorusPrefab(4, 50, 1, 20, XMFLOAT4(Colors::Red));
	m_pTorusMedium->AddChild(m_pTorusSmall);
}

void MinionScene::Update()
{
	const float rotation{ 90 * m_SceneContext.pGameTime->GetTotal() };
	m_pTorusBig->GetTransform()->Rotate(0, rotation, 0);
	m_pTorusMedium->GetTransform()->Rotate(rotation, 0, 0);
	m_pTorusSmall->GetTransform()->Rotate(rotation, 0, 0);
}

void MinionScene::Draw()
{
	//Optional
}

void MinionScene::OnGUI()
{
	ImGui::Text("This only activates if\n SceneSettings.enableOnGUI is True.\n\n");
	ImGui::Text("Use ImGui to add custom\n controllable scene parameters!");
	ImGui::ColorEdit3("Demo ClearColor", &m_SceneContext.settings.clearColor.x, ImGuiColorEditFlags_NoInputs);
}
