# Uncache

``` text
NAME
       uncache − remove cached files from the pagecache

SYNOPSIS
       uncache [−0|−−null] [−n|−−dry‐run] [−v|−−verbose]

DESCRIPTION
       uncache  reads  a list of files from standard input and tries to remove
       all cached blocks of these files from the pagecache.  All files must be
       given as absolute file names.  Unreadable files will be ignored.

       The uncache program only drops cached blocks for the named files.  This
       is in contrast to the /proc/sys/vm/drop_caches sysctl which  will  drop
       cached blocks for all files.

OPTIONS
       −0, −−null
              Files  read from standard input are terminated by a null charac‐
              ter instead of by whitespace.  Useful  when  input  items  might
              contain  white space, quote marks, or backslashes.  The GNU find
              −print0 option produces input suitable for this mode.

       −n, −−dry‐run
              Do not actually do anything.  Can be used together with the ver‐
              bose option to check which files are currently in the pagecache.

       −v, −−verbose
              Verbose mode.

EXAMPLES
       find /var/lib/mysql ‐type f | uncache ‐‐dry‐run ‐‐verbose
              Show which MySQL data files are currently in the  pagecache  but
              do not purge the cached blocks.

       find /var/log ‐type f | uncache
              Remove  cached blocks for all files below the /var/log directory
              from the pagecache.

       find /var/log ‐type f ‐print0 | uncache ‐0
              The same as before but use zero  terminated  strings  to  handle
              unusual file names.
```
