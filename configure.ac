#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([uncache], [1.1.0])
AM_INIT_AUTOMAKE([-Wall foreign])
AC_CONFIG_SRCDIR([uncache.c])
AC_CONFIG_HEADERS([config.h])

# Checks for programs.
AC_PROG_CC
AC_PROG_INSTALL

# Checks for libraries.

# Checks for header files.
AC_CHECK_HEADERS([fcntl.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_TYPE_OFF_T
AC_TYPE_SIZE_T
AC_TYPE_SSIZE_T

# Checks for library functions.
AC_FUNC_MMAP
AC_CHECK_FUNCS([fdatasync getdelim getpagesize munmap posix_fadvise],[],
  [AC_MSG_ERROR([missing library function])])

AC_CONFIG_FILES([Makefile])
AC_OUTPUT
