#include "stdafx.h"
#include "UIManager.h"
#include "Prefabs/Exam/UI/Buttons/UIButton.h"
#include "Exam/ExamAssets.h"

void UIManager::Update(const SceneContext&)
{
	// If there are no buttons, or none of the buttons are selected, return
	if (m_pButtons.empty()) return;

	// For every button, check if it's selected
	if (!m_UsingButtonNavigation)
	{
		for (size_t i{}; i < m_pButtons.size(); ++i)
		{
			// If the button is selected, break out of the loop
			if (m_pButtons[i]->IsSelected())
			{
				m_SelectedButtonIndex = i;
				break;
			}
		}
	}

	if (m_SelectedButtonIndex >= m_pButtons.size()) return;

	UpdateInput();
}

void UIManager::SetFirstSelectedButton(UIButton* pButton)
{
	// Find the index of the button
	const auto it{ std::ranges::find(m_pButtons, pButton) };
	if (it != m_pButtons.end())
	{
		// Set the index of the first selected button
		m_FirstSelectedButtonIndex = std::distance(m_pButtons.begin(), it);
	}
}

void UIManager::AddButton(UIButton* pButton)
{
	// Add the button to the container, but only if it's not already in there
	if (std::ranges::find(m_pButtons, pButton) == m_pButtons.end())
	{
		m_pButtons.emplace_back(pButton);
	}
}

void UIManager::SetSelectedButton(size_t index)
{
	// If the index is out of range, or the index is the same as the current index, return
	if (index >= m_pButtons.size()) return;

	// Deselect the current button
	m_pButtons[m_SelectedButtonIndex]->Deselect();

	// Set the index of the selected button
	m_SelectedButtonIndex = index;
	m_pButtons[m_SelectedButtonIndex]->Select();
}

void UIManager::SetSelectedButton(UIButton* pButton)
{
	if (!pButton) return;

	// Find the index of the button
	const auto it{ std::ranges::find(m_pButtons, pButton) };
	if (it != m_pButtons.end())
	{
		// De-select the current button
		m_pButtons[m_SelectedButtonIndex]->Deselect();

		// Set the index of the selected button
		m_SelectedButtonIndex = std::distance(m_pButtons.begin(), it);
		m_pButtons[m_SelectedButtonIndex]->Select();
	}
}

void UIManager::EnableInput() const
{
	using namespace ExamInput;
	using enum InputActions;

	// Store the pInput
	InputManager* pInput{ GetScene()->GetSceneContext().pInput };

	//InputAction submit{ InputAction{ static_cast<int>(Submit), InputState::pressed, VK_RETURN, VK_LBUTTON, XINPUT_GAMEPAD_A, GamepadIndex::playerOne } };
	//pInput->AddInputAction(submit);

	//InputAction up{ InputAction{ static_cast<int>(Up), InputState::pressed, 'W', VK_UP, XINPUT_GAMEPAD_DPAD_UP, GamepadIndex::playerOne } };
	//pInput->AddInputAction(up);

	//InputAction down{ InputAction{ static_cast<int>(Down), InputState::pressed, 'S', VK_DOWN, XINPUT_GAMEPAD_DPAD_DOWN, GamepadIndex::playerOne } };
	//pInput->AddInputAction(down);

	//InputAction left{ InputAction{ static_cast<int>(Left), InputState::pressed, 'A', VK_LEFT, XINPUT_GAMEPAD_DPAD_LEFT, GamepadIndex::playerOne } };
	//pInput->AddInputAction(left);

	//InputAction right{ InputAction{ static_cast<int>(Right), InputState::pressed, 'D', VK_RIGHT, XINPUT_GAMEPAD_DPAD_RIGHT, GamepadIndex::playerOne } };
	//pInput->AddInputAction(right);

	for (int i{ 0 }; i < 4; ++i)
	{
		InputAction submit{ InputAction{ GetActionID(Submit, i), InputState::pressed, VK_RETURN, VK_LBUTTON, XINPUT_GAMEPAD_A, static_cast<GamepadIndex>(i) } };
		pInput->AddInputAction(submit);

		InputAction up{ InputAction{ GetActionID(Up, i), InputState::pressed, 'W', VK_UP, XINPUT_GAMEPAD_DPAD_UP, static_cast<GamepadIndex>(i) } };
		pInput->AddInputAction(up);

		InputAction down{ InputAction{ GetActionID(Down, i), InputState::pressed, 'S', VK_DOWN, XINPUT_GAMEPAD_DPAD_DOWN, static_cast<GamepadIndex>(i) } };
		pInput->AddInputAction(down);

		InputAction left{ InputAction{ GetActionID(Left, i), InputState::pressed, 'A', VK_LEFT, XINPUT_GAMEPAD_DPAD_LEFT, static_cast<GamepadIndex>(i) } };
		pInput->AddInputAction(left);

		InputAction right{ InputAction{ GetActionID(Right, i), InputState::pressed, 'D', VK_RIGHT, XINPUT_GAMEPAD_DPAD_RIGHT, static_cast<GamepadIndex>(i) } };
		pInput->AddInputAction(right);
	}
}

