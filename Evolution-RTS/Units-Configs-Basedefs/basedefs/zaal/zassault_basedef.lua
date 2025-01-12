unitDef                    = {
	acceleration                 = 1,
	brakeRate                    = 1,
	buildCostEnergy              = 0,
	buildCostMetal               = 75,
	builder                      = false,
	buildTime                    = 5,
	buildpic					 = "zaal_unitpics/zassault.png",
	canAttack                    = true,
	canGuard                     = true,
	canHover                     = true,
	canMove                      = true,
	canPatrol                    = true,
	canstop                      = "1",
	category                     = "ARMORED NOTAIR",
	description                  = [[Armored Assault]],
	energyMake                   = 0,
	energyStorage                = 0,
	energyUse                    = 0,
	explodeAs                    = "BUG_DEATH_LARGE",
	footprintX                   = 4,
	footprintZ                   = 4,
	iconType                     = "zassault",
	idleAutoHeal                 = .5,
	idleTime                     = 2200,
	leaveTracks                  = false,
	maxDamage                    = 600,
	maxSlope                     = 26,
	maxVelocity                  = 3.5,
	maxReverseVelocity           = 1,
	maxWaterDepth                = 10,
	metalStorage                 = 0,
	movementClass                = "ZAALKBOT4",
	name                         = humanName,
	noChaseCategory              = "VTOL",
	objectName                   = objectName,
	script			             = script,
	radarDistance                = 0,
	repairable		             = false,
	selfDestructAs               = "BUG_DEATH_LARGE",
	side                         = "CORE",
	sightDistance                = 700,
	smoothAnim                   = true,
	stealth			             = true,
	seismicSignature             = 2,
	transportbyenemy             = false;
	--  turnInPlace              = false,
	--  turnInPlaceSpeedLimit    = 2.8,
	turnInPlace                  = true,
	turnRate                     = 5000,
	--  turnrate                 = 350,
	unitname                     = unitName,
	upright                      = true,
	workerTime                   = 0,
	sfxtypes                     = { 
		pieceExplosionGenerators = { 
			"blood_spray", 
		}, 

		explosiongenerators      = {
			"custom:blood_spray",
			"custom:blood_explode",
			"custom:dirt",
		},
	},
	sounds                       = {
		underattack              = "other/unitsunderattack1",
		ok                       = {
			"ack",
		},
		select                   = {
			"unitselect",
		},
	},
	weapons                      = {
		[1] = {
			def = "WEAPON",
			maindir = "0 0 1",
			maxangledif = 180,
			onlyTargetCategory   = "BIO LIGHT ARMORED BUILDING",
			badTargetCategory    = "BUILDING WALL",
		},
	},
	customParams                 = {
		isupgraded			  	 = isUpgraded,
		unittype				 = "mobile",
		canbetransported 		 = "true",
		needed_cover             = 3,
		death_sounds             = "bug",
		RequireTech              = tech,
		armortype                = armortype,
		nofriendlyfire	         = "1",
		supply_cost              = supply,
		normaltex 				 = "unittextures/z_normals.dds", 
		factionname	             = "zaal",
		--corpse                   = "energycore",
	},
}

weaponDefs = {
	weapon = {
		interceptedByShieldType		= 4,
		areaofeffect				= 16,
		avoidFriendly				= false,
		avoidFeature				= false,
		collideFriendly				= false,
		collideFeature				= false,
		cegTag                    = "zaalspiketrail-optimized",
		craterboost					= 0,
		cratermult					= 0,
		edgeeffectiveness			= 0,
		explosiongenerator 		  = "custom:chickenspike-large-sparks-burn",
		impulseboost				= 0,
		impulsefactor				= 0,
		interceptedbyshieldtype		= 4,
		model						= "zaal/spike.s3o",
		name						= "Spike",
		noselfdamage				= true,
		range						= 375,
		reloadtime					= 2,
		startvelocity				= 300,
		submissile					= 1,
		targetmoveerror				= 0.5,
		turret						= true,
		waterweapon					= true,
		weaponacceleration			= 70,
		weapontimer					= 1,
		weapontype					= "Cannon",
		weaponvelocity				= 325,
		customparams             = {
			damagetype		     = "default",
			nofriendlyfire	     = 1,
		}, 
		damage = {
			default					= 500,
		},
	},
}