#pragma once
#include "PowerUp.h"

class BombDownPowerUp final : public PowerUp
{
public:
	explicit BombDownPowerUp(const std::wstring& model, const std::wstring& texture);
	~BombDownPowerUp() override = default;

	BombDownPowerUp(const BombDownPowerUp& other) = delete;
	BombDownPowerUp(BombDownPowerUp&& other) noexcept = delete;
	BombDownPowerUp& operator=(const BombDownPowerUp& other) = delete;
	BombDownPowerUp& operator=(BombDownPowerUp&& other) noexcept = delete;

	void Use(Player* pPlayer) override;
};
