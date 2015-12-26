#ifndef __CLIENT_H__
#define __CLIENT_H__

#define GBUFFER_SIZE	1 << 25
#define BUFFER_SIZE	2048

void	read_file();
void	launch_test(char *server, int port);
void	communicate(char *server, int port, int buffer_size);

#endif
