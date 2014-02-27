/*Reads the input of the joystick
* 27-02-2014(Created by Diogo Monteiro)

Input: key_map: address of keyboard mapping array
	

Output: ESC button - 0 for off and 1 for on


*/


struct termios stdin_orig;  // Structure to save parameters

void term_reset() {
        tcsetattr(STDIN_FILENO,TCSANOW,&stdin_orig);
        tcsetattr(STDIN_FILENO,TCSAFLUSH,&stdin_orig);
}

void keyboard_nonblocking() {
        struct termios newt;
        tcgetattr(STDIN_FILENO, &stdin_orig);
        fcntl(STDIN_FILENO, F_SETFL, O_NONBLOCK); // non-blocking
        newt = stdin_orig;
        newt.c_lflag &= ~(ICANON | ECHO);
        tcsetattr(STDIN_FILENO, TCSANOW, &newt);

        atexit(term_reset);
}

int read_kb(int *key_map,char c) {
int i;


//Handle Arrows and ESC		
		if(c==27){
			c=getchar();
			switch (c){
				case -1:
					return 1;
					break;
				case 91:
					c=getchar();
					break;
			}
		}

	switch (c){
		case '0'://SAFE MODE
			*key_map=0;
			break;
		case '1'://PANIC MODE
			*key_map=1;
			break;
		case '2'://MANUAL MODE
			*key_map=2;
			break;
		case '3'://CALIBRATION MODE
			*key_map=3;
			break;
		case '4'://YAW CONTROL MODE
			*key_map=4;
			break;
		case '5':// FULL CONTROL MODE	
		*key_map=5;
			break;
		case 'a'://LIFT UP
		*(key_map+1)+=2;
			break;
		case 'z'://LIFT DOWN
		*(key_map+1)-=2;
			break;
		case 68://LEFT ARROW-ROLL UP
		*(key_map+2)+=2;
			break;
		case 67://RIGHT ARROW-ROLL DOWN
		*(key_map+2)-=2;
			break;
		case 65://UP ARROW-PITCH UP
		*(key_map+3)+=2;
			break;
		case 66://DOWN ARROW-PITCH DOWN
		*(key_map+3)-=2;
			break;
		case 'q'://YAW UP
		*(key_map+4)+=2;
			break;
		case 'w'://YAW DOWN
		*(key_map+4)-=2;
			break;
		case 'u'://YAW CONTROL P UP 
		*(key_map+5)+=2;
			break;
		case 'j'://YAW CONTROL P DOWN 
		*(key_map+5)-=2;
			break;
		case 'i':
		*(key_map+6)+=2;//ROLL/PITCH CONTROL P1 UP 
		break;
		case 'k':
		*(key_map+6)-=2;//ROLL/PITCH CONTROL P1 DOWN 
		break;
		case 'o':
		*(key_map+7)+=2;//ROLL/PITCH CONTROL P2 UP 
		break;
		case 'l':
		*(key_map+7)-=2;//ROLL/PITCH CONTROL P2 DOWN 
		break;
		case 'r':
			for(i=1;i<8;i++){		
				*(key_map+i)=0;//ROLL/PITCH CONTROL P2 DOWN 
			}
		break;
		default:
			printf("\n CHARACTER NOT VALID\n");
			break;
	}
	c=-1;

return 0;


}
