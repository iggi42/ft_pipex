#include "pipex_parse.h"
#include "pipex_utils.h"
#include <libft_arr.h>
#include <libft_io.h>
#include <libft_mem.h>
#include <libft_merle.h>
#include <libft_str.h>

void	print_ptr(void *ptr)
{
	ft_printf("[%p]\n", ptr);
}

void	print_str(char *s)
{
	ft_printf("[%s]\n", s);
}

void	print_cmd(void *ptr)
{
	t_cmd	*c;

	c = ptr;
	ft_printf("cmd at :[%p]\n", ptr);
	print_str_arr(c->argv);
	// ft_arr_each((t_arr)c->argv, print_ptr);
	ft_printf("reds: %p\n", c->reds);
}

t_arr_el	my_parse(t_arr_el el)
{
	t_cmd	*result;

	// ft_printf("parser_input [%s]\n", el);
	result = my_malloc(sizeof(t_cmd));
	ft_bzero(result, sizeof(t_cmd));
	parse_cmd(el, result);
	return (result);
}

int	main(int argc, char **argv)
{
	t_cmd	cmd;
	char	*argv2[] = {"echo", "penis", NULL};
	t_cmd	*my_pipe[2];

	(void)argc;
	(void)argv;
	my_pipe[0] = &cmd;
	my_pipe[1] = NULL;
	cmd.argv = argv2;
	cmd.reds = NULL;
	return (pipex_exec_pipe(my_pipe));
	// if (argc < 2)
	// 	return (EXIT_FAILURE);
	// cmds = ft_arr_nmap((t_arr)argv + 1, argc - 1, my_parse);
	// if (cmds)
	// 	ft_arr_each(*cmds, print_cmd);
}
