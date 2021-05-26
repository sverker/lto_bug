Provoke a potential bug in gcc-10.3.

Build by running bash script ./build

and then run with ./exe

Return status from exe should 0 if OK and 1 if bug is present.


I'm not sure if this is a bug in gcc or if this is undefined behavior.

See function erts_qsort_partion_array() in file utils.c
for more info about the bug.
