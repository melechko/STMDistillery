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
	virtual void Update(uint8_t bNew)=0;
protected:
	void DisplayLedTEMP();
};

class CStartScreen: public CScreen {
public:
	virtual ~CStartScreen();
	virtual CScreen * ProcessKey(uint16_t keys);
	virtual void Init();
	virtual void Update(uint8_t bNew);
private:
	uint8_t m_count;
};
class CMenuScreen: public CScreen{
public:
	virtual ~CMenuScreen();
	virtual CScreen * ProcessKey(uint16_t keys);
	virtual void Init();
	virtual void Update(uint8_t bNew);
private:
	uint8_t m_curr;
	uint8_t m_start;
	void DrawMenu();
};
class CInfoScreen: public CScreen{

};



#endif /* SCREEN_H_ */
