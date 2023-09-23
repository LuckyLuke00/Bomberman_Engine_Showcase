#pragma once
#include "Prefabs/Exam/Level/GridObject.h"

class ExplosionParticleEffect final : public GridObject
{
public:
	explicit ExplosionParticleEffect(const std::wstring& texture);
	~ExplosionParticleEffect() override = default;

	ExplosionParticleEffect(const ExplosionParticleEffect& other) = delete;
	ExplosionParticleEffect(ExplosionParticleEffect&& other) noexcept = delete;
	ExplosionParticleEffect& operator=(const ExplosionParticleEffect& other) = delete;
	ExplosionParticleEffect& operator=(ExplosionParticleEffect&& other) noexcept = delete;

protected:
	void Initialize(const SceneContext& sceneContext) override;

private:
	ParticleEmitterComponent* m_pEmitter{ nullptr };
	const std::wstring m_Texture{ L"" };
};
