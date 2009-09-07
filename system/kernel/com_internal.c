/* -------------------------------- Arctic Core ------------------------------
 * Arctic Core - the open source AUTOSAR platform http://arccore.com
 *
 * Copyright (C) 2009  ArcCore AB <contact@arccore.com>
 *
 * This source code is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 as published by the
 * Free Software Foundation; See <http://www.gnu.org/licenses/old-licenses/gpl-2.0.txt>.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 * for more details.
 * -------------------------------- Arctic Core ------------------------------*/









#include "Os.h"
#include "kernel.h"
#include "ext_config.h"
#include <assert.h>
#include <string.h>


/* Queued message
 *   The messages are put in a queue. Copied to the destination...
 *
 * Unqueued message
 *   Latest message in queue. Put the message in a container ref by message_id.
 *   It's just a copy. The message can be read by "anyone" and as many times as you
 *   would like
 *
 * Add new functions ??
 *   SendMessageNoCopy( ) .. must have GetMsgApa().. this gets to be a resource lock... hmm
 *
 *
 *
 */

StatusType SendMessage( MessageType message_id, ApplicationDataRef dataRef ) {


	message_obj_t *msg;

	// Is the message valid ?
	if( message_id > Oil_GetMessageCnt() ) {
		// TODO: Add error hook here
		return E_COM_ID;
	}

	if( msg->property != SEND_STATIC_INTERNAL ) {
		// TODO: Add error hook here
		return E_COM_ID;
	}

	// Copy the data to interal buffers
	msg = Oil_GetMessage(message_id);

	// copy data
	memcpy(msg->data,dataRef,msg->data_size);


#if 0
	// Is it a queue message?
	switch( msg->notification ) {
	case MESSAGE_NOTIFICATION_ACTION_ACTIVATETASK:
		// TODO: Is this activatetask ???
		break;
	case MESSAGE_NOTIFICATION_ACTION_SETEVENT:
		// TODO:
		break;
	case MESSAGE_NOTIFICATION_ACTION_NONE:
		break;
	default:
		assert(0);
		break;
	}
#endif

	return E_OK;
}

StatusType ReceiveMessage( MessageType message_id, ApplicationDataRef dataRef ) {
	message_obj_t *msg;
	// Check if valid

	// Copy from container to dataRef
	msg = Oil_GetMessage(message_id);
	memcpy(dataRef,msg->data,msg->data_size);

	return E_OK;
}



