/*
** Jason Brillante "Damdoshi"
** Hanged Bunny Studio 2014-2016
**
** File packing system
*/

#include		<stdio.h>
#include		<string.h>
#include		<stdint.h>
#include		<assert.h>
#include		<unistd.h>
#include		<fcntl.h>
#include		<stdlib.h>
#include		"packer.h"

/*
static char		*strnstr(char		*str,
				 const char	*pattern,
				 size_t		limit)
{
  size_t		lenp;
  size_t		lens;
  size_t		i;

  lenp = strlen(pattern);
  lens = strlen(str);
  for (i = 0; i + lenp < limit && i + lenp < lens; ++i)
    if (strncmp(str, pattern, lenp) == 0)
      return (str);
  return (NULL);
}
*/

static void		write_size(const char	*key,
				   char		*cnt,
				   int		cnt_siz,
				   uint32_t	pdf_siz)
{
  char			*dup = (char*)&pdf_siz;
  uint32_t		*ptr = (uint32_t*)cnt;
  int			i;

  // Cipher the key
  for (i = 0; i < (int)sizeof(pdf_siz); ++i)
    dup[i] ^= key[i % keylen];
  for (i = 0; (void*)&ptr[i] < (void*)&cnt[cnt_siz] && ptr[i] != write_pattern; ++i);
  assert(&ptr[i] < &ptr[cnt_siz]);
  memcpy(&ptr[i], &pdf_siz, sizeof(pdf_siz));
}

static void		cipher_pdf(char		*pdf,
				   int		len,
				   const char	*key)
{
  int			i;

  for (i = 0; i < len; ++i)
    pdf[i] ^= key[i % keylen];
}

static void		write_pdf(char		*cnt,
				  int		cnt_siz,
				  const char	*pdf,
				  int		pdf_siz)
{
  int			j;
  int			i;

  for (i = 0; i < cnt_siz; ++i)
    {
      for (j = 0; j < pdf_file_size && i + j < cnt_siz && cnt[i + j] == (char)(j & 0xFF); ++j);
      if (j == pdf_file_size || i + j == cnt_siz)
	{
	  memcpy(&cnt[i], pdf, pdf_siz);
	  for (j = pdf_siz; i + j < pdf_file_size; ++j)
	    cnt[i + j] = rand() % 256;
	  return ;
	}
    }
  assert(0);
}

int			main(int		argc,
			     char		**argv)
{
  static char		pdf_file[pdf_file_size];
  static char		cnt_file[2 * sizeof(pdf_file)];
  int			cnt_siz;
  int			pdf_siz;
  int			cnt_fd;
  int			pdf_fd;

  if (argc != 4)
    {
      printf("%s: container pdf key\n", argv[0]);
      return (EXIT_SUCCESS);
    }
  assert(strlen(argv[3]) == keylen);
  assert((cnt_fd = open(argv[1], O_RDWR)) != -1);
  assert((pdf_fd = open(argv[2], O_RDONLY)) != -1);
  if ((cnt_siz = read(cnt_fd, &cnt_file[0], sizeof(cnt_file))) == -1)
    {
      perror("read container");
      return (EXIT_FAILURE);
    }
  if ((pdf_siz = read(pdf_fd, &pdf_file[0], sizeof(pdf_file))) == -1)
    {
      perror("read pdf");
      return (EXIT_FAILURE);
    }
  close(pdf_fd);
  
  write_size(argv[3], &cnt_file[0], cnt_siz,  pdf_siz);
  cipher_pdf(&pdf_file[0], pdf_siz, argv[3]);
  write_pdf(&cnt_file[0], cnt_siz,  &pdf_file[0], pdf_siz);
  
  lseek(cnt_fd, 0, SEEK_SET);  
  write(cnt_fd, &cnt_file[0], cnt_siz);
  close(cnt_fd);
  return (EXIT_FAILURE);
}

