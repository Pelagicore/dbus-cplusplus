/*
 *
 *  D-Bus++ - C++ bindings for DBus
 *
 *  Copyright (C) 2005-2006  Paolo Durante <shackan@gmail.com>
 *
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */


#include <dbus-c++/debug.h>
#include <dbus-c++/object.h>
#include "internalerror.h"

#include <dbus/dbus.h>

#include "message_p.h"
#include "connection_p.h"

using namespace DBus;

Object::Object( Connection& conn, const char* path, const char* service )
:	_conn(conn), _path(path), _service(service ? service : "")
{
}

Object::~Object()
{
}

struct ObjectAdaptor::Private
{
	static void unregister_function_stub( DBusConnection*, void* );
	static DBusHandlerResult message_function_stub( DBusConnection*, DBusMessage*, void* );
};

static DBusObjectPathVTable _vtable =
{	
	ObjectAdaptor::Private::unregister_function_stub,
	ObjectAdaptor::Private::message_function_stub,
	NULL, NULL, NULL, NULL
};

void ObjectAdaptor::Private::unregister_function_stub( DBusConnection* conn, void* data )
{
 	// XXX: what do we have to do here ?
}

DBusHandlerResult ObjectAdaptor::Private::message_function_stub( DBusConnection*, DBusMessage* dmsg, void* data )
{
	ObjectAdaptor* o = static_cast<ObjectAdaptor*>(data);

	if( o )
	{
		Message msg(new Message::Private(dmsg));	

		debug_log("in object %s", o->path().c_str());
		debug_log(" got message #%d from %s to %s",
			msg.serial(),
			msg.sender(),
			msg.destination()
		);

		return o->handle_message(msg)
			? DBUS_HANDLER_RESULT_HANDLED
			: DBUS_HANDLER_RESULT_NOT_YET_HANDLED;
	}
	else
	{
		return DBUS_HANDLER_RESULT_NOT_YET_HANDLED;
	}
}

ObjectAdaptor::ObjectAdaptor( Connection& conn, const char* path )
:	Object(conn, path, conn.unique_name())
{
	register_obj();
}

ObjectAdaptor::~ObjectAdaptor()
{
	unregister_obj();
}

void ObjectAdaptor::register_obj()
{
	debug_log("registering local object %s", path().c_str());

	if(!dbus_connection_register_object_path(conn()._pvt->conn, path().c_str(), &_vtable, this))
	{
 		throw Error(DBUS_ERROR_NO_MEMORY,"unable to register object path");
	}
	else
	{
		InterfaceTable::const_iterator ii = _interfaces.begin();
		while( ii != _interfaces.end() )
		{
			std::string im = "type='method_call',interface='"+ii->first+"'";
			conn().add_match(im.c_str());
			++ii;
		}
	}
}

void ObjectAdaptor::unregister_obj()
{
	debug_log("unregistering local object %s", path().c_str());

	dbus_connection_unregister_object_path(conn()._pvt->conn, path().c_str());

	InterfaceTable::const_iterator ii = _interfaces.begin();
	while( ii != _interfaces.end() )
	{
		std::string im = "type='method_call',interface='"+ii->first+"'";
		conn().remove_match(im.c_str());
		++ii;
	}
}

void ObjectAdaptor::remit_signal( SignalMessage& sig )
{
	sig.path(path().c_str());

	conn().send(sig);
}

struct WontReturnException
{
	const void* tag;
};

bool ObjectAdaptor::handle_message( const Message& msg )
{
	switch( msg.type() )
	{
		case DBUS_MESSAGE_TYPE_METHOD_CALL:
		{
			const CallMessage& cmsg = reinterpret_cast<const CallMessage&>(msg);
			const char* member		= cmsg.member();
			const char* interface	= cmsg.interface();

			debug_log(" invoking method %s.%s", interface, member);
		
			InterfaceTable::const_iterator ii = _interfaces.find(interface);
			if( ii != _interfaces.end() )
			{
				try
				{
					Message ret = ii->second->invoke_method(cmsg);
					conn().send(ret);
				}
				catch(Error& e)
				{
					ErrorMessage em(cmsg, e.name(), e.message());
					conn().send(em);
				}
				catch(WontReturnException& wre)
				{
					_deferred_returns[wre.tag] = new DeferredReturn(conn(), cmsg, wre.tag);
				}
				return true;
			}
			else
			{
				return false;
			}
		}
		default:
		{
			return false;
		}
	}
}

