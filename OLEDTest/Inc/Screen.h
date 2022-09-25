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
	static uint8_t m_curr;
	static uint8_t m_start;
	void DrawMenu();
};
class CInfoScreen: public CScreen{
public:
	virtual ~CInfoScreen();
	virtual CScreen * ProcessKey(uint16_t keys);
	virtual void Init();
	virtual void Update(uint8_t bNew);
};
class CStartBeginScreen: public CScreen{
public:
	virtual ~CStartBeginScreen();
	virtual CScreen * ProcessKey(uint16_t keys);
	virtual void Init();
	virtual void Update(uint8_t bNew);
private:
	uint16_t m_StopTemp;
	uint8_t bFalse;
	uint8_t m_count;
};
class CWorkScreen: public CScreen{
public:
	virtual ~CWorkScreen();
	virtual CScreen * ProcessKey(uint16_t keys);
	virtual void Init();
	virtual void Update(uint8_t bNew);
};


#endif /* SCREEN_H_ */
