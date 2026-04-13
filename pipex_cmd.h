
#include <libft_ll.h>
#include <libft_mem.h>

#define EXIT_CMD_NOT_FOUND 127
#define EXIT_NO_EXEC_PERM 126

typedef struct s_redi
{
	enum		e_redi_kind
	{
		INVALID = -1,
		IN,
		OUT,
		OUT_APPEND,
		HERE_DOC
	} kind;
	char		*target;
}				t_redi;

// reds are redirections see struct above for what the ll carry
typedef struct s_cmd
{
	char		**argv;
	t_list		*reds;
}				t_cmd;

typedef t_cmd	*t_pipe;
t_byte			pipex_exec_pipe(t_pipe *full_pipe);
