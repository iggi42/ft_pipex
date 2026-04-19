#include "bw.h"
#include "cmd.h"
#include "utils.h"
#include <libft_arr.h>
#include <libft_io.h>
#include <libft_mem.h>
#include <libft_merle.h>
#include <libft_str.h>

t_arr_el	my_parse(t_arr_el el)
{
	t_cmd	*result;

	result = my_malloc(sizeof(t_cmd));
	ft_bzero(result, sizeof(t_cmd));
	parse_cmd(el, result);
	return (result);
}

void	set_arr(t_arr *ar, size_t i, void *new_val)
{
	if(ar == NULL)
		return;
	*ar[i] = new_val;
}

// echo test | cat -n < Makefile
int	main(int argc, char **argv)
{
	t_cmd	*my_pipe[3];
	t_redi	my_dig_out;
	int		res;
	t_cmd cmd1, cmd2;
	(void)argc;
	(void)argv;
	my_pipe[0] = &cmd1;
	my_pipe[1] = &cmd2;
	my_pipe[2] = NULL;
	my_dig_out.kind = IN;
	my_dig_out.target = ft_strdup("Makefile");
	cmd1.argv = ft_split("xxd", ' ');
	cmd1.reds = (t_redi **)ft_arr_new(12);
	cmd1.reds[0] = &my_dig_out;
	// cmd1.reds[1] = NULL;
	cmd2.argv = ft_split("cat -n", ' ');
	cmd2.reds = (t_redi **)ft_arr_new(12);
	// cmd2.reds[0] = &my_dig_out;
	cmd2.reds[1] = NULL;
	// set_arr((t_arr *)&cmd2.reds, 0, &my_dig_out);
	res = pipex_exec_pipe(my_pipe);
	ft_m3_cleanup();
	return (0);
	// if (argc < 2)
	// 	return (EXIT_FAILURE);
	// cmds = ft_arr_nmap((t_arr)argv + 1, argc - 1, my_parse);
	// if (cmds)
	// 	ft_arr_each(*cmds, print_cmd);
}
