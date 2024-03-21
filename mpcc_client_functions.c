/*********************************************************************************************************************************************
 *
 * 	FILE NAME	: client_functions.c
 *
 * 	DESCRIPTION	: This file contains all client side function.
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
 * 			HEADER FILES INCLUDED
 *
 ********************************************************************************************************************************************/
#include	<mpcc_header.h>

/*********************************************************************************************************************************************
 *
 *	Function Name	: mpcc_cli_client_fun
 *
 *	Description	: It will accept sock descriptor and call 
 *
 *	Return		: SUCCESS OR FAILURE
 *
 ********************************************************************************************************************************************/
int mpcc_cli_client_fun(IN int sock_des)
{
	int	choice = 0;
	int	login_ret = 0;
	int	reg_ret = 0;
	int 	send_msg_ret = 0;
	char	*message = NULL;	
	char	ch = '\0';
	

	message =(char*)malloc(sizeof(char) * ACK_SIZE);
	if(NULL == message)
	{
		perror("\nMemory allocation error for message:");
		exit(EXIT_FAILURE);
	}

	printf("\n\t\t-----------------------------------\n");
	printf("\n\t\t------WELCOME TO THE MPCC----------\n");
	printf("\n\t\t-----------------------------------\n");
	printf("\n\t\t1. Login \n\t\t2. Register\n\t\t3. Exit\t");
	printf("\nYour Choice :\t");

        /*check if user wants to login or register */
	if( scanf("%d", &choice) ) 
	{
		switch(choice)
		{
			case 1: strcpy(message,"login");

                                /*send 'login' message to server */
				send_msg_ret = mpcc_cli_send_msg_to_server(sock_des, message); 
                                if(FAILURE == send_msg_ret)
                                {
                                        printf("\nError in sending message to server\n");
                                        exit(EXIT_FAILURE);
                                }

                                /*login to server */
				login_ret = mpcc_cli_login_to_server(sock_des); 
				if(SUCCESS != login_ret)  
				{
                                        /*if client is not registered */
					if(login_ret == NOT_REGISTERED)
						printf("\nYou are not registered to MPCC\n");

                                        /*if client enters wrong password */
					if(login_ret == WRONG_PASSWORD)
						printf("\nWrong credentials entered\n");	

				}
				else if(SUCCESS == login_ret)
				{
					printf("\nSuccessfully logged in to MPCC\t WELCOME..!!\n");
				}
				break;
			
			case 2: strcpy(message,"registr");

                                /* send 'register' message to server */
				send_msg_ret = mpcc_cli_send_msg_to_server(sock_des,message); 

                                if(FAILURE == send_msg_ret)
                                {
                                        printf("\nError in sending message to server\n");
                                        exit(EXIT_FAILURE);
                                }

                                /* registration */
				reg_ret = mpcc_cli_register_to_server(sock_des); 
				if(SUCCESS == reg_ret)
				{
					printf("\nSuccessfully Registered to the MPCC..!!\n,Do you want to Login now ? Press Y/y for login , n for exit\n");
					scanf("%c", &ch);

                                        /* if user wants to login after registration or not */
					if(ch == 'y' OR  ch == 'Y')  
					{
						strcpy( message , "SUCCESS"); 

                                                /*send 'SUCCESS' message to server as user wants to login */
						mpcc_cli_send_msg_to_server(sock_des, message); 

                                                /*login to server */
						login_ret = mpcc_cli_login_to_server(sock_des);
                                		if(SUCCESS == login_ret)
						{
                                       			printf("\nSuccessfully logged in to MPCC\t WELCOME..!!\n");
						}
						else if(LOGIN_TO_SERVER_ERROR == login_ret)
						{
							printf("\n Error in login, Exiting..!!!\n");
							free(message);
							exit(EXIT_FAILURE);
						}
					}
					else 
					{
						printf("\nBye Bye,EXiting..!!!\n");
						free(message);
						exit(EXIT_SUCCESS);
					}
				}
				else if( REGISTER_TO_SERVER_ERROR ==  reg_ret)
				{
					printf("\nExiting..!!!Registeration Faliure\n");
					free(message);
					exit(EXIT_FAILURE);
				}
				
				break;
			case 3:		exit(EXIT_SUCCESS); 

			default : 	printf("\nWrong choice entered..!!\n");
			
		}
	}

	else
	{
		printf("\nYou entered character or string rather than integer, Please enter a valid integer\n");
		free(message);
		exit(EXIT_FAILURE);
	}

	if(SUCCESS != login_ret)
	{
		printf("\nExiting Now....!!\n");
		free(message);
		exit(EXIT_FAILURE);
	}

	/* free the memory */	
	free(message);
	return SUCCESS;

}
	 
