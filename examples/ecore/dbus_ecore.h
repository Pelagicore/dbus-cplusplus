#ifndef __DEMO_DBUS_BROWSER_H
#define __DEMO_DBUS_BROWSER_H

#include <dbus-c++/dbus.h>
#include <dbus-c++/ecore-integration.h>
#include <Ecore.h>

#include "dbus_ecore-glue.h"

class DBusBrowser
: public org::freedesktop::DBus,
  public DBus::IntrospectableProxy,
  public DBus::ObjectProxy
{
public:

	DBusBrowser( ::DBus::Connection& conn );

private:

	void NameOwnerChanged( const ::DBus::String&, const ::DBus::String&, const ::DBus::String& );

	void NameLost( const ::DBus::String& );

	void NameAcquired( const ::DBus::String& );

private:

};

#endif//__DEMO_DBUS_BROWSER_H
