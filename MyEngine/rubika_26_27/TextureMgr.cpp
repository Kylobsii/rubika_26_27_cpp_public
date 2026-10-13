#include "TextureMgr.h"

#include "Logger.h"

#include <filesystem>

TextureData::TextureData()
{
}

TextureData::~TextureData()
{
}

TextureMgr::TextureMgr()
{
}

TextureMgr::~TextureMgr()
{
}

void TextureMgr::Init()
{
}

void TextureMgr::Shut()
{
}

bool TextureMgr::LoadTexture(const std::filesystem::path& path)
{
	Logger::Info("Loading texture: " + path.string());

	TextureData textureData = TextureData();

	if (std::filesystem::exists(path))
	{
		textureData.Texture = sf::Texture(path);
		return true;
	}
	else
	{
		Logger::Warning("The given file paths does not exist: " + path.string());
	}

	return false;
}

bool TextureMgr::LoadMetaData(TextureData DataContainer, std::filesystem::path& path)
{
	Logger::Info("Loading meta data of: " + path.string());

	file<> xmlFile(path.string().c_str());
	xml_document<> doc;
	doc.parse<0>(xmlFile.data());

	std::string rootName = std::string(doc.first_node()->name(), doc.first_node()->name_size());
	if(rootName == "Animations")
	{
		return StoreAnimations(DataContainer, doc.first_node());
	}
	else if(rootName == "Backgrounds")
	{
		return StoreStaticTiles(DataContainer, doc.first_node());
	}
	else
	{
		Logger::Error("Meta data of: '" + path.string() + "' isn't formatted correctly. Root must be either 'Animations' or 'Backgrounds'.");
	}

	return false;
}

bool TextureMgr::StoreAnimations(TextureData DataContainer, xml_node<>* root)
{
	xml_node<>* currAnimNode = root->first_node("Animation");

	if (currAnimNode == 0)
	{
		Logger::Error("Animations nodes aren't formatted correctly. Each node of 'Animations' must be of type 'Animation'.");
		return false;
	}

	std::string currAnimName = "";
	while (std::string(currAnimNode->name(), currAnimNode->name_size()) == "Animation")
	{
		currAnimName = currAnimNode->first_attribute()->value();
		Logger::Info("Storing animation: " + currAnimName);
		DataContainer.AnimationsData[currAnimName] = AnimationNodeToData(currAnimNode);

		currAnimNode = currAnimNode->next_sibling();
	}

	return false;
}

AnimationData TextureMgr::AnimationNodeToData(xml_node<>* node)
{
	AnimationData data = AnimationData();

	xml_node<>* currNode = node->first_node("X");
	if(currNode == 0)
	{
		Logger::Error("Animation node isn't formatted correctly: missing 'X' node. Stored meta data will be incomplete !");
		return data;
	}
	data.StartX = atoi(currNode->value());

	currNode = currNode->first_node("Y");
	if(currNode == 0)
	{
		Logger::Error("Animation node isn't formatted correctly: missing 'Y' node. Stored meta data will be incomplete !");
		return data;
	}
	data.StartY = atoi(currNode->value());

	currNode = currNode->first_node("SizeX");
	if(currNode == 0)
	{
		Logger::Error("Animation node isn't formatted correctly: missing 'SizeX' node. Stored meta data will be incomplete !");
		return data;
	}
	data.SizeX = atoi(currNode->value());

	currNode = currNode->first_node("SizeY");
	if(currNode == 0)
	{
		Logger::Error("Animation node isn't formatted correctly: missing 'SizeY' node. Stored meta data will be incomplete !");
		return data;
	}
	data.SizeY = atoi(currNode->value());

	currNode = currNode->first_node("OffsetX");
	if (currNode == 0)
	{
		Logger::Error("Animation node isn't formatted correctly: missing 'OffsetX' node. Stored meta data will be incomplete !");
		return data;
	}
	data.OffsetX = atoi(currNode->value());

	currNode = currNode->first_node("OffsetY");
	if (currNode == 0)
	{
		Logger::Error("Animation node isn't formatted correctly: missing 'OffsetY' node. Stored meta data will be incomplete !");
		return data;
	}
	data.OffsetY = atoi(currNode->value());

	currNode = currNode->first_node("SpriteNum");
	if (currNode == 0)
	{
		Logger::Error("Animation node isn't formatted correctly: missing 'SpriteNum' node. Stored meta data will be incomplete !");
		return data;
	}
	data.AnimationSpriteCount = atoi(currNode->value());

	currNode = currNode->first_node("SpritesOnLine");
	if (currNode == 0)
	{
		Logger::Error("Animation node isn't formatted correctly: missing 'SpritesOnLine' node. Stored meta data will be incomplete !");
		return data;
	}
	data.SpriteOnLine = atoi(currNode->value());

	currNode = currNode->first_node("Reverted");
	if (currNode == 0)
	{
		Logger::Error("Animation node isn't formatted correctly: missing 'Reverted' node. Stored meta data will be incomplete !");
		return data;
	}
	data.IsReverted = atoi(currNode->value());

	currNode = currNode->first_node("TimeBetweenAnimation");
	if (currNode == 0)
	{
		Logger::Error("Animation node isn't formatted correctly: missing 'TimeBetweenAnimation' node. Stored meta data will be incomplete !");
		return data;
	}
	data.TimeBetweenAnimationInS = std::stof(currNode->value());

	return data;
}

