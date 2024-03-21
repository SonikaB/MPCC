/*********************************************************************************************************************************************
 *
 * 	FILE NAME	: db_function.c
 * 	
 * 	DESCRIPTION	: This file contains all the functions related to handleing data base.
 *
 * 	DATE		NAME		REFERENCE		REASON
 *   
 *      28/Sep/2016     MPCC-TEAM	Initial-Creation	Nalanda Trainee Project
 *
 *      Copyright @ Aricent Technologies & Holdings Limited 2016
 *
 *********************************************************************************************************************************************/

/*********************************************************************************************************************************************
 *
 *		 			HEADER FILES INCLUDED
 *
 ********************************************************************************************************************************************/

#include<mpcc_header.h>
#include<mpcc_db_header.h>

/*********************************************************************************************************************************************
 *
 *		 			GLOBAL VARIABLE INCLUDED
 *
 ********************************************************************************************************************************************/

MYSQL *db_connect;

/*********************************************************************************************************************************************
 *
 * 	Function Name	: mpcc_ser_establish_database_connectivity
 *
 * 	Description	: This function will establish connection between server
 * 	 	          and database.
 *
 * 	Return		: Return User name.
 *
 ********************************************************************************************************************************************/

int mpcc_ser_establish_database_connectivity()
{
    db_connect = mysql_init(NULL);				/* to initialize MYSQL variable with zero */
    if( NULL == db_connect )					 
    {
        printf("\n Db init error\n");				
	return FAILURE;						
    }

    db_connect = mysql_real_connect(db_connect,"tsus023","root","abc123","MPCC",3306,NULL,0); /* to establish connection we have to pass host name and other required feilds */
    if( NULL == db_connect )					
    {
        printf("\n DB connect error\n");			
        return FAILURE;						
    }
    return SUCCESS;						
}


/*********************************************************************************************************************************************
 *
 *	Function Name   : mpcc_ser_update_in_register_clients_table
 *
 *	Description     : It will accept user_id, port_Name, DOB and password and update all this by making a new entry in 
 *			  register_clients_table from data base.
 *
 *	Return          : SUCCESS OR FAILURE.
 *
 *********************************************************************************************************************************************/

int mpcc_ser_update_in_register_clients_table(char *user_id,char *user_Name, char *DOB, char *password)
{
    int ret;

    char *statement = NULL;

    statement = (char*)malloc(sizeof(char) * MAX_LINE);

    if( NULL == statement )
    {
        printf("\n\n\t\tInsufficient Memory\n\n\t\tUnable to allocate\n");
        return FAILURE;;
    }

    ret = mpcc_ser_establish_database_connectivity();
    if( FAILURE == ret )
    {
	printf("\n ERROR OCCURED IN mpcc_ser_establish_database_connectivity() function\n");     
	return FAILURE;
    }
    memset(statement,0,sizeof(statement));
    snprintf(statement,512,"insert into register_users values('%s','%s','%s','%s')",user_id,user_Name,DOB,password);

    ret =  mysql_query(db_connect,statement);
    if( 0 != ret )
    {
        printf("\nERROR OCCURED IN mysql_query() function\n");
        exit(0);
    }

    free(statement);
    mysql_close(db_connect); 
    return SUCCESS;
}

/*********************************************************************************************************************************************
 *
 *	Function Name   : validate_from_register_client_table
 *
 *	Description     : It will accept user_id and password and validate the user by comparing the retrived password from 
 *			  register_clients_table to the received password. 
 *			   
 *	Return          : SUCCESS OR FAILURE.
 *
 *********************************************************************************************************************************************/
