/******************************************************************************
*
*   FILE NAME :err_handler.c
*
*   DESCRIPTION : it contains a function that is called if any error condition 
*		occurs
*
*   Date          Name        Reason              Reference                
*
*   19-Jan-10     Neeraj      Code Review	      LLD
*
*   Copyright(C) 2010 ARICENT Inc. All Rights Reserved
*
******************************************************************************/

/**************************HEADER file ****************************************/

#include "./../HEADER/sdp.h"



/******************************************************************************
*
* FUNCTION NAME: error_handling
*
* DESCRIPTION: Prints the error message along with the error number
*
* RETURNS: Returns void
*
*******************************************************************************/

void error_handling(
        	          int err_level, /*having error level*/
				      int err_no, /*having error number*/
					char * err_msg /*error message*/
		    )
{
    if( err_level >= ZERO )
    {
       	printf ("\nError No : (%d)\n", err_no );
       	printf ("\nError message : (%s)\n", err_msg );
	    
        switch( err_level )
		{
            case 1 :
                    printf ("\nError level : MINOR\n");
        		    break;
                    
	    	case 2 :
		    	printf ("\nError level : MAJOR\n");
		    	break;
                    
	    	case 3 :
		  	  	printf ("\nError level : CRITICAL\n");
		    	break;
                    
	    	default:
		    	printf ("\nERROR LEVEL NOT DEFINED\n");
		    	break;
		}
    }
    
    return;
}

