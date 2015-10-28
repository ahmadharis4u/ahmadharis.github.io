/***********************************************************************
*
*
* FILE NAME : sdp_checker.c
*
* DESCRIPTION   : To check the input file whether it is valid r not.
*
*
*	DATE			NAME			REASON			REFERENCE
*
*	19-JAN-10	Ahmad Haris,		Modfied				LLD
*				Deepshikha
*
* Copyright @ 2010 Aricent Inc. All Rights Reserved.
*
***********************************************************************/

/**************HEADER FILES INCLUSION***************/
#include"./../HEADER/sdp.h"



/**********************************************************************
*
* FUNCTION NAME : sdp_checker
*
* DESCRIPTION   : To check the input file whether it is valid r not.
*
* RETURN TYPE   : return_type
*
* PARAMETERS    : char pointer
*
***********************************************************************/
/**************FUNCTION DEFINITION******************/
static return_type sess_index(char c);
static return_type media_index(char c);
static return_type fieldcount(char c);

return_type sdp_checker(char *file_name  /*file name*/
				)
{
    char   temp_char = INIT;            /*for fgetc*/     
    char   temp_char1 = INIT;           /*for fgetc*/
    char   field_name = INIT;           /*for storing field_name*/
    char   syn_check = INIT;            /*for checking*/
    char   error_string[ERROR_STR_SIZE];/*error_string buffer*/
    FILE   *fp = NULL;                  /*file pointer*/
    int    loop_var = ZERO;             /*loop count*/
    int    colon_present = ZERO;        /*colon flag*/
    int    index = ZERO;                /*buffer index*/
    int    count[ARR_SIZE];             /*for counting fields*/
    int    prev = ONE;                  /*flag*/
    int    curr = ZERO;                 /*flag*/
    int    space_count = ZERO;          /*space counting*/
    int    field_count; /*mandatory :v o s t m,  non-repetitive: v o s i u k a*/

    TRACE("\nTRACE: Entering function sdp_checker\n");
   
   /*initialising arrays*/ 
	memset(error_string, 0, ERROR_STR_SIZE);
    for (index = 0; index < ARR_SIZE; index++)
        count[index] = ZERO;
    
	/****OPENING THE INPUT FILE****/
    fp = fopen(file_name,"r");
	/*error handling*/
    if(NULL == fp)
    {
        error_handling(ERR_MAJOR,SDP_CHECKER_FILE_ERROR,
			"sdp_checker: File error");
        TRACE("\nTRACE: Exiting function sdp_checker with Failure\n");
		return FAILURE;
    }

    /***Checking for mandatory attributes***/
    while(TRUE == TRUE)
    {
        temp_char = (char)fgetc(fp);
		switch(temp_char)
		{
		    case 'v': 
        	        count[0]++;
            	    break;
 
		    case 'o':
    	            count[1]++;
        	        break;
                   
		    case 's':
    	            count[2]++;
        	        break;
	 
		    case 't':
        	        count[3]++;
            	    break;

		    case 'm':
    	            count[4]++;
        	        break;
		}// end of switch

		for(temp_char = (char)fgetc(fp); temp_char != '\n' && 
			temp_char != (char) -1; temp_char = (char)fgetc(fp));

		if(temp_char == (char) -1)
	    	break;
    }//end of while

    for (index = 0; index < COUNT_SIZE; index++)
	{
        if(ZERO == count[index])
		{
        	error_handling(ERR_CRITICAL,
	    			   SDP_CHECKER_MANDATORY_ATTR_ERROR,
			  	"sdp_checker: Mandatory Attribute error");
			if(EOF == fclose(fp))
			{
				error_handling(ERR_MINOR, SDP_CLOSE_ERROR,
		    		   	"sdp_checker: fclose error ");
			}
        	TRACE("\nTRACE: Exiting function sdp_checker with Failure\n");
			return FAILURE;
		}
	}

    rewind(fp); /*go to start of file*/
	/*making all entries as ZERO*/
    for (index = 0; index < ARR_SIZE; index++)
        count[index] = ZERO;
	
    /***Checking for non repeative attributes***/
    while(TRUE == TRUE)
    {
        temp_char = (char)fgetc(fp);

		if(temp_char == 'm') 
        {
            break;
        }

		switch(temp_char)
		{
		    case 'v':
    	            count[0]++;
        	        break;

		    case 'o':
    	            count[1]++;
        	        break;
 
		    case 's':
    	            count[2]++;
        	        break;
          
		    case 'i':
    	            count[3]++;
        	        break;

	    	case 'u':
            	    count[4]++;
               	 	break;
 
	 	   case 'k':
        	        count[5]++;
            	    break;
 
		    case 'a':
    	            count[6]++;
        	        break;

		}//end of switch
		
		/*going to end of line*/
		for (temp_char = (char)fgetc(fp); temp_char != '\n'; 
			temp_char = (char)fgetc(fp));

    }//end of while
    
    if (count[0] > 1 || count[1] > 1 || count[2] > 1 || 
		count[3] > 1 || count[4] >1 || count[5] > 1 || count[6] > 1)
    {
        error_handling(ERR_CRITICAL,
			SDP_CHECKER_NON_REPEATIVE_ATTR_ERROR,
			"sdp_checker: Non-repeative attribute error");
		if(EOF == fclose(fp))
		{
			error_handling(ERR_MINOR, SDP_CLOSE_ERROR,
		    		   	"sdp_checker: fclose error ");
		}
        TRACE("\nTRACE: Exiting function sdp_checker with Failure\n");
		return FAILURE;
    }

    rewind(fp);/*going to start of file*/
    for (temp_char = (char)fgetc(fp); temp_char != '\n'; 
		temp_char = (char)fgetc(fp));
   	

    /***Checking for order of attributes upto media attribute***/
    while (TRUE == TRUE)
    {
        temp_char = (char)fgetc(fp);

		if(temp_char == 'm') 
			break;
		curr = sess_index(temp_char);
		/*checking for valid index*/
		if(curr < prev)
		{
            error_handling(ERR_CRITICAL,SDP_CHECKER_S_ORDER_ERROR,
				"sdp_checker: Session order error");
			if(EOF == fclose(fp))
			{
				error_handling(ERR_MINOR, SDP_CLOSE_ERROR,
		    			   	"sdp_checker: fclose error ");
			}
            TRACE("\nTRACE: Exiting function sdp_checker with Failure\n");
	    	return FAILURE;
		}
        prev = curr;
		for (temp_char = (char)fgetc(fp); temp_char != '\n'; 
			temp_char = (char)fgetc(fp));

    }//end of while
	
    /***Checking for order of attributes from media attribute***/
    rewind(fp); /*going to start of file*/
    temp_char = 'v';	

    while(TRUE == TRUE)
    {

        if(temp_char != 'm')
            temp_char = (char)fgetc(fp);

		if(temp_char == 'm')
		{   
	    	prev = 1;
	    	for (temp_char = (char)fgetc(fp);
				temp_char != '\n' && temp_char != (char) -1; 
				temp_char = (char)fgetc(fp));

	    	if(temp_char == (char) -1) 
				break;
				
			/*dealing with media attributes*/
	    	while(TRUE == TRUE)
	    	{
	        	if((temp_char = (char)fgetc(fp)) == (char)-1) 
					break;

	        	if( temp_char == 'm') 
					break;

				curr = media_index(temp_char);

	        	if(curr < prev)
	        	{
           	    	error_handling(ERR_CRITICAL,
		    		   SDP_CHECKER_M_ORDER_ERROR,
				   		"sdp_checker: Media order error");
					if(EOF == fclose(fp))
					{
						error_handling(ERR_MINOR, SDP_CLOSE_ERROR,
		    				   	"sdp_checker: fclose error ");
					}
               	 	TRACE("\nTRACE: Exiting function sdp_checker with Failure\n");
	            	return FAILURE;
	        	}

                prev = curr;
				
	        	/*checking for '\n' or EOF*/
				for (temp_char = (char)fgetc(fp); 
					temp_char != '\n' && temp_char != (char) -1; 
					temp_char = (char)fgetc(fp));
	    	}//end of while

	    	if(temp_char == (char) -1)
				break;

		}//end of if

		if(temp_char != 'm') 
		for (temp_char = (char)fgetc(fp); temp_char != '\n'; 
			temp_char = (char)fgetc(fp));

    }//end of while

    rewind(fp);

    /***Checking for number of fields in each attribute***/
    while(TRUE == TRUE)
    {
        if((temp_char = (char)fgetc(fp)) == (char) -1)
			break;

		if(temp_char == 'i') 
		{
	    	for (temp_char = (char)fgetc(fp); temp_char != '\n'; 
			temp_char = (char)fgetc(fp));
	    	continue;
		}

		field_count = fieldcount(temp_char);
		for (space_count = 0, temp_char1 = (char)fgetc(fp); 
			temp_char1 != '\n'; temp_char1 = (char)fgetc(fp))
		{

	    	if(temp_char == 'e' && temp_char1 == '(')
	    	{
				for (temp_char1 = (char)fgetc(fp); temp_char1 != '\n'; 
					temp_char1 = (char)fgetc(fp));
	        
				break;
	    	}	

	    	if(temp_char1 == ' ')
	        	space_count++;
		}

		if(temp_char == 'k' || temp_char == 'a')
		{

	    	if(space_count > field_count-1)
	    	{
            	error_handling(ERR_MAJOR,
		    			   SDP_CHECKER_FIELDS_NUM_ERROR,
				   	"sdp_checker: Number of fields error");
				if(EOF == fclose(fp))
				{
					error_handling(ERR_MINOR, SDP_CLOSE_ERROR,
	    				   	"sdp_checker: fclose error ");
				}
            	TRACE("\nTRACE: Exiting function sdp_checker with Failure\n");
	        	return FAILURE;
	    	}
		}

		else if(temp_char == 'm' || temp_char == 'p')
		{

	    	if(space_count < field_count-1)
	    	{
            	error_handling(ERR_MAJOR,
		   			   SDP_CHECKER_FIELDS_NUM_ERROR,
		            	     "sdp_checker: Number of fields error");
				if(EOF == fclose(fp))
				{
					error_handling(ERR_MINOR, SDP_CLOSE_ERROR,
	    				   	"sdp_checker: fclose error ");
				}
            	TRACE("\nTRACE: Exiting function sdp_checker with Failure\n");
	        	return FAILURE;
	    	}
		}
		else
		{
			/*checking for sepration of words*/
	    	if(space_count != field_count-1)
	    	{
            	error_handling(ERR_MAJOR,
		    			   SDP_CHECKER_FIELDS_NUM_ERROR,
				 	"sdp_checker: Number of fields error");
				if(EOF == fclose(fp))
				{
					error_handling(ERR_MINOR, SDP_CLOSE_ERROR,
	    				   	"sdp_checker: fclose error ");
				}
            	TRACE("\nTRACE: Exiting function sdp_checker with Failure\n");
	        	return FAILURE;
	    	}
		}
    }//end of while
    
    
    /*** Checking for syntax ***/

    rewind(fp);

    syn_check = (char)fgetc(fp);

    while(feof(fp) == ZERO)
    {
    	field_name = syn_check;
		colon_present = ZERO;
		syn_check = (char)fgetc(fp);

		if(syn_check != '=')
		{	
        	(void)snprintf(error_string, ERROR_STR_SIZE-1, 
							"Error in '%c' field", field_name);
		 	error_handling(ERR_MAJOR,
		   		   SDP_CHECKER_FIELDS_NUM_ERROR,
				   error_string);
			if(EOF == fclose(fp))
			{
				error_handling(ERR_MINOR, SDP_CLOSE_ERROR,
    				   	"sdp_checker: fclose error ");
			}
         	TRACE("\nTRACE: Exiting function sdp_checker with Failure\n");
	     	return FAILURE;
		}
		syn_check = (char)fgetc(fp);

		if(syn_check == ' ')
		{
	
        	(void)snprintf(error_string, ERROR_STR_SIZE-1,
							"Error in '%c' field", field_name);
        	error_handling(ERR_MAJOR,
		 		   SDP_CHECKER_FIELDS_NUM_ERROR,
			   error_string);
			if(EOF == fclose(fp))
			{
				error_handling(ERR_MINOR, SDP_CLOSE_ERROR,
    				   	"sdp_checker: fclose error ");
			}
         	TRACE("\nTRACE: Exiting function sdp_checker with Failure\n");
	     	return FAILURE;
		}
	
    	for (syn_check = (char)fgetc(fp); syn_check != '\\' && 
			syn_check != '#'; syn_check = (char)fgetc(fp))
		{

			if (field_name == 'b')
			{

				if (syn_check == ':')
					colon_present = 1;
			}
		}

		if(syn_check  == '#')
		{
        	(void)snprintf(error_string, (size_t)ERROR_STR_SIZE-1, 
						"Error in '%c' field", field_name);
        	error_handling(ERR_MAJOR,
	    		   SDP_CHECKER_FIELDS_NUM_ERROR,
				   error_string);
			if(EOF == fclose(fp))
			{
				error_handling(ERR_MINOR, SDP_CLOSE_ERROR,
    				   	"sdp_checker: fclose error ");
			}
        	TRACE("\nTRACE: Exiting function sdp_checker with Failure\n");
			return FAILURE;
		}
		for (loop_var = 0; loop_var < 3; loop_var++)
		{
			syn_check = (char)fgetc(fp);

			if (syn_check != 'r' && syn_check != '\\' && syn_check != 'n')
			{
			
       	    	(void)snprintf(error_string, ERROR_STR_SIZE-1, 
								"Error in '%c' field", field_name);
       	    	error_handling(ERR_MAJOR,
	    			   			SDP_CHECKER_FIELDS_NUM_ERROR,
				   				error_string);
				if(EOF == fclose(fp))
				{
					error_handling(ERR_MINOR, SDP_CLOSE_ERROR,
    					   	"sdp_checker: fclose error ");
				}
            	TRACE("\nTRACE: Exiting function sdp_checker with Failure\n");
	        	return FAILURE;
			}
		}//end of for loop

		if(field_name == 'b' && colon_present != 1)
		{
     	    (void)snprintf(error_string, ERROR_STR_SIZE, 
							"Error in '%c' field", field_name);
       	    error_handling(ERR_MAJOR,
	   		 			  SDP_CHECKER_FIELDS_NUM_ERROR,
				   			error_string);
			if(EOF == fclose(fp))
			{
				error_handling(ERR_MINOR, SDP_CLOSE_ERROR,
   					   	"sdp_checker: fclose error ");
			}
            TRACE("\nTRACE: Exiting function sdp_checker with Failure\n");
	        return FAILURE;
		
		}
	
    	for (syn_check = (char)fgetc(fp); syn_check != '#'; 
			syn_check = (char)fgetc(fp));
			(void)fgetc(fp);

		syn_check = (char)fgetc(fp);
	
    }/*end of while(feof)*/
	
	if(EOF == fclose(fp))
	{
		error_handling(ERR_MINOR, SDP_CLOSE_ERROR,
   			   	"sdp_checker: fclose error ");
    	TRACE("\nTRACE: Exiting function sdp_checker with Failure\n");
    	return FAILURE;
	}
    TRACE("\nTRACE: Exiting function sdp_checker with Success\n");
    return SUCCESS;
}//end of function



