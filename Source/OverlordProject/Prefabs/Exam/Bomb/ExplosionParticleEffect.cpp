#include "stdafx.h"
#include "ExplosionParticleEffect.h"

ExplosionParticleEffect::ExplosionParticleEffect(const std::wstring& texture) :
	m_Texture{ texture }
{}

void ExplosionParticleEffect::Initialize(const SceneContext&)
{
	ParticleEmitterSettings settings{};
	settings.velocity = { .0f, .5f, .0f };
	settings.minSize = .15f;
	settings.maxSize = .5f;
	settings.minEnergy = .05f;
	settings.maxEnergy = .5f;
	settings.minScale = .25f;
	settings.maxScale = .5f;
	settings.minEmitterRadius = .25f;
	settings.maxEmitterRadius = .5f;
	settings.color = { 1.f, 1.f, 1.f, .5f };

	m_pEmitter = AddComponent(new ParticleEmitterComponent(m_Texture, settings, 15));
}
