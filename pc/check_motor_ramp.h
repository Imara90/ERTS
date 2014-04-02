/*--------------
Check motor ramp limit using lift data being sent to the FPGA

1-04-2014

*/
#define UP_MAXRAMP      25
#define DOWN_MAXRAMP    8    
int prev_lift = 0;



void check_motor_ramp(int *actual_lift) {
    int delta = 0;
    
    delta = *actual_lift - prev_lift;     
    if (*actual_lift > 75 && prev_lift > 75)
    {
    
        if(abs(delta) > UP_MAXRAMP )            
        {
           if (delta < 0) // decrement
			{
				*actual_lift = prev_lift - UP_MAXRAMP;
			}
			else // increment
            {
				*actual_lift = prev_lift + UP_MAXRAMP;
			} 
        }
    }
    else
    {
        if(abs(delta) > DOWN_MAXRAMP )            
        {
           if (delta < 0) // decrement
			{
				*actual_lift = prev_lift - DOWN_MAXRAMP;
			}
			else // increment
            {			
				*actual_lift = prev_lift + DOWN_MAXRAMP;
			} 
        }
    }    
}
