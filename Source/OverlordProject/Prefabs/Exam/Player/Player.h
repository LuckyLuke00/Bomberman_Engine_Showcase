#pragma once
#include "Exam/ExamInput.h"
#include "Prefabs/Exam/Level/GridObject.h"
#include <array>

class ColorMaterial_Shadow_Skinned;
class GridMovementComponent;
class PlaceBombComponent;

class Player final : public GridObject
{
public:
	explicit Player(const std::wstring& model, int index);
	~Player() override = default;

	Player(const Player& other) = delete;
	Player(Player&& other) noexcept = delete;
	Player& operator=(const Player& other) = delete;
	Player& operator=(Player&& other) noexcept = delete;

	static void Reset();

	void SetSpawnPoint(const XMINT2& spawnPoint) { m_SpawnPoint = spawnPoint; }
	void SetSpawnPoint(int row, int col) { m_SpawnPoint.x = row; m_SpawnPoint.y = col; }
	void SetLives(int lives) { m_Lives = lives; }
	void Kill();
	void DisableInput();
	void AddScore(int score) { m_Score += score; }
	void SubtractScore(int score) { m_Score -= score; }
	int GetScore() const { return m_Score; }
	int GetLives() const { return m_Lives; }
	std::wstring GetName() const { return m_Name; }

protected:
	void Initialize(const SceneContext& sceneContext) override;
	void OnSceneAttach(GameScene* pScene) override;
	void Update(const SceneContext& sceneContext) override;

private:
	enum class AnimationState : UINT
	{
		Dance,
		Death,
		Drop,
		Floating,
		Idle,
		Lose,
		Run,
	};

	static std::vector<XMVECTORF32> m_ColorVariants;
	static FMOD::Sound* m_pSpawnSound;
	static FMOD::Sound* m_pDeathSound;

	bool m_IsDead{ false };
	bool m_IsInputEnabled{ false };
	bool m_HasDeathSoundPlayed{ false };

	int m_GamepadIndex{ 0 };
	int m_Score{ 0 };
	int m_MaxLives{ 3 };
	int m_Lives{ 3 };
	float m_RespawnTimer{ 0.f };
	float m_RespawnTime{ 2.f };

	std::wstring m_Name{};
	std::array<std::wstring, 4> m_ColorNames{ L"White", L"Red", L"Blue", L"Yellow" };

	ColorMaterial_Shadow_Skinned* m_pBlack{ nullptr };
	ColorMaterial_Shadow_Skinned* m_pEyebrow{ nullptr };
	ColorMaterial_Shadow_Skinned* m_pEyes{ nullptr };
	ColorMaterial_Shadow_Skinned* m_pFace{ nullptr };
	ColorMaterial_Shadow_Skinned* m_pGold{ nullptr };
	ColorMaterial_Shadow_Skinned* m_pPink{ nullptr };
	ColorMaterial_Shadow_Skinned* m_pTShirt{ nullptr };
	ColorMaterial_Shadow_Skinned* m_pBody{ nullptr };

	AnimationState m_AnimationState{ AnimationState::Idle };
	GridMovementComponent* m_pGridMovementComponent{ nullptr };
	ModelAnimator* m_pModelAnimator{ nullptr };
	ModelComponent* m_pModelComponent{ nullptr };
	PlaceBombComponent* m_pPlaceBombComponent{ nullptr };

	XMINT2 m_SpawnPoint{ 1, 1 };

	bool HandleThumbstickInput() const;
	void EnableInput();
	void HandleAnimations();
	void HandleDeath();
	void HandleInput() const;
	void SetPlayerMaterials();

	int GetActionID(ExamInput::InputActions action) const;
};