int mpcc_ser_validate_from_register_clients_table(char *user_id, char *password)
{
    int		index = 0;
    int		num_fields = 0;
    int		ret = 0;
    char	*statement = NULL;
    char	*fetch_password = NULL;
    char	*fetch_user_id = NULL;
    MYSQL_RES 	*result;
    MYSQL_ROW 	row;

    fetch_password = (char*)malloc(sizeof(char) * SIZE);
    if( NULL == fetch_password)
    {
        perror("\nMemory allocation error of fetch_password:");
        return FAILURE;
    }
    
    fetch_user_id = (char*)malloc(sizeof(char) * SIZE);
    if( NULL == fetch_user_id )
    {
        perror("\nMemory allocation error of fetch_user_id:");
	free(fetch_password);
        return FAILURE;
    }

    statement = (char*)malloc(sizeof(char) * MAX_LINE);
    if( NULL == statement )
    {
        perror("\nMemory allocation error of statement:");
	free(fetch_password);
	free(fetch_user_id);
        return FAILURE;
    }
    
    ret = mpcc_ser_establish_database_connectivity();
    if( FAILURE == ret )
    {
	printf("\n Error in mpcc_ser_establish_database_connectivity() function\n");     
        free(fetch_password);
        free(fetch_user_id);
        free(statement);
	return FAILURE;
    }
    memset(statement,0,sizeof(statement));
    snprintf(statement,512,"select password from register_users where User_Id = '%s'",user_id);

    mysql_query(db_connect,statement);

    result = mysql_store_result(db_connect);

    if( NULL == result )
    {
        printf("\n Table is empty\n");
    }

    num_fields = mysql_num_fields(result);

    memset(fetch_password,0,sizeof(fetch_password));

    while ((row = mysql_fetch_row(result)))
    {
      	for(index= 0; index < num_fields; index++)
        {
            strcpy(fetch_password,row[index]);
        }
            printf("\n");
    }
    printf("\nThe password is %s\n",password);

    printf("\nThe retrived password  from db is %s\n",fetch_password);

    if(! strcmp(password,fetch_password) )
    {
        printf("\n\t\tAuthentication is Successful\n\n\t\tClient is already Registed in MPCC\n\n");
        
	free(fetch_password);
        free(fetch_user_id);
        free(statement);
        mysql_library_end();
        mysql_close(db_connect);
        mysql_free_result(result);
        return SUCCESS;
    }
    else
    {
       
	
        memset(statement,0,sizeof(statement));

        snprintf(statement,512,"select user_Id from register_users where User_Id = '%s'",user_id);

        mysql_query(db_connect,statement);

        result = mysql_store_result(db_connect);

        if( NULL == result )
        {
             printf("\n Table is empty\n");
        }

        num_fields = mysql_num_fields(result);
        memset(fetch_user_id,0,sizeof(fetch_user_id));
        while ((row = mysql_fetch_row(result)))
	{

              for(index= 0; index < num_fields; index++)
              {
                   strcpy(fetch_user_id,row[index]);
              }
              printf("\n");
        }
	printf("\n\t\tThe user_id is %s\n",user_id);
        printf("\n\t\tThe retrived user_id  from db is %s\n",fetch_user_id);
        if( strcmp(user_id,fetch_user_id))
        {
             printf("\n User is not Pre-registered in MPCC\n\n\t\t----NEW REGISTRATION FORM-----\n");
             free(fetch_password);
             free(fetch_user_id);
             free(statement);
             mysql_library_end();
             mysql_close(db_connect);
             mysql_free_result(result);
	     return NOT_REGISTERED;

        }
        else
        {
	    printf("\n\t\t Client has entered wrong password\n\n\t\tLogin Again with Correct Password\n");
            free(fetch_password);
            free(fetch_user_id);
            free(statement);
            mysql_library_end();
            mysql_close(db_connect);
            mysql_free_result(result);
	    return WRONG_PASSWORD;
        }
    }
   /*Free memory*/
   free(fetch_password);
   free(fetch_user_id);
   free(statement);
   mysql_library_end();
   mysql_close(db_connect);
   mysql_free_result(result);
   return SUCCESS;
}               
