#pragma once
#include <string>

// Holds the path to textures used in the exam
namespace ExamAssets
{
	const std::wstring BombTexture{ L"Textures/Exam/Bomb.png" };
	const std::wstring BombMesh{ L"Meshes/Exam/Bomb.ovm" };

	const std::wstring ExplosionTexture{ L"Textures/Exam/Explosion.png" };
	const std::wstring ExplosionMesh{ L"Meshes/Exam/Explosion.ovm" };

	const std::wstring FloorDarkTexture{ L"Textures/Exam/FloorDark.png" };
	const std::wstring FloorLightTexture{ L"Textures/Exam/FloorLight.png" };
	const std::wstring FloorMesh{ L"Meshes/Exam/UnitTile.ovm" };

	const std::wstring WallTexture{ L"Textures/Exam/Wall.png" };
	const std::wstring WallMesh{ L"Meshes/Exam/UnitBox.ovm" };

	const std::wstring PillarTexture{ L"Textures/Exam/Pillar.png" };
	const std::wstring PillarMesh{ L"Meshes/Exam/UnitBoxRounded.ovm" };

	const std::wstring BreakableBlockTexture{ L"Textures/Exam/BreakableBlock.png" };
	const std::wstring BreakableBlockMesh{ L"Meshes/Exam/BreakableBox.ovm" };

	const std::wstring BackgroundTexture{ L"Textures/Exam/Background.png" };

	const std::wstring PlayerMesh{ L"Meshes/Exam/Player.ovm" };

	const std::wstring BombUpPowerUpTexture{ L"Textures/Exam/BombUpPowerUp.png" };
	const std::wstring BombDownPowerUpTexture{ L"Textures/Exam/BombDownPowerUp.png" };

	const std::wstring FirePowerUpTexture{ L"Textures/Exam/FirePowerUp.png" };
	const std::wstring FireDownPowerUpTexture{ L"Textures/Exam/FireDownPowerUp.png" };

	const std::wstring SkatePowerUpTexture{ L"Textures/Exam/SkatePowerUp.png" };
	const std::wstring GetaPowerUpTexture{ L"Textures/Exam/GetaPowerUp.png" };

	const std::wstring ExplosionParticleTexture{ L"Textures/Exam/ExplosionEffect.png" };

	const std::wstring SmallFont{ L"SpriteFonts/Consolas_32.fnt" };
	const std::wstring MediumFont{ L"SpriteFonts/Consolas_48.fnt" };

	const std::wstring PowerUpMesh{ L"Meshes/Exam/PowerUp.ovm" };

	const std::wstring TextBackground{ L"Textures/Exam/Menus/TextBackground.png" };

	const std::wstring MainMenuBackground{ L"Textures/Exam/Menus/MainMenu_Background_new.png" };
	const std::wstring PauseMenuBackground{ L"Textures/Exam/Menus/PauseMenu_Background.png" };
	const std::wstring EndScreenBackground{ L"Textures/Exam/Menus/EndScreen_Background.png" };

	const std::string ExplosionSound01{ "Resources/Audio/Explosion_01.wav" };
	const std::string ExplosionSound02{ "Resources/Audio/Explosion_02.wav" };
	const std::string ExplosionSound03{ "Resources/Audio/Explosion_03.wav" };

	const std::string PowerUpSpawnSound{ "Resources/Audio/PowerUp_Spawn.wav" };
	const std::string PowerUpPickupSound{ "Resources/Audio/PowerUp_Pickup.wav" };

	const std::string BombPlaceSound{ "Resources/Audio/BombPlace.wav" };

	const std::string SpawnSound{ "Resources/Audio/Spawn.wav" };

	const std::string SubmitSound{ "Resources/Audio/Submit.wav" };
	const std::string SelectSound{ "Resources/Audio/Select.wav" };

	const std::string DeathSound{ "Resources/Audio/Death.wav" };

	const std::string BattleStartSound{ "Resources/Audio/BattleStart.flac" };
	const std::string BattleMusic{ "Resources/Audio/BattleMusic.flac" };
	const std::string BattleWinSound{ "Resources/Audio/BattleWin.flac" };
	const std::string MainMenuMusic{ "Resources/Audio/MainMenuMusic.flac" };
	const std::string EndScreenMusic{ "Resources/Audio/EndScreenMusic.flac" };
}
