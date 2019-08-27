// UI graphics
console
{
	nopicmip
	nomipmaps
    
	{
		map gfx/ui/ui.png
		blendFunc GL_ONE GL_ZERO
	}
	
	{
		map textures/effects/invismap.tga
		blendFunc add
		tcMod turb 0 .1 0 .1
		tcMod scale 4 4
		tcmod scroll 0.2  .1
	} 	
}

menuback
{
	nopicmip
	{
		map gfx/ui/ui.png
		rgbgen vertex
	}
}

sb/sbbottom
{
	nopicmip
	{
		map gfx/sb/sbbottom.png
		rgbgen vertex
		blendfunc blend
	}
}

sb/sbback
{
	nopicmip
	{
		map gfx/sb/sbback.png
		rgbgen vertex
		blendfunc filter
	}
}

sb/sbheader
{
	nopicmip
	{
		map gfx/sb/sbheader.png
		rgbgen vertex
		blendfunc add
	}
}

sb/rocket
{
	nopicmip
	nomipmaps
	{
		map gfx/sb/rocket.tga
		blendfunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		rgbgen vertex
	}
}

sb/plasma
{
	nopicmip
	nomipmaps
	{
		map gfx/sb/plasma.tga
		blendfunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		rgbgen vertex
	}
}

sb/grenade
{
	nopicmip
	nomipmaps
	{
		map gfx/sb/grenade.tga
		blendfunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		rgbgen vertex
	}
}

sb/slick
{
	nopicmip
	nomipmaps
	{
		map gfx/sb/slick.tga
		blendfunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		rgbgen vertex
	}
}

sb/tele
{
	nopicmip
	nomipmaps
	{
		map gfx/sb/tele.tga
		blendfunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		rgbgen vertex
	}
}

sb/pad
{
	nopicmip
	nomipmaps
	{
		map gfx/sb/pad.tga
		blendfunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		rgbgen vertex
	}
}

sb/door
{
	nopicmip
	nomipmaps
	{
		map gfx/sb/door.tga
		blendfunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		rgbgen vertex
	}
}

sb/obs
{
	nopicmip
	nomipmaps
	{
		map gfx/sb/obs.tga
		blendfunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		rgbgen vertex
	}
}

sb/interference
{
	nopicmip
	nomipmaps
	{
		map gfx/sb/interference.tga
		blendfunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		rgbgen vertex
	}
}

sb/reverse
{
	nopicmip
	nomipmaps
	{
		map gfx/sb/reverse.tga
		blendfunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		rgbgen vertex
	}
}

gfx/ui/frame
{
	nopicmip
	{
		map gfx/ui/frame.png
		blendfunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		rgbgen identity
	}
}

bbox
{
	nopicmip
	cull none
	{
		map gfx/misc/bbox.tga
		blendFunc GL_ONE GL_ONE
		rgbGen vertex
	}
}

bbox_bottom
{
	nopicmip
	cull none
	polygonOffset
	{
		map gfx/misc/bbox2.tga
		blendFunc GL_ONE GL_ONE
		rgbGen vertex
	}
}

lagometer
{
	nopicmip
	{
		map *white
		blendfunc filter
		rgbgen vertex
	}
}

// weapon select frame
gfx/2d/select2
{
	nopicmip
	{
		map gfx/2d/select3.tga
		blendfunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		alphaGen vertex
	}
}

// defrag's item not available
sprites/notAvailable
{
	nopicmip
	sort 10
	{
		map icons/noammo.tga
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	}
}

hud/timerframe
{
	nopicmip
	nomipmaps
	{
		map gfx/hud/timerframe.tga
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		rgbgen vertex
	}
}

hud/checkpointframe
{
	nopicmip
	nomipmaps
	{
		map gfx/hud/timerframe.tga		// will add 8 in total to keep proportional
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		rgbgen vertex
	}
}


hud/health
{
	nopicmip
	nomipmaps
	{
		map gfx/hud/health.tga
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		rgbgen vertex
	}
}

hud/armor
{
	nopicmip
	nomipmaps
	{
		map gfx/hud/armor.tga
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		rgbgen vertex
	}
}

hud/clock
{
	nopicmip
	nomipmaps
	{
		map gfx/hud/clock.tga
		blendfunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		rgbgen vertex
	}
}

hud/key
{
	nopicmip
	nomipmaps
	{
		map icons/key.tga
		blendfunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		rgbgen vertex
	}
}

grapplingChain
{
	nopicmip
	{
		map gfx/misc/grapplingchain.tga
		tcmod scale 16 1
		blendfunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		rgbgen vertex
	}
}

//
// PARTICLE EFFECTS
//
spark
{
	cull none
	{
		map sprites/spark.tga
		blendFunc add
		rgbGen vertex
	}
}

icons/iconr_green
{
	nopicmip
	{
		map icons/iconr_green.tga
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	}
}

models/powerups/armor/energy_grn2
{
	{
		map models/powerups/armor/energy_grn3.tga
		blendFunc GL_ONE GL_ONE
		tcMod scroll 7.4 1.3
	}
}

models/powerups/armor/newgreen
{
	{
		map textures/sfx/specular.tga
		tcGen environment
		rgbGen identity
	}
	{
		map models/powerups/armor/newgreen.tga
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		rgbGen identity
	}
}

