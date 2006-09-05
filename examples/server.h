#ifndef __ECHO_SERVER_H
#define __ECHO_SERVER_H

#include <dbus-c++/dbus-c++.h>

class EchoInterface : 
	public DBus::InterfaceAdaptor
{
public:

	EchoInterface() : DBus::InterfaceAdaptor("org.test.cbus.EchoServer")
	{
		register_method(EchoInterface, Echo, Echo);
	}

	virtual DBus::Message Echo( const DBus::CallMessage& ) = 0;
};

#endif//__ECHO_SERVER_H
