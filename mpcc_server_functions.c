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

/************************************************************************************************************************************************
 *
 *                    Function name : mpcc_ser_server_fun
 *
 *                    Description   : It will check if client wants to login or register and then will do login or registration respectively
 * 
 *                    Return        : SUCCESS / FAILURE
 * 
 * 
 *************************************************************************************************************************************************/

int mpcc_ser_server_fun(int sd)
{
    int		ret_rcv = 0;
    int		ret_if_login = 0;
    int		ret_login = 0;
    int		ret_reg = 0;
    char 	*message = NULL;
    message = (char*)malloc(sizeof(char) * ACK_SIZE);
    if(NULL == message)
    {
	perror("\nMemory Allocation Error of message:");
	return FAILURE;
    } 
    memset(message,'\0',sizeof(message));
    
    ret_rcv = mpcc_uti_recv_data(sd,message);
    
    if(SUCCESS == ret_rcv)
    {
	if(strcasecmp(message,"login") == 0)
	{
    		ret_login = mpcc_ser_login_to_mpcc(sd);
    		if( 0 > ret_login)
    		{
			printf("\n Error in mpcc_ser_login_to_mpcc() function\n");
			return FAILURE;
  		}
	}
	else if(strcasecmp(message,"registr") == 0)
	{
		ret_reg = mpcc_ser_register_to_mpcc(sd);
    		if( 0 > ret_reg )
    		{
			printf("\n Error in login to MPCC\n");
			return FAILURE;
  		}
		else
		{
		
			ret_if_login = mpcc_ser_if_login(sd);
			if(SUCCESS == ret_if_login)
			{
				ret_login = mpcc_ser_login_to_mpcc(sd);
                        	if(FAILURE == ret_login)
       	 	        	{
                       	        	printf("\n Error in logging to MPCC \n");
                               		return FAILURE;
                       		}
			}
		}
		
	}
    }
    else
    {
	printf("\nError In receiving Data\n");
	return FAILURE;
    }

    free(message);
    return SUCCESS;
}


/************************************************************************************************************************************************
 *
 *                      Function name : mpcc_ser_login_to_mpcc
 *
 *                      Description   : It will receive login details from client, will validate from registered
 *                                      users table and then will update in active clients table,if registered
 *
 *                      Return        : SUCCESS / FAILURE
 ************************************************************************************************************************************************/

int mpcc_ser_login_to_mpcc(int sd)
{
        int     ret_login_details = 0;
        int     ret_val = 0;
        int     ret_send_msg_to_client =0;
        char    *user_id = NULL;
        char    *pwd = NULL;

	user_id = (char*)malloc(sizeof(char) * USER_ID_SIZE);
	if(NULL == user_id)
	{
		perror("\nMemory Allocation Error of user_id:");
		return FAILURE;
	} 
	pwd = (char*)malloc(sizeof(char) * PWD_SIZE);
	if(NULL == pwd)
	{
		perror("\nMemory Allocation Error of pwd:");
		return FAILURE;
	} 


        ret_login_details = mpcc_ser_recv_login_details_from_client(sd, user_id, pwd);
        if( FAILURE == ret_login_details )
        {
                printf("\n Error in receiving login details from client\n");
                return FAILURE;
        }

        ret_val = mpcc_ser_validate_from_register_clients_table(user_id, pwd); 

	if( SUCCESS == ret_val )
        {
       		ret_send_msg_to_client = mpcc_ser_send_message_to_client(sd, ret_val);
		if(FAILURE == ret_send_msg_to_client)
		{
			printf("\nError while Sending message to client\n");
			return FAILURE;
		}

        }
	else if( (NOT_REGISTERED == ret_val) OR ( WRONG_PASSWORD == ret_val) )
	{	
       		ret_send_msg_to_client = mpcc_ser_send_message_to_client(sd, ret_val);
                if(FAILURE == ret_send_msg_to_client)
                {
                	printf("\nError in sending message to client \n");
                        return FAILURE;
                }
	}

        else
        {
                printf("\n Error in validating from registered clients table\n");
                return FAILURE;
        }
        free(pwd);
  	free(user_id);
        return SUCCESS;
}

/************************************************************************************************************************************************
 *
 *                      Function name : mpcc_ser_recv_login_details_from_client
 *
 *                      Description   : It will receive login details: user id, password from client
 *
 *                      Return        : SUCCESS / FAILURE
 *
 *
************************************************************************************************************************************************/

