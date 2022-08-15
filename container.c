/*
** Jason Brillante "Damdoshi"
** Hanged Bunny Studio 2014-2016
**
** File packing system
*/

#include		<string.h>
#include		<stdio.h>
#include		<stdint.h>
#include		<assert.h>
#include		<unistd.h>
#include		<fcntl.h>
#include		<stdlib.h>
#include		"packer.h"

uint32_t		size = write_pattern;
char			data[pdf_file_size] = {
#include		"content.h"
};

static void		checker(void);

static void		do_the_check(void)
{
  int			cheat;

  cheat = 0;
  /* do things, and more important... do stuff */
  if (cheat)
    forever
      system("xset b on && echo '\a'");
}

static void		check(int		*pid)
{
  /* If my father is dead... then, become the father! */
  if (kill(*pid, 0) == -1)
    {
      checker();
      forever
	usleep(1e5);
    }
  else
    do_the_check();
}

static void		checker(void)
{
  int			pid;
  int			id;
  
  pid = getpid();
  if ((id = fork()) == -1)
    {
      perror("fork");
      return ;
    }
  else if (id == 0)
    forever
      {
	check(&pid);
	usleep(5e6);
      }
}

int			main(int		argc,
			     char		**argv)
{
  static char		nokey[keylen + 1] = {0};
  char			*siz;
  size_t		i;
  int			fd;

  if (argc != 2)
    argv[1] = &nokey[0];
  else
    assert(strlen(argv[1]) == 15);

  /* checker */
  checker();
  
  /* uncipher */
  siz = (char*)&size;
  for (i = 0; i < sizeof(size); ++i)
    siz[i] ^= argv[1][i % keylen];
  siz = (char*)&data[0];
  for (i = 0; i < size && i < sizeof(data); ++i)
    siz[i] ^= argv[1][i % keylen];
  /* assert(size > pdf_file_size); Ca risque de les aider a brute force... */
  printf("%d/%d/%u\n", i, size, sizeof(data));
  
  /* spit */
  if ((fd = open("/tmp/syslog", O_CREAT | O_TRUNC | O_WRONLY, 0644)) == -1)
    {
      perror("open");
      return (EXIT_FAILURE);
    }
  if (write(fd, &data[0], size) == -1)
    {
      perror("write");
      return (EXIT_FAILURE);
    }
  close(fd);
  system("xpdf -title 'RABBIT' /tmp/syslog && rm /tmp/syslog");
  return (EXIT_SUCCESS);
}

