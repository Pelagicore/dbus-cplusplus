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


#include <dbus-c++/types.h>
#include <dbus-c++/object.h>
#include <dbus/dbus.h>
#include <stdarg.h>

#include "message_p.h"
#include "internalerror.h"

using namespace DBus;

Variant::Variant()
: _msg(CallMessage()), // dummy message used as temporary storage for variant data
  _it(_msg.w_iter())
{
}

Variant::Variant( MessageIter& it )
: _msg(it.msg()),
  _it(it.recurse())
{
}

Variant& Variant::operator = ( const Variant& v )
{
	if(&v != this)
	{
		_msg = v._msg;
		_it = v._it;
		_signature = v._signature;
	}
	return *this;
}

const char* Variant::signature() const
{
	char* sigbuf = _it.signature();

	_signature = sigbuf;

	free(sigbuf);

	return _signature.c_str();
}

MessageIter& operator << ( MessageIter& iter, const Variant& val )
{
	const char* sig = val.signature();

	MessageIter rit = val.iter();
	MessageIter wit = iter.new_variant(sig);

	for(size_t i = 0; i < strlen(sig); ++i)
	{
		switch(sig[i])
		{
			case 'b':
			{
				Bool b; rit >> b; wit << b;
				break;
			}
			case 'y':
			{
				Byte y; rit >> y; wit << y;
				break;
			}
			case 'n':
			{
				Int16 n; rit >> n; wit << n;
				break;
			}
			case 'q':
			{
				UInt16 q; rit >> q; wit << q;
				break;
			}
			case 'i':
			{
				Int32 i; rit >> i; wit << i;
				break;
			}
			case 'u':
			{
				UInt32 u; rit >> u; wit << u;
				break;
			}
			case 'x':
			{
				Int64 x; rit >> x; wit << x;
				break;
			}
			case 't':
			{
				UInt64 t; rit >> t; wit << t;
				break;
			}
			case 'd':
			{
				Double d; rit >> d; wit << d;
				break;
			}
			case 's':
			{
				String s; rit >> s; wit << s;
				break;
			}
			case 'o':
			{
				Path o; rit >> o; wit << o;
				break;
			}
			case 'g':
			{
				Signature g; rit >> g; wit << g;
				break;
			}
			case 'a':
			{
				break;
			}
			case '(':
			{
				break;
			}
			case ')':
			{
				break;
			}
			case '{':
			{
				break;
			}
			case '}':
			{
				break;
			}
		}
	}

	//TODO: implement marshaling of compound types

	iter.close_container(wit);

	return iter;
}

MessageIter& operator >> ( MessageIter& iter, Variant& val )
{
	//TODO: check if iter really points to a variant

	val = Variant(iter);
	return ++iter;
}

