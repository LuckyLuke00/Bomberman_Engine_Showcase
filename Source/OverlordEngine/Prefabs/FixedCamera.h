#pragma once
class FixedCamera final : public GameObject
{
public:
	FixedCamera() = default;
	~FixedCamera() override = default;
	FixedCamera(const FixedCamera& other) = delete;
	FixedCamera(FixedCamera&& other) noexcept = delete;
	FixedCamera& operator=(const FixedCamera& other) = delete;
	FixedCamera& operator=(FixedCamera&& other) noexcept = delete;

protected:
	void Initialize(const SceneContext& sceneContext) override;
};
