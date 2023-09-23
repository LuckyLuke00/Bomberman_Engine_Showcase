#include "stdafx.h"
#include "FirePowerUp.h"
#include "Materials/Shadow/DiffuseMaterial_Shadow.h"
#include "Prefabs/Exam/Player/Player.h"
#include "Components/PlaceBombComponent.h"

FirePowerUp::FirePowerUp(const std::wstring& model, const std::wstring& texture)
{
	DiffuseMaterial_Shadow* pMaterial{ MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow>() };
	if (!texture.empty()) pMaterial->SetDiffuseTexture(texture);

	auto* pModel{ AddComponent(new ModelComponent{ model }) };
	pModel->SetMaterial(pMaterial);
}

void FirePowerUp::Use(Player* pPlayer)
{
	if (!pPlayer) return;

	auto pPlaceBombComponent{ pPlayer->GetComponent<PlaceBombComponent>() };
	if (!pPlaceBombComponent) return;

	pPlaceBombComponent->IncreaseExplosionRadius();
	pPlayer->AddScore(200);
}
