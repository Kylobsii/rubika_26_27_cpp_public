#include "InputMgr.h"

#ifdef _USE_IMGUI
#include <Imgui/imgui.h>
#endif

#include <Globals.h>
#include <InputAction.h>
#include <Logger.h>
#include <map>
#include <vector>

#include <rapidxml/rapidxml.hpp>
#include <rapidxml/rapidxml_utils.hpp>
#include <rapidxml/rapidxml_print.hpp>
#include <filesystem>
#include <fstream>

using namespace rapidxml;



InputMgr::InputMgr()
{
	actionNameBuffer[0] = '\0';
}

InputMgr::~InputMgr()
{
	Destroy();
}

void InputMgr::Init()
{
	gData.DebugMgr->RegisterDebugableWindow("Inputs", this);
}

void InputMgr::Shut()
{
	gData.DebugMgr->UnregisterDebugableWindow("Inputs");
}

void InputMgr::Start()
{
}

void InputMgr::Update(float fDeltaTime)
{
	for(auto& actionKeyPair : registeredKeys)
	{
		std::size_t actionHash = actionKeyPair.first;
		std::vector<sf::Keyboard::Key>& keys = actionKeyPair.second;
		bool isPressed = false;

		for (sf::Keyboard::Key key : keys)
		{
			if (sf::Keyboard::isKeyPressed(key))
			{
				isPressed = true;
			}
		}

		if(isPressed)
		{
			const auto& it = InputActions.find(actionHash);
			if (it != InputActions.end())
			{
				InputAction& action = it->second;
				action.SetValue(1.0f);
			}
		}
		else
		{
			const auto& it = InputActions.find(actionHash);
			if (it != InputActions.end())
			{
				InputAction& action = it->second;
				action.SetValue(0.0f);
			}
		}
	}
}

void InputMgr::Destroy()
{
}

void InputMgr::DrawDebug()
{
#ifdef _USE_IMGUI
	ImGui::SetNextItemWidth(200.0f);
	ImGui::InputText("Action Name", actionNameBuffer, IM_ARRAYSIZE(actionNameBuffer));

	ImGui::SameLine();
	if (ImGui::Button("Create New Action"))
	{
		if(actionNameBuffer[0] == '\0')
		{
			Logger::Warning("Input action name is empty. Creation was cancelled !");
			return;
		}

		newActionName = actionNameBuffer;
		CreateInputAction(newActionName);

		actionNameBuffer[0] = '\0';
	}

	ImGui::Separator();

	if (ImGui::Button("Expand All Actions"))
	{
		for (auto& actionPair : InputActions)
		{
			ImGuiID header_id = ImGui::GetID(actionPair.second.ActionName.c_str());
			ImGuiStorage* storage = ImGui::GetStateStorage();
			storage->SetInt(header_id, 1);
		}
	}
	ImGui::SameLine();

	if (ImGui::Button("Collapse All Actions"))
	{
		for (auto& actionPair : InputActions)
		{
			ImGuiID header_id = ImGui::GetID(actionPair.second.ActionName.c_str());
			ImGuiStorage* storage = ImGui::GetStateStorage();
			storage->SetInt(header_id, 0);
		}
	}

	ImGui::Separator();
	ImGui::Dummy(ImVec2(0.0f, 5.0f));

	if (InputActions.empty())
	{
		ImGui::Text("The input actions list is empty.");
		return;
	}

	for (auto& actionPair : InputActions)
	{
		InputAction& action = actionPair.second;
		if (ImGui::CollapsingHeader(action.ActionName.c_str()))
		{
			std::string field_id = "Delete Action##" + action.ActionName;
			if (ImGui::Button(field_id.c_str()))
			{
				RemoveInputAction(action.ActionName);
				return;
			}

			ImGui::Separator();

			ImGui::Text("Value: %.2f", action.Value);

			ImGui::Text("Binded Keys: ");
			std::size_t actionHash = action.ActionNameHash;
			if(registeredKeys.find(actionHash) != registeredKeys.end())
			{
				std::vector<sf::Keyboard::Key>& keys = registeredKeys[actionHash];
				for (sf::Keyboard::Key key : keys)
				{
					ImGui::Text("  - %d", static_cast<int>(key));
					ImGui::SameLine();
					field_id = "X##" + std::to_string(static_cast<int>(key)) + "##" + action.ActionName;
					if (ImGui::Button(field_id.c_str()))
					{
						UnbindKeyFromInputAction(action.ActionName, key);
						return;
					}
				}
			}
			
			ImGui::Text("Number of Callbacks: %d", static_cast<int>(action.Callbacks.size()));

			ImGui::Separator();

			// Group callbacks by owner
			std::map<void*, std::vector<InputAction::sCallbackInfo*>> callbacksByOwner;
			for (auto& info : action.Callbacks)
			{
				callbacksByOwner[info.Owner].push_back(&info);
			}

			for (auto& ownerPair : callbacksByOwner)
			{
				field_id = "Owner: " + std::string(ownerPair.first == nullptr ? "nullptr" : std::to_string(reinterpret_cast<uintptr_t>(ownerPair.first))) + "##" + action.ActionName;
				if (ImGui::TreeNode(field_id.c_str()))
				{
					for (auto& callbackInfo : ownerPair.second)
					{
						ImGui::BulletText(callbackInfo->DebugName.c_str());

						std::string delete_callback_id = "Delete Callback##" + callbackInfo->DebugName;
						if (ImGui::Button(delete_callback_id.c_str()))
						{
							UnregisterInputActionCallbacks(action.ActionName, ownerPair.first);
						}
						ImGui::SameLine();
					}
					ImGui::TreePop();
				}
			}
			ImGui::Dummy(ImVec2(0.0f, 5.0f));
		}
	}

	ImGui::Dummy(ImVec2(0.0f, 5.0f));
	ImGui::Separator();

	if (ImGui::Button("Save actions to default InputActions File"))
	{
		SaveActionsToFile(actionsFilePath);
	}

#endif
}

