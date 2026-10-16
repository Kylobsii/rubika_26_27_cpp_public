#include "TextureMgr.h"

#include "Logger.h"

#ifdef _USE_IMGUI
#include <Imgui/imgui.h>
#include <Imgui/imgui-SFML.h>
#endif

#include <filesystem>
#include <Globals.h>

TextureData::TextureData()
{
}

TextureData::~TextureData()
{
}

TextureMgr::TextureMgr()
{}

TextureMgr::~TextureMgr()
{
}

void TextureMgr::Init()
{
	gData.DebugMgr->RegisterDebugableWindow("TextureMgr", this);
}

void TextureMgr::Shut()
{
	gData.DebugMgr->UnregisterDebugableWindow("TextureMgr");
}

void TextureMgr::DrawDebug()
{
#ifdef _USE_IMGUI
	ImGui::SetNextItemWidth(500.0f);
	ImGui::InputText("Path", texturePathBuffer, IM_ARRAYSIZE(texturePathBuffer));

	ImGui::SameLine();
	if (ImGui::Button("LoadTexture"))
	{
		texturePath = texturePathBuffer;
		LoadTexture(texturePath);

		texturePathBuffer[0] = '\0';
	}

	ImGui::Separator();

	for(auto& texturePair : Textures)
	{
		TextureData& data = texturePair.second;
		if (ImGui::CollapsingHeader(texturePair.first.c_str()))
		{
			ImGui::Image(data.Texture);
			ImGui::Text("Texture Size: %d x %d", data.Texture.getSize().x, data.Texture.getSize().y);
			for (auto& animPair : data.AnimationsData)
			{
				AnimationData& animData = animPair.second;
				if (ImGui::TreeNode(animPair.first.c_str()))
				{
					ImGui::Text("StartX: %d", animData.StartX);
					ImGui::Text("StartY: %d", animData.StartY);
					ImGui::Text("SizeX: %d", animData.SizeX);
					ImGui::Text("SizeY: %d", animData.SizeY);
					ImGui::Text("OffsetX: %d", animData.OffsetX);
					ImGui::Text("OffsetY: %d", animData.OffsetY);
					ImGui::Text("AnimationSpriteCount: %d", animData.AnimationSpriteCount);
					ImGui::Text("SpriteOnLine: %d", animData.SpriteOnLine);
					ImGui::Text("IsReverted: %s", animData.IsReverted ? "true" : "false");
					ImGui::Text("TimeBetweenAnimationInS: %.2f", animData.TimeBetweenAnimationInS);
					ImGui::TreePop();
				}
				ImGui::Dummy(ImVec2(0.0f, 5.0f));
			}

			ImGui::Separator();

			for (auto& tilePair : data.StaticTilesData)
			{
				StaticTileData& tileData = tilePair.second;
				if (ImGui::TreeNode(tilePair.first.c_str()))
				{
					ImGui::Text("StartX: %d", tileData.StartX);
					ImGui::Text("StartY: %d", tileData.StartY);
					ImGui::Text("SizeX: %d", tileData.SizeX);
					ImGui::Text("SizeY: %d", tileData.SizeY);
					ImGui::Text("IsRevertedX: %s", tileData.IsRevertedX ? "true" : "false");
					ImGui::Text("IsRevertedY: %s", tileData.IsRevertedY ? "true" : "false");
					ImGui::TreePop();
				}
			}
		}
	}
#endif
}

bool TextureMgr::LoadTexture(const std::filesystem::path& path)
{
	Logger::Info("Loading texture: " + path.string());

	if (std::filesystem::exists(path))
	{
		std::string name = path.stem().string();

		auto it = Textures.emplace(name, TextureData());
		TextureData& textureData = it.first->second;

		textureData.Texture = sf::Texture(path);

		std::string pathStr = path.string();
		pathStr.replace(pathStr.find(".png"), 4, ".xml");

		return LoadMetaData(textureData, pathStr);
	}
	else
	{
		Logger::Warning("The given file paths does not exist: " + path.string());
	}

	return false;
}

bool TextureMgr::LoadMetaData(TextureData& DataContainer, std::string pathStr)
{
	Logger::Info("Loading meta data of: " + pathStr);

	file<> xmlFile(pathStr.c_str());
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
		Logger::Error("Meta data of: '" + pathStr + "' isn't formatted correctly. Root must be either 'Animations' or 'Backgrounds'.");
	}

	return false;
}

