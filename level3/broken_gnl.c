/*
 * EXERCISE: BROKEN_GNL (Get Next Line)
 * 
 * DESCRIPTION:
 * Implement get_next_line that reads line by line from a file descriptor.
 * This version may contain special cases or intentional "bugs".
*/

#include "get_next_line.h"
#include <stdio.h>

char *ft_strchr(char *s, int c) //need to fix
{
  int i = 0;
  if(!s)
    return NULL;
  while(s[i] && s[i] != c)
    i++;
  if (s[i] == c)
    return s + i;
  else
    return NULL;
}

void *ft_memcpy(void *dest, const void *src, size_t n)
{
  /*while(--n > 0)
    ((char *)dest)[n - 1] = ((char *)src)[n - 1];
  return dest;*/

  size_t i = 0;

  if (!dest || !src)
		return (NULL);
  while(i < n)
  {
    ((char *)dest)[i] = ((char *)src)[i];
    i++;
  }
  return dest;
}

size_t ft_strlen(char *s)
{
//   size_t res = 0;
//   while (*s)
//   {
//     s++;
//     res++;
//   }
//   return res;
    size_t i = 0;

    if(!s)
        return 0;
    while(s[i])
        i++;
    return i;
}

int str_append_mem(char **s1, char *s2, size_t size2) //make the return value
{
//   size_t size1 = ft_strlen(*s1);
//   char *tmp = malloc(size2 + size1 + 1);
//   if (!tmp)
//     return 0;
//   ft_memcpy(tmp, *s1, size1);
//   ft_memcpy(tmp + size1, s2, size2);
//   tmp[size1 + size2] = '\0';
//   free(*s1);
//   *s1 = tmp;
//   return 1; 

    size_t size1 = ft_strlen(*s1);
    char *tmp = (char *)malloc(size1 + size2 + 1);
    if(!tmp)
        return 0;
    if(s1 && *s1)
        ft_memcpy(tmp, *s1, size1);
    if(s2)
        ft_memcpy(tmp + size1, s2, size2);
    tmp[size1 + size2] = '\0';
    free(*s1);
    *s1 = tmp;
    return 1;
}

int str_append_str(char **s1, char *s2) //s1 = return s2 = save
{
    size_t size2 = ft_strlen(s2);
    return str_append_mem(s1, s2, size2);
}

void *ft_memmove(void *dest, const void *src, size_t n)
{
//   if (dest > src)
//     return ft_memmove(dest, src, n);
//   else if (dest == src)
//     return dest;
//   size_t i = ft_strlen((char *)src) - 1;
//   while (i >= 0)
//   {
//     ((char *)dest)[i] = ((char *)src)[i];
//     i--;
//   }
//   return dest;

    size_t i = 0;

    if(!dest || !src)
        return NULL;
    if(dest < src)
        return (ft_memcpy(dest, src, n));
    else
    {
        i = n - 1;
        while(i >= 0)
        {
            ((char*)dest)[i] = ((char*)src)[i];
            i--;
        }
    }
    return (dest);   
}

char *get_next_line(int fd) //need to trim save
{
  static char b[BUFFER_SIZE + 1] = ""; //static value remains until program end
  char *ret = NULL; //return value
  char *tmp;
  int read_ret;
  
  while(1)
  {
    tmp = ft_strchr(b, '\n');  // check if the save has a new line
    if(tmp) //if b has a newline
    {
        if(!str_append_mem(&ret, b, tmp - b + 1)) //make return
            return free(ret), NULL;
        ft_memmove(b, tmp + 1, ft_strlen(tmp + 1) + 1); //temp + 1 = character after nl and the last +1 is null terminaor
        return ret;
    }
    if(!str_append_str(&ret, b)) //if not finding nl and then save the values in ret
        return NULL;
    read_ret = read(fd, b, BUFFER_SIZE);
    if(read_ret <= 0) //empty string or program ending 
    {
        b[0] = '\0';
        if(ret && *ret)
            return ret;
        free(ret);
        return NULL;
    }
    b[read_ret] = '\0';
  }
}

#include <fcntl.h>  // For open()
#include <unistd.h> // For close()

int main(void)
{
    //int     fd;
    char    *line;
    int     line_count;

    line_count = 1;
    // Replace "test.txt" with a file that exists or create one
    // fd = open("test.txt", O_RDONLY);
    
    // if (fd == -1)
    // {
    //     perror("Error opening file");
    //     return (1);
    // }

    printf("--- Reading started ---\n");
    
    while ((line = get_next_line(1)) != NULL)
    {
        printf("Line [%02d]: %s", line_count++, line);
        
        // Since get_next_line uses malloc for the return string, 
        // we MUST free it in the main loop.
        free(line);
    }

    printf("\n--- Reading finished ---\n");
    
    //close(fd);
    return (0);
}