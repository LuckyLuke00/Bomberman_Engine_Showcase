#include "stdafx.h"
#include "UberMaterial.h"

UberMaterial::UberMaterial() :
	Material<UberMaterial>(L"Effects/UberShader.fx")
{}

void UberMaterial::InitializeEffectVariables()
{
	SetVariable_Vector(L"gLightDirection", { .551f, -.534f, .641f });

	SetVariable_Scalar(L"gUseTextureDiffuse", true);
	SetVariable_Vector(L"gColorDiffuse", { 1.f, .1f, .14f, .0f });

	SetVariable_Vector(L"gColorSpecular", { .0f, .796f, .252f, .0f });
	SetVariable_Scalar(L"gUseTextureSpecularIntensity", true);
	SetVariable_Scalar(L"gShininess", 40);

	SetVariable_Scalar(L"gFlipGreenChannel", true);
	SetVariable_Scalar(L"gUseTextureNormal", true);
	SetVariable_Scalar(L"gUseEnvironmentMapping", true);

	SetVariable_Scalar(L"gReflectionStrength", .8f);
	SetVariable_Scalar(L"gRefractionStrength", .5f);
	SetVariable_Scalar(L"gRefractionIndex", .9f);

	SetVariable_Scalar(L"gUseFresnelFalloff", true);
	SetVariable_Scalar(L"gFresnelPower", 2.f);
	SetVariable_Scalar(L"gFresnelMultiplier", 1.5f);
	SetVariable_Scalar(L"gFresnelHardness", 2.f);

	SetVariable_Scalar(L"gUseSpecularPhong", true);
}

void UberMaterial::SetDiffuseTexture(const std::wstring& assetFile)
{
	SetVariable_Texture(L"gTextureDiffuse", ContentManager::Load<TextureData>(assetFile));
}

void UberMaterial::SetNormalTexture(const std::wstring& assetFile)
{
	SetVariable_Texture(L"gTextureNormal", ContentManager::Load<TextureData>(assetFile));
}

void UberMaterial::SetSpecularTexture(const std::wstring& assetFile)
{
	SetVariable_Texture(L"gTextureSpecularIntensity", ContentManager::Load<TextureData>(assetFile));
}

void UberMaterial::SetOpacityTexture(const std::wstring& assetFile)
{
	SetVariable_Texture(L"gTextureOpacity", ContentManager::Load<TextureData>(assetFile));
}

void UberMaterial::SetEnvironmentTexture(const std::wstring& assetFile)
{
	SetVariable_Texture(L"gCubeEnvironment", ContentManager::Load<TextureData>(assetFile));
}
