/* 
* PSoC.h
*
* Created: 30-10-2020 11:37:40
* Author: HansK
*/


#ifndef __PSoC_H__
#define __PSoC_H__
#include <stdint.h>

class PSoC
{
public:
	PSoC();
	void PSoCHandler(uint8_t& timervalMSBRef, uint8_t& timervalLSBRef, bool& playerDoneRef, uint8_t& sendToPlayerRef, uint8_t& rxDataRef, bool& newDataReadyRef);
private:
	void initPSoC();
	void initiateAddress();
	uint8_t txCount_, PSoCstatus_, TXbyte3_;
}; //PSoC

#endif //__PSoC_H__
