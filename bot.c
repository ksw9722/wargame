#include "global.h"

static void privmsg_handle(int fd, struct Msg *msg);
static void end_list_handle(int fd, struct Msg *msg);
static void ping_handle(int fd, struct Msg *msg);
static void join_handle(int fd, struct Msg *msg);

static char *next_param(char *cmd, char **ptr)
{
	static char buff[MAX_CMD_LEN + 1] = {0};
	int i;
	
	for(i = 0; cmd[i] && cmd[i] != ' '; i++)
	{
		buff[i] = cmd[i];
	}
	buff[i++] = 0;
	*ptr = cmd + i;
	
	return buff;
}
static char *get_nick(const char *from)
{
	static char nick[MAX_NICK_LEN + 1];
	int i;
	for(i = 0; from[i] && from[i] != '!'; i++)
	{
		nick[i] = from[i];
	}
	nick[i] = '\0';
	return nick;
}

static char *get_host(const char *from)
{
	static char host[MAX_FROM_LEN + 1];
	for(; *from && *from != '@'; from++);
	if(*from == '\0')
	{
		return NULL;
	}
	strncpy(host, from + 1, MAX_FROM_LEN);
	return host;
}

void init_handlers(void)
{
	add_handler("376", end_list_handle);
	add_handler("JOIN", join_handle);
	add_handler("PRIVMSG", privmsg_handle);
	add_handler("PING", ping_handle);
}

static void join_handle(int fd, struct Msg *msg)
{
	write_msg(fd, "MODE %s +v %s\r\n", CHANNEL, get_nick(msg->from));
}
static void ping_handle(int fd, struct Msg *msg)
{
	write_msg(fd, "PONG :%s\r\n", msg->data);
}

static void end_list_handle(int fd, struct Msg *msg)
{
	write_msg(fd, "JOIN %s\r\n", CHANNEL);
}

static void privmsg_handle(int fd, struct Msg *msg)
{
	char *ptr, *cmd;
	if(!strcmp(msg->to, NICK) && msg->data[0] == '!')
	{
		cmd = next_param(msg->data, &ptr);
		if(!strcmp(cmd, "!voice"))
		{
			cmd = next_param(ptr, &ptr);
			write_msg(fd, "MODE %s +v %s\r\n", CHANNEL, cmd);
		}
		else if(!strcmp(cmd, "!devoice"))
		{
			cmd = next_param(ptr, &ptr);
			write_msg(fd, "MODE %s -v %s\r\n", CHANNEL, cmd);
		}
		else if(!strcmp(cmd, "!op"))
		{
			cmd = next_param(ptr, &ptr);
			write_msg(fd, "MODE %s +o %s\r\n", CHANNEL, cmd);
		}		
		else if(!strcmp(cmd, "!deop"))
		{
			cmd = next_param(ptr, &ptr);
			write_msg(fd, "MODE %s -o %s\r\n", CHANNEL, cmd);
		}

		else
		{
			write_msg(fd, "PRIVMSG %s :Commands : !voice, !devoice\r\n", get_nick(msg->from));
		}
	}
}
