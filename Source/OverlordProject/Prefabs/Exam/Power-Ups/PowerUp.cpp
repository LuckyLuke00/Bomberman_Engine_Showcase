#include "stdafx.h"
#include "PowerUp.h"
#include "Materials/DiffuseMaterial.h"
#include "Prefabs/Exam/Player/Player.h"
#include "Prefabs/Exam/Level/GridMap.h"
#include "Exam/ExamAssets.h"

FMOD::Sound* PowerUp::m_pPickUpSound{ nullptr };

void PowerUp::PickUp(Player* pPlayer)
{
	if (!m_pPickUpSound)
	{
		SoundManager::Get()->GetSystem()->createSound(ExamAssets::PowerUpPickupSound.c_str(), FMOD_DEFAULT, nullptr, &m_pPickUpSound);
	}

	if (!pPlayer) return;

	Use(pPlayer);

	MarkForDelete();
	SoundManager::Get()->GetSystem()->playSound(m_pPickUpSound, nullptr, false, nullptr);
}
