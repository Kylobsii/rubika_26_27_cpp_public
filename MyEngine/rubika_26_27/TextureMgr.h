#pragma once

#include <SFML/Graphics/Texture.hpp>
#include <rapidxml/rapidxml.hpp>
#include <rapidxml/rapidxml_utils.hpp>

#include <map>
#include <filesystem>
#include <string>

using namespace rapidxml;

struct AnimationData
{
	int StartX = 0;
	int StartY = 0;
	int SizeX = 0;
	int SizeY = 0;
	int OffsetX = 0;
	int OffsetY = 0;
	int AnimationSpriteCount = 0;
	int SpriteOnLine = 0;
	bool IsReverted = false;
	float TimeBetweenAnimationInS = 0.f;
};

struct StaticTileData
{
	int StartX = 0;
	int StartY = 0;
	int SizeX = 0;
	int SizeY = 0;
	bool IsRevertedX = false;
	bool IsRevertedY = false;
};

struct TextureData
{
	TextureData();
	~TextureData();

	sf::Texture Texture;
	std::map<std::string, AnimationData> AnimationsData;
	std::map<std::string, StaticTileData> StaticTilesData;

	friend class TextureMgr;
};

class TextureMgr final
{
public:
	TextureMgr();
	~TextureMgr();

	void Init();
	void Shut();

	std::vector<TextureData> Textures;

	/*
	 * Loads the texture using the given path, including its metadata.
	 * Obviously, this function will also store the loaded data
	 * If errors occur during the loading process, this function must return false.
	 * Return true otherwise.
	*/
	bool LoadTexture(const std::filesystem::path& path);

	/*
	 * Returns the corresponding TextureData using the name of the texture. Returns the default TextureData in case of an unknown name
	 */
	const TextureData& GetTextureData(const std::string& name) const;

private:
	bool LoadMetaData(TextureData DataContainer, std::filesystem::path& path);

	bool StoreAnimations(TextureData DataContainer, xml_node<>* root);
	AnimationData AnimationNodeToData(xml_node<>* node);

	bool StoreStaticTiles(TextureData DataContainer, xml_node<>* root);
	StaticTileData StaticTileNodeToData(xml_node<>* node);
};