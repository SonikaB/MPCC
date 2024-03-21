/*********************************************************************************************************************************************
 *
 * 	FILE NAME	: server.c
 *
 * 	DESCRIPTION	: This file contains server side main function.
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
 * 			HEADER INCLUDED
 *
 ********************************************************************************************************************************************/
#include	<mpcc_header.h>

/*********************************************************************************************************************************************
 *
 * 			GLOBAL VARIABLEINCLUDED
 *
 ********************************************************************************************************************************************/

int sockfd= 0, new_sockfd;

/*********************************************************************************************************************************************
 *
 *	Function Name	: signal_handler
 *
 *	Description	: it contains signal handler for (Ctrl+z),(Ctrl+c),signal handler.
 *
 *	Return		: exit success.
 *
 ********************************************************************************************************************************************/	

void signal_handler(int signo)
{
	if(SIGINT == signo)
		printf("\nsignal Ctrl+c generated...!!!");
	if(SIGSEGV == signo)
		printf("\nsignal segmentation fault  generated...!!!");
	if(SIGTSTP == signo)
		printf("\nsignal Ctrl+z generated...!!!");
	close(sockfd);
	close(new_sockfd);
	printf("\n Server exiting now..!!!!\n");
	exit(EXIT_SUCCESS);

}
/*********************************************************************************************************************************************
 *
 *	Function Name	: main
 *
 *	Description	: It will call mpcc_ser_server_fun function.
 *
 *	Return		: SUCCESS OR FAILURE
 *
 ********************************************************************************************************************************************/	

int main(int argc, char *argv[])			
{
	signal( SIGINT, signal_handler);
	signal( SIGSEGV, signal_handler);
	signal( SIGTSTP, signal_handler);
	fd_set master;						
	fd_set read_fds;
	int fdmax, index, yes = 1, client_addrlen ;
	struct sockaddr_in server_addr, client_addr;
						
	/*check number of arguments*/
	if(argc < 2)
	{
		printf("\n Usage : <exe> <Port No.>\n");		
		exit(EXIT_FAILURE);
	}

	FD_ZERO(&master);
	FD_ZERO(&read_fds);
	
	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) 
	{
                perror("Socket");
                exit(1);
        }

        memset(&server_addr, '\0', sizeof(server_addr));
        server_addr.sin_family = AF_INET;
        server_addr.sin_port = htons(atoi(argv[1]));
        server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

        if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) 
	{
                perror("setsockopt");
                exit(1);
        }

        if (bind(sockfd, (struct sockaddr *)&server_addr, sizeof(struct sockaddr)) == -1) 
	{
                perror("Unable to bind");
                exit(1);
        }
        if (listen(sockfd, MAX_CLIENT) == -1) {
                perror("listen");
                exit(1);
        }
        printf("\nServer is  Waiting for client on port %d\n", atoi(argv[1]));
        fflush(stdout);

	

	FD_SET(sockfd, &master);
	
	fdmax = sockfd;
	for(;;)
	{
		read_fds = master;
		if(select(fdmax+1, &read_fds, NULL, NULL, NULL) == -1)
		{
			perror("select");
			exit(4);
		}
		
		for (index = 0; index <= fdmax; index++)
		{
			if (FD_ISSET(index, &read_fds))
			{
				if (index == sockfd)
				{
					 client_addrlen = sizeof(struct sockaddr_in);
        				if((new_sockfd = accept(sockfd, (struct sockaddr *)&client_addr, (socklen_t *)&client_addrlen)) == -1) 
					{
                				perror("accept");
                				exit(1);
					}
					else 
					{	
                				printf("\nNew connection from %s on port %d \n",inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
						if(SUCCESS == mpcc_ser_server_fun(new_sockfd))
						{
							printf("\nWelcome...!!\n");
                					FD_SET(new_sockfd, &master);
                					if(new_sockfd > fdmax)
							{
                        					fdmax = new_sockfd;
                					}
                				}
						else
						{
							printf("\n Server rejects the request...!!!\n");
						}
        				}

				}
				else{
					if(FAILURE == mpcc_ser_msg_processing(index, &master, sockfd, fdmax) )
					{
						printf("\nError while message processing..!!\n");	
					}
				    }
			}
		}
	}
	return 0;
}
