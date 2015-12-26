#include <stdlib.h>

char *itoa(int n)
{
  int	i;
  int	j;
  char	*out;

  i = 1;
  j = 0;
  while (i <= n)
    {
      i *= 10;
      ++j;
    }
  out = malloc(j * sizeof(char));
  i /= 10;
  j = 0;
  while (i != 0)
    {
      out[j] = ((n / i) % 10) + '0';
      ++j;
      i /= 10;
    }
  out[j] = '\0';
  return (out);
}
