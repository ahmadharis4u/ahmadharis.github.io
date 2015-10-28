/***********************************************************************
*
* FILE NAME     : sdp_add_name.c
*
* DESCRIPTION   : To add the name field to session structure.
*
*
*	DATE			NAME			REASON			REFERENCE
*
*	19-JAN-10	Ahmad Haris			Modfied				LLD
*
* Copyright @ 2010 Aricent Inc. All Rights Reserved.
*
***********************************************************************/

/******************HEADER FILE INCLUSION**********************/
#include"./../HEADER/sdp.h"


/*********************************************************************
*
* FUNCTION NAME : sdp_add_name
*
* DESCRIPTION   : To add the name field to session structure.
*
* RETURN TYPE   : return_type
*
* PARAMETERS    : sdp_session_t pointer, char pointer
*
**********************************************************************/
return_type sdp_add_name(sdp_session_t *sess,  /*session variable*/
                         char *file_name       /*file name*/
						 )
{
    FILE   *fp = NULL;         /*file pointer*/
    char   temp_char = INIT;   /*for fgetc*/
    char   name_field[SIZE];   /*name_field buffer*/
    int    index = ZERO;       /*buffer index*/

    TRACE("\nTRACE: Entering function sdp_add_name\n");
   
   	memset(name_field, 0, SIZE);
    /***OPENING THE INPUT FILE***/
    fp   = fopen(file_name, "r");
    /*error handling*/
    if(NULL == fp)
    {
		error_handling(ERR_MAJOR, SDP_NAME_FILE_ERROR, 
		       "sdp_add_name: File error ");
        TRACE("\nTRACE: Exiting function sdp_add_name with Failure\n");
        return FAILURE;
    }
    
    while(TRUE == TRUE)
    {
        if((int)(temp_char = (char)fgetc(fp)) == -1)
            break;
       
        /***assigning value to session name field of session structure***/ 	
		if(temp_char == 's')
        {
            (void)fgetc(fp);
           /*reading a line*/ 
	    	for(index = 0, temp_char = (char)fgetc(fp); 
	        	temp_char != '\\'; 
				temp_char = (char)fgetc(fp), index ++)
	    	{
                name_field[index] = temp_char;
	    	} /* end of for */
	    
            name_field[index] = '\0';
            sess->s_name = (char *) calloc(strlen(name_field) + ONE, sizeof(char));
            /*error handling*/
			if(NULL == sess->s_name)
            {
				error_handling(ERR_MAJOR, SDP_NAME_MALLOC_ERROR,
			    		   "sdp_add_name: Malloc error ");
				if(EOF == fclose(fp))
				{
					error_handling(ERR_MINOR, SDP_CLOSE_ERROR,
		    			   	"sdp_add_name: fclose error ");
				}
                TRACE("\nTRACE: Exiting function sdp_add_name with Failure\n");
     	        return FAILURE;
            }
            
	    	if (NULL == (strncpy(sess->s_name, name_field,
				strlen(name_field) + 1)))
	    	{
	    
				error_handling(ERR_MAJOR, SDP_STRNCPY_ERROR,
			    	   "sdp_add_name: Strncpy error ");
				if(EOF == fclose(fp))
				{
					error_handling(ERR_MINOR, SDP_CLOSE_ERROR,
		    			   	"sdp_add_name: fclose error ");
				}
                TRACE("\nTRACE: Exiting function sdp_add_name with Failure\n");
     	        return FAILURE;
	    	}
        }

        for(temp_char = (char)fgetc(fp); 
	    	temp_char != '\n'; 
	    	temp_char = (char)fgetc(fp));
	    
    } /* end of while */
    
	if(EOF == fclose(fp))
	{
		error_handling(ERR_MINOR, SDP_CLOSE_ERROR,
   			   	"sdp_add_name: fclose error ");
    	TRACE("\nTRACE: Exiting function sdp_add_name with Failure\n");
   		return FAILURE;
	}
    TRACE("\nTRACE: Exiting function sdp_add_name with Success\n");
    return SUCCESS;
}
