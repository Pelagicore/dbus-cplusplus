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

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#ifdef HAVE_DBUSPP_CONFIG_H
#include <dbus-c++/dbus-c++-config.h>
#endif

#include <dbus-c++/ecore-integration.h>

#include <dbus/dbus.h> // for DBUS_WATCH_*

using namespace DBus;

Ecore::BusTimeout::BusTimeout( Timeout::Internal* ti/*, GMainContext* ctx */)
: Timeout(ti)/*, _ctx(ctx)*/
{
	_enable();
}

Ecore::BusTimeout::~BusTimeout()
{
	_disable();
}

void Ecore::BusTimeout::toggle()
{
	debug_log("ecore: timeout %p toggled (%s)", this, Timeout::enabled() ? "on":"off");

	if(Timeout::enabled())	_enable();
	else			_disable();
}

int Ecore::BusTimeout::timeout_handler( void *data )
{
	Ecore::BusTimeout* t = reinterpret_cast<Ecore::BusTimeout*>(data);

	t->handle();

	return 1; // 1 -> reshedule for next timer interval
}

void Ecore::BusTimeout::_enable()
{
  // TODO: port
  // _source => Ecore_Timer
  // g_source_set_callback => EcoreDispatcher init()
  
  _etimer = ecore_timer_add (((double)Timeout::interval())/1000, timeout_handler, this);
  
	/*_source = g_timeout_source_new();
	g_source_set_callback(_source, timeout_handler, this, NULL);
	g_source_attach(_source, _ctx);*/
}

void Ecore::BusTimeout::_disable()
{
  // TODO: port
  // need to enhance Dispatcher with close
  ecore_timer_del (_etimer);
	//g_source_destroy(_source);
}

struct BusSource
{
  // TODO: needed?
	//GSource source;
	//GPollFD poll;
};

static bool watch_prepare( /*GSource *source,*/ int *timeout )
{
//	debug_log("ecore: watch_prepare");

	*timeout = -1;
	return false;
}

static bool watch_check( /*GSource *source*/ )
{
//	debug_log("ecore: watch_check");

	//BusSource* io = (BusSource*)source;
	return 0;//io->poll.revents ? true : false;
}

// TODO: port parameters
static bool watch_dispatch(/* GSource *source, GSourceFunc callback, */void *data )
{
	/*debug_log("ecore: watch_handler");

	bool cb = callback(data);
	DBus::default_dispatcher->dispatch_pending(); //TODO: won't work in case of multiple dispatchers*/
	return 0;//cb;
}

// TODO: needed?
/*static GSourceFuncs watch_funcs = {
	watch_prepare,
	watch_check,
	watch_dispatch,
	NULL
};*/

// TODO: port parameter
Ecore::BusWatch::BusWatch( Watch::Internal* wi/*, GMainContext* ctx */)
: Watch(wi)/*, _ctx(ctx)*/
{
	_enable();
}

Ecore::BusWatch::~BusWatch()
{
	_disable();
}

void Ecore::BusWatch::toggle()
{
	debug_log("ecore: watch %p toggled (%s)", this, Watch::enabled() ? "on":"off");

	if(Watch::enabled())	_enable();
	else			_disable();
}

void Ecore::BusWatch::watch_handler( void *data )
{
	Ecore::BusWatch* w = reinterpret_cast<Ecore::BusWatch*>(data);

	//BusSource* io = (BusSource*)(w->_source);

	int flags = 0;
	/*if(io->poll.revents & G_IO_IN)
	     flags |= DBUS_WATCH_READABLE;
	if(io->poll.revents & G_IO_OUT)
	     flags |= DBUS_WATCH_WRITABLE;
	if(io->poll.revents & G_IO_ERR)
	     flags |= DBUS_WATCH_ERROR;
	if(io->poll.revents & G_IO_HUP)
	     flags |= DBUS_WATCH_HANGUP;*/

	w->handle(flags);

	//return true;
}

void Ecore::BusWatch::_enable()
{
  ecore_dispatcher_init (&_edispatcher, watch_handler);
  
  // TODO: port this
	/*_source = g_source_new(&watch_funcs, sizeof(BusSource));
	g_source_set_callback(_source, watch_handler, this, NULL);

	int flags = Watch::flags();
	int condition = 0;

	if(flags & DBUS_WATCH_READABLE)
		condition |= G_IO_IN;
//	if(flags & DBUS_WATCH_WRITABLE)
//		condition |= G_IO_OUT;
	if(flags & DBUS_WATCH_ERROR)
		condition |= G_IO_ERR;
	if(flags & DBUS_WATCH_HANGUP)
		condition |= G_IO_HUP;

	GPollFD* poll = &(((BusSource*)_source)->poll);
	poll->fd = Watch::descriptor();
	poll->events = condition;
	poll->revents = 0;

	g_source_add_poll(_source, poll);
	g_source_attach(_source, _ctx);*/
}

void Ecore::BusWatch::_disable()
{
  // TODO: port this
	/*GPollFD* poll = &(((BusSource*)_source)->poll);
	g_source_remove_poll(_source, poll);
	g_source_destroy(_source);*/
}

void Ecore::BusDispatcher::attach( /*GMainContext* ctx */)
{
	//_ctx = ctx ? ctx : g_main_context_default();
}

Timeout* Ecore::BusDispatcher::add_timeout( Timeout::Internal* wi )
{
	Timeout* t = new Ecore::BusTimeout(wi/*, _ctx*/);

	debug_log("ecore: added timeout %p (%s)", t, t->enabled() ? "on":"off");

	return t;
}

void Ecore::BusDispatcher::rem_timeout( Timeout* t )
{
	debug_log("ecore: removed timeout %p", t);

	delete t;
}

Watch* Ecore::BusDispatcher::add_watch( Watch::Internal* wi )
{
	Watch* w = new Ecore::BusWatch(wi/*, _ctx*/);

	debug_log("ecore: added watch %p (%s) fd=%d flags=%d",
		w, w->enabled() ? "on":"off", w->descriptor(), w->flags()
	);
	return w;
}

void Ecore::BusDispatcher::rem_watch( Watch* w )
{
	debug_log("ecore: removed watch %p", w);

	delete w;
}
