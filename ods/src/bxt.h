/*
 * Copyright (c) 2014 Open Grid Computing, Inc. All rights reserved.
 *
 * Confidential and Proprietary
 */

/*
 * Author: Tom Tucker tom at ogc dot us
 */

#ifndef _BXT_H_
#define _BXT_H_

#include <ods/ods_idx.h>
#include <ods/ods.h>
#include "ods_idx_priv.h"

#pragma pack(4)

/**
 * B+ Tree Node Entry
 *
 * Describes a key and the object to which it refers. The key is an
 * obj_ref_t which refers to an arbitrarily sized ODS object that
 * contains an opaque key. The key comparator is used to compare two
 * keys.
 */
typedef struct bxn_entry {
	union {
		struct {
			ods_ref_t head_ref;
			ods_ref_t tail_ref;
		} leaf;
		struct node {
			ods_ref_t key_ref;
			ods_ref_t node_ref;
		} node;
	} u;
} *bxn_entry_t;

typedef struct bxn_record {
	ods_ref_t key_ref;	/* The key */
	ods_idx_data_t value;	/* The value */
	ods_ref_t next_ref;	/* The next record */
	ods_ref_t prev_ref;	/* The previous record */
} *bxn_record_t;

typedef struct bxt_node {
	ods_ref_t parent;	/* NULL if root */
	uint32_t count:16;
	uint32_t is_leaf:16;
	struct bxn_entry entries[];
} *bxt_node_t;

typedef struct bxt_udata {
	struct ods_idx_meta_data idx_udata;
	uint32_t order;		/* The order or each internal node */
	ods_ref_t root_ref;	/* The root of the tree */
	ods_atomic_t depth;	/* The current tree depth */
	ods_atomic_t card;	/* Cardinatliy */
	ods_atomic_t dups;	/* Duplicate keys */
} *bxt_udata_t;

/* Structure to hang on to cached node allocations */
struct bxt_obj_el {
	ods_obj_t obj;
	ods_ref_t ref;
	LIST_ENTRY(bxt_obj_el) entry;
};

/*
 * In memory object that refers to a B+ Tree
 */
typedef struct bxt_s {
	ods_t ods;		/* The ods that contains the tree */
	ods_obj_t udata_obj;
	bxt_udata_t udata;
	ods_idx_compare_fn_t comparator;
	ods_idx_rt_opts_t rt_opts;	/* Run-time flags */
	/*
	 * The node_q keeps a Q of nodes for allocation.
	 */
	ods_atomic_t node_q_depth;
	LIST_HEAD(node_node_q_head, bxt_obj_el) node_q;
	LIST_HEAD(node_el_q_head, bxt_obj_el) el_q;
} *bxt_t;

typedef struct bxt_pos_s {
	ods_ref_t rec_ref;
	uint32_t ent;
} *bxt_pos_t;

typedef struct bxt_iter_s {
	struct ods_iter iter;
	ods_obj_t rec;
	uint32_t ent;
} *bxt_iter_t;

#define BXT_EXTEND_SIZE	(1024 * 1024)
#define BXT_SIGNATURE "BXTREE01"
#pragma pack()

#define UDATA(_o_) ODS_PTR(struct bxt_udata *, _o_)
/* Node entry */
#define N_ENT(_o_,_i_) ODS_PTR(bxt_node_t, _o_)->entries[_i_].u.node
/* Leaf entry */
#define L_ENT(_o_,_i_) ODS_PTR(bxt_node_t, _o_)->entries[_i_].u.leaf
#define NODE(_o_) ODS_PTR(bxt_node_t, _o_)
#define REC(_o_) ODS_PTR(bxn_record_t, _o_)
#define POS(_o_) ODS_PTR(bxt_pos_t, _o_)
#endif
