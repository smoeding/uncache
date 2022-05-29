/**************************************************************************
 *
 * Copyright (c) 2021, 2022 Stefan Möding <stm@kill-9.net>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 **************************************************************************/


#include "config.h"
#include <stdio.h>
#ifdef STDC_HEADERS
# include <stdlib.h>
# include <stddef.h>
#else
# ifdef HAVE_STDLIB_H
#  include <stdlib.h>
# endif
#endif
#if HAVE_FCNTL_H
# include <fcntl.h>
#endif
#include <getopt.h>
#if HAVE_UNISTD_H
# include <unistd.h>
#endif
#include <sys/mman.h>
#if HAVE_SYS_STAT_H
# include <sys/stat.h>
#endif


static int dry_run = 0;
static int verbose = 0;
static int version = 0;
static int delimiter = '\n';

static struct option long_options[] = {
  { "dry-run", no_argument, &dry_run, 1 },
  { "verbose", no_argument, &verbose, 1 },
  { "version", no_argument, &version, 1 },
  { "null", no_argument, &delimiter, '\0' },
};


//
// Count the number of cached blocks for a file
//
int fincore(int fd, off_t size) {
  size_t         page_size = getpagesize();
  int            cached    = 0;
  void          *file_mmap;
  unsigned char *mincore_vec;

  file_mmap = mmap((void *)0, size, PROT_NONE, MAP_SHARED, fd, 0);

  mincore_vec = calloc(1, (size + page_size - 1) / page_size);
  mincore(file_mmap, size, mincore_vec);

  for (size_t i=0; i <= size/page_size; i++) {
    if (mincore_vec[i] & 1) cached++;
  }

  free(mincore_vec);
  munmap(file_mmap, size);

  return cached;
}

//
// uncache a file
//
void uncache(char *file) {
  int fd = open(file, O_RDONLY);

  if (fd >= 0) {
    struct stat info;

    if (fstat(fd, &info) == 0) {
      if (S_ISREG(info.st_mode) > 0) {  /* regular file? */
        int cached = fincore(fd, info.st_size);

        if (cached > 0) {
          if (verbose == 1) printf("%s: %d pages\n", file, cached);

          if (dry_run == 0) {
            // write dirty data blocks to stable storage
            fdatasync(fd);

            // advise kernel that we don't need the blocks
            posix_fadvise(fd, 0, info.st_size, POSIX_FADV_DONTNEED);
          }
        }
      }
    }
    close(fd);
  }
}

int main(int argc, char *argv[]) {
  char    *buffer = NULL;
  size_t   bsize  = 0;
  int      index  = 0;
  int      warned = 0;
  ssize_t  chars;
  int      opt;

  /*
   * parse options
   */
  while ((opt = getopt_long(argc, argv, "0nv", long_options, &index)) != -1) {
    switch(opt) {
    case 0:
      // long option: nothing to do as getopt_long() sets the flag internally
     break;
    case '0':
      delimiter = '\0';
      break;
    case 'n':
      dry_run = 1;
      break;
    case 'v':
      verbose = 1;
      break;
    case '?':
      exit(1);
    }
  }

  if (version == 1) {
    printf("uncache %s\n", PACKAGE_VERSION);
    exit(0);
  }

  /*
   * read file names from stdin
   */
  while ((chars = getdelim(&buffer, &bsize, delimiter, stdin)) != -1) {
    if (chars > 0) {
      buffer[chars-1] = '\000';  // buffer must be NULL terminated

      if (buffer[0] == '/') {
        uncache(buffer);
      }
      else if (warned == 0) {
        fprintf(stderr, "Ignoring relative path names.\n");
        warned = 1;
      }
    }
  }

  free(buffer);

  exit(0);
}
