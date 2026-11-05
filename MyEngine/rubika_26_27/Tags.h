#pragma once

#define ENTITY_TAG_NONE			0
#define ENTITY_TAG_STATIC		1 << 0
#define ENTITY_TAG_DYNAMIC		1 << 1
#define ENTITY_TAG_NEUTRAL		1 << 2
#define ENTITY_TAG_ENEMY		1 << 3
#define ENTITY_TAG_PLAYER		1 << 4

class Entity;

class Tags
{
public: 

	Tags() : parentEntity(nullptr) {}
	Tags(Entity* parent) : parentEntity(parent) {}

	int mask;

	Entity* parentEntity;

	bool HasAnyTags(int tags);
	bool HasAllTags(int tags);

	Tags operator=(const int tags)
	{
		mask = ENTITY_TAG_NONE;
		AddTags(tags);

		return *this;
	}

	Tags operator+=(const int tags)
	{
		AddTags(tags);

		return *this;
	}

	Tags operator-=(const int tags)
	{
		RemoveTags(tags);

		return *this;
	}

private:
	void AddTags(int tags); 
	void RemoveTags(int tags);
};