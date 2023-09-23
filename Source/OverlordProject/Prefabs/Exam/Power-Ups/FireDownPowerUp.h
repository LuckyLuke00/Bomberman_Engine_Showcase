#pragma once
#include "PowerUp.h"

class FireDownPowerUp final : public PowerUp
{
public:
	explicit FireDownPowerUp(const std::wstring& model, const std::wstring& texture);
	~FireDownPowerUp() override = default;

	FireDownPowerUp(const FireDownPowerUp& other) = delete;
	FireDownPowerUp(FireDownPowerUp&& other) noexcept = delete;
	FireDownPowerUp& operator=(const FireDownPowerUp& other) = delete;
	FireDownPowerUp& operator=(FireDownPowerUp&& other) noexcept = delete;

	void Use(Player* pPlayer) override;
};
