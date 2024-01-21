#include <REGX52.H>
#include "Delay.h"
#include "QXa51.h"

unsigned int scanIndividualKey(){
	unsigned int IndividualKeyType=0;
	
	if(key_s2==0){DelayMs(20);while(key_s2==0);DelayMs(20); IndividualKeyType=2;}
	if(key_s3==0){DelayMs(20);while(key_s3==0);DelayMs(20); IndividualKeyType=3;}
	if(key_s4==0){DelayMs(20);while(key_s4==0);DelayMs(20); IndividualKeyType=4;}
	if(key_s5==0){DelayMs(20);while(key_s5==0);DelayMs(20); IndividualKeyType=5;}
	
	return IndividualKeyType;
}
