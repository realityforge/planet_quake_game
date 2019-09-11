/models/powerups/instant/accuracy
{
	{
		map textures/effects/envmapdarkpurp.tga
		blendfunc GL_ONE GL_ZERO
		tcGen environment
                rgbGen identity
	}
}

models/powerups/instant/holo_body
{
	{
		map textures/effects/envmap.tga
		tcgen environment
	}
}

models/powerups/instant/holo_shaft
{
	{
		map textures/effects/tinfx2c.tga
		blendfunc GL_ONE GL_ZERO
		tcGen environment
		rgbGen identity
	}
}

models/powerups/instant/holog
{
	{
		map textures/effects/tinfx2c.tga
		blendfunc GL_ONE GL_ZERO
		tcGen environment
		rgbGen identity
	}
}

models/powerups/instant/holo_red
{
	{
		map textures/effects/envmapred.tga
		tcGen environment
		tcmod rotate 30
		tcmod scroll 1 .1
		blendfunc GL_ONE GL_ONE
		rgbGen identity
	}
}

models/powerups/instant/holo_green
{
	{
		map textures/effects/envmapgreen.tga
		tcGen environment
		tcmod rotate 30
		tcmod scroll 1 .1
		blendfunc GL_ONE GL_ONE
		rgbGen identity
	}
}

models/powerups/instant/holo_blue
{
	{
		map textures/effects/envmapblue.tga
		tcGen environment
		tcmod rotate 30
		tcmod scroll 1 .1
		blendfunc GL_ONE GL_ONE
		rgbGen identity
	}
}

models/powerups/instant/vital
{
	{
		map textures/effects/envmapgreen.tga
		tcGen environment
		tcmod rotate 30
		tcmod scroll 1 .1
		blendfunc GL_ONE GL_ZERO
		rgbGen identity
	}
}

/models/powerups/instant/speed
{
	{
		map textures/effects/envmapsilverblue.tga
		tcGen environment
		blendfunc GL_ONE GL_ZERO
                rgbGen identity
	}
}

powerups/accuracy
{
	deformVertexes wave 100 sin 3 0 0 0
	{
		map textures/effects/accuracymap2.tga
		blendfunc GL_ONE GL_ONE
		tcGen environment
                tcmod rotate 30
		//tcMod turb 0 0.2 0 .2
                tcmod scroll 1 .1
	}
}
powerups/accuracyWeapon
{
	deformVertexes wave 100 sin 0.5 0 0 0
	{
		map textures/effects/accuracymap2.tga
		blendfunc GL_ONE GL_ONE
		tcGen environment
               tcmod rotate 30
		//tcMod turb 0 0.2 0 .2
                tcmod scroll 1 .1
	}
}

powerups/vitality
{
	deformVertexes wave 100 sin 3 0 0 0
	{
		map textures/effects/vitalmap2.tga
		blendfunc GL_ONE GL_ONE
		tcGen environment
                tcmod rotate 30
		//tcMod turb 0 0.2 0 .2
                tcmod scroll 1 .1
		rgbGen wave sin 0.35 0.25 0 1
	}
}
powerups/vitalityWeapon
{
	deformVertexes wave 100 sin 0.5 0 0 0
	{
		map textures/effects/vitalmap2.tga
		blendfunc GL_ONE GL_ONE
		tcGen environment
                tcmod rotate 30
		//tcMod turb 0 0.2 0 .2
                tcmod scroll 1 .1
		rgbGen wave sin 0.35 0.25 0 1
	}
}


icons/accuracy
{
	nopicmip
	{
		map icons/accuracy.tga
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	}
}

icons/vital
{
	nopicmip
	{
		map icons/vital.tga
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	}
}

icons/hologram
{
	nopicmip
	{
		map icons/hologram.tga
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	}
}

icons/speed
{
	nopicmip
	{
		map icons/speed.tga
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	}
}

sprites/holoballoon
{
	{
		map sprites/holoballoon.tga
		blendfunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	}
}