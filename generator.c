/*
** Jason Brillante "Damdoshi"
** Hanged Bunny Studio 2014-2016
**
** File packing system
*/

#include	<stdlib.h>
#include	<assert.h>
#include	<unistd.h>
#include	<fcntl.h>
#include	<stdio.h>
#include	"packer.h"

int		main(void)
{
  FILE		*fd;
  int		i;
  
  assert((fd = fopen("content.h", "w")) != NULL);
  for (i = 0; i < pdf_file_size - 1; ++i)
    fprintf(fd, "%d,", i);
  fprintf(fd, "%d", i);
  fclose(fd);
  return (EXIT_SUCCESS);
}

