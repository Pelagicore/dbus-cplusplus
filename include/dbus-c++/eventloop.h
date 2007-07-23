/*
 *
 *  D-Bus++ - C++ bindings for D-Bus
 *
 *  Copyright (C) 2005-2007  Paolo Durante <shackan@gmail.com>
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


#ifndef __DBUSXX_EVENTLOOP_H
#define __DBUSXX_EVENTLOOP_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <list>

#include "api.h"
#include "dispatcher.h"
#include "util.h"

namespace DBus {

class EepleMainLoop;

class DXXAPI EepleTimeout
{
public:

	EepleTimeout( int interval, bool repeat, EepleMainLoop* );

	virtual ~EepleTimeout();

	bool enabled(){ return _enabled; }
	void enabled(bool e){ _enabled = e; }

	int interval(){ return _interval; }
	void interval(int i){ _interval = i; }

	bool repeat(){ return _repeat; }
	void repeat(bool r){ _repeat = r; }

	void* data(){ return _data; }
	void data(void* d){ _data = d; }

	Slot<void, EepleTimeout&> expired;
	
private:

	bool _enabled;

	int _interval;
	bool _repeat;

	double _expiration;

	void* _data;
	
	EepleMainLoop* _disp;

friend class EepleMainLoop;
};

typedef std::list< EepleTimeout* > Timeouts;

class DXXAPI EepleWatch
{
public:

	EepleWatch( int fd, int flags, EepleMainLoop* );

	virtual ~EepleWatch();

	bool enabled(){ return _enabled; }
	void enabled(bool e){ _enabled = e; }

	int descriptor(){ return _fd; }

	int flags(){ return _flags; }
	void flags( int f ){ _flags = f; }

	int state(){ return _state; }

	void* data(){ return _data; }
	void data(void* d){ _data = d; }

	Slot<void, EepleWatch&> ready;

private:

	bool _enabled;

	int _fd;
	int _flags;
	int _state;

	void* _data;

	EepleMainLoop* _disp;

friend class EepleMainLoop;
};

typedef std::list< EepleWatch* > Watches;

class DXXAPI EepleMainLoop
{
public:

	EepleMainLoop();

	virtual ~EepleMainLoop();

	virtual void dispatch();

private:

	Timeouts _timeouts;
	Watches _watches;

friend class EepleTimeout;
friend class EepleWatch;
};

/* the classes below are those you are going to implement if you
 * want to use another event loop (Qt, Glib, boost, whatever).
 *
 * Don't forget to set 'default_dispatcher' accordingly!
 */

class BusDispatcher;

class DXXAPI BusTimeout : public Timeout, public EepleTimeout
{
	BusTimeout( Timeout::Internal*, BusDispatcher* );

	void toggle();

friend class BusDispatcher;
};

class DXXAPI BusWatch : public Watch, public EepleWatch
{
	BusWatch( Watch::Internal*, BusDispatcher* );

	void toggle();

friend class BusDispatcher;
};

class DXXAPI BusDispatcher : public Dispatcher, public EepleMainLoop
{
public:

	BusDispatcher() : _running(false)
	{}

	~BusDispatcher()
	{}

	virtual void enter();

	virtual void leave();

	virtual void do_iteration();

	virtual Timeout* add_timeout( Timeout::Internal* );

	virtual void rem_timeout( Timeout* );

	virtual Watch* add_watch( Watch::Internal* );

	virtual void rem_watch( Watch* );

	void watch_ready( EepleWatch& );

	void timeout_expired( EepleTimeout& );

private:

	bool _running;
};

} /* namespace DBus */

#endif//__DBUSXX_EVENTLOOP_H
