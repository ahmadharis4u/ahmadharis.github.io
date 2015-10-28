/**********************************************************************
*
* FILE NAME     : sdp_add_key.c
*
* DESCRIPTION   : To add the key field to session structure or
*                 media structure.
*
*	DATE			NAME			REASON			REFERENCE
*
*	19-JAN-10	Ahmad Haris			Modfied				LLD
*
*
* Copyright @ 2010 Aricent Inc. All Rights Reserved.
**********************************************************************/

/*************HEADER FILES INCLUSION**************/
#include"./../HEADER/sdp.h"


/**********************************************************************
*
* FUNCTION NAME : sdp_add_key
*
* DESCRIPTION   : To add the key field to session structure or media 
*                 structure.
*
* RETURN TYPE   : return_type
*
* PARAMETERS    : sdp_session_t pointer
*
***********************************************************************/

/*************FUNCTION DEFINTION******************/
return_type sdp_add_key(sdp_session_t *sess, /*session variable*/
                        char *file_name      /*file name*/
						)
{
    FILE   *fp = NULL;          /*file pointer*/
    char   temp_char = INIT;    /*for fgetc*/
    char   key_field[SIZE];     /*key_field buffer*/
    int    index = ZERO;        /*buffer index*/ 

    TRACE("\nTRACE: Entering function sdp_add_key\n");

	memset(key_field, 0, SIZE);
	
    /***********OPENING THE INPUT FILE*************/
    fp = fopen(file_name, "r");
	/*error handling*/
    if(NULL == fp)
    {
		error_handling(ERR_MAJOR, SDP_KEY_FILE_ERROR, 
		       "sdp_add_key: File error ");
        TRACE("\nTRACE: Exiting function sdp_add_key with Failure\n");
        return FAILURE;
    }
	/*reading file*/
    while(TRUE == TRUE)
    {
        temp_char = (char)fgetc(fp);
		if(temp_char == 'm')
        {
			if(EOF == fclose(fp))
			{
				error_handling(ERR_MINOR, SDP_CLOSE_ERROR,
		    		   	"sdp_add_key: fclose error ");
          	  	TRACE("\nTRACE: Exiting function sdp_add_key with Failure\n");
				return FAILURE;
			}
            TRACE("\nTRACE: Exiting function sdp_add_key with Success\n");
            return SUCCESS;
        }
        /*checking for key field*/
		if(temp_char == 'k')
        {
            (void)fgetc(fp);
	   		/*reading complete line*/ 
            for(index = 0, temp_char = (char)fgetc(fp); 
		    	temp_char != '\\'; 
				temp_char = (char)fgetc(fp), index ++)
	    	{	
               	key_field[index] = temp_char;
	    	}/* end of for loop */
	    
            key_field[index] = '\0';
	    	sess->s_key = (sdp_key_t *) calloc(ONE, sizeof(sdp_key_t));
            /*error handling*/
            if(NULL == sess->s_key)
            {
				error_handling(ERR_MAJOR, SDP_KEY_MALLOC_ERROR,
			    		   "sdp_add_key: Malloc error ");
				if(EOF == fclose(fp))
				{
					error_handling(ERR_MINOR, SDP_CLOSE_ERROR,
		    			   	"sdp_add_key: fclose error ");
				}
                TRACE("\nTRACE: Exiting function sdp_add_key with Failure\n");
    	        return FAILURE;
            }
	    
            sess->s_key->k_method = (char *) calloc(strlen(key_field) + ONE, 
	   		 					    sizeof(char));
		     
            if(NULL == sess->s_key->k_method)
            {
				error_handling(ERR_MAJOR, SDP_KEY_MALLOC_ERROR, 
					       "sdp_add_key: Malloc error ");
				if(EOF == fclose(fp))
				{
					error_handling(ERR_MINOR, SDP_CLOSE_ERROR,
		    			   	"sdp_add_key: fclose error ");
				}
                TRACE("\nTRACE: Exiting function sdp_add_key with Failure\n");
    	        return FAILURE;
            }
	    
            if(NULL == (strncpy(sess->s_key->k_method, key_field,
				strlen(key_field) + 1)))
	    	{
				error_handling(ERR_MAJOR, SDP_STRNCPY_ERROR,
			    		   "sdp_add_key: Strncpy error");
				if(EOF == fclose(fp))
				{
					error_handling(ERR_MINOR, SDP_CLOSE_ERROR,
		    			   	"sdp_add_key: fclose error ");
				}
                TRACE("\nTRACE: Exiting function sdp_add_key with Failure\n");
    	        return FAILURE;
	    	}
	    
	    	sess->s_key->k_enckey = NULL;
        }/* end of while(TRUE == TRUE)*/
       
	   	/*ckecking for '\n'*/ 
		for(temp_char = (char)fgetc(fp); 
	    	temp_char != '\n'; 
	    	temp_char = (char)fgetc(fp));
    }
}
