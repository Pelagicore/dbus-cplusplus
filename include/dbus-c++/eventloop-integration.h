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


#ifndef __DBUSXX_EVENTLOOP_INTEGRATION_H
#define __DBUSXX_EVENTLOOP_INTEGRATION_H

#include <errno.h>
#include "api.h"
#include "dispatcher.h"
#include "util.h"
#include "eventloop.h"

namespace DBus {

/* 
 * Glue between the event loop and the DBus library
 */

class BusDispatcher;

class DXXAPI BusTimeout : public Timeout, public DefaultTimeout
{
	BusTimeout(Timeout::Internal *, BusDispatcher *);

	void toggle();

friend class BusDispatcher;
};

class DXXAPI BusWatch : public Watch, public DefaultWatch
{
	BusWatch(Watch::Internal *, BusDispatcher *);

	void toggle();

friend class BusDispatcher;
};

class DXXAPI Pipe
{
public:
	/*!
	 * Write some data into the communication pipe.
	 *
	 * @param buffer The raw data to write.
	 * @param nbytes The number of bytes to write from the buffer.
	 */
	void write(const void *buffer, unsigned int nbytes);

	/*!
	 * Simply write one single byte into the pipe. This is a shortcut
	 * if there's really no data to transport, but to activate the handler.
	 */
	void signal();
	
private:
	void(*_handler)(const void *data, void *buffer, unsigned int nbyte);
	int fd_write;
  int fd_read;
	const void *data;
	
	// allow construction only in BusDispatcher
	Pipe () {};
	~Pipe () {};

friend class BusDispatcher;
};

class DXXAPI BusDispatcher : public Dispatcher, public DefaultMainLoop
{
public:
	BusDispatcher();
	
	~BusDispatcher() {}

	virtual void enter();

	virtual void leave();

  virtual Pipe *add_pipe(void(*handler)(const void *data, void *buffer, unsigned int nbyte), const void *data);

	virtual void del_pipe (Pipe *pipe);
	
	virtual void do_iteration();

	virtual Timeout *add_timeout(Timeout::Internal *);

	virtual void rem_timeout(Timeout *);

	virtual Watch *add_watch(Watch::Internal *);

	virtual void rem_watch(Watch *);

	void watch_ready(DefaultWatch &);

	void timeout_expired(DefaultTimeout &);

private:
	bool _running;
	int _pipe[2];
	std::list <Pipe*> pipe_list;
};

} /* namespace DBus */

#endif//__DBUSXX_EVENTLOOP_INTEGRATION_H
