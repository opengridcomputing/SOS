dnl Copyright (c) 2015-2016 Open Grid Computing, Inc. All rights reserved.
dnl Copyright (c) 2015-2016 Sandia Corporation. All rights reserved.
dnl Under the terms of Contract DE-AC04-94AL85000, there is a non-exclusive
dnl license for use of this work by or on behalf of the U.S. Government.
dnl Export of this program may require a license from the United States
dnl Government.
dnl
dnl This software is available to you under a choice of one of two
dnl licenses.  You may choose to be licensed under the terms of the GNU
dnl General Public License (GPL) Version 2, available from the file
dnl COPYING in the main directory of this source tree, or the BSD-type
dnl license below:
dnl
dnl Redistribution and use in source and binary forms, with or without
dnl modification, are permitted provided that the following conditions
dnl are met:
dnl
dnl      Redistributions of source code must retain the above copyright
dnl      notice, this list of conditions and the following disclaimer.
dnl
dnl      Redistributions in binary form must reproduce the above
dnl      copyright notice, this list of conditions and the following
dnl      disclaimer in the documentation and/or other materials provided
dnl      with the distribution.
dnl
dnl      Neither the name of Sandia nor the names of any contributors may
dnl      be used to endorse or promote products derived from this software
dnl      without specific prior written permission.
dnl
dnl      Neither the name of Open Grid Computing nor the names of any
dnl      contributors may be used to endorse or promote products derived
dnl      from this software without specific prior written permission.
dnl
dnl      Modified source versions must be plainly marked as such, and
dnl      must not be misrepresented as being the original software.
dnl
dnl
dnl THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
dnl "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
dnl LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
dnl A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
dnl OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
dnl SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
dnl LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
dnl DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
dnl THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
dnl (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
dnl OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

AC_PREREQ(2.63)
AC_INIT([SOS], 3.3.0, tom@ogc.us)
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_AUX_DIR(config)
AM_CONFIG_HEADER(config.h)
AM_INIT_AUTOMAKE([foreign])
AM_PROG_LIBTOOL

dnl Checks for programs
AC_PROG_CC

dnl this will add document options. Please see m4/options.m4.
OPTION_DOC

OPTION_DEFAULT_DISABLE([bwx], [ENABLE_BWX])
OPTION_DEFAULT_ENABLE([yaml], [ENABLE_YAML])

if test -z "$ENABLE_YAML_TRUE"; then
dnl yaml stuff
OPTION_WITH([yaml], [YAML])
OLD_CFLAGS=${CFLAGS}
if test ! -z "${YAML_INCDIR}"; then
   CFLAGS="${CFLAGS} -I${YAML_INCDIR}"
fi
AC_CHECK_HEADER(yaml.h, [],
		AC_MSG_ERROR([yaml.h not found. Please specify yaml prefix with
			--with-yaml configuration option.]))
CFLAGS="${OLD_CFLAGS}"
AC_CHECK_LIB(yaml, yaml_parser_load, [],
		AC_MSG_ERROR([libyaml not found. Please specify yaml prefix with
			--with-yaml configuration option.]),
			[-lyaml ${YAML_LIBDIR_FLAG}])
fi

dnl Reset LIBS variable.
LIBS=""

dnl Swig stuff
OPTION_DEFAULT_DISABLE([swig], [ENABLE_SWIG])
if test -z "$ENABLE_SWIG_TRUE"
then
AM_PATH_PYTHON(2.6)
AC_PROG_SWIG
SWIG_PYTHON
fi

distdir=${PACKAGE_NAME}-${PACKAGE_VERSION}
AC_SUBST(ac_configure_args)
AC_SUBST(prefix)
AC_SUBST(distdir)

AC_CONFIG_FILES([
		Makefile
		doc/Doxyfile
		doc/Makefile
		ods/Makefile
		ods/include/Makefile
		ods/src/Makefile
		sos/Makefile
		sos/include/Makefile
		sos/src/Makefile
		sos/swig/Makefile
		bwx/Makefile
		bwx/include/Makefile
		bwx/src/Makefile
		bwx/swig/Makefile
		rpm/rhel7.spec
])

AC_OUTPUT
