#include "stdafx.h"
#include "Block.h"
#if NDEBUG
#include "Materials/Shadow/DiffuseMaterial_Shadow.h"
#else
#include "Materials/DiffuseMaterial.h"
#endif
#include "Components/PowerUpSpawnerComponent.h"

Block::Block(const std::wstring& model, const std::wstring& texture, bool isBreakable) :
	m_IsBreakable{ isBreakable },
	m_TexturePath{ texture }
{
#if NDEBUG
	DiffuseMaterial_Shadow* pMaterial{ MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow>() };
	auto* pModel{ AddComponent(new ModelComponent{ model }) };
#else
	DiffuseMaterial* pMaterial{ MaterialManager::Get()->CreateMaterial<DiffuseMaterial>() };
	auto* pModel{ AddComponent(new ModelComponent{ model, false }) };
#endif

	if (!m_TexturePath.empty()) pMaterial->SetDiffuseTexture(m_TexturePath);
	pModel->SetMaterial(pMaterial);

	if (m_IsBreakable)
	{
		m_pSpawner = AddComponent(new PowerUpSpawnerComponent{});
	}
}

void Block::Break()
{
	m_pSpawner->SpawnPowerUp(GetPosition(), .25f);
	MarkForDelete();
}
