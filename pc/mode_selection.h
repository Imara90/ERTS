/*--------------
Performs the mode selection

26-03-2014(Created by Diogo Monteiro)

*/
int prev_mode = 0;
int last_control_mode = 0;
int TELEMETRY_FLAG = 0x01;  //1 - CALIBRATION DONE, 2/3 - FORCE CHANGE TO SAFE MODE


void mode_selection(int *sel_mode,int lift) {
    //printf(" \n SELECTED MODE: %d", *sel_mode);
    
    if( *sel_mode == MODE_SAFE || *sel_mode == MODE_ABORT || (prev_mode == MODE_SAFE && (TELEMETRY_FLAG & 0x04 == 0 && lift == 0)) && *sel_mode != MODE_PANIC && ((*sel_mode != MODE_FULL_CONTROL && *sel_mode != MODE_YAW_CONTROL) || (TELEMETRY_FLAG & 0x01) == TRUE) || (*sel_mode == MODE_PANIC && prev_mode != MODE_SAFE) || (*sel_mode == MODE_P && (prev_mode == MODE_YAW_CONTROL || prev_mode == MODE_FULL_CONTROL)) || (prev_mode == MODE_P && *sel_mode == last_control_mode) ) 
    {   
        *sel_mode = *sel_mode;

    }
    else {
        *sel_mode = prev_mode;
    }
    
    //check if change to SAFE MODE was requested
    if( (TELEMETRY_FLAG & 0x02) != 0) {
        
        *sel_mode = MODE_SAFE;
        TELEMETRY_FLAG = TELEMETRY_FLAG & 0X01;   
    }
    
    if( *sel_mode == MODE_SAFE || *sel_mode == MODE_YAW_CONTROL || *sel_mode == MODE_FULL_CONTROL)    
    {
        last_control_mode = *sel_mode;

    }
    
    prev_mode = *sel_mode;
    //printf("  actual MODE: %d", *sel_mode);


}
