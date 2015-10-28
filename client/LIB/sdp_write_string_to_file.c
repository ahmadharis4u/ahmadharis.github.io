/**********************************************************************
*
* FILE NAME     : sdp_write_string_to_file.c
*
* DESCRIPTION   : To add SDP attributes in string into a file
*
* Date          Name        Reason              Reference                
*
* 13-Jan-10     Navjodh     Initial Creation    LLD
* 19-Jan-10     Neeraj      Error Handling      LLD 
* 19-Jan-10     Navjodh     Review              LLD
*
* Copyright (C) 2010 Aricent Inc. All Rights Reserved.
**********************************************************************/

/*************HEADER FILES INCLUSION**************/
#include"./../HEADER/sdp.h"


/**********************************************************************
*
* FUNCTION NAME : sdp_write_string_to_file
*
* DESCRIPTION   : To add SDP attributes in string to a file
*
* RETURN TYPE   : return_type
*
* PARAMETERS    : char pointer, char pointer
*
***********************************************************************/
return_type sdp_write_string_to_file(
                                        char * sdp_string,  /*SDP message*/
				        char *file_name	    /*file name*/
				    )
{
    FILE *fp = NULL;    /* File pointer*/
    char *token = NULL; /* For strtok*/
    
    TRACE("\nTRACE: Entering function sdp_write_string_to_file\n");
    /**********OPENING THE OUTPUT FILE********/
    fp = fopen(file_name, "w");
    if(NULL == fp)
    {
        error_handling(ERR_MAJOR,SDP_FILE_WRITE_ERROR,
			"sdp_write_string_to_file: File write error");
        TRACE("\nTRACE: Exiting function sdp_write_string_to_file with failure\n");
       	return FAILURE;
    }
    token = strtok(sdp_string, ";");
    while(token != NULL)
    {
        fprintf(fp, "%s\\r\\n\n", token);
	token = strtok(NULL, ";");
    }

    if(EOF == fclose(fp))
    {
        error_handling(ERR_MAJOR,SDP_CLOSE_ERROR,
                            "sdp_write_string_to_file: fclose error");
        
        TRACE("\nTRACE: Exiting function sdp_write_string_to_file with failure\n");
        return FAILURE;
    }    
    
    TRACE("\nTRACE: Exiting function sdp_write_string_to_file with success\n");
    return SUCCESS;
}
