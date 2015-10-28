/***********************************************************************
*
* FILE NAME : sdp_add_uri.c
*
* DESCRIPTION   : To add the uri field to session structure.
*
* Date          Name        Reason              Reference                
*
* 19-Jan-10     Navjodh     Code Review         LLD
* 19-Jan-10     Neeraj      Error Handling      LLD
*
* Copyright(C) 2010 Aricent Inc. All Rights Reserved.
*
***********************************************************************/

/*******************HEADER FILE INCLUSION*****************/
#include"./../HEADER/sdp.h"


/***********************************************************************
*
* FUNCTION NAME : sdp_add_uri
*
* DESCRIPTION   : To add the uri field to session structure.
*
* RETURN TYPE   : return_type
*
* PARAMETERS    : sdp_session_t pointer, char pointer
*
***********************************************************************/
return_type sdp_add_uri(
                            sdp_session_t *sess, /*session variable*/
			    char *file_name 	/*file name*/
		        )
{
    FILE   *fp = NULL; /* File pointer */
    char   temp_char = INIT;  /* stores characters read from file */
    char   uri_field[SIZE];   /* stores uri field value */
    int    index = ZERO;        /* index variable */
    TRACE("\nTRACE: Entering function sdp_add_uri\n");

    /*****OPENING THE INPUT FILE*****/
    fp = fopen(file_name, "r");
    if(NULL == fp)
    {
        error_handling(ERR_MAJOR,SDP_URI_FILE_ERROR,"sdp_add_uri: File error ");
        TRACE("\nTRACE: Exiting function sdp_add_uri with failure\n");
		return FAILURE;
    }
    while(TRUE == TRUE)
    {
        if((int)(temp_char = (char)fgetc(fp)) == -1)
            break;
        if(temp_char == 'u')
        {
            (void)fgetc(fp);

      /***Assign uri attribute to session structure***/
            for(index = 0, temp_char = (char)fgetc(fp); 
		temp_char != '\\'; 
		temp_char = (char)fgetc(fp), index ++)
	    {
                uri_field[index] = temp_char;
	    }
		/* end of for block */
            uri_field[index] = '\0';
            sess->s_uri = (char *) calloc(strlen(uri_field) + ONE, sizeof(char));
            
	    if(NULL == sess->s_uri)
            {
                error_handling(ERR_MAJOR, SDP_URI_MALLOC_ERROR,
					"sdp_add_uri : Malloc error");
        	TRACE("\nTRACE: Exiting function sdp_add_uri with failure\n");
                if(EOF == fclose(fp))
                {
                    error_handling(ERR_MINOR, SDP_CLOSE_ERROR,
                               "sdp_add_uri: fclose error");
            
                    TRACE("\nTRACE: Exiting function sdp_add_uri with Failure\n");
                    return FAILURE;
                }
		return FAILURE;
            }
            
            if(NULL == strncpy(sess->s_uri, uri_field, 1+ strlen(uri_field)))
	    {
                error_handling(ERR_MAJOR, SDP_STRNCPY_ERROR,
					"sdp_add_uri : strncpy error");
        	TRACE("\nTRACE: Exiting function sdp_add_uri with failure\n");
                
                if(EOF == fclose(fp))
                {
                    error_handling(ERR_MINOR, SDP_CLOSE_ERROR,
                               "sdp_add_uri: fclose error");
            
                    TRACE("\nTRACE: Exiting function sdp_add_uri with Failure\n");
                    return FAILURE;
                }
	    return FAILURE;
		
	    }
        }
        
        for(temp_char = (char)fgetc(fp); 
	    temp_char != '\n'; 
	    temp_char = (char)fgetc(fp));
    }/* end of while block */

    if(EOF == fclose(fp))
    {
        error_handling(ERR_MINOR, SDP_CLOSE_ERROR,
                            "sdp_add_uri: fclose error");
            
        TRACE("\nTRACE: Exiting function sdp_add_uri with Failure\n");
        return FAILURE;
    }
    TRACE("\nTRACE: Exiting function sdp_add_uri with success\n");
    return SUCCESS;
}
