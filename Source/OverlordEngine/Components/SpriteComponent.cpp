#include "stdafx.h"
#include "SpriteComponent.h"

SpriteComponent::SpriteComponent(const std::wstring& spriteAsset, const XMFLOAT2& pivot, const XMFLOAT4& color) :
	m_SpriteAsset(spriteAsset),
	m_Pivot(pivot),
	m_Color(color)
{}

void SpriteComponent::Initialize(const SceneContext& /*sceneContext*/)
{
	m_pTexture = ContentManager::Load<TextureData>(m_SpriteAsset);
}

void SpriteComponent::SetTexture(const std::wstring& spriteAsset)
{
	m_SpriteAsset = spriteAsset;
	m_pTexture = ContentManager::Load<TextureData>(m_SpriteAsset);
}

void SpriteComponent::Draw(const SceneContext& sceneContext)
{
	if (!m_pTexture) return;

	auto* spriteRenderer{ SpriteRenderer::Get() };
	const auto* transform{ m_pGameObject->GetTransform() };

	spriteRenderer->AppendSprite
	(
		m_pTexture,
		DirectX::XMFLOAT2{ transform->GetWorldPosition().x, transform->GetWorldPosition().y },
		m_Color,
		m_Pivot,
		DirectX::XMFLOAT2{ transform->GetScale().x, transform->GetScale().y },
		MathHelper::QuaternionToEuler(transform->GetWorldRotation()).z,
		transform->GetPosition().z
	);

	spriteRenderer->Draw(sceneContext);
}
