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


#ifndef __DBUSXX_PROPERTY_H
#define __DBUSXX_PROPERTY_H

#include "types.h"
#include "interface.h"

namespace DBus {

template <typename T>
class PropertyAdaptor
{
public:

	PropertyAdaptor() : _data(0)
	{}

	void bind( PropertyData& data )
	{
		_data = &data;
	}

	T operator() (void) const
	{
		T t;
		MessageIter ri = _data->value.reader();
		ri >> t;
		return t;
	}

	PropertyAdaptor& operator = ( const T& t )
	{
		_data->value.clear();
		MessageIter wi = _data->value.writer();
		wi << t;
		return *this;
	}

private:

	PropertyData* _data;
};

struct IntrospectedInterface;

class PropertiesAdaptor : public InterfaceAdaptor
{
public:

	PropertiesAdaptor();

	Message Get( const CallMessage& );

	Message Set( const CallMessage& );

protected:

	virtual void on_get_property( InterfaceAdaptor& interface, const String& property, Variant& value )
	{}

	virtual void on_set_property( InterfaceAdaptor& interface, const String& property, const Variant& value )
	{}

	IntrospectedInterface* const introspect() const;
};

class PropertiesProxy : public InterfaceProxy
{
public:

	PropertiesProxy();

	Variant Get( const String& interface, const String& property );

	void Set( const String& interface, const String& property, const Variant& value );
};

} /* namespace DBus */

#endif//__DBUSXX_PROPERTY_H

