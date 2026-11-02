#include "InputMgr.h"

#ifdef _USE_IMGUI
#include <Imgui/imgui.h>
#endif

#include <Globals.h>
#include <InputAction.h>
#include <Logger.h>
#include <map>
#include <vector>



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
	for(auto& keyActionPair : registeredKeys)
	{
		sf::Keyboard::Key key = keyActionPair.first;
		std::vector<std::size_t>& actionHashes = keyActionPair.second;
		if (sf::Keyboard::isKeyPressed(key))
		{
			for (std::size_t actionHash : actionHashes)
			{
				auto it = InputActions.find(actionHash);
				if (it != InputActions.end())
				{
					InputAction& action = it->second;
					action.SetValue(1.0f);
					for (auto& callbackInfo : action.Callbacks)
					{
						callbackInfo.Callback(action.Value);
					}
				}
			}
		}
		else
		{
			for (std::size_t actionHash : actionHashes)
			{
				auto it = InputActions.find(actionHash);
				if (it != InputActions.end())
				{
					InputAction& action = it->second;
					action.SetValue(0.0f);
					for (auto& callbackInfo : action.Callbacks)
					{
						callbackInfo.Callback(action.Value);
					}
				}
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
			for(auto& keyActionPair : registeredKeys)
			{
				sf::Keyboard::Key key = keyActionPair.first;
				std::vector<std::size_t>& actionHashes = keyActionPair.second;
				if (std::find(actionHashes.begin(), actionHashes.end(), action.ActionNameHash) != actionHashes.end())
				{
					ImGui::Text("  - %d", static_cast<int>(key));

					ImGui::SameLine();
					field_id = "X##" + std::to_string(static_cast<int>(key)) + "##" + action.ActionName;
					if (ImGui::Button(field_id.c_str()))
					{
						UnbindInputActionFromKey(action.ActionName, key);
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

void InputMgr::BindInputActionToKey(const std::string& actionName, sf::Keyboard::Key key)
{
	registeredKeys[key].emplace_back(std::hash<std::string>{}(actionName));
}

void InputMgr::UnbindInputActionFromKey(const std::string& actionName, sf::Keyboard::Key key)
{
	auto it = registeredKeys.find(key);
	if (it != registeredKeys.end())
	{
		for(auto actionIt = it->second.begin(); actionIt != it->second.end(); )
		{
			if (*actionIt == std::hash<std::string>{}(actionName))
			{
				actionIt = it->second.erase(actionIt);
			}
			else
			{
				++actionIt;
			}
		}

		if(it->second.empty())
		{
			registeredKeys.erase(it);
		}
	}
}