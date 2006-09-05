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


#ifndef __DBUSXX_INTERFACE_H
#define __DBUSXX_INTERFACE_H

#include <string>
#include <map>
#include "util.h"

#include "message.h"

namespace DBus {

class Interface;
class IfaceTracker;

typedef std::map<std::string, Interface*>	InterfaceTable;

class Object;

class CallMessage;
class SignalMessage;

class IntrospectedInterface;

class IfaceTracker
{
public:

	virtual const Object* object() const = 0 ;

protected:

	InterfaceTable	_interfaces;

	virtual ~IfaceTracker()
	{}

	virtual void remit_signal( SignalMessage& )
	{}

	virtual Message rinvoke_method( CallMessage& );
};

typedef std::map< std::string, Slot<Message, const CallMessage&> > MethodTable;
typedef std::map< std::string, Slot<void, const SignalMessage&> > SignalTable;

class Interface : public virtual IfaceTracker
{
public:
	
	Interface( const char* name );
	
	virtual ~Interface();

	inline const std::string& iname() const;

	virtual Message invoke_method( const CallMessage& );

	virtual bool dispatch_signal( const SignalMessage& );

	virtual IntrospectedInterface* const introspect() const
	{
		return NULL;
	}
	
private:

	void register_interface( const char* name );

private:

	std::string _name;
};

/*
*/

const std::string& Interface::iname() const
{
	return _name;
}

/*
*/

class InterfaceAdaptor : public Interface
{
public:

	InterfaceAdaptor( const char* name );

	Message invoke_method( const CallMessage& );

	void emit_signal( const SignalMessage& );

protected:

	MethodTable	_methods;
};

/*
*/

class InterfaceProxy : public Interface
{
public:

	InterfaceProxy( const char* name );

	Message invoke_method( const CallMessage& );

	bool dispatch_signal( const SignalMessage& );

protected:

	SignalTable	_signals;
};

# define register_method(interface, method, callback) \
	InterfaceAdaptor::_methods[ #method ] = \
		new DBus::Callback< interface, DBus::Message, const DBus::CallMessage& >(this, & interface :: callback );
	
# define connect_signal(interface, signal) \
	InterfaceProxy::_signals[ #signal ] = \
		new DBus::Callback< interface, void, const DBus::SignalMessage& >(this, & interface :: method );

} /* namespace DBus */

#endif//__DBUSXX_INTERFACE_H
