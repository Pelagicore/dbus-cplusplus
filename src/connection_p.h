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


#ifndef __DBUSXX_CONNECTION_P_H
#define __DBUSXX_CONNECTION_P_H

#include <dbus-c++/connection.h>
#include <dbus-c++/dispatcher.h>
#include <dbus-c++/refptr_impl.h>

#include <dbus/dbus.h>

#include <string>

namespace DBus {

struct Connection::Private
{
	DBusConnection*	conn;

	std::vector<std::string>	names;

	Dispatcher* dispatcher;
	bool do_dispatch();

	MessageSlot disconn_filter;
	bool disconn_filter_function( const Message& );

	Private( DBusConnection* );

	Private( DBusBusType );

	~Private();

	void init();

	static void dispatch_status_stub( DBusConnection*, DBusDispatchStatus, void* );

	static DBusHandlerResult message_filter_stub( DBusConnection*, DBusMessage*, void* );
};

} /* namespace DBus */

#endif//__DBUSXX_CONNECTION_P_H
