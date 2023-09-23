#pragma once

class PongScene final : public GameScene
{
public:
	PongScene();
	~PongScene() override = default;

	PongScene(const PongScene& other) = delete;
	PongScene(PongScene&& other) noexcept = delete;
	PongScene& operator=(const PongScene& other) = delete;
	PongScene& operator=(PongScene&& other) noexcept = delete;

protected:
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void OnGUI() override;

private:
	bool m_Paused{ true };
	bool m_ResetScene{ false };

	const float m_BallSpeed{ 10.f };
	const float m_PaddleSpeed{ 10.f };
	const float m_PaddleWidth{ 4.f };
	const float m_WallOffset{ 11.f };

	FixedCamera* m_pCamera{ nullptr };

	GameObject* m_pBall{ nullptr };

	GameObject* m_pLeftPaddle{ nullptr };
	PxVec3 m_LeftPaddlePosition{ .0f, .0f, .0f };

	GameObject* m_pRightPaddle{ nullptr };
	PxVec3 m_RightPaddlePosition{ .0f, .0f, .0f };

	GameObject* m_pLeftTrigger{ nullptr };

	GameObject* m_pRightTrigger{ nullptr };

	void CreateBall(const PxMaterial* material);
	void CreateCamera();
	void CreatePaddles(const PxMaterial* material);
	void CreateTriggers(const PxMaterial* material);
	void CreateWalls(const PxMaterial* material);

	void OnTrigger(GameObject* pTriggerObject, GameObject* pOtherObject, PxTriggerAction action);

	void ResetScene();

	void HandleInput();
};
