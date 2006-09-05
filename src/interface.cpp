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
#include <dbus-c++/interface.h>

#include "internalerror.h"

using namespace DBus;

Message IfaceTracker::rinvoke_method( CallMessage& msg )
{
	return ErrorMessage(msg, DBUS_ERROR_NOT_SUPPORTED, "there are no proxies in a InterfaceAdaptor");
}

Interface::Interface( const char* name )
:	_name(name)
{
	register_interface(name);
}

Interface::~Interface()
{
	//unregister_interface(name);	//not needed
}

Message Interface::invoke_method( const CallMessage& )
{
	throw Error(DBUS_ERROR_NOT_SUPPORTED, "");
}

bool Interface::dispatch_signal( const SignalMessage& )
{
	throw Error(DBUS_ERROR_NOT_SUPPORTED, "");
}

void Interface::register_interface( const char* name )
{
	debug_log("registering interface %s",name);

	_interfaces[name] = this;
}

InterfaceAdaptor::InterfaceAdaptor( const char* name )
:	Interface(name)
{}

Message InterfaceAdaptor::invoke_method( const CallMessage& msg )
{
	const char* name = msg.member();

	MethodTable::iterator mi = _methods.find(name);
	if( mi != _methods.end() )
	{
		return mi->second.call( msg );
	}
	else
	{
		return ErrorMessage(msg, DBUS_ERROR_UNKNOWN_METHOD, name);
	}
}

void InterfaceAdaptor::emit_signal( const SignalMessage& sig )
{
	SignalMessage& sig2 = const_cast<SignalMessage&>(sig);

	sig2.interface( iname().c_str() );
	remit_signal(sig2);
}

InterfaceProxy::InterfaceProxy( const char* name )
:	Interface(name)
{}

bool InterfaceProxy::dispatch_signal( const SignalMessage& msg )
{
	const char* name = msg.member();

	SignalTable::iterator si = _signals.find(name);
	if( si != _signals.end() )
	{
		si->second.call( msg );
		return true;
	}
	else	
	{
		return false;
	}
}

Message InterfaceProxy::invoke_method( const CallMessage& call )
{
	CallMessage& call2 = const_cast<CallMessage&>(call);

	call2.interface( iname().c_str() );
	return rinvoke_method(call2);
}
