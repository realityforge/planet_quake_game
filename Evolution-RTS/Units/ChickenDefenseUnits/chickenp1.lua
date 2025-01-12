return {
	chickenp1 = {
		acceleration = 1,
		activatewhenbuilt = true,
		-- autoheal = 27,
		bmcode = "1",
		brakerate = 0.6,
		buildcostenergy = 1,
		buildcostmetal = 75,
		builder = false,
		buildpic = "",
		buildpic = "chicken_unitpics/chickenp1.png",
		buildtime = 1,
		canguard = true,
		canmove = true,
		canpatrol = true,
		canstop = "1",
		category = "BIO",
		collisionvolumeoffsets = "0 2 0",
		collisionvolumescales = "30 38 60",
		collisionvolumetype = "box",
		defaultmissiontype = "Standby",
		description = "Chicken Flamer",
		explodeas = "FLAMEBUG_DEATH",
		footprintx = 3,
		footprintz = 3,
		icontype = "td_lit_all",
		leavetracks = true,
		maneuverleashlength = "640",
		mass = 1800,
		maxdamage = 2700,
		maxvelocity = 3,
		movementclass = "CHICKENHKBOT3",
		name = "Bombardier",
		noautofire = false,
		nochasecategory = "VTOL",
		objectname = "ChickenDefenseModels/chicken_listener.s3o",
		script = "ChickenDefenseScripts/chickenp1.cob",
		seismicsignature = 0,
		selfdestructas = "FLAMEBUG_DEATH",
		side = "THUNDERBIRDS",
		sightdistance = 300,
		smoothanim = true,
		steeringmode = "2",
		tedclass = "KBOT",
		trackoffset = 0,
		trackstrength = 8,
		trackstretch = 1,
		tracktype = "ChickenTrack",
		trackwidth = 50,
		turninplace = true,
		turnrate = 5000,
		unitname = "chickenp1",
		upright = false,
		workertime = 0,
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
		weapondefs = {
			chaseweapon = {
				interceptedByShieldType   = 4,
				craterboost = 0,
				cratermult = 0,
				explosiongenerator = "custom:chickenspike-large-sparks-burn",
				impulseboost = 0,
				impulsefactor = 0,
				name = "Claws",
				noselfdamage = true,
				proximitypriority = -4,
				range = 200,
				reloadtime = 30,
				size = 0.00001,
				targetborder = 1,
				tolerance = 5000,
				turret = "true",
				weapontype = "Cannon",
				weaponvelocity = 500,
				damage = {
					default = 0.001,
				},
			},
			flamer = {
				areaofeffect = 64,
				avoidfeature = 0,
				avoidfriendly = 0,
				burst = 12,
				burstrate = 0.01,
				craterboost = 0,
				cratermult = 0,
				firestarter = 100,
				flamegfxtime = 1.9,
				groundbounce = 1,
				impulseboost = 0,
				impulsefactor = 0,
				intensity = 0.9,
				name = "FlameThrower",
				noselfdamage = true,
				proximitypriority = 4,
				range = 410,
				reloadtime = 0.7,
				rgbcolor = "1 0.95 0.9",
				rgbcolor2 = "0.9 0.85 0.8",
				sizegrowth = 1.2,
				soundstart = "ChickenDefenseSounds/Flamhvy1",
				soundtrigger = 0,
				sprayangle = 1800,
				targetmoveerror = 0.001,
				tolerance = 2500,
				turret = "true",
				weapontimer = 1,
				weapontype = "Flame",
				weaponvelocity = 300,
				damage = {
					default = 32,
				},
			},
		},
		weapons = {
			[1] = {
				def = "CHASEWEAPON",
				maindir = "0 0 1",
				onlytargetcategory = "NOTAIR LIGHT ARMORED BUILDING",
			},
			[2] = {
				badtargetcategory = "VTOL",
				def = "FLAMER",
				maindir = "0 0 1",
				maxangledif = 180,
			},
		},
		customparams = {
			unittype = "mobile",
		},
	},
}
