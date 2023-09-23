#pragma once
class SpriteTestScene : public GameScene
{
public:
	SpriteTestScene() :GameScene(L"SpriteTestScene") {}
	~SpriteTestScene() override = default;

	SpriteTestScene(const SpriteTestScene& other) = delete;
	SpriteTestScene(SpriteTestScene&& other) noexcept = delete;
	SpriteTestScene& operator=(const SpriteTestScene& other) = delete;
	SpriteTestScene& operator=(SpriteTestScene&& other) noexcept = delete;

protected:
	void Initialize() override;
	void Update() override;
	void OnGUI() override;

private:
	bool m_AutoRotate{ false };
	float m_TotalRotation{ .0f };
	GameObject* m_pSprite{ nullptr };
};
