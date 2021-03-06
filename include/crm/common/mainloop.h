/*
 * Copyright 2009-2019 the Pacemaker project contributors
 *
 * The version control history for this file may have further details.
 *
 * This source code is licensed under the GNU Lesser General Public License
 * version 2.1 or later (LGPLv2.1+) WITHOUT ANY WARRANTY.
 */

#ifndef CRM_COMMON_MAINLOOP__H
#  define CRM_COMMON_MAINLOOP__H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \file
 * \brief Wrappers for and extensions to glib mainloop
 * \ingroup core
 */

#  include <signal.h> // sighandler_t
#  include <glib.h>

enum mainloop_child_flags {
    /* don't kill pid group on timeout, only kill the pid */
    mainloop_leave_pid_group = 0x01,
};

typedef struct trigger_s crm_trigger_t;
typedef struct mainloop_io_s mainloop_io_t;
typedef struct mainloop_child_s mainloop_child_t;
typedef struct mainloop_timer_s mainloop_timer_t;

void mainloop_cleanup(void);

crm_trigger_t *mainloop_add_trigger(int priority, int (*dispatch) (gpointer user_data),
                                    gpointer userdata);

void mainloop_set_trigger(crm_trigger_t * source);

void mainloop_trigger_complete(crm_trigger_t * trig);

gboolean mainloop_destroy_trigger(crm_trigger_t * source);

#  ifndef HAVE_SIGHANDLER_T
typedef void (*sighandler_t)(int);
#  endif

sighandler_t crm_signal_handler(int sig, sighandler_t dispatch);
gboolean crm_signal(int sig, void (*dispatch) (int sig)); // deprecated

gboolean mainloop_add_signal(int sig, void (*dispatch) (int sig));

gboolean mainloop_destroy_signal(int sig);

bool mainloop_timer_running(mainloop_timer_t *t);

void mainloop_timer_start(mainloop_timer_t *t);

void mainloop_timer_stop(mainloop_timer_t *t);

guint mainloop_timer_set_period(mainloop_timer_t *t, guint period_ms);

mainloop_timer_t *mainloop_timer_add(const char *name, guint period_ms, bool repeat, GSourceFunc cb, void *userdata);

void mainloop_timer_del(mainloop_timer_t *t);


#  include <crm/common/ipc.h>
#  include <qb/qbipcs.h>

struct ipc_client_callbacks {
    int (*dispatch) (const char *buffer, ssize_t length, gpointer userdata);
    void (*destroy) (gpointer);
};

qb_ipcs_service_t *mainloop_add_ipc_server(const char *name, enum qb_ipc_type type,
                                           struct qb_ipcs_service_handlers *callbacks);

void mainloop_del_ipc_server(qb_ipcs_service_t * server);

mainloop_io_t *mainloop_add_ipc_client(const char *name, int priority, size_t max_size,
                                       void *userdata, struct ipc_client_callbacks *callbacks);

void mainloop_del_ipc_client(mainloop_io_t * client);

crm_ipc_t *mainloop_get_ipc_client(mainloop_io_t * client);

struct mainloop_fd_callbacks {
    int (*dispatch) (gpointer userdata);
    void (*destroy) (gpointer userdata);
};

mainloop_io_t *mainloop_add_fd(const char *name, int priority, int fd, void *userdata,
                               struct mainloop_fd_callbacks *callbacks);

void mainloop_del_fd(mainloop_io_t * client);

/*
 * Create a new tracked process
 * To track a process group, use -pid
 */
void mainloop_child_add(pid_t pid,
                        int timeout,
                        const char *desc,
                        void *userdata,
                        void (*callback) (mainloop_child_t * p, pid_t pid, int core, int signo, int exitcode));

void mainloop_child_add_with_flags(pid_t pid,
                        int timeout,
                        const char *desc,
                        void *userdata,
                        enum mainloop_child_flags,
                        void (*callback) (mainloop_child_t * p, pid_t pid, int core, int signo, int exitcode));

void *mainloop_child_userdata(mainloop_child_t * child);
int mainloop_child_timeout(mainloop_child_t * child);
const char *mainloop_child_name(mainloop_child_t * child);

pid_t mainloop_child_pid(mainloop_child_t * child);
void mainloop_clear_child_userdata(mainloop_child_t * child);
gboolean mainloop_child_kill(pid_t pid);

void pcmk_drain_main_loop(GMainLoop *mloop, guint timer_ms,
                          bool (*check)(guint));

#  define G_PRIORITY_MEDIUM (G_PRIORITY_HIGH/2)

#ifdef __cplusplus
}
#endif

#endif
