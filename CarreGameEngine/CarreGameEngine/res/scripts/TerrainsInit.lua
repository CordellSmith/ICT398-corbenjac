--TerrainsInit.lua
--Author: Jack Matters
--Date: 23/05/2018
--Brief: Load in all heightmaps data
--Note: Make filePath relative to CarreGameEngine.vcxproj
--Note: Use this file to load all heightmap data
--Note: "player" MUST BE present and MUST BE spelt correctly
--Note: Needs improvements
AllHeightmaps=
{
	terrain=
	{
		{filePath = "res/terrain/newcity.raw", texFilePath = "res/terrain/grass.jpg", fileSize = 128, scaleX = 100.0, scaleY = 5.0, scaleZ = 100.0, posX = 0.0, posY = 0.0, posZ = 0.0},
	},
	buildings=
	{
		--{filePath = "res/terrain/buildingheightmap.raw", texFilePath = "res/terrain/buildingtexture.jpg", fileSize = 16, scaleX = 100.0, scaleY = 4.0, scaleZ = 100.0, posX = 6000.0, posY = -1.0, posZ = 6000.0},
	},
}
