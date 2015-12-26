#ifndef __TCPCHAT_H__
#define __TCPCHAT_H__

#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <netinet/in.h>

#define BUFFER_SIZE 2048

fd_set	readfds;
fd_set	writefds;
fd_set	exceptfds;
int	fdmax;

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
void		echo(char *args, t_client *client);
void		ack(char *args, t_client *client);
void		compute(char *args, t_client *client);
void		do_echo(char *argcs, t_client *client, int display);
void		add_all(fd_set *set);
void		do_listen(int sock, struct sockaddr_in *sock_in);
void		do_recv();

char		*itoa(int n);

int		receive(t_client *client, char *buffer, int buffer_size);
int		send_buffer(int socket, char *buffer, int len);
int		fibo(int n);

t_client	*add_client(t_client *list, t_client *client);

#endif
