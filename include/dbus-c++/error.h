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


#ifndef __DBUSXX_ERROR_H
#define __DBUSXX_ERROR_H

#include "api.h"
#include "util.h"

#include <exception>

namespace DBus {

class Message;
class InternalError;

class DXXAPI Error : public std::exception
{
public:

	Error();

	Error(InternalError &);

	Error(const char *name, const char *message);
	
	Error(Message &);

	~Error() throw();

	const char *what() const throw();

	const char *name() const;

	const char *message() const;

	void set(const char *name, const char *message);
	// parameters MUST be static strings

	bool is_set() const;

	operator bool() const
	{
		return is_set();
	}

private:

	RefPtrI<InternalError> _int;
};

struct DXXAPI ErrorFailed : public Error
{
	ErrorFailed(const char *msg)
	: Error("org.freedesktop.DBus.Error.Failed", msg)
	{}
};

struct DXXAPI ErrorNoMemory : public Error
{
	ErrorNoMemory(const char *msg)
	: Error("org.freedesktop.DBus.Error.NoMemory", msg)
	{}
};

struct DXXAPI ErrorServiceUnknown : public Error
{
	ErrorServiceUnknown(const char *msg)
	: Error("org.freedesktop.DBus.Error.ServiceUnknown", msg)
	{}
};

struct DXXAPI ErrorNameHasNoOwner : public Error
{
	ErrorNameHasNoOwner(const char *msg)
	: Error("org.freedesktop.DBus.Error.NameHasNoOwner", msg)
	{}
};

struct DXXAPI ErrorNoReply : public Error
{
	ErrorNoReply(const char *msg)
	: Error("org.freedesktop.DBus.Error.NoReply", msg)
	{}
};

struct DXXAPI ErrorIOError : public Error
{
	ErrorIOError(const char *msg)
	: Error("org.freedesktop.DBus.Error.IOError", msg)
	{}
};

struct DXXAPI ErrorBadAddress : public Error
{
	ErrorBadAddress(const char *msg)
	: Error("org.freedesktop.DBus.Error.BadAddress", msg)
	{}
};

struct DXXAPI ErrorNotSupported : public Error
{
	ErrorNotSupported(const char *msg)
	: Error("org.freedesktop.DBus.Error.NotSupported", msg)
	{}
};

struct DXXAPI ErrorLimitsExceeded : public Error
{
	ErrorLimitsExceeded(const char *msg)
	: Error("org.freedesktop.DBus.Error.LimitsExceeded", msg)
	{}
};

struct DXXAPI ErrorAccessDenied : public Error
{
	ErrorAccessDenied(const char *msg)
	: Error("org.freedesktop.DBus.Error.AccessDenied", msg)
	{}
};

struct DXXAPI ErrorAuthFailed : public Error
{
	ErrorAuthFailed(const char *msg)
	: Error("org.freedesktop.DBus.Error.AuthFailed", msg)
	{}
};

struct DXXAPI ErrorNoServer : public Error
{
	ErrorNoServer(const char *msg)
	: Error("org.freedesktop.DBus.Error.NoServer", msg)
	{}
};

struct DXXAPI ErrorTimeout : public Error
{
	ErrorTimeout(const char *msg)
	: Error("org.freedesktop.DBus.Error.Timeout", msg)
	{}
};

struct DXXAPI ErrorNoNetwork : public Error
{
	ErrorNoNetwork(const char *msg)
	: Error("org.freedesktop.DBus.Error.NoNetwork", msg)
	{}
};

struct DXXAPI ErrorAddressInUse : public Error
{
	ErrorAddressInUse(const char *msg)
	: Error("org.freedesktop.DBus.Error.AddressInUse", msg)
	{}
};

struct DXXAPI ErrorDisconnected : public Error
{
	ErrorDisconnected(const char *msg)
	: Error("org.freedesktop.DBus.Error.Disconnected", msg)
	{}
};

struct DXXAPI ErrorInvalidArgs : public Error
{
	ErrorInvalidArgs(const char *msg)
	: Error("org.freedesktop.DBus.Error.InvalidArgs", msg)
	{}
};

struct DXXAPI ErrorFileNotFound : public Error
{
	ErrorFileNotFound(const char *msg)
	: Error("org.freedesktop.DBus.Error.FileNotFound", msg)
	{}
};

struct DXXAPI ErrorUnknownMethod : public Error
{
	ErrorUnknownMethod(const char *msg)
	: Error("org.freedesktop.DBus.Error.UnknownMethod", msg)
	{}
};

struct DXXAPI ErrorTimedOut : public Error
{
	ErrorTimedOut(const char *msg)
	: Error("org.freedesktop.DBus.Error.TimedOut", msg)
	{}
};

struct DXXAPI ErrorMatchRuleNotFound : public Error
{
	ErrorMatchRuleNotFound(const char *msg)
	: Error("org.freedesktop.DBus.Error.MatchRuleNotFound", msg)
	{}
};

struct DXXAPI ErrorMatchRuleInvalid : public Error
{
	ErrorMatchRuleInvalid(const char *msg)
	: Error("org.freedesktop.DBus.Error.MatchRuleInvalid", msg)
	{}
};

struct DXXAPI ErrorSpawnExecFailed : public Error
{
	ErrorSpawnExecFailed(const char *msg)
	: Error("org.freedesktop.DBus.Error.Spawn.ExecFailed", msg)
	{}
};

struct DXXAPI ErrorSpawnForkFailed : public Error
{
	ErrorSpawnForkFailed(const char *msg)
	: Error("org.freedesktop.DBus.Error.Spawn.ForkFailed", msg)
	{}
};

struct DXXAPI ErrorSpawnChildExited : public Error
{
	ErrorSpawnChildExited(const char *msg)
	: Error("org.freedesktop.DBus.Error.Spawn.ChildExited", msg)
	{}
};

struct DXXAPI ErrorSpawnChildSignaled : public Error
{
	ErrorSpawnChildSignaled(const char *msg)
	: Error("org.freedesktop.DBus.Error.Spawn.ChildSignaled", msg)
	{}
};

struct DXXAPI ErrorSpawnFailed : public Error
{
	ErrorSpawnFailed(const char *msg)
	: Error("org.freedesktop.DBus.Error.Spawn.Failed", msg)
	{}
};

struct DXXAPI ErrorInvalidSignature : public Error
{
	ErrorInvalidSignature(const char *msg)
	: Error("org.freedesktop.DBus.Error.InvalidSignature", msg)
	{}
};

struct DXXAPI ErrorUnixProcessIdUnknown : public Error
{
	ErrorUnixProcessIdUnknown(const char *msg)
	: Error("org.freedesktop.DBus.Error.UnixProcessIdUnknown", msg)
	{}
};

struct DXXAPI ErrorSELinuxSecurityContextUnknown : public Error
{
	ErrorSELinuxSecurityContextUnknown(const char *msg)
	: Error("org.freedesktop.DBus.Error.SELinuxSecurityContextUnknown", msg)
	{}
};

} /* namespace DBus */

#endif//__DBUSXX_ERROR_H
