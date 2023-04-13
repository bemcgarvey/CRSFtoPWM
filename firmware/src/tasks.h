/////////////////////////////////////////////////////
// Project: CRSFtoPWM                              //
// File: tasks.h                                   //
// Target: ATSAMD21E18A                            // 
// Compiler: XC32                                  //
// Author: Brad McGarvey                           //
// License: GNU General Public License v3.0        //
// Description: task creation                      //
/////////////////////////////////////////////////////

#ifndef TASKS_H
#define	TASKS_H

#ifdef	__cplusplus
extern "C" {
#endif

extern bool wdtResetHappened; 
void initTasks(void);


#ifdef	__cplusplus
}
#endif

#endif	/* TASKS_H */

