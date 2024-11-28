/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmaes <lmaes@student.42porto.com>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 19:03:45 by lmaes             #+#    #+#             */
/*   Updated: 2024/11/25 19:03:46 by lmaes            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char *prompt()
{
	char *line;

	line = readline(" % ");
	if (!line)
	{
		free(line);
		return (NULL);
	}
	return (line);
}

t_data  *ft_readline(void)
{
	t_data *data;
	char	*line;

	data = malloc(sizeof(t_data));
	if (!data)
		return (NULL);
	line = prompt();		// Le o input
	// Ler quantidade de pipes e dividir os tokens
	// Start parser
	return (data);
}
/*
Separar os tokens em tipos e ordem de entrada
Integers
Chars
Id
Pipes
Simbols
...

Ex: -(x + 5) * 2

		     *			1º Tree node
			/ \
	 	   -   2		Left and Right       - 2º Tree node
		  /		
  		 +				Left of left Token   - 3º Tree node
  		/ \
	   x   5			Left and Rigth Token - 4º Tree node

Parser deve ser recursivo e chamar left e right recursivamente
Se node for +: Retornar soma do left e right
Se node for -: Retornar subtraçao do left e right / Se for logo apos outra soma ou subtraçao, retornar -(resultado left e right)
Se node for Int: Retorna o valor
Se node for ID: Descobrir seu valor e retorna-lo
Possiveis retornos:
NULL = possivel erro
0 = fim do caminho de sub-trees
1 = Sem erros

1º scanToken()
2º resultTree = parse()
3º if (nextToken != \n || nextToken != \0)
		ERROR
resultTree.print()

<------------------------------>
Parse Factor:
F -> ID | Int | (E) | -F

if nextToken == ID
	return (nextToken)
else if nextToken == Int
	return (nextToken)
else if nextToken == "("
	a = parseE()				// Expressao dentro dos parenteses
	if (a == null)
		return (NULL)
	if nextToken == ")"
		return (a)
	else
		return (NULL)
else if nextToken == "-"
	return (parseF(newObj))
else
	return (NULL)

<------------------------------>
Parse Expression:
E -> T + E | T - E | T

First attempt:
a = parseT()
if nextToken == "+"
	b = parseE()
	c = newAdd(a, b)
	return (c)
else if nextToken == "-"
	b = parseE
	c = newSubtract(a, b)
	return (c)
else
	return (a)


Incorrect Associativity:
E -> T - E

a = parseT
if ...
.
.
else if nextToken == "-"
	b = parseE()
	return subtract(a, b)

Input: 7-5+1 should be (7-5)+1
Call parseE: 7 - <...E...> in recursive call
Final result == 7-(5+1)

How to fix:
Reverse T & E
E -> T + E | T - E | T
E -> E - T | E - T | T

a = parseE()
if nextToken == "+"
	b = parseT()
	return (newAdd(a, b))
else if
...




<------------------------------>
*/