/**********************************************************************
*
* FILE NAME     : sdp_client_main.c
*
* DESCRIPTION   : Main program of client side  
*
* Date          Name        Reason              Reference                
*
* 13-Jan-10     Navjodh     Initial Creation    LLD
* 19-Jan-10     Neeraj      Error Handling      LLD
*
* Copyright (C) 2010 Aricent Inc. All Rights Reserved.
*
**********************************************************************/

/*********************HEADER FILE DECLARATIONS***********************/

#include "./../HEADER/sdp.h"

/**********************************************************************
*
* FUNCTION NAME : main
*
* DESCRIPTION   : It calls functions to recieve SDP offer and  send
*                 SDP answer
*                 
*
* RETURN TYPE   : return_type 
*
* PARAMETERS    : int argc, char *argv[]
*
**********************************************************************/


return_type main(
                    int argc,     /* Number of CLI arguments entered */
                    char* argv[]  /* Stores pointers to command line arguments*/
                )
{
	int err_no = ZERO;    /* Variable to save error number */
	sdp_session_t *my_session = NULL; /* pointer to sdp_session
                                            structure */
        char *sdp_msg_string = NULL;      /* pointer to SDP message
                                            string */
        char *sdp_msg_str = NULL;         /* pointer to SDP message
                                           string*/
        int sess_sockdes = 0; /*required for using TCP connection*/

        char addr_buff[FIELD_SIZE]; /*Required for IP address check*/

        char usr_input = INIT;  /* To store user input */

	/* Check if incorrect number of CLI arguments are passed*/
        if(argc  !=  ARGC)
	{       
            error_handling(ERR_MAJOR, SDP_ARGC_ERROR,
                               "[USAGE]: <Executable> <IP Address> <PortNumber> <Output File Name>");

	    exit(EXIT_FAILURE);
	}

        /*Initialize buffer*/
        memset(addr_buff, 0, FIELD_SIZE);
        
        /*Check if entered port number is valid*/
        if(ONE > atoi(argv[2]) || (65535 < (unsigned int)atoi(argv[2])))
        {
            error_handling(ERR_MAJOR, SDP_PORT_ERROR, 
                            "sdp_client_main: Port Number Out of Range");
            
            exit(EXIT_FAILURE);
        }
        else if(MYPORT != atoi(argv[2]))
        {
            error_handling(ERR_MAJOR, SDP_PORT_ERROR,
                             "sdp_client_main : Invalid Port Number");
            
            exit(EXIT_FAILURE);
        }

        /*Check IP address*/
        if(NULL == strncpy(addr_buff, argv[1], 1 + strlen(argv[1])))
	{
                error_handling(ERR_MAJOR, SDP_STRNCPY_ERROR,
					"sdp_client_main : strncpy error");
                exit(EXIT_FAILURE);
        }

        sdp_msg_str = strtok(addr_buff, ".");
        
        if(MARGIN != atoi(sdp_msg_str))
        {
                error_handling(ERR_MAJOR, SDP_IP_ERROR,
					"sdp_client_main : IP Address Out of Range");
                exit(EXIT_FAILURE);
        }
        
        if(SUCCESS != strncmp("10.203.161", argv[1], 
            strlen("10.203.161")))
        {
                error_handling(ERR_MAJOR, SDP_IP_ERROR,
			    "sdp_client_main : Invalid IP Address");

                exit(EXIT_FAILURE);
        }

	log_level = LOG_CRITICAL;  /* set log level */
 	log_max = 10000L;          /* set maximum log entries*/

        /* program and logfile name required for log entries */
 	if(NULL == strncpy(log_file, "sdp_client.log", 1 + strlen("sdp_client.log")))
	{
                error_handling(ERR_MAJOR, SDP_STRNCPY_ERROR,
					"sdp_client_main : strncpy error");
                exit(EXIT_FAILURE);
        }
                
        
 	if(NULL == strncpy(program_name, "sdp_client_main.c", 1 + strlen("sdp_client_main.c")))
	{
                error_handling(ERR_MAJOR, SDP_STRNCPY_ERROR,
					"sdp_client_main : strncpy error");
                exit(EXIT_FAILURE);
        }

	LOGGER(LOG_CRITICAL, "Start of sdp_receiver");
        /*Recieve SDP message into a string*/
	sdp_msg_str = sdp_client_recvr(argv, &sess_sockdes);
	if(sdp_msg_str == NULL)
	{
	    error_handling(ERR_MAJOR, SDP_RECEIVER_ERROR,
                                "sdp_client_main: sdp_recvr error");
	    LOGGER(LOG_CRITICAL, "End of sdp_receiver");
	    exit(EXIT_FAILURE);
	}
	LOGGER(LOG_CRITICAL, "End of sdp_receiver");
	
  	if(strncpy(inputfile, argv[3], strlen(argv[3])+1) == NULL)	
	{
	    error_handling(ERR_MAJOR, SDP_STRNCPY_ERROR,
                            "sdp_client_main: strncpy error");

	    exit(EXIT_FAILURE);
	}
	
	LOGGER(LOG_CRITICAL, "Start of sdp_write_string_to_file");
	if(sdp_write_string_to_file(sdp_msg_str, inputfile) != SUCCESS) 
	{
	    error_handling(ERR_MAJOR, SDP_FILE_WRITE_ERROR,
		            "sdp_client_main: sdp_write_string_to_file error");
	    LOGGER(LOG_CRITICAL, "End of sdp_write_string_to_file");
	    exit(EXIT_FAILURE);
	}
	LOGGER(LOG_CRITICAL, "End of sdp_write_string_to_file");

        /*check for media match. call sdp_add_compare_function */
        if(SUCCESS == sdp_media_comp(inputfile, CLIENT_CONFIG_FILE))
        {
	    printf("\nIncoming SDP message written to file: %s\n\n",argv[3]);
        
            printf("\nSend an Answer to the Server?(Y/N): ");
            scanf(" %c", &usr_input);
            
            if('Y' != toupper(usr_input))
            {
                printf("\nTerminating...\n");
                free(sdp_msg_str); 
                return SUCCESS;
            }
            
            
            
            /*Fill up a structure, Encode the into a string and send as answer*/
	    my_session = (sdp_session_t *)calloc(1, sizeof(sdp_session_t));
	
	    if(NULL == my_session)
	    {
	        error_handling(ERR_MAJOR, SDP_STRING_MALLOC_ERROR,
                              "sdp_client_main: memory allocation error");

	        exit(EXIT_FAILURE);
	    }

        	
	    LOGGER(LOG_CRITICAL, "Start of sdp_populate_message"); 	
	    if(sdp_populate_message(my_session, CLIENT_CONFIG_FILE) != SUCCESS)
	    {
	        error_handling(ERR_MAJOR, SDP_POPULATE_ERROR,
	       "sdp_client_main: sdp_populate_message error");
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
		                "sdp_client_main: sdp_session_to_string error");
		LOGGER(LOG_CRITICAL, "End of sdp_session_to_string"); 	
		
	        myfree(my_session);
                exit(EXIT_FAILURE);
	    }
	    LOGGER(LOG_CRITICAL, "End of sdp_session_to_string"); 	
	
	    printf("%s\n",sdp_msg_string);
        
	    LOGGER(LOG_CRITICAL, "Start of sdp_sender"); 	
	    if(sdp_client_sender(sdp_msg_string, &sess_sockdes) != 0)
	    {
	        error_handling(ERR_MAJOR, SDP_SENDER_ERROR,
		"sdp_client_main: sdp_client_sender error");
                
		LOGGER(LOG_CRITICAL, "End of sdp_sender"); 	
		
	        myfree(my_session);
                exit(EXIT_FAILURE);
	    }
	    LOGGER(LOG_CRITICAL, "End of sdp_sender");

            printf("Answer Sent Successfully!\n");
            
            /*Free memory allocated to SDP structure*/
	    myfree(my_session);
        }
	/*In case no compatible media is found*/
        else
        {
            printf("Error: Compatible Media not found, Aborting\n");
        }
        
        /* free string variables used for storing SDP message */
        free(sdp_msg_string);
        free(sdp_msg_str);
	return SUCCESS;
}	
	
