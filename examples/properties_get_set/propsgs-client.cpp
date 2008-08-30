#include "propsgs-client.h"
#include <signal.h>
#include <iostream>

using namespace org::freedesktop::DBus;

#define P(x) std::cout << #x  << " = " << x << std::endl;

static const char *PROPS_SERVER_NAME = "org.freedesktop.DBus.Examples.Properties";
static const char *PROPS_SERVER_PATH = "/org/freedesktop/DBus/Examples/Properties";

PropsClient::PropsClient(DBus::Connection &connection, const char *path, const char *name)
: DBus::ObjectProxy(connection, path, name)
{
}

void PropsClient::MessageChanged(const std::string& message){
	std::cout << "message received: " << message << std::endl;
};

DBus::BusDispatcher dispatcher;

PropsClient * client;

void niam(int sig)
{
	dispatcher.leave();
	pthread_exit(NULL);
	delete client;
}

#include <pthread.h>

void * test_property_proxy(void * input){
	P("1");
	sleep(2);
	P(client->Version());

	P("2");
	sleep(1);
	P(client->Message());
	
	P("3");
	sleep(1);
	client->Message( "message set by property access" );
	
	P("4");
	sleep(1);
	P(client->Message());
	
	P("5");
	sleep(1);
	client->Data( 1.1 );
}

int main()
{
	signal(SIGTERM, niam);
	signal(SIGINT, niam);

	DBus::default_dispatcher = &dispatcher;

	DBus::Connection conn = DBus::Connection::SessionBus();

	client = new PropsClient(conn, PROPS_SERVER_PATH, PROPS_SERVER_NAME );

	pthread_t thread;
	pthread_create(&thread, NULL, test_property_proxy, 0);

	P("dispatcher.enter();");

	dispatcher.enter();

	return 0;
}
