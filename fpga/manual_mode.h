

#define ENGINE_LIMIT 800
#define LIFT_CONVERSION ENGINE_LIMIT/254
#define ROLLPITCHYAW_CONVERSION ENGINE_LIMIT/127
void manual_mode() {
int i;
//INITIALIZATION
ae[1]=0;ae[2]=0;ae[3]=0;ae[4]=0;

//ROLL
	if((char) roll>=0) {
		ae[4]=((char) roll)*ROLLPITCHYAW_CONVERSION;
	}
	else {
		ae[2]=-((char) roll)*ROLLPITCHYAW_CONVERSION;
	}

//PITCH
if((char) pitch>=0) {
		ae[1]=((char) pitch)*ROLLPITCHYAW_CONVERSION;
	}
	else {
		ae[3]=-((char) pitch)*ROLLPITCHYAW_CONVERSION;
	}

//YAW
if((char) yaw>=0) {
	ae[1]+=((char) yaw)*ROLLPITCHYAW_CONVERSION;
	ae[3]+=((char) yaw)*ROLLPITCHYAW_CONVERSION;
}
else {
	ae[2]+=-((char) yaw)*ROLLPITCHYAW_CONVERSION;
	ae[4]+=-((char) yaw)*ROLLPITCHYAW_CONVERSION;
}
//LIFT
for(i=0; i<=4;i++) {

	ae[i]+=((char)lift+127)*LIFT_CONVERSION;
	if(ae[i]>ENGINE_LIMIT) {
		ae[i]=ENGINE_LIMIT;
	}

}
}

