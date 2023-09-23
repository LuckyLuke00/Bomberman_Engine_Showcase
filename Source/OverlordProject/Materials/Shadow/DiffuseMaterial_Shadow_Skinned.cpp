#include "stdafx.h"
#include "DiffuseMaterial_Shadow_Skinned.h"

DiffuseMaterial_Shadow_Skinned::DiffuseMaterial_Shadow_Skinned() :
	Material(L"Effects/Shadow/PosNormTex3D_Shadow_Skinned.fx")
{}

void DiffuseMaterial_Shadow_Skinned::SetDiffuseTexture(const std::wstring& assetFile)
{
	m_pDiffuseTexture = ContentManager::Load<TextureData>(assetFile);
	SetVariable_Texture(L"gDiffuseMap", m_pDiffuseTexture);
}

void DiffuseMaterial_Shadow_Skinned::InitializeEffectVariables()
{
}

void DiffuseMaterial_Shadow_Skinned::OnUpdateModelVariables(const SceneContext& sceneContext, const ModelComponent* pModel) const
{
	// Get the world matrix from the model component
	const auto& world{ XMLoadFloat4x4(&pModel->GetTransform()->GetWorld()) };

	// Get the light view projection matrix from the shadow map renderer
	const auto& lightVP{ XMLoadFloat4x4(&ShadowMapRenderer::Get()->GetLightVP()) };

	// Calculate the light world view projection matrix
	const auto lightWVP{ world * lightVP };
	XMFLOAT4X4 lightWVPFloat;
	XMStoreFloat4x4(&lightWVPFloat, lightWVP);

	// Set the light world view projection matrix
	SetVariable_Matrix(L"gWorldViewProj_Light", lightWVPFloat);

	// Set the shadow map texture
	SetVariable_Texture(L"gShadowMap", ShadowMapRenderer::Get()->GetShadowMap());

	// Set the light direction
	SetVariable_Vector(L"gLightDirection", sceneContext.pLights->GetDirectionalLight().direction);

	//Retrieve The Animator from the ModelComponent
	const auto pAnimator{ pModel->GetAnimator() };
	//Make sure the animator is not NULL (ASSERT_NULL_)
	ASSERT_NULL_(pAnimator);
	//Retrieve the BoneTransforms from the Animator
	const auto& boneTransforms{ pAnimator->GetBoneTransforms() };
	//Set the 'gBones' variable of the effect (MatrixArray) > BoneTransforms
	SetVariable_MatrixArray(L"gBones", &boneTransforms.begin()->m[0][0], static_cast<UINT>(boneTransforms.size()));
}