int mpcc_ser_recv_login_details_from_client(IN int sd, OUT char* user_id, OUT char* pwd)
{
        int     ret_rcv = 0;

        memset(user_id, '\0', USER_ID_SIZE );
        ret_rcv = mpcc_uti_recv_data(sd, user_id);
        if(FAILURE == ret_rcv)
        {
                printf("\n Error in receiving user id\n");
                exit(EXIT_FAILURE);
        }

        strcpy(user_id,mpcc_uti_decrypt_data(user_id));
	if(NULL == user_id)
	{
		printf("\n User id Is not successfully decrypted\n");
		return FAILURE;
	}
	printf("\n user_id = %s", user_id);

        memset(pwd, '\0', PWD_SIZE );
        ret_rcv = mpcc_uti_recv_data(sd,pwd);
        if(FAILURE == ret_rcv)
        {
                printf("\n Error in receiving user password\n");
                exit(EXIT_FAILURE);
        }

        strcpy(pwd,mpcc_uti_decrypt_data(pwd));
	if(NULL == pwd)
	{
		printf("\n password Is not successfully decrypted\n");
		return FAILURE;
	}

	printf("\n pwd = %s", pwd);
        return SUCCESS;
}

/************************************************************************************************************************************************
 *
 *                      Function name : mpcc_ser_recv_reg_details_from_client
 *
 *                      Description   : It will receive registration details: user id, name, dob and pwd from client
 *
 *                      Return        : SUCCESS / FAILURE
 *
************************************************************************************************************************************************/

int mpcc_ser_recv_reg_details_from_client(IN int sd,OUT char *user_id, OUT char *user_name, OUT char *dob, OUT char *pwd)
{
        int ret_user_id = 0;
        int ret_user_name = 0;
        int ret_pwd =0;
        int ret_dob = 0;

        memset(user_id, '\0', USER_ID_SIZE );
        ret_user_id = mpcc_uti_recv_data(sd, user_id);
        if(FAILURE ==  ret_user_id)
        {
                printf("\nError in receiving user id\n");
                exit(EXIT_FAILURE);
        }

        strcpy(user_id, mpcc_uti_decrypt_data(user_id));
        if(NULL == user_id)
	{
                printf("\nError in decrypt user ID\n");
                return FAILURE;
        }
	printf("\n\n user_id = %s, len = %d", user_id,(int)strlen(user_id) );

        memset(user_name, '\0', USER_NAME_SIZE );
        ret_user_name = mpcc_uti_recv_data(sd, user_name);
        if(FAILURE == ret_user_name)
        {
                printf("\nError in receiving user name\n");
                return FAILURE;
        }

        strcpy(user_name,mpcc_uti_decrypt_data(user_name));
        if(NULL == user_name)
        {
                printf("\nError in decrypt user Name\n");
                return FAILURE;
        }
        
	printf("\n user_name = %s, len = %d", user_name, (int)strlen(user_name) );

        memset(dob, '\0', DOB_SIZE );
	ret_dob = mpcc_uti_recv_data(sd, dob);
        if(FAILURE == ret_dob)
        {
                printf("\nError in receiving user date of birth\n");
                return FAILURE;
        }

        strcpy(dob,mpcc_uti_decrypt_data(dob));
        if(NULL == dob)
        {
                printf("\nError in decrypt user d.o.b\n");
                return FAILURE;
        }
	printf("\n dob = %s, len = %d", dob, (int)strlen(dob) );

        memset(pwd, '\0', PWD_SIZE );
        ret_pwd = mpcc_uti_recv_data(sd, pwd);
        if(FAILURE ==  ret_pwd)
        {
                printf("\nError in receiving user password\n");
                exit(EXIT_FAILURE);
        }

        strcpy(pwd,mpcc_uti_decrypt_data(pwd));
        if(NULL == pwd)
        {
                printf("\nError in decrypt user password\n");
                return FAILURE;
        }
	
	printf("\n pwd = %s, len = %d", pwd, (int)strlen(pwd) );

        return SUCCESS;
}

/************************************************************************************************************************************************
 *
 *                      Function name : mpcc_ser_if_login
 *
 *                      Description   : It receives message from client if user wants to login after registration or not
 *
 *                      Return        : SUCCESS / FAILURE
 *
 *
************************************************************************************************************************************************/

