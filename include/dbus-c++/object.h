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


#ifndef __DBUSXX_OBJECT_H
#define __DBUSXX_OBJECT_H

#include <string>
#include <list>
#include <map>

#include "interface.h"
#include "connection.h"
#include "message.h"
#include "types.h"

namespace DBus {

class Object;

typedef std::list<Object*>	ObjectPList;

class ObjectAdaptor;

class ObjectProxy;

class Object : protected virtual IfaceTracker
{
protected:

	Object( Connection& conn, const char* path, const char* service );
	
public:

	virtual ~Object();

	inline const DBus::Path& path() const;

	inline const std::string& service() const;

	inline const Object* object() const;
	
 	inline Connection& conn();

private:

	//Object( const Object& );

	virtual bool handle_message( const Message& ) = 0;
	virtual void register_obj() = 0;
	virtual void unregister_obj() = 0;

private:

	Connection	_conn;
	DBus::Path	_path;
	std::string	_service;
};

/*
*/

const Object* Object::object() const
{
	return this;
}

Connection& Object::conn()
{
	return _conn;
}

const DBus::Path& Object::path() const
{
	return _path;
}

const std::string& Object::service() const
{
	return _service;
}

/*
*/

class ObjectAdaptor : public Object
{
public:

	struct Private;

	ObjectAdaptor( Connection& conn, const char* path );

	~ObjectAdaptor();

	void remit_signal( SignalMessage& );

protected:

	class DeferredReturn
	{
	public:

		inline MessageIter& writer();

		inline void* tag();

	private:

		DeferredReturn( Connection& conn, const CallMessage& call, const void* tag );

		Connection _conn;
		CallMessage _call;
		MessageIter _writer;
		ReturnMessage _return;
		const void* _tag;

	friend class ObjectAdaptor;
	};

	void return_later( const void* tag );

	void return_now( DeferredReturn* ret );

	void return_error( DeferredReturn* ret, Error& error );

	DeferredReturn* find_return( const void* tag );

private:

	bool handle_message( const Message& );

	void register_obj();
	void unregister_obj();

	typedef std::map<const void*, DeferredReturn*> DeferredReturnMap;
	DeferredReturnMap _deferred_returns;

friend struct Private;
};

void* ObjectAdaptor::DeferredReturn::tag()
{
	return const_cast<void*>(_tag);
}

MessageIter& ObjectAdaptor::DeferredReturn::writer()
{
	return _writer;
}

/*
*/

class ObjectProxy : public Object
{
public:

	ObjectProxy( Connection& conn, const char* path, const char* service = "" );

	~ObjectProxy();

	Message rinvoke_method( CallMessage& );

private:

	bool handle_message( const Message& );

	void register_obj();
	void unregister_obj();

private:

	MessageSlot _filtered;
};

} /* namespace DBus */

#endif//__DBUSXX_OBJECT_H
