#include "stdafx.h"
#include "SpriteFont.h"

SpriteFont::SpriteFont(const SpriteFontDesc& fontDesc) :
	m_FontDesc(fontDesc) {}

XMFLOAT2 SpriteFont::MeasureString(const std::wstring_view& text, const SpriteFont* pFont)
{
	// Calculate the width and height of the string
	float width{};
	float height{};

	// Loop through all the characters in the string
	for (auto character : text)
	{
		// Get the metric of the character
		const auto& metric{ pFont->GetMetric(character) };

		// Add the width and height of the character to the total width and height
		width += metric.advanceX;
		height = std::max(height, static_cast<float>(metric.height + metric.offsetY));
	}

	// Return the width and height of the string
	return XMFLOAT2{ width, height };
}
