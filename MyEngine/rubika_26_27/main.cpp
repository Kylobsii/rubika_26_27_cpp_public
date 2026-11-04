#include <Globals.h>

#include <SFML/Graphics.hpp>
#include <Imgui/imgui.h>
#include <Imgui/imgui-SFML.h>

#include <Profiler.h>
#include <Logger.h>
#include <DebugMgr.h>
#include <SceneMgr.h>
#include <Scene.h>
#include <Entity.h>
#include <TransformComponent.h>
#include <RenderComponent.h>
#include <Sprite.h>
#include <TextureMgr.h>
#include <InputMgr.h>
#include <DynamicColliderComponent.h>
#include <PlayerEntity.h>
#include <ObstacleEntity.h>


int main()
{
    gData.Init();

    sf::RenderWindow window(sf::VideoMode({ 1080, 720 }), "SFML works!", sf::State::Windowed);

    ImGui::SFML::Init(window);

    gData.InputMgr->LoadInputActions();

    const char* texturePath = "../Ressources/IsaacSprite.png";
	const char* textureName = "IsaacSprite";
    gData.TextureMgr->LoadTexture(texturePath);

    Scene* scene = gData.SceneMgr->AddNewScene("Main");
    {
        scene->AddEntity<PlayerEntity>("Player");

        TransformComponent* currObstacleTr;
        currObstacleTr = scene->AddEntity<ObstacleEntity>("Obstacle_0")->GetComponent<TransformComponent>();
        currObstacleTr->SetWorldPosition(sf::Vector2f(212, 100));
        currObstacleTr->SetScale(sf::Vector2f(0.5f, 5));

        currObstacleTr = scene->AddEntity<ObstacleEntity>("Obstacle_1")->GetComponent<TransformComponent>();
        currObstacleTr->SetWorldPosition(sf::Vector2f(100, 212));
        currObstacleTr->SetScale(sf::Vector2f(5, 0.5f));
    }
    gData.SceneMgr->SetCurrentScene("Main");
    gData.SceneMgr->Start();;

    auto toggleDrawCollidersCallback = [](float value)
        {
            gData.DebugMgr->SetDrawColliders(value);
        };
    gData.InputMgr->RegisterInputActionCallback("DebugDrawColliders", "Set Draw Colliders", toggleDrawCollidersCallback, nullptr);

    sf::Clock clock;
    clock.restart();

    while (window.isOpen())
    {
        gData.IncrementFrameCount();

        PROFILER_EVENT_BEGIN(PROFILER_COLOR_BLACK, "Frame %llu", gData.FrameCount);

        int deltaTimeMicroS = clock.getElapsedTime().asMicroseconds();
        float fDeltaTimeMS = (float)deltaTimeMicroS / 1000.f;
        float fDeltaTimeS = fDeltaTimeMS / 1000.f;
        sf::Time imGuiTime = clock.restart();

        PROFILER_EVENT_BEGIN(PROFILER_COLOR_BLUE, "Event & Input");

        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }

#ifdef _USE_IMGUI
            ImGui::SFML::ProcessEvent(window, event.value());
#endif
        }

        PROFILER_EVENT_END();

        PROFILER_EVENT_BEGIN(PROFILER_COLOR_RED, "Update");

		gData.InputMgr->Update(fDeltaTimeS); // Must stay first

		gData.SceneMgr->Update(fDeltaTimeS);

#ifdef _USE_IMGUI
        ImGui::SFML::Update(window, imGuiTime);
#endif

        // sample
        //ImGui::ShowDemoWindow();
        //Logger::DrawLogger();
        gData.DebugMgr->Draw();

        PROFILER_EVENT_END();

        PROFILER_EVENT_BEGIN(PROFILER_COLOR_GREEN, "Draw");
        window.clear();

        if (gData.SceneMgr->currentScene) 
        {
            gData.SceneMgr->currentScene->Draw(window);
        }

#ifdef _USE_IMGUI
        ImGui::SFML::Render(window);
#endif
        window.display();

        PROFILER_EVENT_END();

        PROFILER_EVENT_END();
    }

    ImGui::SFML::Shutdown();

    gData.Shut();
    gData.Destroy();

    return 0;
}