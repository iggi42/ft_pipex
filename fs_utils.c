#include "utils.h"
#include "cmd.h"
#include <stdbool.h>
#include <libft_os.h>
#include <libft_str.h>

bool	is_path(char *cmd)
{
	while (cmd != NULL)
	{
		if (*cmd == '/')
			return (true);
		if (*cmd == '\0')
			return (false);
		cmd++;
	}
	return (false);
}

char	*find_exec_file(char *cmd0)
{
	char	*path;

	if (!is_path(cmd0))
		path = ft_os_search_path(cmd0, __environ);
	else
		path = ft_strdup(cmd0);
	if (path == NULL || access(path, F_OK))
	{
		path = ft_strf("%s: command not found", cmd0);
		error_out(EXIT_CMD_NOT_FOUND, path, 0);
	}
	return (path);
}
