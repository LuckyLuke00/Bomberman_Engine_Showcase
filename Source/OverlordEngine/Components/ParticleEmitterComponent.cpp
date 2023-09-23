#include "stdafx.h"
#include "ParticleEmitterComponent.h"
#include "Misc/ParticleMaterial.h"

ParticleMaterial* ParticleEmitterComponent::m_pParticleMaterial{};

ParticleEmitterComponent::ParticleEmitterComponent(const std::wstring& assetFile, const ParticleEmitterSettings& emitterSettings, UINT particleCount) :
	m_EmitterSettings(emitterSettings),
	m_ParticlesArray(new Particle[particleCount]), //How big is our particle buffer?
	m_ParticleCount(particleCount), //How many particles to draw (max == particleCount)
	m_MaxParticles(particleCount),
	m_AssetFile(assetFile)
{
	m_enablePostDraw = true; //This enables the PostDraw function for the component
}

ParticleEmitterComponent::~ParticleEmitterComponent()
{
	SafeDelete(m_ParticlesArray)
		SafeRelease(m_pVertexBuffer)
}

void ParticleEmitterComponent::Initialize(const SceneContext& sceneContext)
{
	if (!m_pParticleMaterial)
	{
		m_pParticleMaterial = MaterialManager::Get()->CreateMaterial<ParticleMaterial>();
	}

	CreateVertexBuffer(sceneContext);

	m_pParticleTexture = ContentManager::Load<TextureData>(m_AssetFile);
}

void ParticleEmitterComponent::CreateVertexBuffer(const SceneContext& sceneContext)
{
	SafeRelease(m_pVertexBuffer);

	//Create a vertex buffer for the particles
	D3D11_BUFFER_DESC bufferDesc{};
	bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	bufferDesc.ByteWidth = sizeof(VertexParticle) * m_ParticleCount;
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bufferDesc.MiscFlags = 0;

	HANDLE_ERROR(sceneContext.d3dContext.pDevice->CreateBuffer(&bufferDesc, nullptr, &m_pVertexBuffer));
}

void ParticleEmitterComponent::Update(const SceneContext& sceneContext)
{
	const float particleInterval{ (m_EmitterSettings.maxEnergy - m_EmitterSettings.minEnergy) / m_MaxParticles };
	float elapsedTime{ sceneContext.pGameTime->GetElapsed() };
	m_LastParticleSpawn += elapsedTime;

	m_ActiveParticles = 0;

	D3D11_MAPPED_SUBRESOURCE mappedResource{};
	HANDLE_ERROR(sceneContext.d3dContext.pDeviceContext->Map(m_pVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource));
	VertexParticle* pBuffer{ reinterpret_cast<VertexParticle*>(mappedResource.pData) };

	for (UINT i{ 0 }; i < m_ParticleCount; ++i)
	{
		Particle& p{ m_ParticlesArray[i] };
		if (p.isActive)
		{
			UpdateParticle(p, elapsedTime);
		}

		if (!p.isActive && m_LastParticleSpawn >= particleInterval)
		{
			SpawnParticle(p);
			m_LastParticleSpawn = .0f;
		}

		if (p.isActive)
		{
			pBuffer[m_ActiveParticles++] = p.vertexInfo;
		}
	}

	sceneContext.d3dContext.pDeviceContext->Unmap(m_pVertexBuffer, 0);
}

void ParticleEmitterComponent::UpdateParticle(Particle& p, float elapsedTime) const
{
	if (!p.isActive) return;

	p.currentEnergy -= elapsedTime;
	if (p.currentEnergy < 0.f)
	{
		p.isActive = false;
		return;
	}

	const auto velocity{ XMLoadFloat3(&m_EmitterSettings.velocity) };
	const auto position{ XMLoadFloat3(&p.vertexInfo.Position) + (velocity * elapsedTime) };
	XMStoreFloat3(&p.vertexInfo.Position, position);

	const float lifePercent{ p.currentEnergy / p.totalEnergy };

	p.vertexInfo.Color = m_EmitterSettings.color;
	p.vertexInfo.Color.w *= lifePercent * 2.f;

	if (p.sizeChange < 1.0)
	{
		p.vertexInfo.Size = p.initialSize + p.sizeChange * lifePercent;
	}
	else
	{
		p.vertexInfo.Size = p.initialSize + p.sizeChange * (1 - lifePercent);
	}
}