models/flags/start_flag2
{
	deformVertexes wave 100 sin 4 3 0 0.3
	deformVertexes wave 30 sin 0 3 0 .8
	deformVertexes normal .3 .2
	cull none

	{
		map models/flags/start_flag2.tga
		rgbGen identity
	}
	{
		map models/flags/start_fx.tga
		tcGen environment
		blendFunc GL_ONE GL_ONE
		rgbGen identity
	}
	{
		map models/flags/start_flag2.tga
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		rgbGen identity
	}
	{
		map textures/sfx/shadow.tga
		tcGen environment
		blendFunc GL_DST_COLOR GL_ZERO
		rgbGen identity
	}
}

models/flags/finish_flag2
{
	deformVertexes wave 100 sin 4 3 0 0.3
	deformVertexes wave 30 sin 0 3 0 .8
	deformVertexes normal .3 .2
	cull none

	{
		map models/flags/finish_flag2.tga
		rgbGen identity
	}
	{
		map models/flags/finish_fx.tga
		tcGen environment
		blendFunc GL_ONE GL_ONE
		rgbGen identity
	}
	{
		map models/flags/finish_flag2.tga
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		rgbGen identity
	}
	{
		map textures/sfx/shadow.tga
		tcGen environment
		blendFunc GL_DST_COLOR GL_ZERO
		rgbGen identity
	}
}


models/weapons2/grapple/grapple
{
	{
		map models/weapons2/grapple/grapple.tga
		blendFunc GL_ONE GL_ZERO
		rgbGen lightingDiffuse
	}
	{
		map textures/sfx/specular.tga
		blendFunc GL_ONE GL_ONE
		tcmod scroll .1 .1
		tcGen environment
		rgbGen lightingDiffuse
	}
	{
		map models/weapons2/grapple/grapple.tga
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		rgbGen lightingDiffuse
	}
}

models/weapons2/grapple/grapple_j
{
    cull disable
	{
		map models/weapons2/grapple/grapple_j.tga
		blendFunc GL_ONE GL_ZERO
		tcMod turb 0 .5 0 .7
		tcMod scroll 1 -1
		tcmod scale .5 .5
		rgbGen identity
	}
	{
		map textures/liquids/jello2.tga
		blendFunc GL_ONE GL_ONE
		tcmod scale .7 .7
		tcMod turb 0 .4 0 .3
		tcMod scroll .7 -.4
		rgbGen identity
	}
	{
		map textures/effects/tinfx2.tga
		tcGen environment
		blendFunc GL_ONE GL_ONE
		rgbGen lightingDiffuse
	}
}

models/weapons2/grapple/grapple_h
{
    cull disable
    {
		map models/weapons2/grapple/grapple_h.tga
		alphaFunc GE128
		depthWrite
		rgbGen lightingDiffuse
	}
}


gfx/crosshairs/crosshaira
{
	nopicmip
	{
		map gfx/crosshairs/crosshaira.tga
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
        rgbgen vertex
	}
}

gfx/crosshairs/crosshairb
{
	nopicmip
	{
		map gfx/crosshairs/crosshairb.tga
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		rgbgen vertex
	}
}

gfx/crosshairs/crosshairc
{
	nopicmip
	{
		map gfx/crosshairs/crosshairc.tga
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		rgbgen vertex
	}
}

gfx/crosshairs/crosshaird
{
	nopicmip
	{
		map gfx/crosshairs/crosshaird.tga
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		rgbgen vertex
	}
}

gfx/crosshairs/crosshaire
{
	nopicmip
	{
		map gfx/crosshairs/crosshaire.tga
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		rgbgen vertex
	}
}

gfx/crosshairs/crosshairf
{
	nopicmip
	{
		map gfx/crosshairs/crosshairf.tga
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		rgbgen vertex
	}
}

gfx/crosshairs/crosshairg
{
	nopicmip
	{
		map gfx/crosshairs/crosshairg.tga
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		rgbgen vertex
	}
}

gfx/crosshairs/crosshairh
{
	nopicmip
	{
		map gfx/crosshairs/crosshairh.tga
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		rgbgen vertex
	}
}

gfx/crosshairs/crosshairi
{
	nopicmip
	{
		map gfx/crosshairs/crosshairi.tga
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		rgbgen vertex
	}

}

gfx/crosshairs/crosshairj
{
	nopicmip
	{
		map gfx/crosshairs/crosshairj.tga
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		rgbgen vertex
	}
}

gfx/crosshairs/crosshairk
{
	nopicmip
	{
		map gfx/crosshairs/crosshairk.tga
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		rgbgen vertex
	}
}

gfx/crosshairs/crosshairl
{
	nopicmip
	{
		map gfx/crosshairs/crosshairl.tga
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		rgbgen vertex
	}
}

gfx/crosshairs/crosshairm
{
	nopicmip
	{
		map gfx/crosshairs/crosshairm.tga
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		rgbgen vertex
	}
}

gfx/crosshairs/crosshairn
{
	nopicmip
	{
		map gfx/crosshairs/crosshairn.tga
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		rgbgen vertex
	}
}

gfx/crosshairs/crosshairo
{
	nopicmip
	{
		map gfx/crosshairs/crosshairo.tga
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		rgbgen vertex
	}
}

gfx/crosshairs/crosshairp
{
	nopicmip
	{
		map gfx/crosshairs/crosshairp.tga
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		rgbgen vertex
	}
}

gfx/crosshairs/crosshairq
{
	nopicmip
	{
		map gfx/crosshairs/crosshairq.tga
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		rgbgen vertex
	}
}

gfx/crosshairs/crosshairr
{
	nopicmip
	{
		map gfx/crosshairs/crosshairr.tga
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		rgbgen vertex
	}
}

gfx/crosshairs/crosshairs
{
	nopicmip
	{
		map gfx/crosshairs/crosshairs.tga
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		rgbgen vertex
	}
}
