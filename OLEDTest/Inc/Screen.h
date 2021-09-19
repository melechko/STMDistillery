/*
 * Screen.h
 *
 *  Created on: Sep 19, 2021
 *      Author: user
 */

#ifndef SCREEN_H_
#define SCREEN_H_
#include <stdint.h>

class CScreen{
public:
	virtual ~CScreen(){};
	virtual CScreen * ProcessKey(uint16_t keys)=0;
	virtual void Init()=0;
	virtual void Update()=0;
};

class CStartScreen: public CScreen {
public:
	virtual ~CStartScreen();
	virtual CScreen * ProcessKey(uint16_t keys);
	virtual void Init();
	virtual void Update();
private:
	uint8_t m_count;
};




#endif /* SCREEN_H_ */
