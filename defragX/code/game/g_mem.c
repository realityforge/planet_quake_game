#include "g_local.h"

#define POOLSIZE	(768 * 1024)

static char		memoryPool[POOLSIZE];
static int		allocPoint;

void *G_Alloc( size_t size )
{
	char	*p;

	if ( g_debugAlloc.integer ) {
		G_Printf( "G_Alloc of %i bytes (%i left)\n", size, POOLSIZE - allocPoint - ( ( size + 31 ) & ~31 ) );
	}

	if ( allocPoint + size > POOLSIZE ) {
		G_Error( "G_Alloc: failed on allocation of %i bytes", size );
		return NULL;
	}

	p = &memoryPool[allocPoint];

	allocPoint += ( size + 31 ) & ~31;

	return p;
}

void G_InitMemory( void )
{
	allocPoint = 0;
}

void Svcmd_GameMem_f( void )
{
	G_Printf( "Game memory status: %i out of %i bytes allocated\n", allocPoint, POOLSIZE );
}
