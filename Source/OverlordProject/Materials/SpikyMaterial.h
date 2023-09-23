#pragma once
class SpikyMaterial final : public Material<SpikyMaterial>
{
public:
	SpikyMaterial();
	~SpikyMaterial() override = default;

	SpikyMaterial(const SpikyMaterial& other) = delete;
	SpikyMaterial(SpikyMaterial&& other) noexcept = delete;
	SpikyMaterial& operator=(const SpikyMaterial& other) = delete;
	SpikyMaterial& operator=(SpikyMaterial&& other) noexcept = delete;

	void UpdateEffectVariables() const;

	float m_SpikeLength{ .2f };
	XMFLOAT3 m_ColorDiffuse{ Colors::Red };

protected:
	void InitializeEffectVariables() override;
};
