#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <signal.h>
#include <sys/errno.h>
#include <sys/select.h>

#include "rsssd.h"

int main(int argc, char * const *argv) {
  // struct MHD_Daemon *microhttpd;
  if (argc != 3) {
    printf("%s DB PORT\n", argv[0]);
    return EXIT_FAILURE;
  }

  // backpaged_init();

  // backpage_error_t *error = NULL;
  // backpaged_t *daemon = backpaged_new(argv[1], &error);
  // if (daemon == NULL) {
  //   EINA_LOG_DOM_ERR(_domain, "Error: %s", error != NULL ? error->message : "Unknown");
  //   backpaged_shutdown();
  //   return EXIT_FAILURE;
  // }
  // 
  // microhttpd = MHD_start_daemon(
  //   0,
  //   atoi(argv[2]),
  //   NULL,
  //   NULL,
  //   backpaged_dispatch,
  //   daemon,
  //   MHD_OPTION_URI_LOG_CALLBACK, logger, NULL,
  //   MHD_OPTION_NOTIFY_COMPLETED, request_completed_callback,
  //   daemon,
  //   MHD_OPTION_END
  // );
  // if (microhttpd == NULL)
  //   return EXIT_FAILURE;
  // 
  // // TODO: Fork and detach
  // 
  // // TODO: install signal handlers to support shutdown
  // 
  // // Run the main loop
  // // int MHD_get_timeout (struct MHD_Daemon *daemon, unsigned MHD_LONG_LONG *timeout)
  // // int MHD_get_fdset (struct MHD_Daemon *daemon, fd_set * read_fd_set, fd_set * write_fd_set, fd_set * except_fd_set, int *max_fd);
  // 
  // 
  // while(keep_running) {
  //   fd_set readfds;
  //   FD_ZERO(&readfds);
  //   fd_set writefds;
  //   FD_ZERO(&writefds);
  //   fd_set errorfds;
  //   FD_ZERO(&errorfds);
  // 
  //   int max_fd = 0;
  //   int status = MHD_get_fdset(microhttpd, &readfds, &writefds, &errorfds, &max_fd);
  //   assert(status == MHD_YES);
  // 
  //   struct timeval *timeout = NULL;
  //   status = select(max_fd + 1, &readfds, &writefds, &errorfds, timeout);
  //   assert(status >= 0);
  // 
  //   status = MHD_run(microhttpd);
  //   assert(status == MHD_YES);
  // }
  // 
  // // select
  // // MHD_run
  // // Any other stuff
  // // repeat
  // 
  // MHD_stop_daemon(microhttpd);
  // 
  // backpaged_free(daemon);
  // backpaged_shutdown();

  return EXIT_SUCCESS;
}
