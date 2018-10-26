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
	lecTheatre=
	{
		filePath = "Resources/objects/lecture theatre/LBLT.obj", 
		texFilePath = "", 
		{
			scaleX = 150.0,
			scaleY = 150.0, 
			scaleZ = 150.0, 
			posX = 7000.0, 
			posY = 0.0, 
			posZ = 5200.0,
			Affordance = "building"
		},
	},
	table=
	{
		filePath = "Resources/objects/table/Table.obj", 
		texFilePath = "", 
		{
			scaleX = 20.0, 
			scaleY = 20.0, 
			scaleZ = 20.0, 
			posX = 9880.0, 
			posY = 1000.0, 
			posZ = 10100.0,
			Affordance = "table"
		},
	},
	chair=
	{
		filePath = "Resources/objects/chair/office_chair.obj", 
		texFilePath = "", 
		{
			scaleX = 300.0, 
			scaleY = 300.0, 
			scaleZ = 300.0,
			posX = 9600.0, 
			posY = 1000.0, 
			posZ = 10100.0,
			Affordance = "chair"
		},
	},
	player=
	{
		filePath = "Resources/objects/box_stack.obj", 
		texFilePath = "", 
		{
			scaleX = 1.0, 
			scaleY = 1.0, 
			scaleZ = 1.0,
			posX = 14000.0, 
			posY = 900.0, 
			posZ = 10100.0,
			Affordance = "player"
		},
	},
	crate=
	{
		filePath = "Resources/objects/crate/Crate1.obj", 
		texFilePath = "",
		{
			scaleX = 1000.0, 
			scaleY = 1000.0, 
			scaleZ = 1000.0,
			posX = 14000.0, 
			posY = 900.0, 
			posZ = 10100.0,
			Affordance = "crate"
		},
	},
	ball=
	{
		filePath = "Resources/objects/ball/ball.obj", 
		texFilePath = "",
		{
			scaleX = 1.0, 
			scaleY = 1.0, 
			scaleZ = 1.0,
			posX = 14000.0, 
			posY = 1500.0, 
			posZ = 10100.0,
			Affordance = "ball"
		},
	},
	person=
	{
		{
			filePath = "Resources/ai/Person.obj", 
			texFilePath = "",
			scaleX = 300.0, 
			scaleY = 400.0, 
			scaleZ = 300.0,
			posX = 14000.0, 
			posY = 0.0, 
			posZ = 10000.0,
			Affordance = "person"
		},
	},
}
