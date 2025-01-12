return {
	chickenh1b = {
		acceleration = 0.1,
		activatewhenbuilt = true,
		-- autoheal = 24,
		bmcode = 1,
		brakerate = 0.2,
		buildcostenergy = 1,
		buildcostmetal = 40,
		builddistance = 180,
		builder = 1,
		buildpic = "chicken_unitpics/chickenh1b.png",
		buildtime = 1,
		canassist = 0,
		canbuild = 1,
		canguard = 1,
		canmove = 1,
		canpatrol = 1,
		canrepair = 1,
		canstop = 1,
		category = "BIO",
		collide = 0,
		collisionvolumeoffsets = "0 -1 0",
		collisionvolumescales = "10 14 22",
		collisionvolumetype = "box",
		defaultmissiontype = "Standby",
		description = "Chicken Healer",
		energymake = 15,
		explodeas = "WEAVER_DEATH",
		floater = false,
		footprintx = 1,
		footprintz = 1,
		--hidedamage = 1,
		icontype = "orb",
		kamikaze = true,
		kamikazedistance = 60,
		leavetracks = true,
		maneuverleashlength = 640,
		mass = 60,
		maxdamage = 330,
		maxslope = 18,
		maxvelocity = 2.15,
		maxwaterdepth = 5000,
		metalstorage = 1000,
		mobilestandorders = 1,
		movementclass = "CHICKENHKBOT2",
		name = "Weaver",
		noautofire = 0,
		objectname = "ChickenDefenseModels/chicken_droneb.s3o",
		script = "ChickenDefenseScripts/chickenh1b.cob",
		reclaimspeed = 400,
		repairspeed = 0.05,
		seismicsignature = 1,
		selfdestructas = "WEAVER_DEATH",
		side = "THUNDERBIRDS",
		sightdistance = 350,
		smoothanim = true,
		standingmoveorder = 1,
		stealth = 1,
		steeringmode = "2",
		tedclass = "KBOT",
		trackoffset = 1,
		trackstrength = 6,
		trackstretch = 1,
		tracktype = "ChickenTrack",
		trackwidth = 10,
		turninplace = true,
		turnrate = 5000,
		unitname = "chickenh1b",
		upright = false,
		waterline = 8,
		workertime = 0.25,
		featuredefs = {
			dead = {},
			heap = {},
		},
		sfxtypes = {
			explosiongenerators = {
				[1] = "custom:blood_spray",
				[2] = "custom:blood_explode",
				[3] = "custom:dirt",
			},
		},
		customparams = {
			unittype = "mobile",
		},
	},
}
