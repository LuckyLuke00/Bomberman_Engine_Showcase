#pragma once

class GridMap;
class GridObject;

class PowerUpSpawnerComponent final : public BaseComponent
{
public:
	explicit PowerUpSpawnerComponent() = default;
	~PowerUpSpawnerComponent() override = default;

	PowerUpSpawnerComponent(const PowerUpSpawnerComponent& other) = delete;
	PowerUpSpawnerComponent(PowerUpSpawnerComponent&& other) noexcept = delete;
	PowerUpSpawnerComponent& operator=(const PowerUpSpawnerComponent& other) = delete;
	PowerUpSpawnerComponent& operator=(PowerUpSpawnerComponent&& other) noexcept = delete;

	// Function that spawns a random power-up on the grid, based on a chance percentage
	void SpawnPowerUp(const XMINT2& gridIndex, float chancePercentage) const;

protected:
	void Initialize(const SceneContext&) override {};

private:
	int m_PowerUpCount{ 6 };

	GridMap* m_pGridMap{ nullptr };
	GridObject* m_pGridObjectOwner{ nullptr };

	void CreateRandomPowerUp(const XMINT2& gridIndex) const;

	static FMOD::Sound* m_pPowerUpSpawnSound;
};
