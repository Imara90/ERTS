/*--------------
P_CONTROL_MODE

Updates proportional gains of control modes

21-03-2014(Created by Diogo Monteiro)

*/

#define PCONTROL_INIT   22
#define P1CONTROL_INIT  1
#define P2CONTROL_INIT  2


//INITIALIZATION OF P GAINS
int pcontrol_init = PCONTROL_INIT; //P Gain
int p1control_init = P1CONTROL_INIT; //P1 Gain
int p2control_init = P2CONTROL_INIT; //P2 Gain
int pcontrol =  PCONTROL_INIT;
int p1control = P1CONTROL_INIT;
int p2control = P2CONTROL_INIT;


void p_control_mode(int PCONTROL,int P1CONTROL, int P2CONTROL) {


    if (PCONTROL <= 127) {
        pcontrol = pcontrol_init + PCONTROL;
    }
    else {
        pcontrol = pcontrol_init + (PCONTROL - 255);
    }
    
    if (P1CONTROL <= 127) {
        p1control = p1control_init + P1CONTROL;
    }
    else {
        p1control = p1control_init + (P1CONTROL - 255);
    }
    
    if (P2CONTROL <= 127) {
        p2control = p2control_init + P2CONTROL;
    }
    else {
        p2control = p2control_init + (P2CONTROL - 255);
    }

}

