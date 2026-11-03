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


int main()
{
    gData.Init();

    sf::RenderWindow window(sf::VideoMode({ 1080, 720 }), "SFML works!", sf::State::Windowed);

    ImGui::SFML::Init(window);

    gData.InputMgr->LoadInputActions();

    const char* texturePath = "../Ressources/IsaacSprite.png";
	const char* textureName = "IsaacSprite";
    gData.TextureMgr->LoadTexture(texturePath);

    Entity* entity;
    Scene* scene = gData.SceneMgr->AddNewScene("Main");
    {
        entity = scene->AddEntity("MainEntity");
        TransformComponent* tc = entity->AddComponent<TransformComponent>();
        {
            tc->SetWorldPosition(sf::Vector2f(100.f, 100.f));
        }

        RenderComponent* rc = entity->AddComponent<RenderComponent>();
        {
            Sprite* bodySprite = rc->AddNewDrawable<Sprite>("Body", sf::Vector2f(2, 0), 0, sf::Vector2f(1, 1));
            bodySprite->SetVisibility(true);
            bodySprite->SetTexture(textureName);
            bodySprite->SetAnimation("Body_Vertical");

            Sprite* headSprite = rc->AddNewDrawable<Sprite>("Head", sf::Vector2f(-2.5f, -20), 0, sf::Vector2f(1, 1));
            headSprite->SetVisibility(true);
            headSprite->SetTexture(textureName);
            headSprite->SetAnimation("Head_Down");
        }

        DynamicColliderComponent* dcc = entity->AddComponent<DynamicColliderComponent>();
        {
            dcc->SetHitboxSize(sf::Vector2f(25, 30));
            dcc->SetHitboxOffset(sf::Vector2f());
        }
    }
    gData.SceneMgr->SetCurrentScene("Main");
    gData.SceneMgr->Start();;

    auto moveUpCallback = [entity](float value)
        {
			entity->GetComponent<TransformComponent>()->SetMovementDirection(sf::Vector2f(0, -1), value != 0);
        };
    gData.InputMgr->RegisterInputActionCallback("MoveUp", "Up Movement", moveUpCallback, nullptr);

    auto moveDownCallback = [entity](float value)
        {
            entity->GetComponent<TransformComponent>()->SetMovementDirection(sf::Vector2f(0, 1), value != 0);
        };
    gData.InputMgr->RegisterInputActionCallback("MoveDown", "Down Movement", moveDownCallback, nullptr);

    auto moveLeftCallback = [entity](float value)
        {
            entity->GetComponent<TransformComponent>()->SetMovementDirection(sf::Vector2f(-1, 0), value != 0);
        };
    gData.InputMgr->RegisterInputActionCallback("MoveLeft", "Left Movement", moveLeftCallback, nullptr);

    auto moveRightCallback = [entity](float value)
        {
            entity->GetComponent<TransformComponent>()->SetMovementDirection(sf::Vector2f(1, 0), value != 0);
        };
    gData.InputMgr->RegisterInputActionCallback("MoveRight", "Right Movement", moveRightCallback, nullptr);

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