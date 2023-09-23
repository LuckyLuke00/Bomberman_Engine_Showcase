#pragma once
#include "GridObject.h"

class Player;

class GridMap final : public GameObject
{
public:
	static const std::wstring m_Tag;

	explicit GridMap(int rows, int cols);
	~GridMap() override = default;

	GridMap(const GridMap& other) = delete;
	GridMap(GridMap&& other) noexcept = delete;
	GridMap& operator=(const GridMap& other) = delete;
	GridMap& operator=(GridMap&& other) noexcept = delete;

	bool IsOccupied(const XMINT2& gridIndex) const { return IsOccupied(gridIndex.x, gridIndex.y); }
	bool IsOccupied(int row, int col) const;

	bool IsOccupiedByPlayer(const XMINT2& gridIndex) const { return IsOccupiedByPlayer(gridIndex.x, gridIndex.y); }
	bool IsOccupiedByPlayer(int row, int col) const;

	bool IsOutOfBounds(const XMINT2& gridIndex) const { return IsOutOfBounds(gridIndex.x, gridIndex.y); }
	bool IsOutOfBounds(int row, int col) const;

	GridObject* GetGridObjectAt(const XMINT2& gridIndex) const { return GetGridObjectAt(gridIndex.x, gridIndex.y); }
	GridObject* GetGridObjectAt(int row, int col) const;

	void AddGridObject(GridObject* pGridObject) { m_pGridObjects.emplace_back(pGridObject); }
	void AddPlayer(Player* pPlayer) { m_pPlayers.emplace_back(pPlayer); }
	std::vector<Player*>& GetPlayers() { return m_pPlayers; }

	void RemoveGridObject(GridObject* pGridObject);
	void RemovePlayer(Player* pPlayer);

	void AddDeadPlayer(Player* pPlayer) { if (pPlayer) m_DeadPlayers.emplace_back(pPlayer); }
	std::vector<Player*>& GetDeadPlayers() { return m_DeadPlayers; }

	XMINT2 GetGridIndex(const XMFLOAT3& position) const;

	int GetRows() const { return m_Rows; }
	int GetCols() const { return m_Cols; }

	void Reset();

protected:
	void Initialize(const SceneContext& sceneContext) override;

private:
	const int m_Rows{ 0 };
	const int m_Cols{ 0 };

	std::vector<GridObject*> m_pGridObjects{};
	std::vector<Player*> m_pPlayers{};
	std::vector<Player*> m_DeadPlayers{};

	void SetupGrid();

	void SetUpFloor() const;
	void SetUpWalls();
	void SetUpPillars();
	void SetUpBreakableBlocks();

	int FindPlayerIndex(int row, int col) const;
	int FindGridObjectIndex(int row, int col) const;
};
