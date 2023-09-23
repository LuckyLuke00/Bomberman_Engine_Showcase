#pragma once
#include "GridObject.h"

class PowerUpSpawnerComponent;

class Block : public GridObject
{
public:
	explicit Block(const std::wstring& model, const std::wstring& texture, bool isBreakable = true);
	~Block() override = default;

	Block(const Block& other) = delete;
	Block(Block&& other) noexcept = delete;
	Block& operator=(const Block& other) = delete;
	Block& operator=(Block&& other) noexcept = delete;

	bool IsBreakable() const { return m_IsBreakable; }
	void Break();

private:
	bool m_IsBreakable{ false };
	std::wstring m_TexturePath{};
	PowerUpSpawnerComponent* m_pSpawner{ nullptr };
};