/*********************************************************************************************************************************************
 *
 *	Function Name	: mpcc_cli_get_login_details
 *
 *	Description	: It will get user id and password from client
 *
 *	Return		: SUCCESS / FAILURE
 *
 ********************************************************************************************************************************************/
int mpcc_cli_get_login_details(OUT char *user_id , OUT char *pwd)
{
	memset(user_id, '\0', sizeof(user_id) );
	printf("\n---------------------------------\n");
        printf("\n\t\tEnter User id:");
        scanf("%s", user_id);
	
	memset(pwd, '\0', sizeof(pwd) );
        printf("\n\t\tEnter Password:");
        scanf("%s", pwd);

        return SUCCESS;
}
	
/*********************************************************************************************************************************************
 *
 *	Function Name	: mpcc_cli_get_registration_details
 *
 *	Description	: It will get user id, name, dob and password from client
 *
 *	Return		: SUCCESS / FAILURE
 *
 ********************************************************************************************************************************************/
int mpcc_cli_get_registration_details(OUT char *user_id, OUT char *user_name, OUT char *dob, OUT char *pwd)
{

        char    *re_pwd = NULL;			/*re-enter password */
	int i =0;

	printf("\n\t\t-----------------------------------\n");
	printf("\n\t\t------MPCC REGISTRATION WINDOW-----\n");
	printf("\n\t\t-----------------------------------\n");
	fflush(stdout);

	re_pwd = (char*)malloc(sizeof(char) * PWD_SIZE);
	if(NULL == re_pwd)
	{
		perror("\nMemory allocation error for re_pwd:");
		exit(EXIT_FAILURE);
	}
        memset(user_id, '\0', USER_ID_SIZE);
        printf("\n\t\tEnter user id :");
	while(getchar() != '\n');
        fgets(user_id, USER_ID_SIZE-1, stdin);
	user_id[strlen(user_id) - 1] = '\0';
        do
        {
		if(strlen(pwd) < 8  && (i != 0) )
		{
		    printf("\nEntered Password is Weak, password should be equal to or more than 8 characters\nTry Again..!!\n");
		}
		if( (strcmp(pwd,re_pwd) != 0) )
		{
		    printf("\nPassword and Re-enter Password doesn't match\nTry Again..!!\n");
		}
	
        	memset(pwd, '\0', PWD_SIZE );
                printf("\n\t\tEnter password :");
        	fgets(pwd, PWD_SIZE, stdin);
		pwd[strlen(pwd) - 1] = '\0';
		
        	memset(re_pwd, '\0', PWD_SIZE );
                printf("\n\t\tPlease re-enter password :");
        	fgets(re_pwd, PWD_SIZE, stdin);
		re_pwd[strlen(re_pwd) - 1] = '\0';
		i++;
        }while((strncasecmp( pwd, re_pwd, strlen(pwd) ) != 0)  OR  (strlen(pwd) < 8));

        memset(user_name, '\0', USER_NAME_SIZE );
        printf("\n\t\tEnter user name :");
        fgets(user_name, USER_NAME_SIZE, stdin);
	user_name[strlen(user_name) - 1] = '\0';

        memset(dob, '\0', DOB_SIZE );
        printf("\n\t\tEnter d.o.b format:(dd/mm/yyyy) :");
        fgets(dob, DOB_SIZE, stdin);
	dob[strlen(dob) - 1] = '\0';

        /*free the memory */
        free(re_pwd);

	return SUCCESS;
}

/*********************************************************************************************************************************************
 *
 *	Function Name	: mpcc_cli_is_registered_client
 *
 *	Description	: It will check if client is already registered or not
 *
 *	Return		: SUCCESS / FAILURE
 *
 ********************************************************************************************************************************************/
