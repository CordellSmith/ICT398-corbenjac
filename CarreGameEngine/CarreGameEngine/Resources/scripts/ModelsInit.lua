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
		lecTheatre=
		{
			filePath = "Resources/objects/lecture theatre/LBLT_Complete.obj", 
			scale = "200.0 150.0 150.0", 
			pos = "7000.0 0.0 5200", 
			rot = "0.0 0.0 0.0", 
			Affordance = "building"
		},
	},
	table=
	{
		table1=
		{
			filePath = "Resources/objects/table/Table.obj", 
			scale = "800.0 1000.0 800.0", 
			pos = "0.0 0.0 0.0",
			rot = "0.0 0.0 0.0", 			
			Affordance = "table"
		},
	},
	chair=
	{
		chair1=
		{
			filePath = "Resources/objects/chair/office_chair.obj", 
			scale = "300.0 300.0 300.0", 
			pos = "12000.0 100.0 10100.0", 
			rot = "0.0 0.0 0.0", 
			Affordance = "chair"
		},
		chair2=
		{
			filePath = "Resources/objects/chair/office_chair.obj", 
			scale = "300.0 300.0 300.0", 
			pos = "14000.0 100.0 6000.0", 
			rot = "0.0 0.0 0.0", 
			Affordance = "chair"
		},
	},
	player=
	{
		player=
		{
			filePath = "Resources/objects/box_stack.obj", 
			scale = "1.0 1.0 1.0", 
			pos = "12000.0 1200.0 10100.0", 
			rot = "0.0 0.0 0.0", 
			Affordance = "player"
		},
	},
	crate=
	{
		crate=
		{
			filePath = "Resources/objects/crate/Crate1.obj", 
			scale = "1000.0 1000.0 1000.0", 
			pos = "14000.0 900.0 10100.0", 
			rot = "0.0 0.0 0.0", 
			Affordance = "crate"
		},
	},
	ball=
	{
		ball=
		{
			filePath = "Resources/objects/ball/ball.obj", 
			scale = "0.5 0.5 0.5", 
			pos = "14000.0 1500.0 10100.0", 
			rot = "0.0 0.0 0.0", 
			Affordance = "ball"
		},
	},
	person=
	{
		person1=
		{
			filePath = "Resources/ai/Person.obj", 
			scale = "300.0 400.0 300.0", 
			pos = "14000.0 0.0 10000.0", 
			rot = "0.0 0.0 0.0", 
			Affordance = "person"
		},
		person2=
		{
			filePath = "Resources/ai/Person.obj", 
			scale = "300.0 400.0 300.0", 
			pos = "15000.0 0.0 10000.0", 
			rot = "0.0 0.0 0.0", 
			Affordance = "person"
		},
		person3=
		{
			filePath = "Resources/ai/Person.obj", 
			scale = "300.0 400.0 300.0", 
			pos = "15000.0 0.0 10000.0", 
			rot = "0.0 0.0 0.0", 
			Affordance = "person"
		},
	},
}
