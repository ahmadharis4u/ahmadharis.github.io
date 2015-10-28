/**********************************************************************
*
* FILE NAME : sdp_hash_append.c
*
* DESCRIPTION   : To append a '#' at end of each line in a file and 
*                 store it in a new file
*
*	DATE			NAME			REASON			REFERENCE
*
*	19-JAN-10	Ahmad Haris			Modfied				LLD
*
* Copyright @ 2010 Aricent Inc. All Rights Reserved.
*
***********************************************************************/

/*************HEADER FILES INCLUSION**************/
#include"./../HEADER/sdp.h"


/**********************************************************************
*
* FUNCTION NAME : sdp_hash_append
*
* DESCRIPTION   : To append a '#' at end of each line in a file and store it in 
*		  a new file
*
* RETURN TYPE   : return_type
*
* PARAMETERS    : char pointer
*
***********************************************************************/
return_type sdp_hash_append(char * file_name)
{
    FILE *fp1 = NULL;                /*file pointer*/
    FILE *fp2 = NULL;                /*file pointer*/
    char *token = NULL;              /*for strtok*/
    char new_file_name[FILE_SIZE];   /*new file name*/
    char file_name_temp[FILE_SIZE];  /*temp file name*/
    char temp_char = INIT;           /*for fgetc*/
    long location = ZERO;            /*storing location*/
    
    TRACE("\nTRACE: Entering function sdp_hash_append\n");
	
	/*initialising arrays*/
	memset(new_file_name, 0, FILE_SIZE);
	memset(file_name_temp, 0, FILE_SIZE);
    
    if(NULL == strncpy(file_name_temp, file_name,
		strlen(file_name) + 1))
    {

        error_handling(ERR_MAJOR,SDP_STRNCPY_ERROR,
			"sdp_hash-append: strncpy error");

        TRACE("\nTRACE: Exiting function sdp_hash_append with failure\n");
       	return FAILURE;
    }
	
    token = strtok (file_name_temp, ".");   /*To get filename token*/

    /**********OPENING THE INPUT FILE********/
    fp1 = fopen(file_name, "r");
	/*error handling*/
    if(NULL == fp1)
    {
        error_handling(ERR_MAJOR,SDP_FILE_HASH_ERROR,
			"sdp_hash-append: File open error");
        TRACE("\nTRACE: Exiting function sdp_hash_append with failure\n");
       	return FAILURE;
    }
    
    if(NULL != token)
        (void)snprintf(new_file_name, (size_t)FILE_SIZE,"%s.tmp",token);
    

    /***********OPENING TEMP FILE**********/
    fp2 = fopen(new_file_name, "w");
	/*error handling*/
    if(NULL == fp2)
    {
        error_handling(ERR_MAJOR,SDP_FILE_HASH_ERROR,
			"sdp_hash-append: File open error");
		if(EOF == fclose(fp1))
		{
			error_handling(ERR_MINOR, SDP_CLOSE_ERROR,
	    		   	"sdp_hash-append: fclose error ");
		}
        TRACE("\nTRACE: Exiting function sdp_hash_append with failure\n");
		return FAILURE;
    }
    
    while((temp_char = (char) fgetc(fp1)) != (char) EOF)
    {   
        location = ftell(fp1) - 1;
        (void)fseek(fp1,location, SEEK_SET); 
   
		while((temp_char = (char) fgetc(fp1)) != '\n')
            (void)fputc(temp_char, fp2);
        if(SUCCESS > fprintf(fp2, "#\n")) /*Adding # at end of each line*/
		{
			error_handling(ERR_MAJOR, SDP_FILE_WRITE_ERROR,
				"sdp_hash-append: fprintf error ");
			if(EOF == fclose(fp1) || EOF == fclose(fp2))
			{
				error_handling(ERR_MINOR, SDP_CLOSE_ERROR,
	    			   	"sdp_hash-append: fclose error ");
			}
			TRACE("\nTRACE: Exiting function sdp_hash_append with failure\n");
			return FAILURE;
		}
    
    }//end of while loop
    
	if(EOF == fclose(fp1) || EOF == fclose(fp2))
	{
		error_handling(ERR_MINOR, SDP_CLOSE_ERROR,
   			   	"sdp_hash-append: fclose error ");
		TRACE("\nTRACE: Exiting function sdp_hash_append with failure\n");
		return FAILURE;
	}

    if(NULL == strncpy(inputfile,new_file_name, 
		strlen(new_file_name) + 1))
    {
	
        error_handling(ERR_MAJOR,SDP_STRNCPY_ERROR,
			"sdp_hash-append: strncpy error");
        TRACE("\nTRACE: Exiting function sdp_hash_append with failure\n");
       	return FAILURE;
    }

    TRACE("\nTRACE: Exiting function sdp_hash_append with success\n");
    return SUCCESS;
	
}//end of function

