
/**********************************************************************
*
* FILE NAME     : sdp_recvr.c
*
* DESCRIPTION   : Receives the sdp message into a string
*
* Date          Name        Reason              Reference                
*
* 13-Jan-10     Navjodh     Initial Creation    LLD
* 19-Jan-10     Neeraj      Error Handling      LLD
*
* Copyright (C) 2010 Aricent Inc. All Rights Reserved.
**********************************************************************/


/*************HEADER FILES INCLUSION**************/
#include"./../HEADER/sdp.h"


/**********************************************************************
*
* FUNCTION NAME : sdp_client_recvr
*
* DESCRIPTION   : Receives the sdp message into a string
*
* RETURN TYPE   : char *
*
* PARAMETERS    : command line arguments (port no. and ip addr)
*
***********************************************************************/

char* sdp_client_recvr(
                        char* argv[], /* Commandline line arguments
                                       array */
                        int *p_sess_sockdes /* pointer to socket
                                               descriptor */
                      )
{
	int socket_desc = ZERO,     /* socket descriptor */
	    write_ret = ZERO,       /* return value of write */
	    read_size = ZERO,       /* return value of read */
	    pointer = ZERO,         /* stores the number of
                                       characters in each read*/
	    sdp_msg_size = ZERO,    /* stores the size of SDP
                                       message */
	    size = ZERO;            /* size of buffer*/

	char buffer[MAXBUFFER];     /* buffer to store send and
                                       recieve strings */
        
	char* sdp_msg = NULL;        /* pointer to sdp message */
	
	struct sockaddr_in serveraddress;  /* structure for storing 
                                              server info*/
	
        memset(buffer, 0, MAXBUFFER);

	TRACE("\nTRACE: Entering function sdp_receiver\n");
	/* create a tcp socket */
	socket_desc = socket(AF_INET, SOCK_STREAM, 0);

	if (socket_desc < ZERO)
	{
		error_handling(ERR_MAJOR,SDP_RECEIVER_SOCKET_ERROR,
			"sdp_receiver: socket creation failed");
	        TRACE("\nTRACE: Exiting function sdp_receiver with failure\n");
		return NULL;
	}
	

	/* initializing the server address structure */
	memset(&serveraddress, 0, sizeof(serveraddress));
	serveraddress.sin_family = AF_INET;
	serveraddress.sin_port = htons((uint16_t)atoi(argv[2]));
	serveraddress.sin_addr.s_addr = inet_addr(argv[1]);


	printf("\nConnecting to Server...");
	/* sending a connect request to server */
	if (connect(socket_desc, 
		  (struct sockaddr*)&serveraddress, 
		 (socklen_t) sizeof(serveraddress)) < ZERO)
	{
		error_handling(ERR_MAJOR,SDP_RECEIVER_CONNECT_ERROR,
			"sdp_receiver: connection failed");
	        TRACE("\nTRACE: Exiting function sdp_receiver with failure\n");
		return NULL;
	}

        memset(buffer, 0, MAXBUFFER);

	printf("Done\n");

	/* reading the size of sdp message to follow */
	read_size = (int)read(socket_desc, buffer,(size_t)MAXBUFFER);
        
        if(-1 == read_size)
        {
             error_handling(ERR_MAJOR, SDP_READ_ERROR,
                            "sdp_client_sender: Read Error");

             TRACE("\nTRACE: Exiting function sdp_client_recvr with failure\n");
            return NULL;
        }

        
	
	sdp_msg_size = atoi(buffer);
        if(ZERO == sdp_msg_size)
        {
             printf("\nClient Rejected by Server\n");

             TRACE("\nTRACE: Exiting function sdp_client_recvr with failure\n");
            return NULL;
        }
	
	printf("\nSDP msg size: %d\n", sdp_msg_size);
	
	printf("\nRequesting SDP Offer\n");

	/* send SDP request to sender */
	strcpy(buffer, "SDP REQUEST");
	size = (int)strlen(buffer) + 1;
	write_ret = (int)write(socket_desc, buffer,(size_t)size);
        if(-1 == write_ret)
        {
            error_handling(ERR_MAJOR, SDP_WRITE_ERROR,
                            "sdp_client_recvr: Write error");

                        TRACE("\nTRACE: Exiting function sdp_client_recvr with failure\n");
    
	return NULL;
        }

	
	sdp_msg = (char*) malloc((sdp_msg_size+1) * sizeof(char));
	
	if(NULL == sdp_msg)
	{
		error_handling(ERR_MAJOR,
				SDP_RECEIVER_MESSAGE_ERROR,
				"sdp_recvr: Maloc error");
	        TRACE("\nTRACE: Exiting function sdp_client_recvr with failure\n");
		return NULL;
	}
	sdp_msg[0] = '\0';
	
        printf("\nRecieving SDP Offer from server...");

	while(TRUE == TRUE)
	{
	
		/* receive the entire sdp message from sender */
		pointer = 0;

		while(pointer < sdp_msg_size)
		{
			read_size = (int)read(socket_desc, 
					 sdp_msg + pointer ,
					 (size_t)sdp_msg_size);
	                
                        if(-1 == read_size)
                        {
                            error_handling(ERR_MAJOR, SDP_READ_ERROR,
                            "sdp_client_sender: Read error");

                            TRACE("\nTRACE: Exiting function sdp_client_recvr with failure\n");
                            return NULL;
                        }
                        
			pointer = pointer + read_size;
		}//end of while loop

		sdp_msg[pointer] = '\0';
		
		printf("Done\n");
                printf("\nRecieved SDP Offer: %s\n", sdp_msg);
			
		/* send SDP acknowledgement to sender */
		
		if(NULL == strcpy(buffer, "SDP OFFER RECEIVED"))
		{
		
		   	error_handling(ERR_MAJOR, SDP_RECEIVER_MESSAGE_ERROR,
				        "sdp_recvr: Maloc error");
	      	  	TRACE("\nTRACE: Exiting function sdp_client_recvr with failure\n");
			return NULL;
		}

		size = (int)strlen(buffer) + 1;
		write_ret = (int) write(socket_desc, buffer, (size_t) size);

                if(-1 == write_ret)
                {
                    error_handling(ERR_MAJOR, SDP_WRITE_ERROR,
                                    "sdp_client_recvr: Write error");

                    TRACE("\nTRACE: Exiting function sdp_client_recvr with failure\n");
    
	            return NULL;
                }
                
                memset(buffer, 0, MAXBUFFER);
                
		read_size = (int) read(socket_desc, buffer, (size_t) MAXBUFFER);
                if(-1 == read_size)
                {
                    error_handling(ERR_MAJOR, SDP_READ_ERROR,
                                    "sdp_client_sender: Read error");

                    TRACE("\nTRACE: Exiting function sdp_client_recvr with failure\n");
                    return NULL;
                }
			
	        printf("\nMessage from server: %s\n", buffer);
                
		if(strncmp(buffer, "OK", 2) == SUCCESS)
		{
                        *p_sess_sockdes = socket_desc; /*For use in sdp
                                                      answer sender*/

	        	TRACE("\nTRACE: Exiting function sdp_client_recvr with success\n");
			return sdp_msg;
		}
		else
		{
	        	TRACE("\nTRACE: Exiting function sdp_client_recvr with failure\n");
			free(sdp_msg);
			return (char*)NULL; 
		}
		
	}//end of while
        
	free(sdp_msg);
	return (char*)NULL;
}//end of function
