#pragma once
#include "PowerUp.h"

class BombUpPowerUp final : public PowerUp
{
public:
	explicit BombUpPowerUp(const std::wstring& model, const std::wstring& texture);
	~BombUpPowerUp() override = default;

	BombUpPowerUp(const BombUpPowerUp& other) = delete;
	BombUpPowerUp(BombUpPowerUp&& other) noexcept = delete;
	BombUpPowerUp& operator=(const BombUpPowerUp& other) = delete;
	BombUpPowerUp& operator=(BombUpPowerUp&& other) noexcept = delete;

	void Use(Player* pPlayer) override;
};
