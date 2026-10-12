#pragma once

#include <map>
#include <string>

class Scene;

class SceneMgr
{
public:
	~SceneMgr();

	void Init();
	void Shut();

	std::map<std::string, Scene> scenes;
	Scene* currentScene;

	virtual void Update(float fDeltaTime);
	virtual void Destroy();

	void AddNewScene(std::string name = "DefaultScene");
	void AddExistingScene(Scene* scene);

	void SetCurrentScene(Scene* scene);
	void SetCurrentScene(std::string name);

	void DrawSceneList();

private:
	bool singleOutCurrentScene = true;
};

