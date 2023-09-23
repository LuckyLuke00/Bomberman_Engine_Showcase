#pragma once
class ColorMaterial_Shadow_Skinned : public Material<ColorMaterial_Shadow_Skinned>
{
public:
	ColorMaterial_Shadow_Skinned();
	~ColorMaterial_Shadow_Skinned() override = default;

	ColorMaterial_Shadow_Skinned(const ColorMaterial_Shadow_Skinned& other) = delete;
	ColorMaterial_Shadow_Skinned(ColorMaterial_Shadow_Skinned&& other) noexcept = delete;
	ColorMaterial_Shadow_Skinned& operator=(const ColorMaterial_Shadow_Skinned& other) = delete;
	ColorMaterial_Shadow_Skinned& operator=(ColorMaterial_Shadow_Skinned&& other) noexcept = delete;

	void SetColor(const DirectX::XMVECTORF32& color);

protected:
	void InitializeEffectVariables() override;
	void OnUpdateModelVariables(const SceneContext& /*sceneContext*/, const ModelComponent* /*pModel*/) const override;

private:
	TextureData* m_pDiffuseTexture{};
};
