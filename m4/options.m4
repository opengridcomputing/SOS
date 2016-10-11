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

dnl SYNOPSIS: OPTION_DEFAULT_ENABLE([name], [enable_flag_var])
dnl EXAMPLE: OPTION_DEFAULT_ENABLE([mysql], [ENABLE_MYSQL])
AC_DEFUN([OPTION_DEFAULT_ENABLE], [
AC_ARG_ENABLE($1, [  --disable-$1     Disable the $1 module],
        [       if test x$enableval = xno ; then
                        disable_$2=yes
			echo $1 module is disabled
                fi
        ])
AM_CONDITIONAL([$2], [test "$disable_$2" != "yes"])
])

dnl SYNOPSIS: OPTION_DEFAULT_DISABLE([name], [enable_flag_var])
dnl EXAMPLE: OPTION_DEFAULT_DISABLE([mysql], [ENABLE_MYSQL])
AC_DEFUN([OPTION_DEFAULT_DISABLE], [
AC_ARG_ENABLE($1, [  --enable-$1     Enable the $1 module],
        [       if test x$enableval = xyes ; then
                        enable_$2=yes
			echo $1 module is enabled
                fi
        ])
AM_CONDITIONAL([$2], [test "$enable_$2" == "yes"])
])

dnl SYNOPSIS: OPTION_WITH([name], [VAR_BASE_NAME])
dnl EXAMPLE: OPTION_WITH([sos], [SOS])
dnl NOTE: With VAR_BASE_NAEM being SOS, this macro will set SOS_INCIDR and
dnl 	SOS_LIBDIR to the include path and library path respectively.
AC_DEFUN([OPTION_WITH], [
AC_ARG_WITH(
	$1,
	AS_HELP_STRING(
		[--with-$1@<:@=path@:>@],
		[Specify $1 path @<:@default=/usr/local@:>@]
	),
	[WITH_$2=$withval],
	[WITH_$2=/usr/local]
)

if test -d $WITH_$2/lib64; then
	$2_LIB64DIR=$WITH_$2/lib64
	$2_LIBDIR_FLAG="-L$WITH_$2/lib64"
	LDFLAGS="$LDFLAGS -Wl,-rpath-link,$WITH_$2/lib64"
fi
if test -d $WITH_$2/lib; then
	$2_LIBDIR=$WITH_$2/lib
	$2_LIBDIR_FLAG="$$2_LIBDIR_FLAG -L$WITH_$2/lib"
	LDFLAGS="$LDFLAGS -Wl,-rpath-link,$WITH_$2/lib"
fi
if test -d $WITH_$2/include; then
	$2_INCDIR=$WITH_$2/include
	$2_INCDIR_FLAG=-I$WITH_$2/include
fi
AC_SUBST([$2_LIBDIR], [$$2_LIBDIR])
AC_SUBST([$2_LIB64DIR], [$$2_LIB64DIR])
AC_SUBST([$2_INCDIR], [$$2_INCDIR])
AC_SUBST([$2_LIBDIR_FLAG], [$$2_LIBDIR_FLAG])
AC_SUBST([$2_INCDIR_FLAG], [$$2_INCDIR_FLAG])
])

dnl Similar to OPTION_WITH, but a specific case for MYSQL
AC_DEFUN([OPTION_WITH_MYSQL], [
AC_ARG_WITH(
	[mysql],
	AS_HELP_STRING(
		[--with-mysql@<:@=path@:>@],
		[Specify mysql path @<:@default=/usr/local@:>@]
	),
	[	dnl $withval is given.
		WITH_MYSQL=$withval
		mysql_config=$WITH_MYSQL/bin/mysql_config
	],
	[	dnl $withval is not given.
		mysql_config=`which mysql_config`
	]
)

if test $mysql_config
then
	MYSQL_LIBS=`$mysql_config --libs`
	MYSQL_INCLUDE=`$mysql_config --include`
else
	AC_MSG_ERROR([Cannot find mysql_config, please specify
			--with-mysql option.])
fi
AC_SUBST([MYSQL_LIBS])
AC_SUBST([MYSQL_INCLUDE])
])
dnl this could probably be generalized for handling lib64,lib python-binding issues
AC_DEFUN([OPTION_WITH_EVENT],[
  EVENTLIBS="-levent -levent_pthreads"
  AC_ARG_WITH([libevent],
  [  --with-libevent=DIR      use libevent in DIR],
  [ case "$withval" in
    yes|no)
      AC_MSG_RESULT(no)
      ;;
    *)
     EVENTINC="-I$withval/include"
     EVENTLIBS="-L$withval/lib -L$withval/lib64 $EVENTLIBS"
      ;;
    esac ])
  option_old_libs=$LIBS
  AC_CHECK_LIB(event, event_base_new, [],
      AC_MSG_ERROR([libevent not found.]),[$EVENTLIBS])
  AC_SUBST(EVENTLIBS)
  AC_SUBST(EVENTINC)
  LIBS=$option_old_libs
])

dnl
AC_DEFUN([OPTION_DOC_GENERATE],[
if test -z "$ENABLE_DOC_$1_TRUE"
then
	GENERATE_$1=YES
else
	GENERATE_$1=NO
fi
AC_SUBST(GENERATE_$1)
])

dnl For doxygen-based doc
AC_DEFUN([OPTION_DOC],[
OPTION_DEFAULT_DISABLE([doc], [ENABLE_DOC])
OPTION_DEFAULT_DISABLE([doc-html], [ENABLE_DOC_HTML])
OPTION_DEFAULT_DISABLE([doc-latex], [ENABLE_DOC_LATEX])
OPTION_DEFAULT_ENABLE([doc-man], [ENABLE_DOC_MAN])
OPTION_DEFAULT_DISABLE([doc-graph], [ENABLE_DOC_GRAPH])
OPTION_DOC_GENERATE(HTML)
OPTION_DOC_GENERATE(LATEX)
OPTION_DOC_GENERATE(MAN)
OPTION_DOC_GENERATE(GRAPH)
])
