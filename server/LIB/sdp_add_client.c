/**********************************************************************
*
* FILE NAME     : sdp_add_client.c
*
* DESCRIPTION   : To add the client to database.
*
*	DATE			NAME			REASON			REFERENCE
*
*	14-JAN-10	Deepshikha		Initial Creation	LLD
*	19-JAN-10	Ahmad Haris		Error Handling		LLD
*
* Copyright @ 2010 Aricent Inc. All Rights Reserved.
**********************************************************************/

/*************HEADER FILES INCLUSION*************/
#include"./../HEADER/sdp.h"


/***********************************************************************
*
* FUNCTION NAME : sdp_add_client
*
* DESCRIPTION   : To add the client to database.
*
* RETURN TYPE   : return_type
*
***********************************************************************/
/*************FUNCTION DEFINTION*****************/

return_type sdp_add_client(struct sockaddr_in *client_address)
{
	FILE *fptr = NULL ;
	char port[FIELD_SIZE];
	char address[FIELD_SIZE];

	TRACE("\nTRACE: Entering function sdp_add_client\n");

	/*Opening file*/
	if(NULL == (fptr = fopen("client_database.txt" , "a")))
	{
		error_handling(ERR_MAJOR, SDP_FIELDS_FILE_ERROR,
						"sdp_add_client : fopen error.");
		TRACE("\nTRACE: Exiting function sdp_add_client with failure.\n");
		return FAILURE;
	}
	if(NULL == strncpy(address , inet_ntoa((*client_address).sin_addr), FIELD_SIZE))
	{
		error_handling(ERR_MAJOR, SDP_STRNCPY_ERROR,
						"sdp_add_client : strncpy error.");
		TRACE("\nTRACE: Exiting function sdp_add_client with failure.\n");
		return FAILURE;
	}
	(void)snprintf(port, (size_t)sizeof(uint32_t), "%u", htonl((*client_address).sin_port));

	if(FAILURE == fprintf(fptr, "%s %s\n", address, port)) /*writing to file*/
	{
		error_handling(ERR_MAJOR, SDP_FILE_WRITE_ERROR,
						"sdp_add_client : fprintf error.");
		TRACE("\nTRACE: Exiting function sdp_add_client with failure.\n");
		return FAILURE;
	}
	
	if(EOF == fclose(fptr))
	{
		error_handling(ERR_MAJOR, SDP_CLOSE_ERROR,
						"sdp_add_client : fclose error.");
		TRACE("\nTRACE: Exiting function sdp_add_client with failure.\n");
		return FAILURE;
	}
	TRACE("\nTRACE: Exiting function sdp_add_client with success.\n");
	return SUCCESS;
}
