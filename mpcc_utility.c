/*********************************************************************************************************************************************
 *
 * 	FILE NAME	: utility.c
 *
 * 	DESCRIPTION	: This file contains all common functions that are  used by both server and client .
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

#include <mpcc_header.h>

/*********************************************************************************************************************************************
 *  
 *                 FUNCTION NAME	: mpcc_uti_send_data
 *                 DESCRIPTION		: send data to socket
 *                 RETURNS		: SUCCESS OR FAILURE
 *      
 *********************************************************************************************************************************************/

 
int mpcc_uti_send_data(int sd, char *buf)
{
        int len =0,					/*Initialize len ,len2, bytes_send,total_bytes,ret*/
            len2 =0,
            bytes_send =0,
            total_bytes =0,
            ret =0;

        len = strlen(buf);				/*To count length of string */

        len2 = htons(len);			

        ret = send(sd, &len2, sizeof(int), 0);  	/*send the length of string*/
        if(ret == -1)					/*Error handler to cheak length is sent or not*/
        {
                perror("\n Error in sending data");	
                exit(EXIT_FAILURE);
        }

        total_bytes = len ;								
        while(bytes_send < total_bytes)			
        {
                ret = send(sd, (buf+bytes_send), (total_bytes-bytes_send), 0);		/*send the value of string*/
                if(ret == -1)								/*Error handler to check string is sent or not*/
                {
                        perror("\n Error in sending data");				
                        exit(EXIT_FAILURE);
                }
                bytes_send+=ret;
        }
        return SUCCESS;									/*Return of mpcc_uti_send_data fuction*/
}

/*********************************************************************************************************************************************
 *  
 *                 FUNCTION NAME	: mpcc_uti_recv_data
 *                 DESCRIPTION		: receive data from socket
 *                 RETURNS 		: SUCCESS OR FAILURE
 *      
 *********************************************************************************************************************************************/


int mpcc_uti_recv_data(int sd, char *buf)
{
        int len =0,						/*Initialize len ,len2, bytes_send,total_bytes,ret*/
            len2 =0,
            bytes_recv =0,
            total_bytes =0,
            ret =0;

        ret = recv(sd, &len2, sizeof(int), 0);			/*To recv the length of string*/
        if(ret == -1)						 /*Error handler to cheak length is Received or not*/
        {
                perror("\n Error in sending data:");
                return FAILURE;
        }
        len = ntohs(len2);

        total_bytes = len ;
        while(bytes_recv < total_bytes)
        {
                ret = recv(sd, (buf+bytes_recv), (total_bytes-bytes_recv), 0);    /*Receive the value of string*/
                if(ret == -1)							  /*Error handler to check string is received or not*/
                {
                        perror("\n Error in sending data:");
                        return FAILURE;
                }
                bytes_recv+=ret;
        }
        return SUCCESS;								/*Return of mpcc_uti_recv_data fuction*/
}

/*********************************************************************************************************************************************
 *
 *                 FUNCTION NAME	: mpcc_uti_encrypt_data
 *                 DESCRIPTION		: Receive plain data and encrypt it.
 *                 RETURNS		: Encrypted text
 *
 *********************************************************************************************************************************************/

char* mpcc_uti_encrypt_data(char* str)
{
    	int index  = 0,
		length = 0;

	length = strlen(str);				/*Count The length of string*/
	for(index = 0; index < length ; index++ )
    	{
         	str[index] = str[index] - 15;		/*Encrypt The whole string according to ascii value*/
    	}

    	return str;					/*returns encrypted text*/
}

/*********************************************************************************************************************************************
 *
 *                 FUNCTION NAME	: mpcc_uti_decrypt_data
 *                 DESCRIPTION		: It receives encrypted data and decrypts it.
 *                 RETURNS 		: Plain Text
 *
 *********************************************************************************************************************************************/

char*  mpcc_uti_decrypt_data(char* str)
{
    	int length = 0,
    	 	index = 0;
   	char 	*d_str = NULL;
	d_str = (char*)malloc(sizeof(char) * MAX);
	if(NULL == d_str)
	{
		perror("Memory Allocation error to d_str:");
		return FAILURE;
	} 
	length = strlen(str);					/*calculate length of string*/
	memset(d_str, '\0',sizeof(d_str));
	strcpy(d_str, str);					/*copy the string into another string*/
    	for(index = 0; index < length; index++ )
    	{
            	d_str[index] = d_str[index] + 15;		/*Decrypt the whole string according to ascii value*/
    	}
	strcpy(str, d_str);					/*Free memory*/
	free(d_str);	
    	return str;					 	/*returns plain text*/
}

/*********************************************************************************************************************************************
 *  
 *                 FUNCTION NAME	: mpcc_uti_display_msg
 *                 DESCRIPTION		: receive srting from socket
 *                 RETURNS		: SUCCESS OR FAILURE
 *
 *********************************************************************************************************************************************/

int mpcc_uti_display_msg(char* str)
{
	 if(str)
	 {
		printf("\n\nReceived Message:%s\n",str);
		return SUCCESS;
	 }
	 else
		return FAILURE;			
}

/*********************************************************************************************************************************************
 *  
 *                 FUNCTION NAME	: mpcc_ser_send_data_server
 *                 DESCRIPTION		: send data to socket
 *                 RETURNS		: total_bytes.
 *      
 *********************************************************************************************************************************************/

int mpcc_ser_send_data_server(int sd, char *buf)
{
        int len =0,					/*Initialize len,len2,bytes_send,total_bytes,ret*/
            len2 =0,
            bytes_send =0,
            total_bytes =0,
            ret =0;

        len = strlen(buf);				/*Calculete the length of string*/

        len2 = htons(len);

        ret = send(sd, &len2, sizeof(int), 0);		/*send the length of string*/
        if(ret == -1)					/*error handler to check length sent or not*/
        {
                perror("\n Error in sending data");
                exit(EXIT_FAILURE);
        }

        total_bytes = len ;
        while(bytes_send < total_bytes)
        {
                ret = send(sd, (buf+bytes_send), (total_bytes-bytes_send), 0);	/*send the value of string*/
                if(ret == -1)							/*error handler to check string sent or not*/
                {
                        perror("\n Error in sending data");
                        exit(EXIT_FAILURE);
                }
                bytes_send+=ret;
        }
        return total_bytes;							/*return total no of bytes to send*/
}

/*********************************************************************************************************************************************
 *  
 *                 FUNCTION NAME	: mpcc_ser_recv_data_server
 *                 DESCRIPTION		: receive data from socket
 *                 RETURNS 		: total_bytes.
 *      
 *********************************************************************************************************************************************/


int mpcc_ser_recv_data_server(int sd, char *buf)
{
        int len =0,
            len2 =0,
            bytes_recv =0,
            total_bytes =0,
            ret =0;

        ret = recv(sd, &len2, sizeof(int), 0);			/*receive length of string*/
        if(ret == -1)						/*error handler to check length is received or not*/
        {
                perror("\n Error in sending data:");
                return FAILURE;
        }
        len = ntohs(len2);

        total_bytes = len ;
        while(bytes_recv < total_bytes)
        {
                ret = recv(sd, (buf+bytes_recv), (total_bytes-bytes_recv), 0);	/*receive total string*/
                if(ret == -1)							/*error handler to check length is sent or not*/
                {
                        perror("\n Error in sending data:");
                        return FAILURE;
                }
                bytes_recv+=ret;
        }
	buf[total_bytes] = '\0';						
        return total_bytes;							/*return total no of bytes to receive*/
}
