#include "stdafx.h"
#include "MainGame.h"

/*LAB Content*/
//#define W3
//#define W4
//#define W5
//#define W6
//#define W7
//#define W8
//#define W9
//#define W10

/*MILESTONE Content*/
//#define MILESTONE_1
//#define MILESTONE_2

/*EXAM Content*/
#define EXAM

#pragma region Lab/Milestone Includes
#ifdef W3
#include "Scenes/Week 3/MinionScene.h"
#include "Scenes/Week 3/ComponentTestScene.h"
#include "Scenes/Week 3/PongScene.h"
#endif

#ifdef W4
#include "Scenes/Week 4/ModelTestScene.h"
#include "Scenes/Week 4/UberMaterialScene.h"
#include "Scenes/Week 4/SpikyScene.h"
#include "Scenes/Week 4/SpriteTestScene.h"
#endif

#ifdef W5
#include "Scenes/Week 5/SoftwareSkinningScene_1.h"
#include "Scenes/Week 5/SoftwareSkinningScene_2.h"
#include "Scenes/Week 5/SoftwareSkinningScene_3.h"
#endif

#ifdef W6
#include "Scenes/Week 6/HardwareSkinningScene.h"
#endif

#ifdef W7
#include "Scenes/Week 7/FontTestScene.h"
#include "Scenes/Week 7/PickingScene.h"
#include "Scenes/Week 7/CharacterScene.h"
#endif

#ifdef W8
#include "Scenes/Week 8/ShadowMappingScene.h"
#endif

#ifdef W9
#include "Scenes/Week 9/ParticleScene.h"
#endif

#ifdef W10
#include "Scenes/Week 10/PostProcessingScene.h"
#endif

#ifdef MILESTONE_1
#include "Scenes/Week 3/PongScene.h"
#include "Scenes/Week 4/ModelTestScene.h"
#include "Scenes/Week 4/UberMaterialScene.h"
#include "Scenes/Week 4/SpikyScene.h"
#include "Scenes/Week 4/SpriteTestScene.h"
#include "Scenes/Week 5/SoftwareSkinningScene_3.h"
#include "Scenes/Week 6/HardwareSkinningScene.h"
#endif

#ifdef MILESTONE_2
#include "Scenes/Week 7/FontTestScene.h"
#include "Scenes/Week 7/CharacterScene.h"
#include "Scenes/Week 7/PickingScene.h"
#include "Scenes/Week 8/ShadowMappingScene.h"
#include "Scenes/Week 9/ParticleScene.h"
#include "Scenes/Week 10/PostProcessingScene.h"
#endif

#ifdef EXAM
#include "Scenes/Exam/MainMenuScene.h"
#include "Scenes/Exam/LevelScene.h"
#include "Scenes/Exam/EndScene.h"
#endif

#pragma endregion

//Game is preparing
void MainGame::OnGamePreparing(GameContext& gameContext)
{
	//Here you can change some game settings before engine initialize
	//gameContext.windowWidth=... (default is 1280)
	//gameContext.windowHeight=... (default is 720)

	//gameContext.windowTitle = L"GP2 - Milestone 1 (2023) | (2DAE15) Kinoo Lucas";
	//gameContext.windowTitle = L"GP2 - Milestone 2 (2023) | (2DAE15) Kinoo Lucas";
	gameContext.windowTitle = L"GP2 - Exam Project (2023) | (2DAE15) Kinoo Lucas";
}

void MainGame::Initialize()
{
#ifdef W3
	SceneManager::Get()->AddGameScene(new MinionScene());
	SceneManager::Get()->AddGameScene(new ComponentTestScene());
	SceneManager::Get()->AddGameScene(new PongScene());
#endif

#ifdef W4
	SceneManager::Get()->AddGameScene(new ModelTestScene());
	SceneManager::Get()->AddGameScene(new UberMaterialScene());
	SceneManager::Get()->AddGameScene(new SpikyScene());
	SceneManager::Get()->AddGameScene(new SpriteTestScene());
#endif

#ifdef W5
	SceneManager::Get()->AddGameScene(new SoftwareSkinningScene_1());
	SceneManager::Get()->AddGameScene(new SoftwareSkinningScene_2());
	SceneManager::Get()->AddGameScene(new SoftwareSkinningScene_3());
#endif

#ifdef W6
	SceneManager::Get()->AddGameScene(new HardwareSkinningScene());
#endif

#ifdef W7
	SceneManager::Get()->AddGameScene(new FontTestScene());
	SceneManager::Get()->AddGameScene(new PickingScene());
	SceneManager::Get()->AddGameScene(new CharacterScene());
#endif

#ifdef W8
	SceneManager::Get()->AddGameScene(new ShadowMappingScene());
#endif

#ifdef W9
	SceneManager::Get()->AddGameScene(new ParticleScene());
#endif

#ifdef W10
	SceneManager::Get()->AddGameScene(new PostProcessingScene());
#endif

#ifdef MILESTONE_1
	SceneManager::Get()->AddGameScene(new PongScene());
	SceneManager::Get()->AddGameScene(new ModelTestScene());
	SceneManager::Get()->AddGameScene(new UberMaterialScene());
	SceneManager::Get()->AddGameScene(new SpikyScene());
	SceneManager::Get()->AddGameScene(new SpriteTestScene());
	SceneManager::Get()->AddGameScene(new SoftwareSkinningScene_3());
	SceneManager::Get()->AddGameScene(new HardwareSkinningScene());
#endif

#ifdef MILESTONE_2
	SceneManager::Get()->AddGameScene(new FontTestScene());
	SceneManager::Get()->AddGameScene(new CharacterScene());
	SceneManager::Get()->AddGameScene(new PickingScene());
	SceneManager::Get()->AddGameScene(new ShadowMappingScene());
	SceneManager::Get()->AddGameScene(new ParticleScene());
	SceneManager::Get()->AddGameScene(new PostProcessingScene());
#endif

#ifdef EXAM
	SceneManager::Get()->AddGameScene(new MainMenuScene());
	SceneManager::Get()->AddGameScene(new LevelScene());
	SceneManager::Get()->AddGameScene(new EndScene());
#endif
}

LRESULT MainGame::WindowProcedureHook(HWND /*hWnd*/, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (message == WM_KEYUP)
	{
		if ((lParam & 0x80000000) != 0x80000000)
			return -1;

		//[F1] Toggle Scene Info Overlay
		if (wParam == VK_F1)
		{
			const auto pScene = SceneManager::Get()->GetActiveScene();
			pScene->GetSceneSettings().Toggle_ShowInfoOverlay();
		}

		//[F2] Toggle Debug Renderer (Global)
		if (wParam == VK_F2)
		{
			DebugRenderer::ToggleDebugRenderer();
			return 0;
		}

		//[F3] Previous Scene
		if (wParam == VK_F3)
		{
			SceneManager::Get()->PreviousScene();
			return 0;
		}

		//[F4] Next Scene
		if (wParam == VK_F4)
		{
			SceneManager::Get()->NextScene();
			return 0;
		}

		//[F5] If PhysX Framestepping is enables > Next Frame
		if (wParam == VK_F6)
		{
			const auto pScene = SceneManager::Get()->GetActiveScene();
			pScene->GetPhysxProxy()->NextPhysXFrame();
		}
	}

	return -1;
}
