#include "stdafx.h"
#include "SkatePowerUp.h"
#include "Materials/Shadow/DiffuseMaterial_Shadow.h"
#include "Prefabs/Exam/Player/Player.h"
#include "Components/GridMovementComponent.h"

SkatePowerUp::SkatePowerUp(const std::wstring& model, const std::wstring& texture)
{
	DiffuseMaterial_Shadow* pMaterial{ MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow>() };
	if (!texture.empty()) pMaterial->SetDiffuseTexture(texture);

	auto* pModel{ AddComponent(new ModelComponent{ model }) };
	pModel->SetMaterial(pMaterial);
}

void SkatePowerUp::Use(Player* pPlayer)
{
	if (!pPlayer) return;

	auto pGridMovementComponent{ pPlayer->GetComponent<GridMovementComponent>() };
	if (!pGridMovementComponent) return;

	pGridMovementComponent->IncreaseMoveSpeed();
	pPlayer->AddScore(400);
}
