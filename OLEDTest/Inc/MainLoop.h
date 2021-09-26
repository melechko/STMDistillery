/*
 * MainLoop.h
 *
 *  Created on: Sep 26, 2021
 *      Author: user
 */

#ifndef MAINLOOP_H_
#define MAINLOOP_H_

/* C++ detection */
#ifdef __cplusplus
extern "C" {
#endif
extern volatile uint32_t timer1;
void MainLoopInit();
void MainLoopRun();
/* C++ detection */
#ifdef __cplusplus
}
#endif

#endif /* MAINLOOP_H_ */
