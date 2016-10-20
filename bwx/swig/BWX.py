#!/usr/bin/python

# Copyright (c) 2015-2016 Open Grid Computing, Inc. All rights reserved.
#
# This software is available to you under a choice of one of two
# licenses.  You may choose to be licensed under the terms of the GNU
# General Public License (GPL) Version 2, available from the file
# COPYING in the main directory of this source tree, or the BSD-type
# license below:
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions
# are met:
#
#      Redistributions of source code must retain the above copyright
#      notice, this list of conditions and the following disclaimer.
#
#      Redistributions in binary form must reproduce the above
#      copyright notice, this list of conditions and the following
#      disclaimer in the documentation and/or other materials provided
#      with the distribution.
#
#      Neither the name of Open Grid Computing nor the names of any
#      contributors may be used to endorse or promote products derived
#      from this software without specific prior written permission.
#
#      Modified source versions must be plainly marked as such, and
#      must not be misrepresented as being the original software.
#
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
# "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
# LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
# A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
# OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
# SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
# LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
# DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
# THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
# (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
# OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

import time
import sys
import sosdb.sos
import sosdb.SOS
import os

class Iterator(object):
    def __init__(self, container, schemaName, attrName, order=None):
        self.container = container
        self.schema = self.container.schema(schemaName)
        self.attr = self.schema.attr(attrName)

    def key(self):
        return sosdb.SOS.Key(self.attr)

    def begin(self):
        return self.attr.iterator().begin()

    def next(self):
        return self.attr.iterator().next()

    def prev(self):
        return self.attr.iterator().prev()

    def end(self):
        return self.attr.iterator().end()

    def inf(self, key):
        return self.attr.iterator().inf(key)

    def sup(self, key):
        return self.attr.iterator().sup(key)

def set_er_up():
    sosdb.SOS.Object.def_fmt = SOS.Object.table_fmt

    container = sosdb.SOS.Container("/NVME/0/SOS_ROOT/BWX_Job_Data")
    job_iter = Iterator(container, "Job", "Id")
    sample_iter = Iterator(container, "Sample", "JobTime")
    return (job_iter, sample_iter)

if __name__ == "__main__":
    import datetime

    sosdb.SOS.Object.def_fmt = sosdb.SOS.Object.table_fmt

    container = sosdb.SOS.Container("/NVME/0/SOS_ROOT/BWX_Job_Data")
    job_iter = Iterator(container, "Job", "Id")
    sample_iter = Iterator(container, "Sample", "JobTime")
    sample_key = sample_iter.key()

    job = job_iter.begin()
    print(job.table_header())
    while job is not None:
        print(job)
        jobtime = int(job.Id) << 32
        sample_key.set(str(jobtime))
        sample = sample_iter.sup(sample_key)
        while sample and (int(sample.JobTime) >> 32) == job.Id:
            print(sample.current_freemem)
            sample = sample_iter.next()
        job = job_iter.next()

