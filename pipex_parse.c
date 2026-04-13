#include "pipex_cmd.h"
#include <libft_arr.h>
#include <libft_io.h>
#include <libft_ll.h>
#include <libft_mem.h>
#include <libft_str.h>

enum e_redi_kind	detect_redi(char *s)
{
	if (s == NULL)
		return (INVALID);
	if (!ft_strncmp(s, ">>", 2) && s[2] != '\0')
		return (OUT_APPEND);
	if (!ft_strncmp(s, "<<", 2) && s[2] != '\0')
		return (HERE_DOC);
	if (s[0] == '>' && s[1] != '\0')
		return (OUT);
	if (s[0] == '<' && s[1] != '\0')
		return (IN);
	return (INVALID);
}

t_redi	*is_redi_op(char *s)
{
	enum e_redi_kind	kind;
	t_redi				*result;

	kind = detect_redi(s);
	if (kind == INVALID)
		return (NULL);
	result = ft_malloc(sizeof(t_redi));
	result->kind = kind;
	if (kind == OUT || kind == IN)
		result->target = ft_strdup(s + 1);
	else
		result->target = ft_strdup(s + 2);
	// ft_printf("redi target %s\n", result->target);
	return (result);
}

// very simply syntax for now
// ft_split and redirection operator get
size_t	parse_cmd(char *textform, t_cmd *target)
{
	char	**split;
	t_list	*argv;
	t_list	*redis;
	size_t	i;
	t_redi	*r_cache;

	split = ft_split(textform, ' ');
	i = ft_arr_len((t_arr)split);
	argv = NULL;
	redis = NULL;
	while (i--)
	{
		r_cache = is_redi_op(split[i]);
		if (r_cache)
		{
			ft_free(split[i]);
			ft_lst_push(&redis, r_cache);
		}
		else
			ft_lst_push(&argv, split[i]);
	}
	ft_free(split);
	target->argv = (char **)ft_lst2arr(argv);
	target->reds = redis;
	return (ft_strlen(textform));
}
