/*
 * Copyright 2004-2019 the Pacemaker project contributors
 *
 * The version control history for this file may have further details.
 *
 * This source code is licensed under the GNU Lesser General Public License
 * version 2.1 or later (LGPLv2.1+) WITHOUT ANY WARRANTY.
 */

#ifndef SCHED_ALLOCATE__H
#  define SCHED_ALLOCATE__H

#  include <glib.h>
#  include <crm/common/xml.h>
#  include <crm/pengine/status.h>
#  include <crm/pengine/complex.h>
#  include <crm/pengine/internal.h>
#  include <pcmki/pcmki_scheduler.h>

struct resource_alloc_functions_s {
    GHashTable *(*merge_weights) (resource_t *, const char *, GHashTable *, const char *, float,
                                  enum pe_weights);
    node_t *(*allocate) (resource_t *, node_t *, pe_working_set_t *);
    void (*create_actions) (resource_t *, pe_working_set_t *);
     gboolean(*create_probe) (resource_t *, node_t *, action_t *, gboolean, pe_working_set_t *);
    void (*internal_constraints) (resource_t *, pe_working_set_t *);

    void (*rsc_colocation_lh) (pe_resource_t *, pe_resource_t *,
                               rsc_colocation_t *, pe_working_set_t *);
    void (*rsc_colocation_rh) (pe_resource_t *, pe_resource_t *,
                               rsc_colocation_t *, pe_working_set_t *);

    void (*rsc_location) (pe_resource_t *, pe__location_t *);

    enum pe_action_flags (*action_flags) (action_t *, node_t *);
    enum pe_graph_flags (*update_actions) (pe_action_t *, pe_action_t *,
                                           pe_node_t *, enum pe_action_flags,
                                           enum pe_action_flags,
                                           enum pe_ordering,
                                           pe_working_set_t *data_set);

    void (*expand) (resource_t *, pe_working_set_t *);
    void (*append_meta) (resource_t * rsc, xmlNode * xml);
};

extern GHashTable *rsc_merge_weights(resource_t * rsc, const char *rhs, GHashTable * nodes,
                                     const char *attr, float factor, enum pe_weights flags);

extern GHashTable *clone_merge_weights(resource_t * rsc, const char *rhs, GHashTable * nodes,
                                       const char *attr, float factor, enum pe_weights flags);

GHashTable *pcmk__bundle_merge_weights(pe_resource_t *rsc, const char *rhs,
                                       GHashTable *nodes, const char *attr,
                                       float factor, enum pe_weights flags);

extern GHashTable *native_merge_weights(resource_t * rsc, const char *rhs, GHashTable * nodes,
                                        const char *attr, float factor, enum pe_weights flags);

extern GHashTable *group_merge_weights(resource_t * rsc, const char *rhs, GHashTable * nodes,
                                       const char *attr, float factor, enum pe_weights flags);

extern node_t *native_color(resource_t * rsc, node_t * preferred, pe_working_set_t * data_set);
extern void native_create_actions(resource_t * rsc, pe_working_set_t * data_set);
extern void native_internal_constraints(resource_t * rsc, pe_working_set_t * data_set);
void native_rsc_colocation_lh(pe_resource_t *lh_rsc, pe_resource_t *rh_rsc,
                              rsc_colocation_t *constraint,
                              pe_working_set_t *data_set);
void native_rsc_colocation_rh(pe_resource_t *lh_rsc, pe_resource_t *rh_rsc,
                              rsc_colocation_t *constraint,
                              pe_working_set_t *data_set);
extern void rsc_ticket_constraint(resource_t * lh_rsc, rsc_ticket_t * rsc_ticket,
                                  pe_working_set_t * data_set);
extern enum pe_action_flags native_action_flags(action_t * action, node_t * node);

void native_rsc_location(pe_resource_t *rsc, pe__location_t *constraint);
extern void native_expand(resource_t * rsc, pe_working_set_t * data_set);
extern gboolean native_create_probe(resource_t * rsc, node_t * node, action_t * complete,
                                    gboolean force, pe_working_set_t * data_set);
extern void native_append_meta(resource_t * rsc, xmlNode * xml);

extern node_t *group_color(resource_t * rsc, node_t * preferred, pe_working_set_t * data_set);
extern void group_create_actions(resource_t * rsc, pe_working_set_t * data_set);
extern void group_internal_constraints(resource_t * rsc, pe_working_set_t * data_set);
void group_rsc_colocation_lh(pe_resource_t *lh_rsc, pe_resource_t *rh_rsc,
                             rsc_colocation_t *constraint,
                             pe_working_set_t *data_set);
void group_rsc_colocation_rh(pe_resource_t *lh_rsc, pe_resource_t *rh_rsc,
                             rsc_colocation_t *constraint,
                             pe_working_set_t *data_set);
extern enum pe_action_flags group_action_flags(action_t * action, node_t * node);
void group_rsc_location(pe_resource_t *rsc, pe__location_t *constraint);
extern void group_expand(resource_t * rsc, pe_working_set_t * data_set);
extern void group_append_meta(resource_t * rsc, xmlNode * xml);

pe_node_t *pcmk__bundle_color(pe_resource_t *rsc, pe_node_t *preferred,
                              pe_working_set_t *data_set);
