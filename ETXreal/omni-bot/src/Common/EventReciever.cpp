////////////////////////////////////////////////////////////////////////////////
// 
// $LastChangedBy: jswigart $
// $LastChangedDate: 2010-08-28 07:12:05 +0200 (Sa, 28 Aug 2010) $
// $LastChangedRevision: 32 $
//
////////////////////////////////////////////////////////////////////////////////

#include "PrecompCommon.h"
#include "EventReciever.h"
#include "IGame.h"
#include "ScriptManager.h"

EventReciever::EventReciever()
{
}

EventReciever::~EventReciever()
{
}

void EventReciever::SendEvent(const MessageHelper &_message, obuint32 _targetState)
{
	ProcessEventImpl(_message,_targetState);
}

void EventReciever::ProcessEventImpl(const MessageHelper &_message, obuint32 _targetState)
{
	// Subclasses can override this function to perform additional stuff
	gmMachine *pMachine = ScriptManager::GetInstance()->GetMachine();
	CallbackParameters cb(_message.GetMessageId(), pMachine);
	ProcessEvent(_message, cb);
}
