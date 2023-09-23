#include "stdafx.h"
#include "SpriteFontLoader.h"

SpriteFont* SpriteFontLoader::LoadContent(const ContentLoadInfo& loadInfo)
{
	const auto pReader = new BinaryReader();
	pReader->Open(loadInfo.assetFullPath);

	if (!pReader->Exists())
	{
		Logger::LogError(L"Failed to read the assetFile!\nPath: \'{}\'", loadInfo.assetSubPath);
		return nullptr;
	}

	//See BMFont Documentation for Binary Layout

	//Parse the Identification bytes (B,M,F)
	const char idBytes[]{ pReader->Read<char>(), pReader->Read<char>(), pReader->Read<char>() };

	//If Identification bytes doesn't match B|M|F,
	if (idBytes[0] != 'B' || idBytes[1] != 'M' || idBytes[2] != 'F')
	{
		Logger::LogError(L"SpriteFontLoader::LoadContent > Not a valid .fnt font");
		return nullptr;
	}

	//Parse the version (version 3 required)
	const int version{ pReader->Read<char>() };
	if (version != 3)
	{
		Logger::LogError(L"SpriteFontLoader::LoadContent > Only .fnt version 3 is supported");
		return nullptr;
	}

	//Valid .fnt file >> Start Parsing!
	//use this SpriteFontDesc to store all relevant information (used to initialize a SpriteFont object)
	SpriteFontDesc fontDesc{};

	//**********
	// BLOCK 0 *
	//**********
	{
		//Retrieve the blockId and blockSize
		pReader->MoveBufferPosition(7);

		//Retrieve the FontSize [fontDesc.fontSize]
		fontDesc.fontSize = pReader->Read<short>();

		//Move the binreader to the start of the FontName [BinaryReader::MoveBufferPosition(...) or you can set its position using BinaryReader::SetBufferPosition(...))
		pReader->MoveBufferPosition(12);

		//Retrieve the FontName [fontDesc.fontName]
		fontDesc.fontName = pReader->ReadNullString();
	}

	//**********
	// BLOCK 1 *
	//**********
	{
		//Retrieve the blockId and blockSize
		pReader->MoveBufferPosition(9);

		//Retrieve Texture Width & Height [fontDesc.textureWidth/textureHeight]
		fontDesc.textureWidth = pReader->Read<short>();
		fontDesc.textureHeight = pReader->Read<short>();

		//Retrieve PageCount
		const short pagecount{ pReader->Read<short>() };

		//> if pagecount > 1
		if (pagecount > 1)
		{
			//	> Log Error (Only one texture per font is allowed!)
			Logger::LogError(L"SpriteFontLoader::LoadContent > Only one texture per font is allowed!");
			return nullptr;
		}

		//Advance to Block2 (Move Reader)
		pReader->MoveBufferPosition(5);
	}

	//**********
	// BLOCK 2 *
	//**********
	{
		//Retrieve the blockId and blockSize
		pReader->MoveBufferPosition(5);

		//Retrieve the PageName (BinaryReader::ReadNullString)
		const std::wstring pageName{ pReader->ReadNullString() };

		//Construct the full path to the page texture file
		//	>> page texture should be stored next to the .fnt file, pageName contains the name of the texture file
		//	>> full texture path = asset parent_path of .fnt file (see loadInfo.assetFullPath > get parent_path) + pageName (filesystem::path::append)
		//	>> Load the texture (ContentManager::Load<TextureData>) & Store [fontDesc.pTexture]
		fontDesc.pTexture = ContentManager::Load<TextureData>(loadInfo.assetFullPath.parent_path().append(pageName));
	}

	//**********
	// BLOCK 3 *
	//**********
	{
		//Retrieve the blockId and blockSize
		//Retrieve Character Count (see documentation)
		pReader->MoveBufferPosition(1);
		const auto charCount{ pReader->Read<int>() / 20 };

		std::unordered_map<byte, int> channelMap
		{
			{ 1, 2 },   // Bit 0 represents channel B
			{ 2, 1 },   // Bit 1 represents channel G
			{ 4, 0 },   // Bit 2 represents channel R
			{ 8, 3 },   // Bit 3 represents channel A
			{ 16, 4 }   // Bit 4 represents channel RGBA
		};

		//Create loop for Character Count, and:
		for (size_t i{ 0 }; i < charCount; ++i)
		{
			//> Retrieve CharacterId (store Local) and cast to a 'wchar_t'
			const auto charId{ static_cast<wchar_t>(pReader->Read<int>()) };

			//> Create instance of FontMetric (struct)
			FontMetric metric{};

			//	> Set Character (CharacterId) [FontMetric::character]
			metric.character = charId;

			//	> Retrieve Xposition (store Local)
			const auto xPosition{ pReader->Read<short>() };

			//	> Retrieve Yposition (store Local)
			const auto yPosition{ pReader->Read<short>() };

			//	> Retrieve & Set Width [FontMetric::width]
			metric.width = pReader->Read<short>();

			//	> Retrieve & Set Height [FontMetric::height]
			metric.height = pReader->Read<short>();

			//	> Retrieve & Set OffsetX [FontMetric::offsetX]
			metric.offsetX = pReader->Read<short>();

			//	> Retrieve & Set OffsetY [FontMetric::offsetY]
			metric.offsetY = pReader->Read<short>();

			//	> Retrieve & Set AdvanceX [FontMetric::advanceX]
			metric.advanceX = pReader->Read<short>();

			//	> Retrieve & Set Page [FontMetric::page]
			metric.page = pReader->Read<char>();

			//	> Retrieve Channel (BITFIELD!!!)
			const auto channel{ pReader->Read<char>() };
			metric.channel = static_cast<unsigned char>(channelMap[channel]);

			//		> See documentation for BitField meaning [FontMetrix::channel]
			//	> Calculate Texture Coordinates using Xposition, Yposition, fontDesc.TextureWidth & fontDesc.TextureHeight [FontMetric::texCoord]
			metric.texCoord.x = xPosition / static_cast<float>(fontDesc.textureWidth);
			metric.texCoord.y = yPosition / static_cast<float>(fontDesc.textureHeight);

			//> Insert new FontMetric to the metrics [font.metrics] map
			//	> key = (wchar_t) charId
			//	> value = new FontMetric
			fontDesc.metrics[charId] = metric;
		}
	}
	//(loop restarts till all metrics are parsed)

	//Done!
	delete pReader;
	return new SpriteFont(fontDesc);
}

void SpriteFontLoader::Destroy(SpriteFont* objToDestroy)
{
	SafeDelete(objToDestroy);
}
