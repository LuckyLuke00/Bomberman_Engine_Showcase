#include "stdafx.h"
#include "UberMaterialScene.h"

#include "Materials/UberMaterial.h"

void UberMaterialScene::Initialize()
{
	m_SceneContext.settings.drawGrid = false;
	m_SceneContext.settings.enableOnGUI = true;

	m_pSphere = new GameObject();
	m_pSphere->GetTransform()->Scale(15.f);
	AddChild(m_pSphere);

	m_pMaterial = MaterialManager::Get()->CreateMaterial<UberMaterial>();
	m_pSphere->AddComponent(new ModelComponent(L"Meshes/Sphere.ovm"))->SetMaterial(m_pMaterial);

	// Set material textures
	m_pMaterial->SetEnvironmentTexture(L"Textures/Sunol_Cubemap.dds");
	m_pMaterial->SetDiffuseTexture(L"Textures/Skulls_Diffusemap.tga");
	m_pMaterial->SetNormalTexture(L"Textures/Skulls_Normalmap.tga");
	m_pMaterial->SetSpecularTexture(L"Textures/Skulls_Heightmap.tga");
}

void UberMaterialScene::Update()
{
	m_pSphere->GetTransform()->Rotate(.0f, 20.f * m_SceneContext.pGameTime->GetTotal(), .0f);
}

void UberMaterialScene::OnGUI()
{
	m_pMaterial->DrawImGui();
}
