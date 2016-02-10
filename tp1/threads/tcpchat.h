#ifndef __TCPCHAT_H__
#define __TCPCHAT_H__

#define BUFFER_SIZE 4096

struct moche
{
  int			csock;
  struct sockaddr_in	*csin;
};

void	open_connection(int port);
void	wait_client(int sock, struct sockaddr_in *sock_in);
void	listen_client(int sock, struct sockaddr_in *sock_in);

void	*thread_entry(void *arg);

#endif
