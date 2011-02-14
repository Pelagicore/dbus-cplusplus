#ifndef __DEMO_ECHO_SERVER_H
#define __DEMO_ECHO_SERVER_H

#include <dbus-c++/dbus.h>
#include "echo-server-glue.h"

class EchoServer
: public org::freedesktop::DBus::EchoDemo_adaptor,
  public DBus::IntrospectableAdaptor,
  public DBus::ObjectAdaptor
{
public:

	EchoServer(DBus::Connection &connection);

	int32_t Random();

	std::string Hello(const std::string &name);

	DBus::Variant Echo(const DBus::Variant &value);

	std::vector< uint8_t > Cat(const std::string &file);

	int32_t Sum(const std::vector<int32_t> & ints);

	std::map< std::string, std::string > Info();

  std::vector< ::DBus::Struct< std::vector< ::DBus::Struct< uint32_t, uint32_t > >, std::string > > Foo() {};

  void Foo2(const int32_t& direction, int32_t& result, std::vector< ::DBus::Struct< std::vector< ::DBus::Struct< uint32_t, uint32_t > >, std::string > >& array) {};
};

#endif//__DEMO_ECHO_SERVER_H
