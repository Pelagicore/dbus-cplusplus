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


#include <dbus-c++/eventloop.h>
#include <dbus-c++/debug.h>

#include <sys/poll.h>
#include <sys/time.h>

#include <dbus/dbus.h>

using namespace DBus;

static double millis( timeval tv )
{
	return (tv.tv_sec*1000.0 + tv.tv_usec/1000.0);
}
	
EepleTimeout::EepleTimeout( int interval, bool repeat, EepleMainLoop* ed )
: _enabled(true), _interval(interval), _repeat(repeat), _expiration(0), _data(0), _disp(ed)
{
	timeval now;
	gettimeofday(&now, NULL);

	_expiration = millis(now) + interval;

	_disp->_timeouts.push_back(this);
}

EepleTimeout::~EepleTimeout()
{
	_disp->_timeouts.remove(this);
}

EepleWatch::EepleWatch( int fd, int flags, EepleMainLoop* ed )
: _enabled(true), _fd(fd), _flags(flags), _state(0), _data(0), _disp(ed)
{
	_disp->_watches.push_back(this);
}

EepleWatch::~EepleWatch()
{
	_disp->_watches.remove(this);
}

EepleMainLoop::EepleMainLoop()
{
}

EepleMainLoop::~EepleMainLoop()
{
	Watches::iterator wi = _watches.begin();
	while(wi != _watches.end())
	{
		Watches::iterator wmp = wi;
		++wmp;
		delete (*wi);
		wi = wmp;
	}

	Timeouts::iterator ti = _timeouts.begin();
	while(ti != _timeouts.end())
	{
		Timeouts::iterator tmp = ti;
		++tmp;
		delete (*ti);
		ti = tmp;
	}
}

void EepleMainLoop::dispatch()
{
	int nfd = _watches.size();

	pollfd fds[nfd];

	Watches::iterator wi = _watches.begin();

	for(nfd = 0; wi != _watches.end(); ++wi)
	{
		if((*wi)->enabled())
		{
			fds[nfd].fd = (*wi)->descriptor();
			fds[nfd].events = (*wi)->flags();
			fds[nfd].revents = 0;

			++nfd;
		}
	}

	int wait_min = 10000;

	Timeouts::iterator ti;

	for(ti = _timeouts.begin(); ti != _timeouts.end(); ++ti)
	{
		if((*ti)->enabled() && (*ti)->interval() < wait_min)
			wait_min = (*ti)->interval();
	}

	//int rfd = poll(fds, nfd, wait_min);
	//if(rfd) debug_log("%d descriptors ready");

	poll(fds, nfd, wait_min);

	timeval now;
	gettimeofday(&now, NULL);

	double now_millis = millis(now);

	ti = _timeouts.begin();

	while(ti != _timeouts.end())
	{
		Timeouts::iterator tmp = ti;
		++tmp;

		if((*ti)->enabled() && now_millis >= (*ti)->_expiration)
		{
			(*ti)->expired(*(*ti));

			if((*ti)->_repeat)
			{
				(*ti)->_expiration = now_millis + (*ti)->_interval;
			}

		}

		ti = tmp;
	}

	for(int j = 0; j < nfd; ++j)
	{
		Watches::iterator wi;

		for(wi = _watches.begin(); wi != _watches.end();)
		{
			Watches::iterator tmp = wi;
			++tmp;

			if((*wi)->enabled() && (*wi)->_fd == fds[j].fd)
			{
				if(fds[j].revents)
				{
					(*wi)->_state = fds[j].revents;

					(*wi)->ready(*(*wi));

					fds[j].revents = 0;
				}
			}

			wi = tmp;
		}
	}
}

/*
*/

BusTimeout::BusTimeout( Timeout::Internal* ti, BusDispatcher* bd )
: Timeout(ti), EepleTimeout(Timeout::interval(), true, bd)
{
	EepleTimeout::enabled(Timeout::enabled());
}

void BusTimeout::toggle()
{
	debug_log("timeout %p toggled (%s)", this, Timeout::enabled() ? "on":"off");

	EepleTimeout::enabled(Timeout::enabled());
}

BusWatch::BusWatch( Watch::Internal* wi, BusDispatcher* bd )
: Watch(wi), EepleWatch(Watch::descriptor(), 0, bd)
{
	int flags = POLLHUP | POLLERR;

	if(Watch::flags() & DBUS_WATCH_READABLE)
		flags |= POLLIN;
	if(Watch::flags() & DBUS_WATCH_WRITABLE)
		flags |= POLLOUT;

	EepleWatch::flags(flags);
	EepleWatch::enabled(Watch::enabled());
}

void BusWatch::toggle()
{
	debug_log("watch %p toggled (%s)", this, Watch::enabled() ? "on":"off");

	EepleWatch::enabled(Watch::enabled());
}

void BusDispatcher::enter()
{
	debug_log("entering dispatcher %p", this);

	_running = true;

	while(_running)
	{
		do_iteration();
	}

	debug_log("leaving dispatcher %p", this);
}

void BusDispatcher::leave()
{
	_running = false;
}

void BusDispatcher::do_iteration()
{
	dispatch_pending();
	dispatch();
}

Timeout* BusDispatcher::add_timeout( Timeout::Internal* ti )
{
	BusTimeout* bt = new BusTimeout(ti, this);

	bt->expired = new Callback<BusDispatcher, void, EepleTimeout&>(this, &BusDispatcher::timeout_expired);
	bt->data(bt);

	debug_log("added timeout %p (%s)", bt, ((Timeout*)bt)->enabled() ? "on":"off");

	return bt;
}

void BusDispatcher::rem_timeout( Timeout* t )
{
	debug_log("removed timeout %p", t);

	delete t;
}

Watch* BusDispatcher::add_watch( Watch::Internal* wi )
{
	BusWatch* bw = new BusWatch(wi, this);

	bw->ready = new Callback<BusDispatcher, void, EepleWatch&>(this, &BusDispatcher::watch_ready);
	bw->data(bw);

	debug_log("added watch %p (%s) fd=%d flags=%d",
		bw, ((Watch*)bw)->enabled() ? "on":"off", ((Watch*)bw)->descriptor(), ((Watch*)bw)->flags()
	);

	return bw;
}

void BusDispatcher::rem_watch( Watch* w )
{
	debug_log("removed watch %p", w);

	delete w;
}

void BusDispatcher::timeout_expired( EepleTimeout& et )
{
	debug_log("timeout %p expired", &et);

	BusTimeout* timeout = reinterpret_cast<BusTimeout*>(et.data());

	timeout->handle();
}

void BusDispatcher::watch_ready( EepleWatch& ew )
{
	BusWatch* watch = reinterpret_cast<BusWatch*>(ew.data());

	debug_log("watch %p ready, flags=%d state=%d",
		watch, ((Watch*)watch)->flags(), watch->state()
	);

	int flags = 0;

	if(watch->state() & POLLIN)
		flags |= DBUS_WATCH_READABLE;
	if(watch->state() & POLLOUT)
		flags |= DBUS_WATCH_WRITABLE;
	if(watch->state() & POLLHUP)
		flags |= DBUS_WATCH_HANGUP;
	if(watch->state() & POLLERR)
		flags |= DBUS_WATCH_ERROR;

	watch->handle(flags);
}

