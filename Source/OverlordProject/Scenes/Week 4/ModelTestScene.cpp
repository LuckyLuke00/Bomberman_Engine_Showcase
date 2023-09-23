#include "stdafx.h"
#include "ModelTestScene.h"

#include "Materials/ColorMaterial.h"
#include "Materials/DiffuseMaterial.h"

void ModelTestScene::Initialize()
{
	auto& pPhysX{ PxGetPhysics() };
	auto pDefaultMaterial{ pPhysX.createMaterial(.5f, .5f, .5f) };

	// Ground plane
	GameSceneExt::CreatePhysXGroundPlane(*this, pDefaultMaterial);

	//// Color material
	//auto pMaterial{ MaterialManager::Get()->CreateMaterial<ColorMaterial>() };
	//pMaterial->SetColor(XMFLOAT4{ Colors::Red });

	// Diffuse material
	auto pMaterial{ MaterialManager::Get()->CreateMaterial<DiffuseMaterial>() };
	pMaterial->SetDiffuseTexture(L"Textures/Chair_Dark.dds");

	// Chair
	const auto pModel{ new ModelComponent(L"Meshes/Chair.ovm") };
	pModel->SetMaterial(pMaterial);

	m_pChair = new GameObject();
	m_pChair->AddComponent(pModel);
	AddChild(m_pChair);

	const auto pConvexMesh{ ContentManager::Load<PxConvexMesh>(L"Meshes/Chair.ovpc") };
	auto pRigidBody{ m_pChair->AddComponent(new RigidBodyComponent(false)) };
	pRigidBody->AddCollider(PxConvexMeshGeometry(pConvexMesh), *pDefaultMaterial);

	m_pChair->GetTransform()->Translate(.0f, 5.f, .0f);
}

void ModelTestScene::OnGUI()
{
	ImGui::Text("This only activates if\n SceneSettings.enableOnGUI is True.\n\n");
	ImGui::Text("Use ImGui to add custom\n controllable scene parameters!");
	ImGui::ColorEdit3("Demo ClearColor", &m_SceneContext.settings.clearColor.x, ImGuiColorEditFlags_NoInputs);
}
