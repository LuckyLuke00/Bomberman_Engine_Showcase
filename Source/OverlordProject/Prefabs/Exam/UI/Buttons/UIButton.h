#pragma once

class UIButton final : public GameObject
{
public:
	UIButton(SpriteFont* pFont, const std::wstring& text, DirectX::XMFLOAT2 position = { .0f, .0f });
	~UIButton() override = default;
	UIButton(UIButton& other) = delete;
	UIButton(UIButton&& other) noexcept = delete;
	UIButton& operator=(UIButton& other) = delete;
	UIButton& operator=(UIButton&& other) noexcept = delete;

	bool IsSelected() const { return m_IsSelected; }
	void Select();
	void Deselect() { m_IsSelected = false; m_IsUsingNavigation = true; }

	void OnClick() const;

	void SetNormalColor(const DirectX::XMFLOAT4& color) { m_NormalColor = color; }
	void SetOnClickFunction(const std::function<void()>& onClickFunction) { m_OnClickFunction = onClickFunction; }
	void SetPosition(const DirectX::XMFLOAT2& position) { m_Position = position; }
	void SetSelectedColor(const DirectX::XMFLOAT4& color) { m_SelectedColor = color; }
	void SetText(const std::wstring_view& text) { m_Text = text; }

	// Set the navigation buttons for the button
	void SetNavigationButtons(UIButton* pUp, UIButton* pDown, UIButton* pLeft, UIButton* pRight);
	bool IsMouseHovering() const;

	// Getter for the navigation buttons
	UIButton* GetUpButton() const { return m_pNavigationButtons[0]; }
	UIButton* GetDownButton() const { return m_pNavigationButtons[1]; }
	UIButton* GetLeftButton() const { return m_pNavigationButtons[2]; }
	UIButton* GetRightButton() const { return m_pNavigationButtons[3]; }

	static void SetIsUsingNavigation(bool isUsingNavigation) { m_IsUsingNavigation = isUsingNavigation; }

protected:
	void Initialize(const SceneContext&) override;
	void Update(const SceneContext&) override;

private:
	static FMOD::Sound* m_pSubmitSound;
	static FMOD::Sound* m_pSelectSound;

	static bool m_IsUsingNavigation;

	std::vector<UIButton*> m_pNavigationButtons;

	SpriteFont* m_pFont{ nullptr };
	std::wstring m_Text{ L"Button Text" };
	DirectX::XMFLOAT2 m_Position{ .0f, .0f };
	DirectX::XMFLOAT4 m_CurrentColor{ Colors::Gray };
	DirectX::XMFLOAT4 m_NormalColor{ Colors::Gray };
	DirectX::XMFLOAT4 m_SelectedColor{ Colors::White };

	// Function that will be called when the button is clicked
	std::function<void()> m_OnClickFunction{ nullptr };

	// Mouse hovering
	bool m_IsSelected{ false };

	// Function that calculates if the mouse is hovering over the button
	void CalculateMouseHovering();
	void UpdateButtonColor();
};