void UIManager::UpdateInput()
{
	using namespace ExamInput;
	using enum InputActions;

	const InputManager* pInput{ GetScene()->GetSceneContext().pInput };

	for (int i{ 0 }; i < 4; ++i)
	{
		if (pInput->IsActionTriggered(GetActionID(Submit, i)))
		{
			if (!m_pButtons[m_SelectedButtonIndex]->IsSelected()) return;

			// Prevents the button from being clicked when the mouse is not hovering over the button
			if (InputManager::IsMouseButton(InputState::pressed, VK_LBUTTON) && !m_pButtons[m_SelectedButtonIndex]->IsMouseHovering())
			{
				UIButton::SetIsUsingNavigation(false);
				return;
			}

			m_pButtons[m_SelectedButtonIndex]->OnClick();
			return;
		}

		if (pInput->IsActionTriggered(GetActionID(Up, i)))
		{
			m_UsingButtonNavigation = true;
			if (!m_pButtons[m_SelectedButtonIndex]->IsSelected())
			{
				SetSelectedButton(m_FirstSelectedButtonIndex);
				return;
			}

			SetSelectedButton(m_pButtons[m_SelectedButtonIndex]->GetUpButton());

			return;
		}

		if (pInput->IsActionTriggered(GetActionID(Down, i)))
		{
			m_UsingButtonNavigation = true;
			if (!m_pButtons[m_SelectedButtonIndex]->IsSelected())
			{
				SetSelectedButton(m_FirstSelectedButtonIndex);
				return;
			}

			SetSelectedButton(m_pButtons[m_SelectedButtonIndex]->GetDownButton());

			return;
		}

		if (pInput->IsActionTriggered(GetActionID(Left, i)))
		{
			m_UsingButtonNavigation = true;
			if (!m_pButtons[m_SelectedButtonIndex]->IsSelected())
			{
				SetSelectedButton(m_FirstSelectedButtonIndex);
				return;
			}

			SetSelectedButton(m_pButtons[m_SelectedButtonIndex]->GetLeftButton());

			return;
		}

		if (pInput->IsActionTriggered(GetActionID(Right, i)))
		{
			m_UsingButtonNavigation = true;
			if (!m_pButtons[m_SelectedButtonIndex]->IsSelected())
			{
				SetSelectedButton(m_FirstSelectedButtonIndex);
				return;
			}

			SetSelectedButton(m_pButtons[m_SelectedButtonIndex]->GetRightButton());

			return;
		}
	}
}

int UIManager::GetActionID(ExamInput::InputActions action, int gamepadIndex) const
{
	return static_cast<int>(action) + gamepadIndex * ExamInput::UIActionCount;
}