int mpcc_cli_is_registered_client(IN int sock_des)
{
        int     rcv_data = 0;
        char    *message = NULL;

	message = (char*)malloc( sizeof(char) * ACK_SIZE);
	if(NULL == message)
	{
		perror("\nMemory allocation error for message:");
		exit(EXIT_FAILURE);
	}
        memset(message, '\0', sizeof(message) );

        /* receive message for successful login */
        rcv_data = mpcc_uti_recv_data(sock_des, message);
	printf("\nServer message for login : %s", message);
        if(FAILURE == rcv_data)
        {
                printf("\nError In receiving data\n");
		free(message);
                exit(EXIT_FAILURE);
        }

        /* if client enters wrong password */
	if(0 == strcasecmp(message, "WRONG_PASSWORD"))
	{
		printf("\nwrong password\n");
		free(message);
		return WRONG_PASSWORD;
	}

        /* if client is not registered */
	if(0 == strcasecmp(message, "NOT_REGISTERED"))
	{
		printf("\nnot registered user\n");
		free(message);
		return NOT_REGISTERED;
	}

        /*free the memory */
	free(message);
        return SUCCESS;
}

/*********************************************************************************************************************************************
 *
 *	Function Name	: mpcc_cli_is_registered_successfully
 *
 *	Description	: It will check if client registration is succesfull or not
 *	
 *	Return		: SUCCESS / FAILURE
 *
 ********************************************************************************************************************************************/
int mpcc_cli_is_registered_successfully(int sd)
{

        char    *buff = NULL;
        int     ret=0;

	buff = (char*)malloc(sizeof(char) * ACK_SIZE);
	if(NULL == buff)
	{
		perror("\nMemory allocation error for buff:");
		exit(EXIT_FAILURE);
	}
        memset(buff, '\0', sizeof(buff));
 
        /* if registration is successful or not */
        ret = mpcc_uti_recv_data(sd,buff);               /*if client is registered or not */

        /* if successful registration */
        if( (strcasecmp(buff, "SUCCESS" ) == 0) )
        {
		free(buff);
                return SUCCESS;
        }
        else
	{
                /* if not successful registration */
		free(buff);
                return FAILURE;
	}

        /*free the memory */
	free(buff);

	return SUCCESS;
}

/*********************************************************************************************************************************************
 *
 *	Function Name	: mpcc_cli_login_to_server
 *
 *	Description	: It will login client to server
 *
 *	Return		: SUCCESS / FAILURE
 *
 ********************************************************************************************************************************************/

int mpcc_cli_login_to_server(IN int sock_des)
{
        int      ret_get_login_detail = 0;
        int      ret_send_login_detail = 0;
        int      ret_is_reg_client = 0;
        char     *user_id = NULL;
        char     *pwd = NULL;

	user_id = (char*)malloc(sizeof(char) * USER_ID_SIZE);
	if(NULL == user_id)
	{
		perror("\nMemory allocation error for user_id:");
		exit(EXIT_FAILURE);
	}
	pwd = (char*)malloc(sizeof(char) * PWD_SIZE);
	if(NULL == pwd)
	{
		perror("\nMemory allocation error for pwd:");
		free(user_id);
		exit(EXIT_FAILURE);
	}

	printf("\n\t\t--------Welcome to MPCC--------");

        /* getting user id and password from client */
        
	ret_get_login_detail = mpcc_cli_get_login_details(user_id,pwd);  
        if(FAILURE == ret_get_login_detail)
        {
                printf("\nGet Login details failed\n");
		free(user_id);
		free(pwd);
                exit(EXIT_FAILURE);
        }

        /* sending user id and password to server */
	ret_send_login_detail = mpcc_cli_send_login_details_to_server( sock_des, user_id, pwd );
        if(FAILURE == ret_send_login_detail)
        {
                printf("\nSend Login details failed\n");
		free(user_id);
		free(pwd);
                exit(EXIT_FAILURE);
        }


        /* checking if client is registered or not */
        ret_is_reg_client = mpcc_cli_is_registered_client(sock_des);

        /*if user id does not match */
	if(NOT_REGISTERED == ret_is_reg_client)
        {
		printf("\nnot registered on mpcc\n");

	       	free(user_id);
		free(pwd);
                return NOT_REGISTERED ;
        }


        /* if password is incorrect */
	if(WRONG_PASSWORD == ret_is_reg_client)
	{
		printf("\nWrong password entered\n");
	
	 	free(user_id);
		free(pwd);
		return WRONG_PASSWORD;
	}


        /*free the memory */
	free(user_id);
	free(pwd);

        /* registered client, successfully logged in */
        return SUCCESS;
}

/*********************************************************************************************************************************************
 *
 *	Function Name	: mpcc_cli_register_to_server
 *
 *	Description	: It will do client registration
 *
 *	Return		: SUCCESS / FAILURE
 *
 ********************************************************************************************************************************************/
