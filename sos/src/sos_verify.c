/*
 * Copyright (c) 2014-2015 Open Grid Computing, Inc. All rights reserved.
 *
 * This software is available to you under a choice of one of two
 * licenses.  You may choose to be licensed under the terms of the GNU
 * General Public License (GPL) Version 2, available from the file
 * COPYING in the main directory of this source tree, or the BSD-type
 * license below:
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 *      Redistributions of source code must retain the above copyright
 *      notice, this list of conditions and the following disclaimer.
 *
 *      Redistributions in binary form must reproduce the above
 *      copyright notice, this list of conditions and the following
 *      disclaimer in the documentation and/or other materials provided
 *      with the distribution.
 *
 *      Neither the name of Open Grid Computing nor the names of any
 *      contributors may be used to endorse or promote products derived
 *      from this software without specific prior written permission.
 *
 *      Modified source versions must be plainly marked as such, and
 *      must not be misrepresented as being the original software.
 *
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/**
 * \file sos_verify.c
 * \author Narate Taerat (narate at ogc dot us)
 * \brief Verify a given sos store
 *
 * Given a sos store path (a sos store path is the string before _sos.OBJ, e.g.
 * if you have /data/abc_sos.OBJ, /data/abc is the sos store path), this program
 * will check if the store (i.e. the data ODS and indices) is not corrupted. If
 * a '--recover' option is given, it will try to recover the corrupted store.
 */
#include <stdio.h>
#include <unistd.h>
#include <getopt.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/fcntl.h>

#include "sos.h"
#include "sos_priv.h"

const char *usage_str = "\
Usage: sos_verify [OPTIONS] -s <SOS_PATH> \n\
    where <SOS_PATH> is the sos path (e.g. if you have /a/b_sos.OBJ, the\n\
    SOS_PATH is /a/b)\n\
\n\
    The program exits with code 0 if the store is healthy.\n\
    Otherwise, it exits with code -1.\n\
\n\
OPTIONS:\n\
    -i,--idxonly\n\
        Only verify indices.\n\
\n\
    -s,--store STORE_PATH\n\
        Path of the SOS store (required).\n\
\n\
    -h,--help\n\
        Print this message.\n\
";

const char *short_opt = "is:?";
const struct option long_opt[] = {
	{"idxonly",  no_argument,        NULL,  'i'},
	{"store",    required_argument,  NULL,  's'},
	{"help",     no_argument,        NULL,  '?'},
	{0,          0,                  0,     0},
};

void usage()
{
	printf("%s\n", usage_str);
	_exit(-1);
}

int recover = 0;
const char *sos_path = NULL;

int main(int argc, char **argv)
{
	int attr_count;
	int attr_id;
	int rc = 0;
	char c;
	sos_iter_t iter;
	uint64_t count_base = 0;
	uint64_t count;
	int idxonly = 0;

arg_loop:
	c = getopt_long(argc, argv, short_opt, long_opt, NULL);
	if (c < 0)
		goto out_arg_loop;
	switch (c) {
	case 'i':
		idxonly = 1;
		break;
	case 's':
		sos_path = optarg;
		break;
	default:
		usage();
	}
	goto arg_loop;
out_arg_loop:
	if (!sos_path) {
		printf("ERROR: No sos path given\n");
		usage();
	}

	sos_t sos = sos_open(sos_path, O_RDWR);
	if (!sos) {
		printf("ERROR: Cannot open SOS: %s\n", sos_path);
		_exit(-1);
	}

	/* Verify _sos ODS */
	if (!idxonly) {
		if (ods_verify(sos->ods) != 0) {
			printf("ERROR: SOS ODS corrupted\n");
			_exit(-1);
		}
		printf("INFO: %s:ODS verified\n", sos_path);
	}

	/* Verify Index */
	attr_count = sos_get_attr_count(sos);
	for (attr_id = 0; attr_id < attr_count; attr_id++) {
		sos_attr_t attr = sos_obj_attr_by_id(sos, attr_id);
		if (!sos_attr_has_index(attr))
			continue;
		rc = sos_verify_index(sos, attr_id);
		if (rc)
			goto corrupted;
		iter = sos_attr_iter_new(sos, attr_id);
		count = 0;
		sos_iter_begin(iter);
		while (sos_iter_next(iter) == 0)
			count++;
		if (count_base == 0)
			count_base = count;
		if (count != count_base)
			goto corrupted;

		printf("INFO: Index %s:%s verified\n", sos_path, attr->name);

		continue;

	corrupted:
		rc = -1;
		printf("ERROR: Index corrupted: %s:%s\n", sos_path, attr->name);
		break;
	}

	return rc;
}
