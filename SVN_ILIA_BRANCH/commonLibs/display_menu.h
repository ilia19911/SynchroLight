#ifndef __display_menu_H
#define __display_menu_H

#include "include.h"

enum
{
	main_page,
	rf_bind_page
};

enum
{
null,
button_1,
button_2,
button_3,
button_4
};

void menu_handler(void);
void menu_init(void);

#endif
	
	
