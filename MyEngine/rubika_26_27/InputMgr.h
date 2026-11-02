#pragma once

#include "DebugMgr.h"

#include <unordered_map>
#include <string>
#include <functional>
#include <SFML/Window/Keyboard.hpp>


class InputAction;

class InputMgr : public IDebugable
{
public:
	InputMgr();
	~InputMgr();

	void Init();
	void Shut();

	virtual void Start();
	virtual void Update(float fDeltaTime);
	virtual void Destroy();

	virtual void DrawDebug() override;

	void CreateInputAction(const std::string& actionName);
	void RemoveInputAction(const std::string& actionName);

	void RegisterInputActionCallback(const std::string& actionName, const std::string& debugName, std::function<void(float)> callback, void* owner);
	void UnregisterInputActionCallbacks(const std::string& actionName, void* owner);

	void BindInputActionToKey(const std::string& actionName, sf::Keyboard::Key key);
	void UnbindInputActionFromKey(const std::string& actionName, sf::Keyboard::Key key);

protected:
	std::unordered_map<std::size_t, InputAction> InputActions;
	std::unordered_map<sf::Keyboard::Key, std::vector<std::size_t>> registeredKeys;

private:
	char actionNameBuffer[128];
	std::string newActionName;
};