void ObjectAdaptor::return_later( const void* tag )
{
	WontReturnException wre = { tag };

	throw wre;
}

void ObjectAdaptor::return_now( DeferredReturn* ret )
{
	ret->_conn.send(ret->_return);

	DeferredReturnMap::iterator di = _deferred_returns.find(ret->_tag);

	delete di->second;

	_deferred_returns.erase(di);
}

void ObjectAdaptor::return_error( DeferredReturn* ret, Error& error )
{
	ret->_conn.send(ErrorMessage(ret->_call, error.name(), error.message()));

	DeferredReturnMap::iterator di = _deferred_returns.find(ret->_tag);

	delete di->second;

	_deferred_returns.erase(di);
}

ObjectAdaptor::DeferredReturn* ObjectAdaptor::find_return( const void* tag )
{
	DeferredReturnMap::iterator di = _deferred_returns.find(tag);

	return di != _deferred_returns.end() ? di->second : NULL;
}

ObjectAdaptor::DeferredReturn::DeferredReturn( Connection& conn, const CallMessage& call, const void* tag )
: _conn(conn), _call(call), _return(_call), _tag(tag)
{
	_writer = _return.w_iter(); //todo: verify
}

/*
*/

ObjectProxy::ObjectProxy( Connection& conn, const char* path, const char* service )
:	Object(conn, path, service)
{
	register_obj();
}

ObjectProxy::~ObjectProxy()
{
	unregister_obj();
}

void ObjectProxy::register_obj()
{
	debug_log("registering remote object %s", path().c_str());

	_filtered = new Callback<ObjectProxy, bool, const Message&>(this, &ObjectProxy::handle_message);
	
	conn().add_filter(_filtered);

	InterfaceTable::const_iterator ii = _interfaces.begin();
	while( ii != _interfaces.end() )
	{
		std::string im = "type='signal',interface='"+ii->first+"'";
		conn().add_match(im.c_str());
		++ii;
	}
	
//	conn().add_match("type='signal'");
//	conn().add_match("type='method_call'");
}

void ObjectProxy::unregister_obj()
{
	debug_log("unregistering remote object %s", path().c_str());
	
	InterfaceTable::const_iterator ii = _interfaces.begin();
	while( ii != _interfaces.end() )
	{
		std::string im = "type='signal',interface='"+ii->first+"'";
		conn().remove_match(im.c_str());
		++ii;
	}
//	conn().remove_match("type='method_call'");
//	conn().remove_match("type='signal'");

	conn().remove_filter(_filtered);
}

Message ObjectProxy::rinvoke_method( CallMessage& call )
{
	call.path(path().c_str());
	call.destination(service().c_str());

	return conn().send_blocking(call, 2000);
}

bool ObjectProxy::handle_message( const Message& msg )
{
	switch( msg.type() )
	{
		case DBUS_MESSAGE_TYPE_SIGNAL:
		{
			const SignalMessage& smsg = reinterpret_cast<const SignalMessage&>(msg);
			const char* interface	= smsg.interface();
			const char* member	= smsg.member();

			debug_log("filtered signal %s(in %s) from remote object %s", member, interface, msg.sender());

			InterfaceTable::const_iterator ii = _interfaces.find(interface);
			if( ii != _interfaces.end() )
			{
				return ii->second->dispatch_signal(smsg);
			}
			else
			{
				return false;
			}
		}
		case DBUS_MESSAGE_TYPE_METHOD_RETURN:
		{
			debug_log("filtered method return from remote object %s", msg.sender());

			//TODO:
			return false;
		}
		case DBUS_MESSAGE_TYPE_ERROR:
		{
			debug_log("filtered error from remote object %s", msg.sender());

			//TODO:
			return false;
		}
		default:
		{
			return false;
		}
	}
}