int mpcc_cli_register_to_server(IN int sd)
{
        int     ret = 0;
        int     ret_get_reg_detail =0;
        int     ret_send_reg_detail =0;
        char    *user_id = NULL;
        char    *user_name= NULL;
        char    *pwd = NULL;
        char    *dob = NULL;


	user_id = (char*)malloc(sizeof(char) * USER_ID_SIZE);
	if(NULL == user_id)
	{
		perror("\nMemory allocation error for user_id:");
		exit(EXIT_FAILURE);
	}
	user_name = (char*)malloc(sizeof(char) * USER_NAME_SIZE);
	if(NULL == user_name)
	{
		perror("\nMemory allocation error for user_name:");
		free(user_id);
		exit(EXIT_FAILURE);
	}
	pwd = (char*)malloc(sizeof(char) * PWD_SIZE);
	if(NULL == pwd)
	{
		perror("\nMemory allocation error for pwd:");
		free(user_id);
		free(user_name);
		exit(EXIT_FAILURE);
	}
	dob = (char*)malloc(sizeof(char) * DOB_SIZE);
	if(NULL == dob)
	{
		perror("\nMemory allocation error for dob:");
		free(user_id);
		free(user_name);
		free(pwd);
		exit(EXIT_FAILURE);
	}
        /* getting registration details from client */
        ret_get_reg_detail = mpcc_cli_get_registration_details( user_id, user_name,dob, pwd);
        if(FAILURE == ret_get_reg_detail)
        {
                printf("Error in getting registration details \n");
		free(user_id);
		free(user_name);
		free(pwd);
		free(dob);
                exit(EXIT_FAILURE);
        }

        /* sending registration details to server */
        ret_send_reg_detail = mpcc_cli_send_registration_details_to_server(sd,user_id,user_name,dob, pwd);
        if(FAILURE == ret_send_reg_detail)
        {
                printf("Error in sending registration details to server \n");
		free(user_id);
		free(user_name);
		free(pwd);
		free(dob);
                exit(EXIT_FAILURE);
        }

        /* receive message from server if client is successfully registered or not */
        ret = mpcc_cli_is_registered_successfully(sd);
        if(SUCCESS == ret)
        {
                printf("\t You are successfully registered to MPCC\n");
        }
        else
        {
                printf("\t You are not registered successfully \n");
		free(user_id);
		free(user_name);
		free(pwd);
		free(dob);
                return REGISTER_TO_SERVER_ERROR;
        }
	
        /*free the memory */
	free(user_id);
	free(user_name);
	free(pwd);
	free(dob);
        return SUCCESS;
}

/*********************************************************************************************************************************************
 *
 *	Function Name	: mpcc_cli_send_login_details_to_server
 *
 *	Description	: It will send clients login details to server
 *
 *	Return		: SUCCESS / FAILURE
 *
 ********************************************************************************************************************************************/
int mpcc_cli_send_login_details_to_server(IN int sock_des, IN char *user_id, IN char *pwd)
{
        int     ret_send = 0;

        strcpy(user_id, mpcc_uti_encrypt_data( user_id ));
	if(NULL == user_id)
	{
		printf("\nError while decrypting user_id\n");
		return FAILURE;
	}
	ret_send = mpcc_uti_send_data(sock_des, user_id);
        if(FAILURE == ret_send)
        {
                printf("\nError in sending user id\n");
                exit(EXIT_FAILURE);
        }

        strcpy(pwd,mpcc_uti_encrypt_data( pwd ));
	if(NULL == pwd)
	{
		printf("\nError while decrypting pwd\n");
		return FAILURE;
	}
	ret_send = mpcc_uti_send_data(sock_des, pwd);
        if(FAILURE == ret_send)
        {
                printf("\nError in sending pwd\n");
                exit(EXIT_FAILURE);
        }

        return SUCCESS;
}

/*********************************************************************************************************************************************
 *
 *	Function Name	: mpcc_cli_send_msg_to_server
 *
 *	Description	: It will send message to server if user wants to login or register
 *
 *	Return		: SUCCESS / FAILURE
 *
 ********************************************************************************************************************************************/
int mpcc_cli_send_msg_to_server(IN int sd,IN char  *str)
{
	char 	*message = NULL;		/* send to server */
        int     ret_send = 0;           	/* return send */

	message = (char*)malloc(sizeof(char) * ACK_SIZE);
	if(NULL == message)
	{
		perror("\nMemory allocation error for message:");
		exit(EXIT_FAILURE);
	}
	strcpy( message , str); 		 
        ret_send = mpcc_uti_send_data(sd,message);
        if(FAILURE == ret_send)
        {
                printf("\nError in sending message\n");
                exit(EXIT_FAILURE);
        }
        
	free(message);
        return SUCCESS;

}
/*********************************************************************************************************************************************
 *
 *	Function Name	: mpcc_cli_send_registration_details_to_server
 *
 *	Description	: It will send clients registration details to server after encrypting
 *
 *	Return		: SUCCESS / FAILURE
 *
 ********************************************************************************************************************************************/

