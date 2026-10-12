#include "Logger.h"
#include "Scene.h"
#include "Entity.h"

#ifdef _USE_IMGUI
#include <Imgui/imgui.h>
#endif
#include <Globals.h>

#include "SceneMgr.h"


SceneMgr::~SceneMgr()
{
	Destroy();
}

void Logger::Init()
{
	gData.DebugMgr->RegisterDebugableWindow("Scene List", this);
}

void Logger::Shut()
{
	gData.DebugMgr->UnregisterDebugableWindow("Scene List");
}

void SceneMgr::Update(float fDeltaTime)
{
	currentScene->Update(fDeltaTime);
}

void SceneMgr::Destroy()
{
	scenes.clear();
	currentScene = nullptr;
}

void SceneMgr::AddNewScene(std::string name)
{
	if(scenes.find(name) != scenes.end())
	{
		Logger::Warning("Scene with name '" + name + "' already exists. Creation was cancelled !");
		return;
	}

	scenes.emplace(name, Scene(name));
}

void SceneMgr::AddExistingScene(Scene* scene)
{
	if (scenes.find(scene->name) != scenes.end())
	{
		Logger::Warning("Scene with name '" + scene->name + "' already exists. Creation was cancelled !");
		return;
	}

	scenes.emplace(scene->name, *scene);
}

void SceneMgr::SetCurrentScene(Scene* scene)
{
	if (scenes.find(scene->name) != scenes.end())
	{
		// Switch to the new scene

		currentScene = scene;
		currentScene->Start();
	}
	else 
	{
		Logger::Warning("Scene with name '" + scene->name + "' isn't registered in the Scene manager. Register it before trying to switch. Switch was cancelled !");
	}
}

void SceneMgr::SetCurrentScene(std::string name)
{
	auto it = scenes.find(name);
	if (it != scenes.end())
	{
		// Switch to the new scene

		currentScene = &it->second;
		currentScene->Start();
	}
	else
	{
		Logger::Warning("Scene with name '" + name + "' isn't registered in the Scene manager. Register it before trying to switch. Switch was cancelled !");
	}
}

void SceneMgr::DrawSceneList()
{
#ifdef _USE_IMGUI
	if (ImGui::Button("Create New Scene"))
	{
	}
	ImGui::SameLine();

	if (ImGui::Button("Delete Scene"))
	{
	}
	ImGui::SameLine();

	if (ImGui::Button("Show All Details"))
	{
		// expand all drop downs
	}
	ImGui::SameLine();

	if (ImGui::Button("Hide All Details"))
	{
		// collapse all drop downs
	}

	if (ImGui::BeginCombo("Scene Name", currentScene->name.c_str())) 
	{
		for (int n = 0; n < currentScene->entities.size(); n++) 
		{
			const bool is_selected = n == 0;

			// Render each selectable item in the loop
			if (ImGui::Selectable(currentScene->entities[n]->name.c_str(), is_selected)) 
			{
			}

			// Set the initial focus when opening the combo (scrollbar positioning)
			if (is_selected) 
			{
				ImGui::SetItemDefaultFocus();
			}
		}
		ImGui::EndCombo(); // Must always call EndCombo if BeginCombo returns true
	}
#endif
}
