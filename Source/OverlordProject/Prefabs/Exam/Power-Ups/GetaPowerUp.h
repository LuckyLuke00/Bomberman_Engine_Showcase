#pragma once
#include "PowerUp.h"

class GetaPowerUp final : public PowerUp
{
public:
	explicit GetaPowerUp(const std::wstring& model, const std::wstring& texture);
	~GetaPowerUp() override = default;

	GetaPowerUp(const GetaPowerUp& other) = delete;
	GetaPowerUp(GetaPowerUp&& other) noexcept = delete;
	GetaPowerUp& operator=(const GetaPowerUp& other) = delete;
	GetaPowerUp& operator=(GetaPowerUp&& other) noexcept = delete;

	void Use(Player* pPlayer) override;
};
