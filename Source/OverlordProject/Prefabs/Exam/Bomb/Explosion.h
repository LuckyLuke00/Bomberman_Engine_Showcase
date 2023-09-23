#pragma once
#include "Prefabs/Exam/Level/GridObject.h"

class ExplosionParticleEffect;

class Explosion final : public GridObject
{
public:
	explicit Explosion(const std::wstring& model, const std::wstring& texture, float lifeTime = 1.f);
	~Explosion() override = default;

	Explosion(const Explosion& other) = delete;
	Explosion(Explosion&& other) noexcept = delete;
	Explosion& operator=(const Explosion& other) = delete;
	Explosion& operator=(Explosion&& other) noexcept = delete;

protected:
	void Initialize(const SceneContext& sceneContext) override;
	void Update(const SceneContext& sceneContext) override;

private:
	float m_LifeTime{ 1.f };
	float m_Timer{ 0.f };

	ExplosionParticleEffect* m_pExplosionParticleEffect{ nullptr };
};
