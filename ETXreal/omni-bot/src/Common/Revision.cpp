////////////////////////////////////////////////////////////////////////////////
// 
// $LastChangedBy: ken.nickel $
// $LastChangedDate: 2010-09-18 22:01:41 +0200 (Sa, 18 Sep 2010) $
// $LastChangedRevision: 142 $
//
////////////////////////////////////////////////////////////////////////////////

#include "PrecompCommon.h"
#include "Revision.h"

namespace Revision
{
	String Number()
	{
		String RevisionNumber = "$LastChangedRevision: 142 $";
		size_t numStart = RevisionNumber.find_last_of(":")+2;
		size_t numEnd = RevisionNumber.find_last_of(" ");
		return RevisionNumber.substr(numStart,numEnd-numStart);
	}
	String Date()
	{
		String RevisionDate = "$LastChangedDate: 2010-09-18 22:01:41 +0200 (Sa, 18 Sep 2010) $";
		size_t dateStart = RevisionDate.find_last_of("(")+1;
		size_t dateEnd = RevisionDate.find_last_of(")");
		return RevisionDate.substr(dateStart,dateEnd-dateStart);
	}
};




































































































































