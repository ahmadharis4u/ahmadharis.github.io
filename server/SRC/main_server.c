/**********************GLOBAL VARIABLES************************************
* FILE NAME	: main_server.c
*
* DESCRIPTION   : Main of server side
*
*
*	DATE			NAME			REASON			REFERENCE
*
*	14-JAN-10	Ahmad Haris		Initial Creation	LLD
*	19-JAN-10	Ahmad Haris		Error Handling		LLD
*
* Copyright @ 2010 Aricent Inc. All Rights Reserved.
**********************************************************************/

/**************HEADER FILE DECALARATIONS************/

#include"./../HEADER/sdp.h"


/**********************************************************************
*
* FUNCTION NAME : main
*
* DESCRIPTION   : It calls the function to encode sdp offer and multicasts.
*
*
* RETURN TYPE   : return_type
*
* PARAMETERS    : int argc, char *argv[]
*
**********************************************************************/



return_type main(int argc, 		/*argument count*/
				 char* argv[]	/*argument array*/
				)
{
	int err_no = ZERO;
	char *sdp_msg_string = NULL; 
	sdp_session_t *my_session = NULL;
	char buff[FILE_SIZE];

	if(argc  !=  ARGC)
	{
        error_handling(ERR_MAJOR, SDP_ARGC_ERROR,
						"Usage : <executable> <file name>");
		exit(EXIT_FAILURE);
	}

	memset(buff, 0, FILE_SIZE);
	if(gethostname(buff, FILE_SIZE) != SUCCESS)
	{
        error_handling(ERR_MAJOR, SDP_GETHOSTNAME_ERROR,
						"sdp_server_main : Gethostname error");
		exit(EXIT_FAILURE);
	}
	printf("Hostname is : %s\n ", buff);
	LOGGER(LOG_CRITICAL, "Start of sdp_hash_append"); 	
	if(sdp_hash_append(argv[1]) != SUCCESS)
	{
        error_handling(ERR_MAJOR, SDP_FILE_HASH_ERROR,
						"sdp_server_main : sdp_hash_append error.");
		LOGGER(LOG_CRITICAL, "End of sdp_hash_append");
		exit(EXIT_FAILURE);
	}
	LOGGER(LOG_CRITICAL, "End of sdp_hash_append");
 	
	log_level = LOG_CRITICAL;
	log_max = LOG_MAX;
	if(NULL == strncpy(log_file,"sdp_server.log", strlen("sdp_server.log") + 1))
	{
		error_handling(ERR_MAJOR, SDP_STRNCPY_ERROR,
						"sdp_server_main : strncpy error.");
		exit(EXIT_FAILURE);
	}
	if(NULL == strncpy(program_name,"main_server.c", strlen("main_server.c") + 1))
	{
		error_handling(ERR_MAJOR, SDP_STRNCPY_ERROR,
						"sdp_server_main : strncpy error.");
		exit(EXIT_FAILURE);
	}
       
        LOGGER(LOG_CRITICAL, "Start of sdp_checker"); 	
    err_no=sdp_checker(inputfile);
	if(err_no  != SUCCESS)
	{
		error_handling(ERR_MAJOR, SDP_CHECKER_ERROR,
						"sdp_server_main : sdp_checker error.");
        	LOGGER(LOG_CRITICAL, "End of sdp_checker"); 	
	     exit(EXIT_FAILURE);
	}
        LOGGER(LOG_CRITICAL, "End of sdp_checker"); 	
	
	my_session = (sdp_session_t *)calloc(1, sizeof(sdp_session_t));
	
	if(NULL == my_session)
	{
		error_handling(ERR_MAJOR, SDP_STRING_MALLOC_ERROR,
						"sdp_server_main : malloc error.");
		exit(EXIT_FAILURE);
	}

        LOGGER(LOG_CRITICAL, "Start of sdp_populate_message"); 	
	if(sdp_populate_message(my_session, inputfile) != SUCCESS)
	{
		error_handling(ERR_MAJOR, SDP_POPULATE_ERROR,
						"sdp_server_main : sdp_populate_message error.");
        	LOGGER(LOG_CRITICAL, "End of sdp_populate_message");
		myfree(my_session);
		exit(EXIT_FAILURE);
	}
        LOGGER(LOG_CRITICAL, "End of sdp_populate_message"); 	
	
	LOGGER(LOG_CRITICAL, "Start of sdp_session_to_string"); 	
	sdp_msg_string = sdp_session_to_str(my_session, &err_no);
	if(sdp_msg_string == NULL)
	{
		error_handling(ERR_MAJOR, SDP_SESSION_TO_STR_ERROR,
						"sdp_server_main : sdp_session_to_str error.");
		LOGGER(LOG_CRITICAL, "End of sdp_session_to_string");
		myfree(my_session);
		exit(EXIT_FAILURE);
	}
	LOGGER(LOG_CRITICAL, "End of sdp_session_to_string"); 	
	myfree(my_session);
	
	printf("%s\n",sdp_msg_string);
        
	LOGGER(LOG_CRITICAL, "Start of sdp_sender"); 	
	if(sdp_server_sender(sdp_msg_string) != SUCCESS)
	{
		error_handling(ERR_MAJOR, SDP_SENDER_ERROR,
						"sdp_server_main : sdp_server_sender error.");
		LOGGER(LOG_CRITICAL, "End of sdp_server_sender");
		free(sdp_msg_string);
		exit(EXIT_FAILURE);
	}
	LOGGER(LOG_CRITICAL, "End of sdp_server_sender"); 	

    free(sdp_msg_string);
	return SUCCESS;
}	
