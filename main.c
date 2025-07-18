#include "includes/minishell.h"

void	ft_excecute(ASTNode*command, t_mini_sh*sh)
{
	int	status;
	char 	*path;

	status = 0;
	path = ft_strdup(command->args[0]);

	sh->mypid = fork();
	if (sh->mypid == -1)
	{
		perror("fork");
		exit(EXIT_FAILURE);
	}
	else if (sh->mypid == 0)
	{
		//comentarioooooo
		if (execve(path, command->args, NULL) == -1)
		{
			printf("%s: No such file or directory\n",path);
			free(path);
			exit(EXIT_FAILURE);
		}
	}
	else
		waitpid(sh->mypid, &status, 0);
	free(path);
}

int ft_strlen_array(char **s) // cuenta las palabras split 
{	
	int i;

	i = 0;
	if (!s)
		return (0);
	while (s[i])
		i++;
	return (i);
	
}
// access ( path , modo  X_OK  significa que  access esta preaguntando al USER si se puede ejecutar )    Si se puede devuelve 0  y si falla devuelve -1
int ft_check_access(char *path)
{

	if (access(path, X_OK) == 0)
		return (1);
	else 
		return (-1);
}

char *ft_get_path(ASTNode*command) // intento de refactor
{
	char 	*path;
	char 	**path_splited;
	int	len;
	int	count;
	char 	*to_exec;

	path = getenv("PATH");
	path_splited = ft_split(path,':');
	count = 0;
	len = ft_strlen_array(path_splited);

	to_exec = ft_strjoin("/", command->args[0]);	
	printf("numero de palabras[%d]\n\n", len); // eliminar
						   
/*	{	 Para ver las string  despues del split con : de delimitador
		int i = 0;
		while (i < len)
		{	printf("*%s*\n\n",path_splited[i]);  i++; }// a eliminar
	 
						
	}                  */ 
	
	
	while (count < len)
	{
		path_splited[count] = ft_strjoin(path_splited[count], to_exec);
		if (ft_check_access(path_splited[count]) == 1)
		{
			
			printf("command FFOUND \n"); // eliminar
			printf("*%d*\n",count); // a eliminar
			free(to_exec);
			return (path_splited[count]);
		}
/*		else  
		{
			free(path_splited[count]);
//			free(**path_splited);
			free(path_splited);
			free(to_exec);

		}              */
		printf("*%s*\n",path_splited[count]); // a eliminar
		count++;					
	}
	return (NULL);
//	free(to_exec);
//	return (path_splited[count]); // solo para que devuelva algo  
}


void	ft_excecute_path(ASTNode*command, t_mini_sh*sh)
{
	int	status;
	char *to_exec;

	to_exec = ft_get_path(command);
	
	status = 0;
	printf("****final[%s]\n", to_exec);
	sh->mypid = fork();
	if (sh->mypid == -1)
	{
		perror("fork");
		exit(EXIT_FAILURE);
	}
	else if (sh->mypid == 0)
	{
//		printf("[%s]\n",path); todo el path
		if (execve(to_exec, command->args, NULL) == -1) // algo no me cuadra en este if 
		{
			printf("%s: command not found\n",command->args[0]);
			free(to_exec);
			exit(EXIT_FAILURE);
		}  
	
	}
	else
		waitpid(sh->mypid, &status, 0);
	free(to_exec);
}

void ft_getinput(t_mini_sh*sh)
{
	char	*input;
	//char 	**args;
	(void)sh;	
	while (1)
	{
		input = readline(GRN "Minishell$>☠ " NRM);
		if (!input)
			perror("input error");
		if (input[0] == ' ' || input[0] =='\0')
		{
			free(input);
			ft_printf("");
		}
		else 
		{
			add_history(input);
			ft_printf("Has escrito " BLU "%s\n" NRM,input);
		

			Token *tokens = tokenizer(input);
			if (!tokens)
				continue ;
			Token *tokcpy = tokens;
			while (tokcpy)
			{
				printf("TOKEN %d: %s\n", tokcpy->type, tokcpy->value);
				tokcpy = tokcpy->next;
			}
			ASTNode *node = parse(&tokens);
			print_ast(node, 0);

			if (node->type != NODE_COMMAND)
				node = node->left;
			printf("*-*" BLU "%s" NRM "*-*\n",node->args[0]);
		//	if(node->args[0][0] == '/')   para otras opciones
			if(ft_strchr(node->args[0], '/') != NULL)
				ft_excecute(node, sh);
			else
				ft_excecute_path(node, sh);				
		}
	}


}

int	main(int argc, char**argv,char **env)
{
	t_mini_sh shell;	
	
	(void)argv;
	(void)env;
	if (argc == 1)
	{
		shell.mypid = getpid();
		shell.input = 0;
		shell.myfd = 1;	
		shell.envi = env;
		ft_getinput(&shell);
	}

	return (1);
}
