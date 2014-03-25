/*All functions related to Keyboard reading
* 27-02-2014(Created by Diogo Monteiro)


Output: ESC button - 0 for off and 1 for on


*/

#include "Definitions.h"
#include "Package.h"
/*------------------------------------------------------------------
 * Keyboard Reading
 * Created by Diogo Monteiro
 *------------------------------------------------------------------
 */
int read_kb(int *key_map,char* c) {
int i;
//Handle Arrows and ESC		
		if(*c==27){
			*c=getchar();
			switch (*c){
				case -1:
					return 1;//ESC
					break;
				case 91:
					*c=getchar();//ARROWS
					break;
			}
		}

	switch (*c){
		case 48://SAFE MODE (key '0')
			*key_map= MODE_SAFE;
			break;
		case 49://PANIC MODE (key '1')
			*key_map= MODE_PANIC;
			break;
		case 50://MANUAL MODE (key '2')
			*key_map= MODE_MANUAL;
			break;
		case 51://CALIBRATION MODE (key '3')
			*key_map= MODE_CALIBRATION;
			break;
		case 52://YAW CONTROL MODE (key '4')
			*key_map= MODE_YAW_CONTROL;
			break;
		case 53:// FULL CONTROL MODE (key '5')
			*key_map= MODE_FULL_CONTROL;
			break;
		case 54:// P GAIN MODE (key '6')
			*key_map= MODE_P;
			break;
		case 126://Del
			*key_map= MODE_ABORT;
			break;
		case 'a'://LIFT UP
			*(key_map+1) = TrimToMaxLift(*(key_map+1) + 2);
			break;
		case 'z'://LIFT DOWN
			*(key_map+1) = TrimToMaxLift(*(key_map+1) - 2);;
			break;
		case 68://LEFT ARROW-ROLL DOWN
			*(key_map+2) = TrimToMaxByte(*(key_map+2) - 2);;
			break;
		case 67://RIGHT ARROW-ROLL UP
			*(key_map+2) = TrimToMaxByte(*(key_map+2) + 2);;
			break;
		case 65://UP ARROW-PITCH UP
			*(key_map+3) = TrimToMaxByte(*(key_map+3) + 2);;
			break;
		case 66://DOWN ARROW-PITCH DOWN
			*(key_map+3) = TrimToMaxByte(*(key_map+3) - 2);;
			break;
		case 'q'://YAW UP
			*(key_map+4) = TrimToMaxByte(*(key_map+4) + 2);;
			break;
		case 'w'://YAW DOWN
			*(key_map+4) = TrimToMaxByte(*(key_map+4) - 2);;
			break;
		case 'u'://YAW CONTROL P UP 
			*(key_map+5) = TrimToMaxByte(*(key_map+5) + 2);;
			break;
		case 'j'://YAW CONTROL P DOWN 
			*(key_map+5) = TrimToMaxByte(*(key_map+5) - 2);;
			break;
		case 'i':
			*(key_map+6) = TrimToMaxByte(*(key_map+6) + 2);;//ROLL/PITCH CONTROL P1 UP
			break;
		case 'k':
			*(key_map+6) = TrimToMaxByte(*(key_map+6) - 2);;//ROLL/PITCH CONTROL P1 DOWN
			break;
		case 'o':
			*(key_map+7) = TrimToMaxByte(*(key_map+7) + 2);;//ROLL/PITCH CONTROL P2 UP
			break;
		case 'l':
			*(key_map+7) = TrimToMaxByte(*(key_map+7) - 2);;//ROLL/PITCH CONTROL P2 DOWN
			break;
		case 'r':
			for(i=1;i<8;i++){		
				*(key_map+i)=0;//ROLL/PITCH CONTROL P2 DOWN 
			}
			break;
		default:
			//printf("\n [%i] CHARACTER NOT VALID\n",c);
			break;
	}
//	*c=-1;

return 0;


}
