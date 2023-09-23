#include "stdafx.h"
#include "PowerUpSpawnerComponent.h"
#include "Exam/ExamAssets.h"
#include "Prefabs/Exam/Power-Ups/BombDownPowerUp.h"
#include "Prefabs/Exam/Power-Ups/BombUpPowerUp.h"
#include "Prefabs/Exam/Power-Ups/FireDownPowerUp.h"
#include "Prefabs/Exam/Power-Ups/FirePowerUp.h"
#include "Prefabs/Exam/Power-Ups/GetaPowerUp.h"
#include "Prefabs/Exam/Power-Ups/SkatePowerUp.h"

FMOD::Sound* PowerUpSpawnerComponent::m_pPowerUpSpawnSound{ nullptr };

void PowerUpSpawnerComponent::SpawnPowerUp(const XMINT2& gridIndex, float chancePercentage) const
{
	if (MathHelper::randF(0.f, 1.f) > chancePercentage) return;

	CreateRandomPowerUp(gridIndex);

	if (!m_pPowerUpSpawnSound)
	{
		SoundManager::Get()->GetSystem()->createSound(ExamAssets::PowerUpSpawnSound.c_str(), FMOD_DEFAULT, nullptr, &m_pPowerUpSpawnSound);
	}
}

// I hate this implementation but because GameObject has deleted copy/move constructors and assignment operators
// We can't easily create copies of the power-ups, so this is the best I could come up with
void PowerUpSpawnerComponent::CreateRandomPowerUp(const XMINT2& gridIndex) const
{
	const int randomIndex{ MathHelper::randI(0, m_PowerUpCount - 1) };

	PowerUp* pPowerUp{ nullptr };

	switch (randomIndex)
	{
	case 0:
		pPowerUp = new FirePowerUp{ ExamAssets::PowerUpMesh, ExamAssets::FirePowerUpTexture };
		break;

	case 1:
		pPowerUp = new FireDownPowerUp{ ExamAssets::PowerUpMesh, ExamAssets::FireDownPowerUpTexture };
		break;

	case 2:
		pPowerUp = new BombUpPowerUp{ ExamAssets::PowerUpMesh, ExamAssets::BombUpPowerUpTexture };
		break;

	case 3:
		pPowerUp = new BombDownPowerUp{ ExamAssets::PowerUpMesh, ExamAssets::BombDownPowerUpTexture };
		break;

	case 4:
		pPowerUp = new SkatePowerUp{ ExamAssets::PowerUpMesh, ExamAssets::SkatePowerUpTexture };
		break;

	case 5:
		pPowerUp = new GetaPowerUp{ ExamAssets::PowerUpMesh, ExamAssets::GetaPowerUpTexture };
		break;

	default:
		break;
	}

	if (pPowerUp)
	{
		pPowerUp->SetDimensions(1, 1);
		pPowerUp->OffsetPosition(.0f, -.5f, .0f);
		pPowerUp->SetPosition(gridIndex.x, gridIndex.y);
		pPowerUp->SetCollision(false);
		pPowerUp->SetPickUp(true);
		pPowerUp->MarkForAdd();

		SoundManager::Get()->GetSystem()->playSound(m_pPowerUpSpawnSound, nullptr, false, nullptr);
	}
}
