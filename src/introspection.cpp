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


#include <dbus-c++/introspection.h>
#include <dbus-c++/message.h>
#include <dbus-c++/xml.h>

#include <dbus/dbus.h>

using namespace DBus;

static const char* introspectable_name = "org.freedesktop.DBus.Introspectable";

IntrospectableAdaptor::IntrospectableAdaptor()
: InterfaceAdaptor(introspectable_name)
{
	register_method(IntrospectableAdaptor, Introspect, Introspect);
}

Message IntrospectableAdaptor::Introspect( const CallMessage& call )
{
	debug_log("requested introspection data");
	
	Xml::Node iroot("node");

	InterfaceTable::const_iterator iti;

	for(iti = _interfaces.begin(); iti != _interfaces.end(); ++iti)
	{
		debug_log("introspecting interface %s", iti->first.c_str());

		IntrospectedInterface* const intro = iti->second->introspect();
		if(intro)
		{
			Xml::Node& iface = iroot.add(Xml::Node("interface"));
			iface.set("name", intro->name);

			for(const IntrospectedMethod* m = intro->methods; m->args; ++m)
			{
				Xml::Node& method = iface.add(Xml::Node("method"));
				method.set("name", m->name);

				for(const IntrospectedArgument* a = m->args; a->type; ++a)
				{
					Xml::Node& arg = method.add(Xml::Node("arg"));
					if(a->name) arg.set("name", a->name);
					arg.set("direction", a->in ? "in" : "out");
					arg.set("type", a->type);
				}
			}

			for(const IntrospectedMethod* m = intro->signals; m->args; ++m)
			{
				Xml::Node& method = iface.add(Xml::Node("signal"));
				method.set("name", m->name);

				for(const IntrospectedArgument* a = m->args; a->type; ++a)
				{
					Xml::Node& arg = method.add(Xml::Node("arg"));
					if(a->name) arg.set("name", a->name);
					arg.set("type", a->type);
				}
			}
		}
	}
	std::string xml = DBUS_INTROSPECT_1_0_XML_DOCTYPE_DECL_NODE + iroot.to_xml();

	ReturnMessage reply(call);
	MessageIter wi = reply.w_iter();
	wi.append_string(xml.c_str());
	return reply;
}

IntrospectedInterface* const IntrospectableAdaptor::introspect() const
{
	static IntrospectedArgument Introspect_args[] =
	{
		{ "data", "s", false },
		{ 0, 0, 0 }
	};
	static IntrospectedMethod Introspectable_methods[] =
	{
		{ "Introspect", Introspect_args },
		{ 0, 0 }
	};
	static IntrospectedMethod Introspectable_signals[] =
	{
		{ 0, 0 }
	};
	static IntrospectedInterface Introspectable_interface =
	{
		introspectable_name, Introspectable_methods, Introspectable_signals
	};
	return &Introspectable_interface;
}

IntrospectableProxy::IntrospectableProxy()
: InterfaceProxy(introspectable_name)
{}

std::string IntrospectableProxy::Introspect()
{
	DBus::CallMessage call;

	call.member("Introspect");

	DBus::Message ret = invoke_method(call);

	DBus::MessageIter ri = ret.r_iter();
	const char* str = ri.get_string();

	return str;
}
