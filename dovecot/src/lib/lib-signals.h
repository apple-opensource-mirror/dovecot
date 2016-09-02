#ifndef LIB_SIGNALS_H
#define LIB_SIGNALS_H

#include <signal.h>

enum libsig_flags {
	/* Signal handler will be called later from IO loop when it's safe to
	   do any kind of work */
	LIBSIG_FLAG_DELAYED	= 0x01,
	/* Restart syscalls instead of having them fail with EINTR */
	LIBSIG_FLAG_RESTART	= 0x02
};
#define LIBSIG_FLAGS_SAFE (LIBSIG_FLAG_DELAYED | LIBSIG_FLAG_RESTART)

typedef void signal_handler_t(const siginfo_t *si, void *context);

/* Number of times a "termination signal" has been received.
   These signals are SIGINT, SIGQUIT and SIGTERM. Callers can compare this to
   their saved previous value to see if a syscall returning EINTR should be
   treated as someone wanting to end the process or just some internal signal
   that should be ignored, such as SIGCHLD.

   This is marked as volatile so that compiler won't optimize away its
   comparisons. It may not work perfectly everywhere, such as when accessing it
   isn't atomic, so you shouldn't heavily rely on its actual value. */
extern volatile unsigned int signal_term_counter;

/* Convert si_code to string */
const char *lib_signal_code_to_str(int signo, int sicode);

/* Set signal handler for specific signal. */
void lib_signals_set_handler(int signo, enum libsig_flags flags,
			     signal_handler_t *handler, void *context);
/* Ignore given signal. */
void lib_signals_ignore(int signo, bool restart_syscalls);
void lib_signals_unset_handler(int signo,
			       signal_handler_t *handler, void *context);

/* Remove and add the internal I/O handler back. This is necessary to get
   the delayed signals to work when using multiple I/O loops. */
void lib_signals_reset_ioloop(void);

void lib_signals_init(void);
void lib_signals_deinit(void);

#endif
