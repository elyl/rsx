#ifndef __TCPCHAT_H__
#define __TCPCHAT_H__

#define BUFFER_SIZE 255

typedef struct s_client
{
  int			sock;
  struct sockaddr_in	sock_in;
  struct s_client	*next;
  struct s_client	*prev;
}	t_client;

typedef struct s_client_list
{
  t_client	*list;
  pthread_t	nb_clients;
  int		mutex;
}	t_client_list;

typedef struct s_command
{
  char	*text;
  void	(*fn)(char*, t_client*);
}	t_command;

void		start_listening(int port);
void		delete_client(t_client *client);
void		send_all(char *buffer, size_t len);
void		close_client(t_client *client);
void		echo(char *arg);
void		ack(char *arg);
void		compute(char *agr);

void		*listen_client(void *arg);

size_t		strlenn(const char *buffer);

t_client	*add_client(t_client *list, t_client *client);

#endif
