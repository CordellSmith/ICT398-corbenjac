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
		{
			filePath = "Resources/objects/lecture theatre/LBLT.obj", 
			texFilePath = "", 
			scale = "150.0 150.0 150.0", 
			pos = "7000.0 0.0 5200", 
			Affordance = "building"
		},
	},
	table=
	{
		{
			filePath = "Resources/objects/table/Table.obj", 
			texFilePath = "", 
			scale = "20.0 20.0 20.0", 
			pos = "9880.0 1000.0 10100.0", 
			Affordance = "table"
		},
	},
	chair=
	{
		{
			filePath = "Resources/objects/chair/office_chair.obj", 
			texFilePath = "", 
			scale = "300.0 300.0 300.0", 
			pos = "9600.0 1000.0 10100.0", 
			Affordance = "chair"
		},
	},
	player=
	{
		{
			filePath = "Resources/objects/box_stack.obj", 
			texFilePath = "", 
			scale = "1.0 1.0 1.0", 
			pos = "14000.0 900.0 10100.0", 
			Affordance = "player"
		},
	},
	crate=
	{
		{
			filePath = "Resources/objects/crate/Crate1.obj", 
			texFilePath = "",
			scale = "1000.0 1000.0 1000.0", 
			pos = "14000.0 900.0 10100.0", 
			Affordance = "crate"
		},
	},
	ball=
	{
		{
			filePath = "Resources/objects/ball/ball.obj", 
			texFilePath = "",
			scale = "1.0 1.0 1.0", 
			pos = "14000.0 1500.0 10100.0", 
			Affordance = "ball"
		},
	},
	person=
	{
		{
			filePath = "Resources/ai/Person.obj", 
			texFilePath = "",
			scale = "300.0 400.0 300.0", 
			pos = "14000.0 0.0 10000.0", 
			Affordance = "person"
		},
	},
}