int mpcc_ser_if_login(int sd)
{

        int     ret_rcv=0;
        char    *buff = NULL;
	
	buff = (char*)malloc(sizeof(char) *ACK_SIZE);
	if(NULL == buff)
	{
		perror("\nMemory Allocation Error of buff:");
		return FAILURE;
	}
        memset(buff,'\0',sizeof(buff));
	ret_rcv = mpcc_uti_recv_data(sd,buff);
        if(FAILURE == ret_rcv)
        {
                printf("\nError occured in receiving client choice.. if he wants to login now or not \n");
		free(buff);
                return FAILURE;
        }
        if((strcasecmp(buff, "SUCCESS") == 0))
        {
		free(buff);
                return SUCCESS;
        }
	free(buff);
        return FAILURE;
}

/************************************************************************************************************************************************
 *
 *                      Function name : mpcc_ser_register_to_mpcc
 *
 *                      Description   : It will receive registration details from client, will update in registered users table
 *                                      and then will send message to client if user is successfully registered or not
 *
 *                      Return        : SUCCESS / FAILURE
 *
*************************************************************************************************************************************************/


int mpcc_ser_register_to_mpcc(int sd)
{
        int     ret_update = 0;
        int     ret_recv = 0;
        int     ret_send = 0;
        char    *user_id = NULL;
        char    *user_name = NULL;
        char    *pwd = NULL;
        char    *dob = NULL;
	user_id = (char*)malloc(sizeof(char) * USER_ID_SIZE);
	if(NULL == user_id)
	{
		perror("\nMemory Allocation Error of user_id:");
		return FAILURE;
	} 
	user_name = (char*)malloc(sizeof(char) * USER_NAME_SIZE);
	if(NULL == user_name)
	{
		perror("\nMemory Allocation Error of user_name:");
		free(user_id);
		return FAILURE;
	} 
	pwd = (char*)malloc(sizeof(char) * PWD_SIZE);
	if(NULL == pwd)
	{
		perror("\nMemory Allocation Error of pwd:");
		free(user_id);
		free(user_name);
		return FAILURE;
	} 
	dob = (char*)malloc(sizeof(char) * DOB_SIZE);
	if(NULL == dob)
	{
		perror("\nMemory Allocation Error of dob:");
		free(user_id);
		free(pwd);
		free(user_name);
		return FAILURE;
	} 
        ret_recv = mpcc_ser_recv_reg_details_from_client(sd, user_id, user_name, dob, pwd);
        if(SUCCESS == ret_recv)
        {
                ret_update = mpcc_ser_update_in_register_clients_table(user_id,user_name,dob,pwd);
                if(SUCCESS == ret_update)
                {
                        printf("\n Updated in register_clients table successfully \n");
                        ret_send = mpcc_ser_send_message_to_client(sd, ret_update);
                        if(FAILURE == ret_send)
                        {
                                printf("\nError in sending message to client\n");
				free(user_id);
				free(pwd);
				free(user_name);
				free(dob);
                                return FAILURE;
                        }
                }

                else
                {
                        printf("\nError in updating in register_clients table \n");
                        ret_send = mpcc_ser_send_message_to_client(sd, ret_update);
                        if(0 > ret_send)
                        {
                                printf("\nError in sending message to client\n");
			
				free(user_id);
				free(pwd);
				free(user_name);
				free(dob);
                                return FAILURE;
                        }
			
			free(user_id);
			free(pwd);
			free(user_name);
			free(dob);
                        return FAILURE;
                }
        }

        else
        {
                printf("\n Error occured in receiving registration details from client\n");	
		free(user_id);
		free(pwd);
		free(user_name);	
		free(dob);
                return FAILURE;
        }
	
		free(user_id);
		free(pwd);
		free(user_name);
		free(dob);	
	        return SUCCESS;
}

/************************************************************************************************************************************************
 *
 *                      Function name : mpcc_ser_send_message_to_client
 *
 *                      Description   : It will send message to client if user has entered right login details or not
 *
 *                      Return        : SUCCESS / FAILURE
 *
************************************************************************************************************************************************/

int mpcc_ser_send_message_to_client(int sd, int ret)
{
    	char 	*message = NULL;
   	message = (char*)malloc(sizeof(char) * ACK_SIZE);
	if(NULL == message)
	{
		perror("\nMemory Allocation Error of message:");
		return FAILURE;
	} 

        memset(message, '\0', sizeof(message));
        if( SUCCESS == ret)
        {
                strcpy( message , "SUCCESS");
                mpcc_uti_send_data(sd, message);
        }      
        else if( NOT_REGISTERED == ret)
        {
                strcpy( message , "NOT_REGISTERED");
                mpcc_uti_send_data(sd, message);
        }	
        else if( WRONG_PASSWORD == ret)
        {
                strcpy( message , "WRONG_PASSWORD");
                mpcc_uti_send_data(sd, message);
        }
        else
        {
                strcpy( message , "FAILURE");
                mpcc_uti_send_data(sd, message);
        }
 	free(message);
        return SUCCESS;
}


