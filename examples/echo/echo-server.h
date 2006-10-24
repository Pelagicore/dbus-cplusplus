#ifndef __DEMO_ECHO_SERVER_H
#define __DEMO_ECHO_SERVER_H

#include <dbus-c++/dbus.h>
#include "echo-glue.h"

class EchoServer
: public org::freedesktop::DBus::EchoDemo,
  public DBus::IntrospectableAdaptor,
  public DBus::ObjectAdaptor
{
public:

	EchoServer( DBus::Connection& connection );

	DBus::Int32 Random();

	DBus::String Hello( const DBus::String & name );

	DBus::Variant Echo( const DBus::Variant & value );

	DBus::Int32 Sum( const std::vector<DBus::Int32> & ints );
};

#endif//__DEMO_ECHO_SERVER_H
