/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sheila <sheila@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+	#+#+#+#+  
		+#+           */
/*   Created: 2023/11/24 19:19:51 by shrodrig          #+#    #+#             */
/*   Updated: 2024/06/11 17:54:50 by sheila           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_get_line(char *str)
{
	char	*line;
	int		i;

	i = 0;
	if (!str[i])
		return (NULL);
	while (str[i] && str[i] != '\n')
		i++;
	if (str[i] == '\0')
		line = (char *)malloc(sizeof(char) * i + 1);
	else
		line = (char *)malloc(sizeof(char) * i + 2);
	if (!line)
		return (NULL);
	i = 0;
	while (str[i] && str[i] != '\n')
	{
		line[i] = str[i];
		i++;
	}
	if (str[i] == '\n')
		line[i++] = '\n';
	line[i] = '\0';
	return (line);
}

char	*ft_get_newstr(char *str)
{
	char	*newstr;
	int		i;
	int		j;

	i = 0;
	while (str[i] && str[i] != '\n')
		i++;
	if (!str[i])
	{
		free(str);
		return (NULL);
	}
	newstr = (char *)malloc(sizeof(char) * ft_strlen(str) - i);
	if (!newstr)
		return (NULL);
	j = 0;
	i++;
	while (str[i])
		newstr[j++] = str[i++];
	newstr[j] = '\0';
	free(str);
	return (newstr);
}

char	*ft_read_line(int fd, char *str)
{
	char	*buffer;
	int		bytesread;

	buffer = (char *)malloc((BUFFER_SIZE + 1) * sizeof(char));
	if (!buffer)
		return (NULL);
	bytesread = 1;
	while (!ft_strchr_gnl(str, '\n') && bytesread != 0)
	{
		bytesread = read(fd, buffer, BUFFER_SIZE);
		if (bytesread == -1)
		{
			free(buffer);
			free(str);
			return (NULL);
		}
		buffer[bytesread] = '\0';
		str = ft_strjoin_gnl(str, buffer);
	}
	free(buffer);
	if (ft_strlen(str) != 0)
		return (str);
	free(str);
	return (NULL);
}

char	*get_next_line(int fd)
{
	char		*line;
	static char	*str;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	str = ft_read_line(fd, str);
	if (!str)
		return (NULL);
	line = ft_get_line(str);
	str = ft_get_newstr(str);
	return (line);
}
