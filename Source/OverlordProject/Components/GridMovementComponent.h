#pragma once

class GridMap;
class GridObject;

class GridMovementComponent final : public BaseComponent
{
public:
	explicit GridMovementComponent(float moveSpeed = 1.f);
	~GridMovementComponent() override = default;

	GridMovementComponent(const GridMovementComponent& other) = delete;
	GridMovementComponent(GridMovementComponent&& other) noexcept = delete;
	GridMovementComponent& operator=(const GridMovementComponent& other) = delete;
	GridMovementComponent& operator=(GridMovementComponent&& other) noexcept = delete;

	// Normalize the amount
	void IncreaseMoveSpeed() { m_MoveTime = std::max(.1f, m_MoveTime - .1f); }
	void DecreaseMoveSpeed() { m_MoveTime = std::min(m_MaxMoveTime, m_MoveTime + .1f); }

	void MoveNorth();
	void MoveSouth();
	void MoveEast();
	void MoveWest();

	bool IsMoving() const;

protected:
	void Initialize(const SceneContext& sceneContext) override;
	void Update(const SceneContext& sceneContext) override;

private:
	GridMap* m_pGridMap{ nullptr };
	GridObject* m_pGridObjectOwner{ nullptr };
	XMINT3 m_OriginalDimensions{ 0, 0, 0 };

	XMFLOAT3 m_CurrentPosition{ 0, 0, 0 };
	XMFLOAT3 m_TargetPosition{ 0, 0, 0 };
	XMINT2 m_CurrentGridPosition{ 0, 0 };

	float m_MoveTime{ 1.f };
	float m_MoveTimer{ .0f };
	float m_MaxMoveTime{ 1.f };

	bool CanMoveTo(const XMINT2& gridIndex) const;
};
