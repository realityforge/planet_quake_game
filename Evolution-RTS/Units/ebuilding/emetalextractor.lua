-- UNITDEF -- EMETALEXTRACTOR --
--------------------------------------------------------------------------------

unitName = [[emetalextractor]]

--------------------------------------------------------------------------------

metalMultiplier = 1

buildCostMetal = 20
energyUse = 0

primaryCEG = "custom:fusionreactionnuclear-1color"

humanName = [[Metal Extractor]]

objectName = [[emetalextractor2.s3o]]
script = [[emetalextractor.cob]]

tech = [[tech0]]
armortype = [[building]]

VFS.Include("units-configs-basedefs/basedefs/buildings/emetalextractor_basedef.lua")

unitDef.weaponDefs = weaponDefs
--------------------------------------------------------------------------------

return lowerkeys({ [unitName]    = unitDef })

--------------------------------------------------------------------------------