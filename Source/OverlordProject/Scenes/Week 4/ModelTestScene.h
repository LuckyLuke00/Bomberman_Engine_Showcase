#pragma once
class ModelTestScene final : public GameScene
{
public:
	ModelTestScene() : GameScene(L"Component Test Scene") {}
	~ModelTestScene() override = default;

	ModelTestScene(const ModelTestScene& other) = delete;
	ModelTestScene(ModelTestScene&& other) noexcept = delete;
	ModelTestScene& operator=(const ModelTestScene& other) = delete;
	ModelTestScene& operator=(ModelTestScene&& other) noexcept = delete;

protected:
	void Initialize() override;
	void OnGUI() override;

private:
	GameObject* m_pChair{ nullptr };
};
