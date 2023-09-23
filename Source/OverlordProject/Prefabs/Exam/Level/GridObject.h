#pragma once

class GridMap;

class GridObject : public GameObject
{
public:
	explicit GridObject() = default;
	~GridObject() override = default;

	GridObject(const GridObject& other) = delete;
	GridObject(GridObject&& other) noexcept = delete;
	GridObject& operator=(const GridObject& other) = delete;
	GridObject& operator=(GridObject&& other) noexcept = delete;

	void SetCollision(bool hasCollision) { m_HasCollision = hasCollision; }
	void SetDimensions(int width, int height);
	void SetPickUp(bool pickUp) { m_PickUp = pickUp; }
	void SetPosition(const XMINT2& position) { SetPosition(position.x, position.y); }
	void SetPosition(int row, int col);
	void SetScale(float width, float height);

	bool HasCollision() const { return m_HasCollision; }
	bool IsPickUp() const { return m_PickUp; }
	void OffsetPosition(float x = 0, float y = 0, float z = 0);
	XMINT2 GetDimensions() const { return m_Dimensions; }
	XMINT2 GetPosition() const { return m_Position; }

	void MarkForAdd();
	void MarkForDelete();

	static void Pause() { m_IsPaused = true; };
	static void UnPause() { m_IsPaused = false; };

	static bool IsPaused() { return m_IsPaused; };

	// Getter for the objects to destroy
	static std::vector<GridObject*>& GetObjectsToDestroy() { return m_pObjectsToDestroy; }
	static std::vector<GridObject*>& GetObjectsToAdd() { return m_pObjectsToAdd; }

protected:
	GridMap* GetGridMap();

private:
	bool m_HasCollision{ true };
	bool m_PickUp{ false };

	GridMap* m_pGridMap{ nullptr };

	static bool m_IsPaused;
	static std::vector<GridObject*> m_pObjectsToAdd;
	static std::vector<GridObject*> m_pObjectsToDestroy;

	XMFLOAT3 m_Offset{ .0f, .0f, .0f };
	XMINT2 m_Dimensions{ 1, 1 };
	XMINT2 m_Position{ 0, 0 };
};