void InputMgr::CreateInputAction(const std::string& actionName)
{
	size_t actionHash = std::hash<std::string>{}(actionName);

	auto it = InputActions.find(actionHash);
	if (it == InputActions.end())
	{
		InputAction* action = new InputAction(actionName);

		InputActions[actionHash] = *action;
	}
	else
	{
		Logger::Warning("Input action with name '" + actionName + "' already exists. Creation was cancelled !");
	}
}

void InputMgr::RemoveInputAction(const std::string& actionName)
{
	size_t actionHash = std::hash<std::string>{}(actionName);
	auto it = InputActions.find(actionHash);
	if (it != InputActions.end())
	{
		InputActions.erase(it);
	}
	else
	{
		Logger::Warning("Input action with name '" + actionName + "' doesn't exist. Removal was cancelled !");
	}
}

void InputMgr::RegisterInputActionCallback(const std::string& actionName, const std::string& debugName, std::function<void(float)> callback, void* owner)
{
	size_t actionHash = std::hash<std::string>{}(actionName);
	auto it = InputActions.find(actionHash);
	if (it != InputActions.end())
	{
		it->second.RegisterCallback(debugName, callback, owner);
	}
	else
	{
		Logger::Warning("Input action with name '" + actionName + "' doesn't exist. Callback registration was cancelled !");
	}
}

void InputMgr::UnregisterInputActionCallbacks(const std::string& actionName, void* owner)
{
	size_t actionHash = std::hash<std::string>{}(actionName);
	auto it = InputActions.find(actionHash);
	if (it != InputActions.end())
	{
		it->second.UnregisterCallbacks(owner);
	}
	else
	{
		Logger::Warning("Input action with name '" + actionName + "' doesn't exist. Callback unregistration was cancelled !");
	}
}

void InputMgr::BindKeyToInputAction(const std::string& actionName, sf::Keyboard::Key key)
{
	size_t hashName = std::hash<std::string>{}(actionName);
	registeredKeys[hashName].emplace_back(key);
}