/**********************************************************************
*
* FUNCTION NAME : sess_index
*
* DESCRIPTION   : It gives position of attribute in session attribute
*		  list
*
* RETURN TYPE   : return_type
*
* PARAMETERS    : char
*
***********************************************************************/
return_type sess_index(char ch  /*character to be matched*/
                      )
{
    int index = ZERO;
    char attr_arr[SESS_ARR_SIZE] = {'v','o','s','i','u','e','p','c','b','t','r','z','k','a','\0'};
    TRACE("\nTRACE: Entering function sess_index\n");
    
	for(index = 0; attr_arr[index] != '\0'; index++)
        if(attr_arr[index] == ch)
        {
     	    TRACE("\nTRACE: Exiting function sess_index with Success\n");
		    return(index + 1);
        }
     
	 TRACE("\nTRACE: Exiting function sess_index with Success\n");
     return SUCCESS;
}



/**********************************************************************
*
* FUNCTION NAME : media_index
*
* DESCRIPTION   : It gives position of attribute in media attribute list
*
* RETURN TYPE   : return_type
*
* PARAMETERS    : char
*
***********************************************************************/
return_type media_index(char ch  /*character to be matched*/
                       )
{
    int index = ZERO;
    char attr_arr[MEDIA_ARR_SIZE] = {'i','c','b','k','a','\0'};
    TRACE("\nTRACE: Entering function media_index\n");
    
	for(index = 0; attr_arr[index] != '\0'; index++)
        if(attr_arr[index] == ch)
        {
     	    TRACE("\nTRACE: Exiting function media_index with Success\n");
		    return(index + 1);
        }
     
	 TRACE("\nTRACE: Exiting function media_index with Success\n");
     return SUCCESS;
}




/**********************************************************************
*
* FUNCTION NAME : fieldcount
*
* DESCRIPTION   : It gives number of fields in eac attribute
*
* RETURN TYPE   : return_type
*
* PARAMETERS    : char
*
***********************************************************************/
return_type fieldcount(char ch /*character to be matched*/
                      )
{
    struct field
    {
        char temp_char2;
        int num;
    }field[FIELDCOUNT_SIZE]={{'v',1},{'o',6},{'s',1},{'u',1},{'e',1},{'p',1},{'c',3},{'b',1},{'t',2},{'r',4},{'z',4},{'k',2},{'a',2},{'m',4}};

    int index = ZERO;
    TRACE("\nTRACE: Entering function fieldcount\n");
    
	for(index = 0; index < FIELDCOUNT_SIZE; index++)
        if(field[index].temp_char2 == ch)
        {
     	    TRACE("\nTRACE: Exiting function fieldcount with Success\n");
		    return(field[index].num);
        }
    
	TRACE("\nTRACE: Exiting function fieldcount with Success\n");
    return SUCCESS;
}