int mpcc_cli_send_registration_details_to_server(IN int sd,
                                         IN char *user_id,
                                          IN char *user_name,
                                           IN char *dob,
                                            IN char *pwd)
{
        int     ret = 0;

	printf("\n user_id = %s, len = %d", user_id, (int) strlen(user_id) );

        /* encrypt user id */
        strcpy(user_id, mpcc_uti_encrypt_data( user_id ));
	if(NULL == user_id)
	{
		printf("\nError while decrypting user_id\n");
		return FAILURE;
	}
	printf("\n E_user_id = %s", user_id);

        /* send user id to server */
        ret = mpcc_uti_send_data(sd, user_id);
        if(FAILURE == ret)
        {
                printf("error in sending user_id\n");
                exit(EXIT_FAILURE);
        }


	printf("\n user_name = %s, len = %d", user_name, (int) strlen(user_name) );

        /*encrypt user name */
        strcpy(user_name,mpcc_uti_encrypt_data( user_name ));
	if(NULL == user_name)
	{
		printf("\nError while decrypting user_name\n");
		return FAILURE;
	}

        /*send user name to server */
        ret = mpcc_uti_send_data(sd, user_name);
        if(FAILURE == ret)
        {
                printf("error in sending user name\n");
                exit(EXIT_FAILURE);
        }


        /*encrypt date of birth */
	printf("\n dob = %s, len = %d", dob, (int) strlen(dob) );
        strcpy(dob,mpcc_uti_encrypt_data( dob ));
	if(NULL == dob)
	{
		printf("\nError while decrypting dob\n");
		return FAILURE;
	}

        /*send date of birth to server */
        ret = mpcc_uti_send_data(sd, dob);
        if(FAILURE == ret)
        {
                printf("error in sending date of birth\n");
                exit(EXIT_FAILURE);
	}


	printf("\n pwd = %s, len = %d", pwd, (int) strlen(pwd) );

        /*encrypt password */
        strcpy(pwd, mpcc_uti_encrypt_data( pwd ));
	if(NULL == pwd)
	{
		printf("\nError while decrypting pwd\n");
		return FAILURE;
	}
	printf("\n E_pwd = %s", pwd);

        /*send password to server */
        ret = mpcc_uti_send_data(sd, pwd);
        if(FAILURE == ret)
        {
                printf("error in sending password\n");
                exit(EXIT_FAILURE);
	}
	
        return SUCCESS;
}


/*********************************************************************************************************************************************
 *
 *	Function Name	: mpcc_cli_chatting
 *
 *	Description	: Chatting function at client side to communicate between multiple clients. 
 *
 *	Return		: SUCCESS / FAILURE
 *
 ********************************************************************************************************************************************/

int mpcc_cli_chatting(int index, int sockfd)
{
        char *send_buf = NULL;
        char *recv_buf = NULL;
        int ret_send = 0 , ret_recv = 0 ;


	send_buf = (char*)malloc(sizeof(char) * MAX);
	if(NULL == send_buf)
	{
		perror("\nMemory allocation error for send_buf:");
		exit(EXIT_FAILURE);
	}
	recv_buf = (char*)malloc(sizeof(char) * MAX);
	if(NULL == recv_buf)
	{
		perror("\nMemory allocation error for recv_buf:");
		exit(EXIT_FAILURE);
	}


        if(index == 0)
        {
		memset(send_buf , '\0', MAX);
                fgets(send_buf, MAX, stdin);
                if (strcmp(send_buf , "exit\n") == 0)
                {
                	 
			free(recv_buf);
			free(send_buf);
		       	return EXIT;
                }
                else
		{
                        ret_send = mpcc_uti_send_data(sockfd, send_buf);
		}
        }
        else
        {
		memset(recv_buf , '\0', MAX);
                ret_recv = mpcc_uti_recv_data(sockfd, recv_buf);
                printf("%s\n" , recv_buf);
                fflush(stdout);
        }

        /*free the strings */
	free(send_buf);
	free(recv_buf);
        return SUCCESS;
}
