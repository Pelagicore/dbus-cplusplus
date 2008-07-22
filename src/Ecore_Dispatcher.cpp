#include "../include/dbus-c++/Ecore_Dispatcher.h"

#include <assert.h>
#include <linux/limits.h> // TODO: port and protect for other systems

// TODO: add deinit method to close handler

#define BUFFER_GLOBAL_SIZE PIPE_BUF // defined in linux/limits.h

int ecore_dispatcher_async_handler (void *data, Ecore_Fd_Handler *fdh)
{
  int fd;
  Ecore_Dispatcher *dp_local = (Ecore_Dispatcher*) data;
  char buf[dp_local->m_count];
  
  fd = ecore_main_fd_handler_fd_get(fdh);
  
  int recBytes = 0;

  recBytes = e_read (fd, buf, dp_local->m_count);
    
  if (recBytes <= 0)
  {
    fprintf (stderr, "Error while reading pipe!\n");
    exit (1);
  }
  
  assert (recBytes == (signed) dp_local->m_count);
  
  dp_local->m_update_func (buf/*, recBytes*/);
  
  return 1;
}

void ecore_dispatcher_init (Ecore_Dispatcher *dp, void (update) (void*/*, size_t*/))
{
  int fd[2];
  Ecore_Fd_Handler *fd_handler;

  dp->m_update_func = update;

   /* Create the file descriptors */
  if (pipe(fd) == 0) 
  {
    dp->m_fd_read = fd[0];
    dp->m_fd_write = fd[1];

    fcntl(dp->m_fd_read, F_SETFL, O_NONBLOCK);
    fd_handler = ecore_main_fd_handler_add (dp->m_fd_read,
                                            ECORE_FD_READ,
                                            ecore_dispatcher_async_handler,
                                            dp,
                                            NULL, NULL);
    
    ecore_main_fd_handler_active_set(fd_handler, ECORE_FD_READ);
  }
  else
  {
    fprintf (stderr, "pipe() failed\n");
    exit (1);
  }
}

void ecore_dispatcher_close (Ecore_Dispatcher *dp)
{
  close (dp->m_fd_read);
  close (dp->m_fd_write);
}

void ecore_dispatcher_signal (Ecore_Dispatcher *dp, void *data, size_t count)
{
  dp->m_count = count;
  
  e_write (dp->m_fd_write, data, count);
}

