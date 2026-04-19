
#include <stdlib.h>

void	panic(int exit_code);

void	*my_malloc(size_t size);
void	print_str_arr(char **ar);

pid_t	ft_fork(void);
void	error_out(int exit_code, char *msg, int error_code);
void ft_close(int fd);
