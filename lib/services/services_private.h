/*
 * Copyright 2010-2018 Red Hat, Inc.
 *
 * This source code is licensed under the GNU Lesser General Public License
 * version 2.1 or later (LGPLv2.1+) WITHOUT ANY WARRANTY.
 */

#ifndef SERVICES_PRIVATE__H
#  define SERVICES_PRIVATE__H

#  include <glib.h>
#  include "crm/services.h"

#if SUPPORT_DBUS
#  include <dbus/dbus.h>
#endif

#define MAX_ARGC        255
struct svc_action_private_s {
    char *exec;
    char *args[MAX_ARGC];

    uid_t uid;
    gid_t gid;

    guint repeat_timer;
    void (*callback) (svc_action_t * op);
    void (*fork_callback) (svc_action_t * op);

    int stderr_fd;
    mainloop_io_t *stderr_gsource;

    int stdout_fd;
    mainloop_io_t *stdout_gsource;

    int stdin_fd;
#if SUPPORT_DBUS
    DBusPendingCall* pending;
    unsigned timerid;
#endif
};

G_GNUC_INTERNAL
GList *services_os_get_directory_list(const char *root, gboolean files, gboolean executable);

G_GNUC_INTERNAL
gboolean services_os_action_execute(svc_action_t * op);

G_GNUC_INTERNAL
GList *resources_os_list_ocf_providers(void);

G_GNUC_INTERNAL
GList *resources_os_list_ocf_agents(const char *provider);

G_GNUC_INTERNAL
gboolean services__ocf_agent_exists(const char *provider, const char *agent);

G_GNUC_INTERNAL
GList *resources_os_list_nagios_agents(void);

G_GNUC_INTERNAL
gboolean services__nagios_agent_exists(const char *agent);

G_GNUC_INTERNAL
gboolean cancel_recurring_action(svc_action_t * op);

G_GNUC_INTERNAL
gboolean recurring_action_timer(gpointer data);

G_GNUC_INTERNAL
gboolean operation_finalize(svc_action_t * op);

G_GNUC_INTERNAL
void services_add_inflight_op(svc_action_t *op);

G_GNUC_INTERNAL
void services_untrack_op(svc_action_t *op);

G_GNUC_INTERNAL
gboolean is_op_blocked(const char *rsc);

#if SUPPORT_DBUS
G_GNUC_INTERNAL
void services_set_op_pending(svc_action_t *op, DBusPendingCall *pending);
#endif

#endif  /* SERVICES_PRIVATE__H */