void pcmk__bundle_create_actions(pe_resource_t *rsc,
                                 pe_working_set_t *data_set);
gboolean pcmk__bundle_create_probe(pe_resource_t *rsc, pe_node_t *node,
                                   pe_action_t *complete, gboolean force,
                                   pe_working_set_t *data_set);
void pcmk__bundle_internal_constraints(pe_resource_t *rsc,
                                       pe_working_set_t *data_set);
void pcmk__bundle_rsc_colocation_lh(pe_resource_t *lh_rsc,
                                    pe_resource_t *rh_rsc,
                                    rsc_colocation_t *constraint,
                                    pe_working_set_t *data_set);
void pcmk__bundle_rsc_colocation_rh(pe_resource_t *lh_rsc,
                                    pe_resource_t *rh_rsc,
                                    rsc_colocation_t *constraint,
                                    pe_working_set_t *data_set);
void pcmk__bundle_rsc_location(pe_resource_t *rsc, pe__location_t *constraint);
enum pe_action_flags pcmk__bundle_action_flags(pe_action_t *action,
                                               pe_node_t *node);
void pcmk__bundle_expand(pe_resource_t *rsc, pe_working_set_t *data_set);
void pcmk__bundle_append_meta(pe_resource_t *rsc, xmlNode *xml);

extern node_t *clone_color(resource_t * rsc, node_t * preferred, pe_working_set_t * data_set);
extern void clone_create_actions(resource_t * rsc, pe_working_set_t * data_set);
extern void clone_internal_constraints(resource_t * rsc, pe_working_set_t * data_set);
void clone_rsc_colocation_lh(pe_resource_t *lh_rsc, pe_resource_t *rh_rsc,
                             rsc_colocation_t *constraint,
                             pe_working_set_t *data_set);
void clone_rsc_colocation_rh(pe_resource_t *lh_rsc, pe_resource_t *rh_rsc,
                             rsc_colocation_t *constraint,
                             pe_working_set_t *data_set);
void clone_rsc_location(pe_resource_t *rsc, pe__location_t *constraint);
extern enum pe_action_flags clone_action_flags(action_t * action, node_t * node);
extern void clone_expand(resource_t * rsc, pe_working_set_t * data_set);
extern gboolean clone_create_probe(resource_t * rsc, node_t * node, action_t * complete,
                                   gboolean force, pe_working_set_t * data_set);
extern void clone_append_meta(resource_t * rsc, xmlNode * xml);

void apply_master_prefs(resource_t *rsc);
node_t *color_promotable(resource_t *rsc, pe_working_set_t *data_set);
void create_promotable_actions(resource_t *rsc, pe_working_set_t *data_set);
void promote_demote_constraints(resource_t *rsc, pe_working_set_t *data_set);
void promotable_constraints(resource_t *rsc, pe_working_set_t *data_set);
void promotable_colocation_rh(resource_t *lh_rsc, resource_t *rh_rsc,
                              rsc_colocation_t *constraint,
                              pe_working_set_t *data_set);

/* extern resource_object_functions_t resource_variants[]; */
extern resource_alloc_functions_t resource_class_alloc_functions[];
gboolean is_active(pe__location_t *cons);

extern gboolean unpack_rsc_order(xmlNode * xml_obj, pe_working_set_t * data_set);

extern gboolean unpack_rsc_colocation(xmlNode * xml_obj, pe_working_set_t * data_set);

extern gboolean unpack_location(xmlNode * xml_obj, pe_working_set_t * data_set);

extern gboolean unpack_rsc_ticket(xmlNode * xml_obj, pe_working_set_t * data_set);

void LogNodeActions(pe_working_set_t * data_set, gboolean terminal);
void LogActions(resource_t * rsc, pe_working_set_t * data_set, gboolean terminal);
void pcmk__bundle_log_actions(pe_resource_t *rsc, pe_working_set_t *data_set,
                              gboolean terminal);

extern void rsc_stonith_ordering(resource_t * rsc, action_t * stonith_op,
                                 pe_working_set_t * data_set);

enum pe_graph_flags native_update_actions(pe_action_t *first, pe_action_t *then,
                                          pe_node_t *node,
                                          enum pe_action_flags flags,
                                          enum pe_action_flags filter,
                                          enum pe_ordering type,
                                          pe_working_set_t *data_set);
enum pe_graph_flags group_update_actions(pe_action_t *first, pe_action_t *then,
                                         pe_node_t *node,
                                         enum pe_action_flags flags,
                                         enum pe_action_flags filter,
                                         enum pe_ordering type,
                                         pe_working_set_t *data_set);
enum pe_graph_flags pcmk__multi_update_actions(pe_action_t *first,
                                               pe_action_t *then,
                                               pe_node_t *node,
                                               enum pe_action_flags flags,
                                               enum pe_action_flags filter,
                                               enum pe_ordering type,
                                               pe_working_set_t *data_set);

gboolean update_action_flags(action_t * action, enum pe_action_flags flags, const char *source, int line);
gboolean update_action(pe_action_t *action, pe_working_set_t *data_set);
void complex_set_cmds(resource_t * rsc);
void pcmk__log_transition_summary(const char *filename);
void clone_create_pseudo_actions(
    resource_t * rsc, GListPtr children, notify_data_t **start_notify, notify_data_t **stop_notify,  pe_working_set_t * data_set);
#endif
