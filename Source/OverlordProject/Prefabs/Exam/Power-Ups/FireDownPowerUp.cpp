#include "stdafx.h"
#include "FireDownPowerUp.h"
#include "Materials/Shadow/DiffuseMaterial_Shadow.h"
#include "Prefabs/Exam/Player/Player.h"
#include "Components/PlaceBombComponent.h"

FireDownPowerUp::FireDownPowerUp(const std::wstring& model, const std::wstring& texture)
{
	DiffuseMaterial_Shadow* pMaterial{ MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow>() };
	if (!texture.empty()) pMaterial->SetDiffuseTexture(texture);

	auto* pModel{ AddComponent(new ModelComponent{ model }) };
	pModel->SetMaterial(pMaterial);
}

void FireDownPowerUp::Use(Player* pPlayer)
{
	if (!pPlayer) return;

	auto pPlaceBombComponent{ pPlayer->GetComponent<PlaceBombComponent>() };
	if (!pPlaceBombComponent) return;

	pPlaceBombComponent->DecreaseExplosionRadius();
}
