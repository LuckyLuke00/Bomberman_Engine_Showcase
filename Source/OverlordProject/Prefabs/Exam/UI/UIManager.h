#pragma once
#include "Exam/ExamInput.h"

class UIButton;

class UIManager final : public GameObject
{
public:
	UIManager() = default;
	~UIManager() override = default;
	UIManager(UIManager& other) = delete;
	UIManager(UIManager&& other) noexcept = delete;
	UIManager& operator=(UIManager& other) = delete;
	UIManager& operator=(UIManager&& other) noexcept = delete;

	void AddButton(UIButton* pButton);
	void SetFirstSelectedButton(UIButton* pButton);
	void SetSelectedButton(size_t index);
	void SetSelectedButton(UIButton* pButton);

	std::vector<UIButton*>& GetButtons() { return m_pButtons; }

	void EnableInput() const;

protected:
	void Update(const SceneContext&) override;

private:
	bool m_UsingButtonNavigation{ false };
	size_t m_FirstSelectedButtonIndex{ 0 };
	size_t m_SelectedButtonIndex{ 0 };
	std::vector<UIButton*> m_pButtons;

	void UpdateInput();
	int GetActionID(ExamInput::InputActions action, int gamepadIndex) const;
};