bool TextureMgr::StoreAnimations(TextureData& DataContainer, xml_node<>* root)
{
	xml_node<>* currAnimNode = root->first_node("Animation");

	if (currAnimNode == 0)
	{
		Logger::Error("Animations nodes aren't formatted correctly. Each node of 'Animations' must be of type 'Animation'.");
		return false;
	}

	std::string currAnimName = "";
	while (currAnimNode && std::string(currAnimNode->name(), currAnimNode->name_size()) == "Animation")
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

	currNode = node->first_node("Y");
	if(currNode == 0)
	{
		Logger::Error("Animation node isn't formatted correctly: missing 'Y' node. Stored meta data will be incomplete !");
		return data;
	}
	data.StartY = atoi(currNode->value());

	currNode = node->first_node("SizeX");
	if(currNode == 0)
	{
		Logger::Error("Animation node isn't formatted correctly: missing 'SizeX' node. Stored meta data will be incomplete !");
		return data;
	}
	data.SizeX = atoi(currNode->value());

	currNode = node->first_node("SizeY");
	if(currNode == 0)
	{
		Logger::Error("Animation node isn't formatted correctly: missing 'SizeY' node. Stored meta data will be incomplete !");
		return data;
	}
	data.SizeY = atoi(currNode->value());

	currNode = node->first_node("OffsetX");
	if (currNode == 0)
	{
		Logger::Error("Animation node isn't formatted correctly: missing 'OffsetX' node. Stored meta data will be incomplete !");
		return data;
	}
	data.OffsetX = atoi(currNode->value());

	currNode = node->first_node("OffsetY");
	if (currNode == 0)
	{
		Logger::Error("Animation node isn't formatted correctly: missing 'OffsetY' node. Stored meta data will be incomplete !");
		return data;
	}
	data.OffsetY = atoi(currNode->value());

	currNode = node->first_node("SpriteNum");
	if (currNode == 0)
	{
		Logger::Error("Animation node isn't formatted correctly: missing 'SpriteNum' node. Stored meta data will be incomplete !");
		return data;
	}
	data.AnimationSpriteCount = atoi(currNode->value());

	currNode = node->first_node("SpritesOnLine");
	if (currNode == 0)
	{
		Logger::Error("Animation node isn't formatted correctly: missing 'SpritesOnLine' node. Stored meta data will be incomplete !");
		return data;
	}
	data.SpriteOnLine = atoi(currNode->value());

	currNode = node->first_node("Reverted");
	if (currNode == 0)
	{
		Logger::Error("Animation node isn't formatted correctly: missing 'Reverted' node. Stored meta data will be incomplete !");
		return data;
	}
	data.IsReverted = atoi(currNode->value());

	currNode = node->first_node("TimeBetweenAnimation");
	if (currNode == 0)
	{
		Logger::Error("Animation node isn't formatted correctly: missing 'TimeBetweenAnimation' node. Stored meta data will be incomplete !");
		return data;
	}
	data.TimeBetweenAnimationInS = std::stof(currNode->value());

	return data;
}

bool TextureMgr::StoreStaticTiles(TextureData& DataContainer, xml_node<>* root)
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

	currNode = node->first_node("Y");
	if (currNode == 0)
	{
		Logger::Error("Tile node isn't formatted correctly: missing 'Y' node. Stored meta data will be incomplete !");
		return data;
	}
	data.StartY = atoi(currNode->value());

	currNode = node->first_node("SizeX");
	if (currNode == 0)
	{
		Logger::Error("Tile node isn't formatted correctly: missing 'SizeX' node. Stored meta data will be incomplete !");
		return data;
	}
	data.SizeX = atoi(currNode->value());

	currNode = node->first_node("SizeY");
	if (currNode == 0)
	{
		Logger::Error("Tile node isn't formatted correctly: missing 'SizeY' node. Stored meta data will be incomplete !");
		return data;
	}
	data.SizeY = atoi(currNode->value());

	currNode = node->first_node("RevertedX");
	if (currNode == 0)
	{
		Logger::Error("Tile node isn't formatted correctly: missing 'RevertedX' node. Stored meta data will be incomplete !");
		return data;
	}
	data.IsRevertedX = atoi(currNode->value());

	currNode = node->first_node("RevertedY");
	if (currNode == 0)
	{
		Logger::Error("Tile node isn't formatted correctly: missing 'RevertedY' node. Stored meta data will be incomplete !");
		return data;
	}
	data.IsRevertedY = atoi(currNode->value());

	return data;
}

const TextureData& TextureMgr::GetTextureData(const std::string& name) const
{
	auto it = Textures.find(name);
	if (it != Textures.end())
	{
		return it->second;
	}

	Logger::Warning(name + " is an unknown name for texture data, returning default texture data!");

	return DefaultTextureData;
}

const TextureData& TextureMgr::GetDefaultTextureData()
{
	return DefaultTextureData;
}


