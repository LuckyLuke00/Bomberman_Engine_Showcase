#pragma once
#include "PowerUp.h"

class FirePowerUp final : public PowerUp
{
public:
	explicit FirePowerUp(const std::wstring& model, const std::wstring& texture);
	~FirePowerUp() override = default;

	FirePowerUp(const FirePowerUp& other) = delete;
	FirePowerUp(FirePowerUp&& other) noexcept = delete;
	FirePowerUp& operator=(const FirePowerUp& other) = delete;
	FirePowerUp& operator=(FirePowerUp&& other) noexcept = delete;

	void Use(Player* pPlayer) override;
};
