--ModelInit.lua
--Author: Jack Matters
--Date: 17/05/2018
--Brief: Load in all models and their positions
--Note: Make filePath relative to CarreGameEngine.vcxproj
--Note: Use this file to load model .obj, scale, and position only
--Note: "player" MUST BE present and MUST BE spelt correctly
--Note: Needs improvements
AllModels=
{
	rock=
	{
		{filePath = "Resources/objects/rocks/stone_1.obj", texFilePath = "Resources/objects/rocks/Very_Craggy_Grey_Rock_Moss_UV_CM_1.jpg", scaleX = 100.0, scaleY = 100.0, scaleZ = 100.0, posX = 6500.0, posY = 150.0, posZ = 5475.0, AI = false},
	},
	player=
	{
		{filePath = "Resources/objects/taxi/taxi.obj", texFilePath = "", scaleX = 50.0, scaleY = 50.0, scaleZ = 50.0, posX = 12700.0, posY = -90.0, posZ = 10000.0, AI = false},
	},
	lecTheatre=
	{
		{filePath = "Resources/objects/LectureTheatre/LBLT.obj", texFilePath = "", scaleX = 150.0, scaleY = 150.0, scaleZ = 150.0, posX = 7000.0, posY = -1300.0, posZ = 5200.0, AI = false},
	},
	table=
	{
		{filePath = "Resources/objects/Table/Table.obj", texFilePath = "", scaleX = 100.0, scaleY = 100.0, scaleZ = 100.0, posX = 7000.0, posY = -1300.0, posZ = 5200.0, AI = false},
	}
}
