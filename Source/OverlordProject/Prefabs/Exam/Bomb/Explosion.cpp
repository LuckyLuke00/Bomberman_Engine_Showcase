#include "stdafx.h"
#include "Explosion.h"
#include "Materials/DiffuseMaterial.h"
#include "Prefabs/Exam/Level/GridMap.h"
#include "Prefabs/Exam/Player/Player.h"
#include "Prefabs/Exam/Level/Block.h"
#include "Prefabs/Exam/Bomb/Bomb.h"
#include "ExplosionParticleEffect.h"
#include "Exam/ExamAssets.h"
#include "Scenes/Exam/LevelScene.h"

Explosion::Explosion(const std::wstring& model, const std::wstring& texture, float lifeTime) :
	m_LifeTime{ lifeTime }
{
	DiffuseMaterial* pMaterial{ MaterialManager::Get()->CreateMaterial<DiffuseMaterial>() };
	if (!texture.empty()) pMaterial->SetDiffuseTexture(texture);

	AddComponent(new ModelComponent{ model })->SetMaterial(pMaterial);

	m_pExplosionParticleEffect = new ExplosionParticleEffect{ ExamAssets::ExplosionParticleTexture };
	m_pExplosionParticleEffect->SetDimensions(1, 1);
	m_pExplosionParticleEffect->SetCollision(false);
	m_pExplosionParticleEffect->MarkForAdd();
}

void Explosion::Initialize(const SceneContext&)
{
	m_pExplosionParticleEffect->SetPosition(GetPosition());
	SetScale(.01f, .01f);
}

void Explosion::Update(const SceneContext& sceneContext)
{
	if (IsPaused()) return;

	GridObject* pGridObject{ GetGridMap()->GetGridObjectAt(GetPosition()) };

	Block* pBlock{ dynamic_cast<Block*>(pGridObject) };
	if (pBlock && pBlock->IsBreakable()) pBlock->Break();

	Player* pPlayer{ dynamic_cast<Player*>(pGridObject) };
	if (pPlayer && !LevelScene::GameEnd()) pPlayer->Kill();

	Bomb* pBomb{ dynamic_cast<Bomb*>(pGridObject) };
	if (pBomb) pBomb->Explode();

	m_Timer += sceneContext.pGameTime->GetElapsed();
	if (m_Timer >= m_LifeTime)
	{
		m_Timer = .0f;
		MarkForDelete();
		m_pExplosionParticleEffect->MarkForDelete();
	}
}
