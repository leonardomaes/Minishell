/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmaes <lmaes@student.42porto.com>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 12:50:00 by lmaes             #+#    #+#             */
/*   Updated: 2024/11/07 12:50:02 by lmaes            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/* void sighandler(int signum) {
   printf("\nCaught signal %d, coming out...\n", signum);
   exit(1);
}

int main()
{
   // Press Ctrl+C to ignore (SIGINT)
   signal(SIGQUIT, sighandler);
   while(1) {
	  printf("Going to sleep for a second...\n");
	  sleep(1); 
   }
	//print_path(".");
	return (0);
} */

/* int main() {
	char term_buffer[2048]; // Buffer suficiente para descrever o terminal
	char *termtype = getenv("TERM");

	if (!termtype) {
		printf("Erro: variável TERM não definida.\n");
		return 1;
	}

	int status = tgetent(term_buffer, termtype);
	if (status == 1) {
		printf("Terminal %s configurado com sucesso!\n", termtype);
	} else if (status == 0) {
		printf("Terminal %s não encontrado.\n", termtype);
	} else {
		printf("Erro ao acessar o banco de dados Termcap.\n");
	}
	return 0;
} */

/*   int     main()
  {	
    const char *name;
    char  *bp;
    char  *term;
    int   height;
    int   width;

    bp = malloc(sizeof(*bp));
    name = "TERM";
    if ((term = getenv(name)) == NULL)
      return (1);
    printf("My terminal is %s.\n", term);
    tgetent(bp, term);
    height = tgetnum("li");
    width = tgetnum("co");
    printf("H : %d\nL : %d\n", height, width);
    free(bp);
    return (0);
  } */

/*   #include        <stdio.h>
  #include        <curses.h>
  #include        <term.h>
  #include        <stdlib.h>

  int     main()
  {
    const char *name;
    char  *bp;
    char  *term;
    int   flag;

    bp = malloc(sizeof(*bp));
    name = "TERM";
    if ((term = getenv(name)) == NULL)
      return (1);
    printf("My terminal is %s.\n", term);
    tgetent(bp, term);
    flag = tgetflag("hs");
    printf("flag hs = %d\n", flag);
    free(bp);
    return (0);
  } */

/* int main()
{
	DIR* dir = opendir(".");
	if (dir == NULL)
	{
		return (1);
	}
	struct dirent* entity;
	entity = readdir(dir);
	while (entity != NULL)
	{
		printf("%s\n", entity->d_name);
		entity = readdir(dir);
	}

	closedir(dir);
	return (0);
} */