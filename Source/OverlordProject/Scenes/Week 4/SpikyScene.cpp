#include "stdafx.h"
#include "SpikyScene.h"
#include "Materials/SpikyMaterial.h"

void SpikyScene::Initialize()
{
	m_SceneContext.settings.drawGrid = false;
	m_SceneContext.settings.enableOnGUI = true;

	m_pSphere = new GameObject();
	m_pSphere->GetTransform()->Scale(15.f);
	AddChild(m_pSphere);

	m_pMaterial = MaterialManager::Get()->CreateMaterial<SpikyMaterial>();
	m_pSphere->AddComponent(new ModelComponent(L"Meshes/OctaSphere.ovm"))->SetMaterial(m_pMaterial);
}

void SpikyScene::Update()
{
	// Rotate the sphere around it's y axis at 20 degrees per second
	m_pSphere->GetTransform()->Rotate(.0f, 20.f * m_SceneContext.pGameTime->GetTotal(), .0f);
}

void SpikyScene::OnGUI()
{
	ImGui::ColorEdit3("Color", &m_pMaterial->m_ColorDiffuse.x);
	ImGui::SliderFloat("Spike Length", &(m_pMaterial->m_SpikeLength), 0.f, 1.f);
	m_pMaterial->UpdateEffectVariables();
}
