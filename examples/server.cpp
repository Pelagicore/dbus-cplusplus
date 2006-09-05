#include "server.h"
#include <signal.h>


class EchoServer : 
	public EchoInterface, 
	public DBus::IntrospectableAdaptor,
	public DBus::ObjectAdaptor
{
public:
	EchoServer( DBus::Connection& conn ) : DBus::ObjectAdaptor(conn, "/org/test/cbus/EchoServer")
	{}

	DBus::Message Echo( const DBus::CallMessage& msg )
	{
		DBus::MessageIter r = msg.r_iter();

		DBus::ReturnMessage ret(msg);
		DBus::MessageIter w = ret.w_iter();

		const char* string = r.get_string();

		w.append_string(string);

		return ret;
	}
};


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
	conn.request_name("org.test.cbus");

	EchoServer server(conn);

	dispatcher.enter();

	return 0;
}
