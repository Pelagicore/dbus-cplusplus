#ifndef ECORE_DISPATCHER_H
#define ECORE_DISPATCHER_H

#include <Ecore.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#ifdef _WIN32

# include <winsock2.h>

# define e_write(fd, buffer, count) \
  send((fd), (char *)(buffer), count, 0)

# define e_read(fd, buffer, count) \
  recv((fd), (char *)(buffer), count, 0)

#else

# include <unistd.h>
# include <fcntl.h>

# define e_write(fd, buffer, count) \
  write((fd), (buffer), count)

# define e_read(fd, buffer, count) \
  read((fd), (buffer), count)

#endif /* _WIN32 */

struct _Ecore_Dispatcher
{
  int m_fd_read;
  int m_fd_write;
  size_t m_count;
  void (*m_update_func) (void*/*, size_t*/);
};

typedef struct _Ecore_Dispatcher Ecore_Dispatcher;

void ecore_dispatcher_init (Ecore_Dispatcher *dp, void (update) (void*/*, size_t*/));
void ecore_dispatcher_signal (Ecore_Dispatcher *dp, void *data, size_t count);
void ecore_dispatcher_close (Ecore_Dispatcher *dp);

#endif // ECORE_DISPATCHER_H

