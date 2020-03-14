////////////////////////////////////////////////////////////////////////////////
// 
// $LastChangedBy: jswigart $
// $LastChangedDate: 2010-09-24 05:36:40 +0200 (Fr, 24 Sep 2010) $
// $LastChangedRevision: 176 $
//
////////////////////////////////////////////////////////////////////////////////

#ifndef __EVENTRECIEVER_H__
#define __EVENTRECIEVER_H__

#include "MessageHelper.h"
//#include "CallbackParameters.h"

class CallbackParameters;

#define HANDLER(evt) case evt: if(_message.GetMessageId()==evt) _cb.DebugName(#evt);

// class: EventReciever
//		Base class to provide event processing capabilities to subclasses
class EventReciever
{
public:

	void SendEvent(const MessageHelper &_message, obuint32 _targetState = 0);
	
	EventReciever();
	virtual ~EventReciever();
protected:
	virtual void ProcessEventImpl(const MessageHelper &_message, obuint32 _targetState);
	virtual void ProcessEvent(const MessageHelper &_message, CallbackParameters &_cb) {}
};

#endif
