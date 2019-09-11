lightningWeb
{
	cull none
	{
		map gfx/misc/lightning3.tga
		blendFunc GL_ONE GL_ONE
		rgbgen wave sin 0.3 0.2 0 8.1
		rgbgen wave sin 0.2 0.1 0 2.1
		tcmod scale  -1.3 -1
		tcMod scroll -7.2 0
	}
}

lightningWeb_red
{
	cull none
	{
		map gfx/misc/lightning3_red.tga
		blendFunc GL_ONE GL_ONE
		rgbgen wave sin 0.3 0.2 0 8.1
		rgbgen wave sin 0.2 0.1 0 2.1
		tcmod scale  -1.3 -1
		tcMod scroll -7.2 0
	}
}

lightningWeb_purple
{
	cull none
	{
		map gfx/misc/lightning3_purple.tga
		blendFunc GL_ONE GL_ONE
		rgbgen wave sin 0.3 0.2 0 8.1
		rgbgen wave sin 0.2 0.1 0 2.1
		tcmod scale  -1.3 -1
		tcMod scroll -7.2 0
	}
}

lightningAmmoExpl
{
	sort nearest
	cull none
	{
		map gfx/misc/lightningammo1.tga
		blendFunc GL_ONE GL_ONE
		rgbgen wave sin 0.3 0.2 0 8.1
		rgbgen wave sin 0.2 0.1 0 2.1
//		tcMod scroll -1 0
		tcMod scale 8.0 1.0
	}
}

lightningAmmoBall
{
	deformVertexes wave 100 sin 1 0 0 0
	{
                map textures/sfx/specular.tga
		tcGen environment
		tcMod turb 0 0.15 0 0.3
                tcmod rotate 333
                tcmod scroll .3 .3
		blendfunc GL_ONE GL_ONE
	}
}

//
// special effects as seen on players
//
powerups/protection
{
	deformVertexes wave 100 sin 1 0 0 0
	{
		map textures/effects/tinfx.tga
		tcGen environment
		tcMod turb 0 0.35 0 0.3
                tcmod rotate 233
                tcmod scroll .3 .3
		blendfunc GL_ONE GL_ZERO
	}
}

powerups/protection_blue
{
	deformVertexes wave 100 sin 1 0 0 0
	{
		map textures/effects/tinfx_blue.tga
		tcGen environment
		tcMod turb 0 0.35 0 0.3
                tcmod rotate 233
                tcmod scroll .3 .3
		blendfunc GL_ONE GL_ZERO
	}
}

powerups/protection_red
{
	deformVertexes wave 100 sin 1 0 0 0
	{
		map textures/effects/tinfx_red.tga
		tcGen environment
		tcMod turb 0 0.35 0 0.3
                tcmod rotate 233
                tcmod scroll .3 .3
		blendfunc GL_ONE GL_ZERO
	}
}

gfx/2d/altfire_logo_medium
{
	nopicmip
	nomipmaps
	{
		map gfx/2d/altfire_logo_medium.tga
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		rgbgen vertex
	}
}

models/alternatefire/bfg_shockwave
{
	cull none 
	deformVertexes wave 100 sin 0.5 0 0 0
	{
		map textures/effects/quadmap2.tga
		blendfunc GL_ONE GL_ONE
		tcGen environment
                tcmod rotate 60
		tcMod turb 0 0.2 0 0.2
                tcmod scroll 0.7 0.4
	}
}
models/alternatefire/red_sphere
{
	{
		map textures/effects/envmapgold2.tga
                tcGen environment
                tcmod rotate 33
                tcmod scroll 1 1
		blendfunc GL_ONE GL_ONE
	}
}

models/alternatefire/bfg_shockwave_red
{
	cull none 
	deformVertexes wave 100 sin 0.5 0 0 0
	{
		map textures/effects/quadmapred.tga
		blendfunc GL_ONE GL_ONE
		tcGen environment
                tcmod rotate 60
		tcMod turb 0 0.2 0 0.2
                tcmod scroll 0.7 0.4
	}
}

models/ammo/grenade2_body
{
	{
		map textures/effects/tinfx2c.tga
		blendfunc GL_ONE GL_ZERO
		tcGen environment
		rgbGen identity
	}
}

models/ammo/grenade2_cyl
{
	{
		map textures/effects/envmapgold.tga
		blendfunc GL_ONE GL_ONE
		tcGen environment
                tcmod rotate 30
		//tcMod turb 0 0.2 0 .2
                tcmod scroll 1 .1
		rgbGen wave sin 0.75 0.25 0 1
	}
}

models/ammo/grenade2_cyl_blue
{
	{
		map textures/effects/envmapbfg.tga
		blendfunc GL_ONE GL_ONE
		tcGen environment
                tcmod rotate 30
		//tcMod turb 0 0.2 0 .2
                tcmod scroll 1 .1
		rgbGen wave sin 0.75 0.25 0 1
	}
}

models/ammo/grenade2_spikes
{
	{
		map textures/effects/tinfx2c.tga
		blendfunc GL_ONE GL_ZERO
		tcGen environment
		rgbGen identity
	}
}