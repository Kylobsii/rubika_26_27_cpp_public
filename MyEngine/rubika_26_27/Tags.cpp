#include "Tags.h"
#include "Globals.h"
#include "SceneMgr.h"
#include "Scene.h"

void Tags::AddTags(int tags)
{
	mask |= tags;

	if ((tags & ENTITY_TAG_DYNAMIC) != 0)
	{
		gData.SceneMgr->currentScene->UpdateEntitySorting(parentEntity, true, false);
	}

	if ((tags & ENTITY_TAG_STATIC) != 0)
	{
		gData.SceneMgr->currentScene->UpdateEntitySorting(parentEntity, false, false);
	}
}

void Tags::RemoveTags(int tags)
{
	mask &= ~tags;

	if ((tags & ENTITY_TAG_DYNAMIC) != 0)
	{
		gData.SceneMgr->currentScene->UpdateEntitySorting(parentEntity, true, true);
	}

	if ((tags & ENTITY_TAG_STATIC) != 0)
	{
		gData.SceneMgr->currentScene->UpdateEntitySorting(parentEntity, false, true);
	}
}

bool Tags::HasAllTags(int tags)
{
	return (mask & tags) == tags;
}

bool Tags::HasAnyTags(int tags)
{
	return (mask & tags) != 0;
}
