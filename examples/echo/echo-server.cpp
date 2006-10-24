#include "echo-server.h"
#include <stdlib.h>
#include <signal.h>

static const char* ECHO_SERVER_NAME = "org.freedesktop.DBus.Examples.Echo";
static const char* ECHO_SERVER_PATH = "/org/freedesktop/DBus/Examples/Echo";

EchoServer::EchoServer( DBus::Connection& connection )
: DBus::ObjectAdaptor(connection, ECHO_SERVER_PATH)
{
}

DBus::Int32 EchoServer::Random()
{
	return rand();
}

DBus::String EchoServer::Hello( const DBus::String& name )
{
	return "Hello " + name + "!";
}

DBus::Variant EchoServer::Echo( const DBus::Variant& value )
{
	this->Echoed(value);

	return value;
}

DBus::Int32 EchoServer::Sum( const std::vector<DBus::Int32>& ints )
{
	DBus::Int32 sum = 0;

	for(size_t i = 0; i < ints.size(); ++i) sum += ints[i];

	return sum;	
}


DBus::BusDispatcher dispatcher;

void niam( int sig )
{
	dispatcher.leave();
}

int main()
{
	signal(SIGTERM, niam);
	signal(SIGINT, niam);

	DBus::default_dispatcher = &dispatcher;

	DBus::Connection conn = DBus::Connection::SessionBus();
	conn.request_name(ECHO_SERVER_NAME);

	EchoServer server(conn);

	dispatcher.enter();

	return 0;
}
