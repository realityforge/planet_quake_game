////////////////////////////////////////////////////////////////////////////////
// 
// $LastChangedBy: jswigart $
// $LastChangedDate: 2010-08-28 07:12:05 +0200 (Sa, 28 Aug 2010) $
// $LastChangedRevision: 32 $
//
////////////////////////////////////////////////////////////////////////////////

#ifndef __GMUTILITYLIBRARY_H__
#define __GMUTILITYLIBRARY_H__

/*

Version History:

09/12/2005 Version 0.9.5 OJW
- Initial Release


*/

#include "gmVariable.h"

namespace gmUtility
{
	enum DumpOptions
	{
		DUMP_RECURSE		= (1<<0),
		DUMP_FUNCTIONS		= (1<<1),
		DUMP_REFERENCES		= (1<<2),
		DUMP_TYPEFUNCTIONS	= (1<<3),
		DUMP_ALL			= DUMP_RECURSE|DUMP_FUNCTIONS|DUMP_REFERENCES|DUMP_TYPEFUNCTIONS,
	};

	// Utility functions
	bool DumpGlobals(const String &_file, int _flags);
	bool DumpTable(gmMachine *_machine, const String &_file, const String &_name, int _flags);
	bool DumpTable(gmMachine *_machine, File &outFile, const String &_name, gmTableObject *_tbl, int _flags);
	void DumpTableInfo(gmMachine *_machine, const int _flags, gmTableObject *_table, char *_buffer, int _buflen, int _lvl, File &_file);
};

class gmMachine;
void gmBindUtilityLib(gmMachine * a_machine);

#endif
