#include <stdio.h>

int
main (int argc, char **argv)
{
  FILE *outf;
  int i;

  if (argc < 3) {
    fprintf (stderr, "Usage: %s 1|2 arguments\n", argv[0]);
    return 1;
  }

  if (argv[1][0] == '1')
    outf = stdout;
  else
    outf = stderr;

  for (i=2; i<argc; i++)
    if (i == argc-1)
      fprintf (outf, "%s\n", argv[i]);
    else
      fprintf (outf, "%s ", argv[i]);

  return 0;
}
