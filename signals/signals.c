/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: israetor <israetor@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 12:35:40 by israetor          #+#    #+#             */
/*   Updated: 2025/08/26 12:48:00 by israetor         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minishell.h"
#include <signal.h>

volatile sig_atomic_t g_signal_vol = 0;


void	signal_reset_prompt(int signo)
{
	(void)signo;
	g_signal_vol = SIGINT;
	if(g_signal_vol == SIGINT)
	{
	//if (rl_line_buffer && rl_line_buffer[0] == '\0')
		write(1, "\n", 1);
	//else
	//	write(1, "\n^C\n", 4);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
	}
}

void	set_signals_interactive(void)
{
	struct sigaction	act;

	ignore_sigquit();
	ft_memset(&act, 0, sizeof(act));
	act.sa_handler = &signal_reset_prompt;
	sigaction(SIGINT, &act, NULL);
//	ft_disable_ctrl_backslash();
}



void	set_signals_noninteractive(void)
{
	struct sigaction	act;

	ft_memset(&act, 0, sizeof(act));
	act.sa_handler = &signal_sigint;
	sigaction(SIGINT, &act, NULL);
	sigaction(SIGQUIT, &act, NULL);
//	ft_disable_ctrl_backslash();
}

void	signal_sigint(int signal)
{
	(void)signal;
	g_signal_vol = SIGINT;
	rl_on_new_line();
	printf("solo nueva linea\n");
}

void	ignore_sigquit(void)
{
	struct sigaction	act;

	ft_memset(&act, 0, sizeof(act));
	act.sa_handler = SIG_IGN;
	sigaction(SIGQUIT, &act, NULL);
}

/* Ignorar SIGINT/SIGQUIT en el padre durante la espera de un hijo */
void	set_signals_parent_during_exec(void)
{
	struct sigaction	act;

	ft_memset(&act, 0, sizeof(act));
	act.sa_handler = SIG_IGN;
	sigaction(SIGINT, &act, NULL);
	sigaction(SIGQUIT, &act, NULL);
}

/* Restaurar por defecto SIGINT/SIGQUIT en el hijo antes de execve */
void	set_signals_child_exec(void)
{
	struct sigaction	act;

	ft_memset(&act, 0, sizeof(act));
	act.sa_handler = SIG_DFL;
	sigaction(SIGINT, &act, NULL);
	sigaction(SIGQUIT, &act, NULL);
}

/*
void    ft_disable_ctrl_backslash(void)
{
    struct termios term;

    if (tcgetattr(STDIN_FILENO, &term) == -1)
    {
        perror("tcgetattr");
        return;
    }
    term.c_lflag &= ~ECHOCTL;
    if (tcsetattr(STDIN_FILENO, TCSANOW, &term) == -1)
        perror("tcsetattr");
}*/
/*


void handle_sig_c(int sig)
{
    (void)sig;
    g_signal_vol = 1;
   if (rl_line_buffer && rl_line_buffer[0] == '\0') 
   {
      rl_on_new_line();
      rl_redisplay();
			// printf("^C\n");
		  printf("\n");

      rl_replace_line("", 0);
					rl_on_new_line();
			rl_redisplay();
	
    }
		else{ 
		 // printf("^C\n");
      rl_replace_line("", 0);
			rl_on_new_line();
			rl_redisplay();
	}
example de uso

if (g_signal == 1  || g_signal == SIGINT)
	{
		rl_on_new_line();
		rl_redisplay();
		ft_putstr_fd("\n", 1);
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_redisplay();
	}
	else if (g_signal == 0)
	{
		g_signal = S_SIGINT_CMD;
		ft_putstr_fd("\n", 1);
		rl_on_new_line();
	}
	else if (g_signal == 1)
	{
		ioctl(0, TIOCSTI, '\n');
		exit(0);
	}
	sigint_handler_aux();

////finn ***//*

}


void handle_sig_quit(int sig)
{
    (void)sig;
    g_signal_vol = SIGQUIT;
   // rl_on_new_line();
    rl_replace_line("", 0);
    rl_redisplay();
}

void ft_disable_echoctl(void)
{
    struct termios term;

    if (tcgetattr(STDIN_FILENO, &term) == -1)
    {
        perror("tcgetattr");
        return;
    }
    term.c_lflag &= ~ECHOCTL;
    if (tcsetattr(STDIN_FILENO, TCSANOW, &term) == -1)
        perror("tcsetattr");
}

void ft_setup_own(void)
{
// Restaurar señales en el hijo
	struct sigaction sa;
	sa.sa_handler = SIG_DFL;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	if (sigaction(SIGINT, &sa, NULL) == -1)
        printf("Error setting SIGINT handler\n");
    sigaction(SIGQUIT, &sa, NULL);


}


void ft_setup_signals()
{
    struct sigaction sa_int;
    struct sigaction sa_quit;

    sa_int.sa_handler = handle_sig_c;
    sigemptyset(&sa_int.sa_mask);
    sa_int.sa_flags = SA_RESTART;

    sa_quit.sa_handler = handle_sig_quit;
    sigemptyset(&sa_quit.sa_mask);
    sa_quit.sa_flags = SA_RESTART;

    sigaction(SIGINT, &sa_int, NULL);    // Ctrl+C
		sigaction(SIGQUIT, &sa_quit, NULL);  // Ctrl+ \      
//		ft_disable_echoctl();
//}




*/
/*

tcsetattr(int fd, int optional_actions, const struct termios *termios_p)

Qué hace: Cambia la configuración del terminal a la que pongas en termios_p.

optional_actions define cuándo aplicar el cambio:

TCSANOW → inmediato.

TCSADRAIN → espera a vaciar salida antes de aplicar.

TCSAFLUSH → como TCSADRAIN pero también limpia la entrada pendiente.

Retorno: 0 si va bien, -1 en error.

Ejemplo: desactivar eco de control (^C, ^\):

struct termios term;
tcgetattr(STDIN_FILENO, &term);
term.c_lflag &= ~ECHOCTL;  // oculta ^C, ^\
tcsetattr(STDIN_FILENO, TCSANOW, &term);

*/
