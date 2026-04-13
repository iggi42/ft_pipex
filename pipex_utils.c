#include <fcntl.h>
#include <libft_io.h>
#include <libft_mem.h>
#include <libft_merle.h>
#include <unistd.h>

void	panic(int exit_code)
{
	ft_m3_cleanup();
	exit(exit_code);
}

void	*my_malloc(size_t size)
{
	void	*result;

	result = ft_malloc(size);
	if (result)
		return (result);
	ft_putendl_fd("OUT OF MEMORY", STDOUT_FILENO);
	panic(EXIT_FAILURE);
	return (NULL);
}

void	print_str_arr(char **ar)
{
	ft_printf("[");
	while (ar != NULL && *ar != NULL)
	{
		ft_printf(" [%p] ", *ar);
		ar++;
	}
	ft_printf("]\n");
}
