#ifndef __PRIMASK_H_
#define __PRIMASK_H_

#define __sti()  __set_PRIMASK(0)
#define __cli()  __set_PRIMASK(1)
#define __save_flags(f)    do{f = __get_PRIMASK();}while(0)
#define __restore_flags(f) do{__set_PRIMASK(f);}while(0)
#define __save_and_cli(f)  do{f = __get_PRIMASK();__cli();}while(0)

#define save_and_cli(f)	        __save_and_cli(f)
#define restore_flags(f)	__restore_flags(f)

#define _HI(io)			(io##_PORT->BSRR =  io##_PIN)
#define _LO(io)			(io##_PORT->BRR = io##_PIN)
#define _IS_HI(io)		((io##_PORT->IDR & io##_PIN) != 0)
#define _IS_LO(io)		((io##_PORT->IDR & io##_PIN) == 0)

typedef unsigned short		hal_flags;

#endif