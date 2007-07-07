#include "echo-client.h"
#include <iostream>
#include <pthread.h>
#include <signal.h>

using namespace std;

static const char* ECHO_SERVER_NAME = "org.freedesktop.DBus.Examples.Echo";
static const char* ECHO_SERVER_PATH = "/org/freedesktop/DBus/Examples/Echo";

EchoClient::EchoClient( DBus::Connection& connection, const char* path, const char* name )
: DBus::ObjectProxy(connection, path, name)
{
}

void EchoClient::Echoed( const DBus::Variant& value )
{
	cout << "!";
}

/* NOTE: using many threads is likely to trigger a race condition in the event loop
 * in fact access to the list of timeouts and watches inside the default dispatcher
 * implementation (DBus::BusDispatcher) is NOT serialized
 *
 * (yes, I'm well aware of how much this sucks)
 */
static const int THREADS = 1;

static bool spin = true;

void* greeter_thread( void* )
{
	DBus::Connection conn = DBus::Connection::SessionBus();

	EchoClient client(conn, ECHO_SERVER_PATH, ECHO_SERVER_NAME);

	while(spin)
	{
		client.Hello("client");

		cout << "*";
	}

	return NULL;
}

DBus::BusDispatcher dispatcher;

void niam( int sig )
{
	spin = false;

	dispatcher.leave();
}

int main()
{
	signal(SIGTERM, niam);
	signal(SIGINT, niam);

	DBus::_init_threading();

	DBus::default_dispatcher = &dispatcher;

	pthread_t threads[THREADS];

	for(int i = 0; i < THREADS; ++i)
	{
		pthread_create(threads+i, NULL, greeter_thread, NULL);
	}

	dispatcher.enter();

	for(int i = 0; i < THREADS; ++i)
	{
		pthread_join(threads[i], NULL);
	}

	return 0;
}
