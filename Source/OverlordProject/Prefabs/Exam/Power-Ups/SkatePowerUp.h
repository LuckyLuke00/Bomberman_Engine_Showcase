#pragma once
#include "PowerUp.h"

class SkatePowerUp final : public PowerUp
{
public:
	explicit SkatePowerUp(const std::wstring& model, const std::wstring& texture);
	~SkatePowerUp() override = default;

	SkatePowerUp(const SkatePowerUp& other) = delete;
	SkatePowerUp& operator=(const SkatePowerUp& other) = delete;
	SkatePowerUp(SkatePowerUp&& other) noexcept = delete;
	SkatePowerUp& operator=(SkatePowerUp&& other) noexcept = delete;

	void Use(Player* pPlayer) override;
};
