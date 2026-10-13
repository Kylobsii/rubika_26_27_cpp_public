#include "Logger.h"
#include "Scene.h"
#include "Entity.h"

#ifdef _USE_IMGUI
#include <Imgui/imgui.h>
#endif
#include <Globals.h>

#include "SceneMgr.h"


SceneMgr::SceneMgr()
{
	scenes = std::map<std::string, Scene>();
	currentScene = nullptr;

	sceneNameBuffer[0] = '\0';
}

SceneMgr::~SceneMgr()
{
	Destroy();
}

void SceneMgr::Init()
{
	gData.DebugMgr->RegisterDebugableWindow("Scene List", this);
}

void SceneMgr::Shut()
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

	// returns a pair <pair <key, val>, bool success>
	auto it = scenes.emplace(name, Scene(name));

	if (currentScene == nullptr && it.second) // second = bool success
	{ 
		currentScene = &it.first->second; // first = pair <key, val>, first->second = val = Scene
	}
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

void SceneMgr::DeleteScene(std::string name)
{
	if (scenes.find(name) == scenes.end())
	{
		Logger::Warning("Scene with name '" + name + "' doesn't exist. Deletion was cancelled !");
		return;
	}
	if (currentScene != nullptr && currentScene->name == name)
	{
		currentScene = nullptr;
	}
	scenes.erase(name);
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

void SceneMgr::DrawDebug()
{
#ifdef _USE_IMGUI
	ImGui::SetNextItemWidth(200.0f);
	ImGui::InputText("Scene Name", sceneNameBuffer, IM_ARRAYSIZE(sceneNameBuffer));

	ImGui::SameLine();
	if (ImGui::Button("Create New Scene"))
	{
		newSceneName = sceneNameBuffer;
		AddNewScene(newSceneName);

		sceneNameBuffer[0] = '\0';
	}

	ImGui::Separator();

	if (ImGui::Button("Expand All Scenes"))
	{
		for (auto& scenePair : scenes)
		{
			ImGuiID header_id = ImGui::GetID(scenePair.first.c_str());
			ImGuiStorage* storage = ImGui::GetStateStorage();
			storage->SetInt(header_id, 1);
		}
	}
	ImGui::SameLine();

	if (ImGui::Button("Collapse All Scenes"))
	{
		for (auto& scenePair : scenes)
		{
			ImGuiID header_id = ImGui::GetID(scenePair.first.c_str());
			ImGuiStorage* storage = ImGui::GetStateStorage();
			storage->SetInt(header_id, 0);
		}
	}

	ImGui::Separator();
	ImGui::Dummy(ImVec2(0.0f, 5.0f));

	if(scenes.empty())
	{
		ImGui::Text("The scene list is empty.");
		return;
	}

	for (auto& scenePair : scenes)
	{
		Scene& scene = scenePair.second;
		if (ImGui::CollapsingHeader(scene.name.c_str()))
		{
			std::string field_id = "Set as active##" + scene.name;
			if (ImGui::Button(field_id.c_str()))
			{
				if (&scene == nullptr)
				{
					Logger::Warning("Scene with name '" + std::string(sceneNameBuffer) + "' isn't registered in the Scene manager. Register it before trying to switch. Switch was cancelled !");
				}
				else
				{
					currentScene = &scene;
					currentScene->Start();
				}
			}
			ImGui::SameLine();

			field_id = "Delete Scene##" + scene.name;
			if (ImGui::Button(field_id.c_str()))
			{
				DeleteScene(scene.name);
				return;
			}

			ImGui::Separator();

			ImGui::Text("Active : %s", currentScene == &scene ? "Yes" : "No");
			ImGui::Text("Number of Entities: %d", static_cast<int>(scene.entities.size()));

			ImGui::SetNextItemWidth(200.0f);
			field_id = "Entity Name##" + scene.name;
			ImGui::InputText(field_id.c_str(), scene.entityNameBuffer, IM_ARRAYSIZE(scene.entityNameBuffer));

			ImGui::SameLine();

			field_id = "Create New Entity##" + scene.name;
			if (ImGui::Button(field_id.c_str()))
			{
				scene.newEntityName = std::string(scene.entityNameBuffer) + "##" + std::to_string(scene.GetEntityID());
				scene.AddEntity(scene.newEntityName);

				scene.entityNameBuffer[0] = '\0';
			}

			ImGui::Separator();

			for (Entity* entity : scene.entities)
			{
				if (ImGui::TreeNode(entity->name.c_str()))
				{
					ImGui::Text("Entity Name: %s", entity->name.c_str());
					ImGui::Text("Position: (%.2f, %.2f)", entity->transform.GetWorldPosition().x, entity->transform.GetWorldPosition().y);
					ImGui::Text("Rotation: %.2f degrees", entity->transform.GetRotation());
					ImGui::Text("Scale: (%.2f, %.2f)", entity->transform.GetScale().x, entity->transform.GetScale().y);
					
					field_id = "Delete Entity##" + entity->name;
					if (ImGui::Button(field_id.c_str()))
					{
						scene.RemoveEntity(entity);
					}
					
					ImGui::TreePop();
				}
			}
			ImGui::Dummy(ImVec2(0.0f, 5.0f));
		}
	}
#endif
}