void InputMgr::UnbindKeyFromInputAction(const std::string& actionName, sf::Keyboard::Key key)
{
	size_t hashName = std::hash<std::string>{}(actionName);

	auto it = registeredKeys.find(hashName);
	if (it != registeredKeys.end())
	{
		for(auto keyIt = it->second.begin(); keyIt != it->second.end(); )
		{
			if (*keyIt == key)
			{
				keyIt = it->second.erase(keyIt);
			}
			else
			{
				++keyIt;
			}
		}

		if(it->second.empty())
		{
			registeredKeys.erase(it);
		}
	}
}

void InputMgr::SaveActionsToFile(const std::string& filePath)
{
	if (std::filesystem::exists(filePath))
	{
		std::filesystem::remove(filePath);
	}

	xml_document<> doc;
	xml_node<>* root = doc.allocate_node(node_element, "InputActions");

	for (const auto& pair : InputActions)
	{
		xml_node<>* actionNode = doc.allocate_node(node_element, "InputAction");
		actionNode->append_attribute(doc.allocate_attribute("name", pair.second.ActionName.c_str()));

		size_t actionHashName = std::hash<std::string>{}(pair.second.ActionName);
		if(registeredKeys.find(actionHashName) != registeredKeys.end())
		{
			xml_node<>* keysNode = doc.allocate_node(node_element, "Keys");
			for (auto key : registeredKeys[actionHashName])
			{
				xml_node<>* keyNode = doc.allocate_node(node_element, "Key");
				char* attribute = doc.allocate_string(std::to_string(static_cast<int>(key)).c_str());
				keyNode->append_attribute(doc.allocate_attribute("code", attribute));
				keysNode->append_node(keyNode);
			}
			actionNode->append_node(keysNode);
		}

		root->append_node(actionNode);
	}
	doc.append_node(root);

	std::ofstream file(filePath);
	if (file.is_open())
	{
		file << doc;
		file.close();
	}
	else
	{
		Logger::Error("Failed to open file for saving input actions: " + filePath);
	}
}

void InputMgr::LoadInputActions()
{
	LoadActionsFromFile(actionsFilePath);
}

void InputMgr::LoadActionsFromFile(const std::string& filePath)
{
	if (std::filesystem::exists(filePath))
	{
		file<> xmlFile(filePath.c_str());
		xml_document<> doc;
		doc.parse<0>(xmlFile.data());

		std::string rootName = std::string(doc.first_node()->name(), doc.first_node()->name_size());
		xml_node<>* root = doc.first_node("InputActions");

		if (rootName == "InputActions")
		{
			xml_node<>* currNode = root->first_node("InputAction");

			if (currNode == nullptr)
			{
				Logger::Error("Failed to load default InputActions, '../Ressources/InputActions.xml' doesn't contain any InputAction node. Please reconfigure the input actions by hand, and export the new configuration from the debug window.");
			}

			std::string currActionName = "";
			size_t currActionHash;
			while (currNode && std::string(currNode->name(), currNode->name_size()) == "InputAction")
			{
				currActionName = currNode->first_attribute()->value();
				Logger::Info("Setting InputAction: " + currActionName);

				CreateInputAction(currActionName);

				xml_node<>* keysNode = currNode->first_node("Keys");
				if (keysNode)
				{
					keysNode = keysNode->first_node("Key");

					while (keysNode && std::string(keysNode->name(), keysNode->name_size()) == "Key")
					{
						int keyCode = std::stoi(keysNode->first_attribute("code")->value());

						Logger::Info(std::format("Adding binding: {}", keyCode));
						BindKeyToInputAction(currActionName, sf::Keyboard::Key(keyCode));

						keysNode = keysNode->next_sibling();
					}
				}

				currNode = currNode->next_sibling();
			}

			Logger::Info("Finished setting InputActions.");
		}
		else
		{
			Logger::Error("Failed to load default InputActions, '../Ressources/InputActions.xml' isn't formatted correctly. Please reconfigure the input actions by hand, and export the new configuration from the debug window.");
		}
	}
	else
	{
		Logger::Error("Failed to load default InputActions, '../Ressources/InputActions.xml' is missing.");
	}
}
