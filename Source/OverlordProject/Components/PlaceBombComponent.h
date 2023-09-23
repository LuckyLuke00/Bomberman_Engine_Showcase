#pragma once

class GridMap;
class GridObject;
class Bomb;

class PlaceBombComponent final : public BaseComponent
{
public:
	explicit PlaceBombComponent(int maxBombs = 1, int explosionRadius = 1);
	~PlaceBombComponent() override = default;

	PlaceBombComponent(const PlaceBombComponent& other) = delete;
	PlaceBombComponent(PlaceBombComponent&& other) noexcept = delete;
	PlaceBombComponent& operator=(const PlaceBombComponent& other) = delete;
	PlaceBombComponent& operator=(PlaceBombComponent&& other) noexcept = delete;

	void SetBombTimer(float bombTimer) { m_BombTimerMax = bombTimer; }
	void SetExplosionRadius(int explosionRadius) { m_ExplosionRadius = explosionRadius; }
	void IncreaseExplosionRadius() { m_ExplosionRadius = std::min(m_ExplosionRadius + 1, m_ExplosionRadiusMax); }
	void DecreaseExplosionRadius() { m_ExplosionRadius = std::max(1, m_ExplosionRadius - 1); }
	void IncreaseBombs() { m_Bombs = std::min(m_Bombs + 1, m_BombsMax); }
	void DecreaseBombs() { m_Bombs = std::max(1, m_Bombs - 1); }
	void SetMaxBombs(int maxBombs) { m_Bombs = maxBombs; m_pLiveBombs.reserve(m_Bombs); }

	void SetGridMap(GridMap* pGridMap) { m_pGridMap = pGridMap; }
	void SetGridObjectOwner(GridObject* pGridObjectOwner) { m_pGridObjectOwner = pGridObjectOwner; }

	void PlaceBomb();
	void OnBombExploded(Bomb* pBomb);

protected:
	void Initialize(const SceneContext& sceneContext) override;

private:
	std::vector<FMOD::Sound*> m_pExplosionSounds{};
	FMOD::Sound* m_pPlaceBombSound{ nullptr };

	int m_ExplosionRadius{ 1 };
	int m_ExplosionRadiusMax{ 5 };
	int m_Bombs{ 1 };
	int m_BombsMax{ 5 };

	float m_BombTimerMax{ 3.f };
	float m_BombTimer{ .0f };

	GridMap* m_pGridMap{ nullptr };
	GridObject* m_pGridObjectOwner{ nullptr };

	std::vector<Bomb*> m_pLiveBombs;
};