/************************************************************************************************************************************************
 *
 *                      Function name : mpcc_ser_broadcast_msg
 *
 *                      Description   : It will broadcast message from one client to other clients
 *
 *                      Return        : Nothing
 *
************************************************************************************************************************************************/

void mpcc_ser_broadcast_msg(int j, int i, int sockfd, int nbytes_recvd, char *recv_buf, fd_set *master)
{
        if (FD_ISSET(j, master))
        {
                if (j != sockfd && j != i)
                {
                        if (mpcc_ser_send_data_server(j, recv_buf) == -1)
                        {
                                perror("send");
                        }
                }
        }
}

/************************************************************************************************************************************************
 *
 *                      Function name : mpcc_ser_msg_processing
 *
 *                      Description   : It will broadcast message from one client to other clients
 *
 *                      Return        : SUCCESS/FAILURE
 *
************************************************************************************************************************************************/

int mpcc_ser_msg_processing(int i, fd_set *master, int sockfd, int fdmax)
{
        int nbytes_recvd, j;
        char *recv_buf = NULL;
	recv_buf = (char*)malloc(sizeof(char) * MAX);
	if(NULL == recv_buf)
	{
		perror("\nMemory Allocation Error of recv_buf:");
		return FAILURE;
	}

        memset(recv_buf , '\0', sizeof(recv_buf));
        if ((nbytes_recvd = mpcc_ser_recv_data_server(i, recv_buf)) <= 0)
        {

                if (nbytes_recvd == 0)
                {
                        printf("socket %d hung up\n", i);
                }
                else
                {
                        perror("recv");
                }
                close(i);
                FD_CLR(i, master);
        }
        else
        {       printf("%s\n", recv_buf);
		if(FAILURE == mpcc_ser_write_to_file(recv_buf))
		{
			printf("\nError while writing message to file ..!!\n");	
		}
                for(j = 0; j <= fdmax; j++)
                {
                mpcc_ser_broadcast_msg(j, i, sockfd, nbytes_recvd, recv_buf, master );

                }
        }
	free(recv_buf);
	return SUCCESS;
}

/************************************************************************************************************************************************
 *
 *                      Function name : mpcc_ser_write_to_file
 *
 *                      Description   : The function recieves a chat message which gets appended in a file. When the file
 *                                      exceeds 20kb in size, it gets truncated to a size of 10 KB
 *
 *                      Return        : SUCCESS / FAILURE
 *
************************************************************************************************************************************************/


int mpcc_ser_write_to_file(char *message)
{
	struct stat filestats;
	int file_descriptor;
	int message_len = 0;
	int size = 0;
	int ret_write = 0;
	int ret_read = 0;
	char *truncated_message = NULL ;


	
	file_descriptor = open("chat_messages_file.txt", O_RDWR |O_APPEND);
	if(0 > file_descriptor)
	{
		perror("\n\n Error opening chat_message file \n\n");
		free(truncated_message);
		return FAILURE;
	}
	message_len = strlen(message);
	ret_write = write(file_descriptor,message,message_len);
	if(0 > ret_write)
	{
		perror("\n\nError in writing \n\n");
		free(truncated_message);
		return FAILURE;
	}
	fstat(file_descriptor, &filestats);
	if(FILE_SIZE < filestats.st_size)
	{
		truncated_message = (char *)malloc( sizeof(char) * TRUNCATED_FILE_SIZE );
		if ( NULL == truncated_message )
		{
			printf("\n ERROR occured in malloc() \n");
			return FAILURE;
		}
		lseek(file_descriptor, (off_t)TRUNCATED_FILE_SIZE, SEEK_SET); 
	        fstat(file_descriptor, &filestats);
		size = filestats.st_size;
		ret_read = read(file_descriptor, truncated_message, size);
		if(0 > ret_read)
		{
			perror("\n\nError in reading file\n\n");
			free(truncated_message);
			return FAILURE;
		
		}
		ftruncate(file_descriptor,(off_t)0);
 		ret_write = write(file_descriptor, truncated_message ,strlen(truncated_message));	
		if(0 > ret_write)
		{
			perror("\n\nError in writing string in file after truncate \n\n");
			free(truncated_message);
			return FAILURE;
		}
	}
	
	fstat(file_descriptor, &filestats);
	free(truncated_message);
	return SUCCESS;
}

