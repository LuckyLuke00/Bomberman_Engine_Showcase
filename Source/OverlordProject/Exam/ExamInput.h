#pragma once
namespace ExamInput
{
	enum class InputActions
	{
		// UI Actions
		Up,
		Down,
		Left,
		Right,
		Submit,

		// Player Actions
		MoveNorth,
		MoveEast,
		MoveSouth,
		MoveWest,
		PlaceBomb,
		Pause,
	};

	constexpr int PlayerActionCount{ 6 };
	constexpr int UIActionCount{ 5 };
}
