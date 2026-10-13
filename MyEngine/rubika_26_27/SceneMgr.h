#pragma once

#include <map>
#include <string>

class Scene;

class SceneMgr : IDebugable
{
public:
	SceneMgr();
	~SceneMgr();

	void Init();
	void Shut();

	std::map<std::string, Scene> scenes;
	Scene* currentScene;

	virtual void Update(float fDeltaTime);
	virtual void Destroy();

	void AddNewScene(std::string name = "DefaultScene");
	void AddExistingScene(Scene* scene);

	void DeleteScene(std::string name);

	void SetCurrentScene(Scene* scene);
	void SetCurrentScene(std::string name);

	virtual void DrawDebug() override;

private:
	char sceneNameBuffer[128];
	std::string newSceneName;
};