bool TextureMgr::StoreStaticTiles(TextureData DataContainer, xml_node<>* root)
{
	xml_node<>* currTileNode = root->first_node();

	if (currTileNode == 0)
	{
		Logger::Error("Animations nodes aren't formatted correctly. Each node of 'Animations' must be of type 'Animation'.");
		return false;
	}

	std::string currTileName = "";
	while (std::string(currTileNode->name(), currTileNode->name_size()) == "Tile")
	{
		currTileName = currTileNode->first_attribute()->value();
		Logger::Info("Storing tile: " + currTileName);
		DataContainer.StaticTilesData[currTileName] = StaticTileNodeToData(currTileNode);

		currTileNode = currTileNode->next_sibling();
	}

	return false;
}

StaticTileData TextureMgr::StaticTileNodeToData(xml_node<>* node)
{
	StaticTileData data = StaticTileData();

	xml_node<>* currNode = node->first_node("X");
	if (currNode == 0)
	{
		Logger::Error("Tile node isn't formatted correctly: missing 'X' node. Stored meta data will be incomplete !");
		return data;
	}
	data.StartX = atoi(currNode->value());

	currNode = currNode->first_node("Y");
	if (currNode == 0)
	{
		Logger::Error("Tile node isn't formatted correctly: missing 'Y' node. Stored meta data will be incomplete !");
		return data;
	}
	data.StartY = atoi(currNode->value());

	currNode = currNode->first_node("SizeX");
	if (currNode == 0)
	{
		Logger::Error("Tile node isn't formatted correctly: missing 'SizeX' node. Stored meta data will be incomplete !");
		return data;
	}
	data.SizeX = atoi(currNode->value());

	currNode = currNode->first_node("SizeY");
	if (currNode == 0)
	{
		Logger::Error("Tile node isn't formatted correctly: missing 'SizeY' node. Stored meta data will be incomplete !");
		return data;
	}
	data.SizeY = atoi(currNode->value());

	currNode = currNode->first_node("RevertedX");
	if (currNode == 0)
	{
		Logger::Error("Tile node isn't formatted correctly: missing 'RevertedX' node. Stored meta data will be incomplete !");
		return data;
	}
	data.IsRevertedX = atoi(currNode->value());

	currNode = currNode->first_node("RevertedY");
	if (currNode == 0)
	{
		Logger::Error("Tile node isn't formatted correctly: missing 'RevertedY' node. Stored meta data will be incomplete !");
		return data;
	}
	data.IsRevertedY = atoi(currNode->value());

	return data;
}

//const TextureData& TextureMgr::GetTextureData(const std::string& name) const
//{
//	
//}
