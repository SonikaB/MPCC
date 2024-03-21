/*********************************************************************************************************************************************
 *
 * 	FILE NAME	: client.c
 *
 * 	DESCRIPTION	: This file contains client side main function.
 *
 * 	DATE		NAME		REFERENCE		REASON
 *
 * 	27-Sept-2016	MPCC_TEAM	Initial-Creation	Nalanda Trainee Project
 *
 * 	Copyright @ Aricent Technologies & Holdings Limited 2016
 *
 *********************************************************************************************************************************************/

/*********************************************************************************************************************************************
 *
 * 			HEADERS INCLUDED
 *
 ********************************************************************************************************************************************/

#include    <mpcc_header.h>
/*********************************************************************************************************************************************
 *
 *	Function Name	: main
 *
 *	Description	: It will call mpcc_cli_client_function and mpcc_cli_chatting function. 
 *
 *	Return		: SUCCESS OR FAILURE
 *
 ********************************************************************************************************************************************/



int 	sockfd ; 
void signal_close(int signo)
{
	printf("\nClosing sockfd\n");
	//send_msg_to_server();
	close(sockfd);
	exit(0);
}		
	
int main(int argc, char *argv[])
{
	//int 	sockfd =0, 
	int	fdmax = 0, 
		index = 0, 
		ret_chat = 0;
	struct sockaddr_in server_addr;
	fd_set master;
	fd_set read_fds;
	signal(SIGINT, signal_close);
	if(argc < 3)
	{
		printf("\nUsage : <exe> <Server's I.P.Adress> <Server's Port>\n");
		exit(EXIT_FAILURE);
	}
	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) 
	{
                perror("\nSocket Error");
                exit(EXIT_FAILURE);
        }
	memset( &server_addr, 0, sizeof(server_addr));
        server_addr.sin_family = AF_INET;
        server_addr.sin_port = htons(atoi(argv[2]));
        server_addr.sin_addr.s_addr = inet_addr(argv[1]);

        if(connect(sockfd, (struct sockaddr *)&server_addr, sizeof(struct sockaddr)) == -1) 
	{
                perror("connect");
                exit(1);
        }
	
	FD_ZERO(&master);
        FD_ZERO(&read_fds);
        FD_SET(0, &master);
        FD_SET(sockfd, &master);
	fdmax = sockfd;
	
	if(SUCCESS != mpcc_cli_client_fun(sockfd))
	{
		printf("\n error while login / Register.., Exiting..!!!\n");
		close(sockfd);
		exit(EXIT_FAILURE);
	}

	while(1)
	{
		read_fds = master;
		if(select(fdmax+1, &read_fds, NULL, NULL, NULL) == -1)
		{
			perror("select");
			exit(EXIT_FAILURE);
		}
		
		for(index=0; index <= fdmax; index++ )
		{
		
				if(FD_ISSET(index, &read_fds))
				{
					ret_chat = mpcc_cli_chatting(index, sockfd);
					if(SUCCESS != ret_chat)
					{
						if(EXIT == ret_chat)
							printf("\nClient Exited\n");
						else
							printf("\nError while chatting, Exiting..!!\n");	
						close(sockfd);
						exit(EXIT_FAILURE);
					}
				}
		}
	
	}
	
	return 0;
}