void ParticleEmitterComponent::SpawnParticle(Particle& p)
{
	p.isActive = true;

	// Energy Initialization
	p.totalEnergy = MathHelper::randF(m_EmitterSettings.minEnergy, m_EmitterSettings.maxEnergy);
	p.currentEnergy = p.totalEnergy;

	// Random direction
	XMVECTOR randomDirection{ 1.f , .0f, .0f };
	XMMATRIX randomRotationMatrix{ XMMatrixRotationRollPitchYaw(MathHelper::randF(-XM_PI, XM_PI), MathHelper::randF(-XM_PI, XM_PI), MathHelper::randF(-XM_PI, XM_PI)) };
	randomDirection = XMVector3TransformNormal(randomDirection, randomRotationMatrix);

	// Position Initialization
	const float distance{ MathHelper::randF(m_EmitterSettings.minEmitterRadius, m_EmitterSettings.maxEmitterRadius) };
	const auto position{ XMLoadFloat3(&GetTransform()->GetWorldPosition()) };
	XMStoreFloat3(&p.vertexInfo.Position, position + (randomDirection * distance));

	// Size Initialization
	p.vertexInfo.Size = p.initialSize = MathHelper::randF(m_EmitterSettings.minSize, m_EmitterSettings.maxSize);
	p.sizeChange = MathHelper::randF(m_EmitterSettings.minScale, m_EmitterSettings.maxScale);

	// Rotation Initialization
	p.vertexInfo.Rotation = MathHelper::randF(-XM_PI, XM_PI);

	// Color Initialization
	p.vertexInfo.Color = m_EmitterSettings.color;
}

void ParticleEmitterComponent::PostDraw(const SceneContext& sceneContext)
{
	m_pParticleMaterial->SetVariable_Matrix(L"gWorldViewProj", sceneContext.pCamera->GetViewProjection());
	m_pParticleMaterial->SetVariable_Matrix(L"gViewInverse", sceneContext.pCamera->GetViewInverse());
	m_pParticleMaterial->SetVariable_Texture(L"gParticleTexture", m_pParticleTexture);

	const auto& techniqueContext{ m_pParticleMaterial->GetTechniqueContext() };
	sceneContext.d3dContext.pDeviceContext->IASetInputLayout(techniqueContext.pInputLayout);
	sceneContext.d3dContext.pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);

	UINT stride{ sizeof(VertexParticle) };
	UINT offset{ 0 };
	sceneContext.d3dContext.pDeviceContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);

	D3DX11_TECHNIQUE_DESC techDesc{};
	techniqueContext.pTechnique->GetDesc(&techDesc);
	const auto& technique{ techniqueContext.pTechnique };
	for (UINT p{ 0 }; p < techDesc.Passes; ++p)
	{
		technique->GetPassByIndex(p)->Apply(0, sceneContext.d3dContext.pDeviceContext);
		sceneContext.d3dContext.pDeviceContext->Draw(m_ActiveParticles, 0);
	}
}

void ParticleEmitterComponent::DrawImGui()
{
	if (ImGui::CollapsingHeader("Particle System"))
	{
		ImGui::SliderUInt("Count", &m_ParticleCount, 0, m_MaxParticles);
		ImGui::InputFloatRange("Energy Bounds", &m_EmitterSettings.minEnergy, &m_EmitterSettings.maxEnergy);
		ImGui::InputFloatRange("Size Bounds", &m_EmitterSettings.minSize, &m_EmitterSettings.maxSize);
		ImGui::InputFloatRange("Scale Bounds", &m_EmitterSettings.minScale, &m_EmitterSettings.maxScale);
		ImGui::InputFloatRange("Radius Bounds", &m_EmitterSettings.minEmitterRadius, &m_EmitterSettings.maxEmitterRadius);
		ImGui::InputFloat3("Velocity", &m_EmitterSettings.velocity.x);
		ImGui::ColorEdit4("Color", &m_EmitterSettings.color.x, ImGuiColorEditFlags_NoInputs);
	}
}
