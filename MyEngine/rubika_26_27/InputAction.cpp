#include "InputAction.h"

InputAction::InputAction(const std::string& name)
{
	SetActionName(name);
}

void InputAction::SetActionName(const std::string& name)
{
	ActionName = name;
	ActionNameHash = std::hash<std::string>{}(name);
}

void InputAction::SetValue(float value)
{
	if(value != Value)
	{
		Value = value;
		for(auto& callbackInfo : Callbacks)
		{
			if(callbackInfo.Callback)
			{
				callbackInfo.Callback(Value);
			}
		}
	}
}

void InputAction::RegisterCallback(const std::string& debugName, std::function<void(float)> callback, void* owner)
{
	sCallbackInfo info;

	info.DebugName = debugName;
	info.Callback = callback;
	info.Owner = owner;

	Callbacks.push_back(info);
}

void InputAction::UnregisterCallbacks(void* owner)
{
	for(auto it = Callbacks.begin(); it != Callbacks.end(); )
	{
		if(it->Owner == owner)
		{
			it = Callbacks.erase(it);
		}
		else
		{
			++it;
		}
	}
}
