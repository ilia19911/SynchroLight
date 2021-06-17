#include "include.h"

uint8_t my_ledmap[X_MATRIX_SIZE][Y_MATRIX_SIZE];

//================================================================================
void  	led_maping_init_leds(leds *this_leds)
{
		my_map.my_leds = this_leds;
}
//================================================================================
void led_maping_set_map()
{
	led_maping_temp_matrix_set();	// временная усановка матрицы, позже сделать чтение из файла
	for(uint16_t i = 0; i < X_MATRIX_SIZE; i++)
	{
		memcpy(my_map.matrix[i],my_ledmap[i],Y_MATRIX_SIZE);
	}
}
//================================================================================
uint8_t led_map_set_pixel(uint8_t x, uint8_t y, leds this_leds)
{
	uint16_t led_number = my_map.matrix[x][y];
	my_map.my_leds[led_number].red 	 =	this_leds.red  * RED_LED_POW;
	my_map.my_leds[led_number].green =	this_leds.green   * GREEN_LED_POW;
	my_map.my_leds[led_number].blue  =	this_leds.blue * BLUE_LED_POW;
	return 0;
}
//================================================================================
leds get_led_state(uint8_t x, uint8_t y)
{
	uint16_t led_number = my_map.matrix[x][y];
	
	return my_map.my_leds[led_number];
}
//================================================================================

//инициализация массива с данными о матрице светодиодов .ТЕСТОВАЯ ФУНКЦИЯ, позже избавиться
void led_maping_temp_matrix_set()
{
			my_ledmap[0][0]  = 	 0;
			my_ledmap[0][1]  = 	 1;
			my_ledmap[0][2]  = 	 2;
			my_ledmap[0][3]  = 	 3;
			my_ledmap[0][4]  = 	 4;
			my_ledmap[0][5]  = 	 5;
			my_ledmap[0][6]  = 	 6;
			my_ledmap[0][7]  = 	 7;
			my_ledmap[0][8]  = 	 8;
			my_ledmap[0][9]  = 	 9;
	
			my_ledmap[0][10] = 	10;
			my_ledmap[0][11] = 	11;
			my_ledmap[0][12] = 	12;
			my_ledmap[0][13] = 	13;
			my_ledmap[0][14] = 	14;
			my_ledmap[0][15] = 	15;
			my_ledmap[0][16] = 	16;
			my_ledmap[0][17] = 	17;
			my_ledmap[0][18] = 	18;
			my_ledmap[0][19] = 	19;
		
			my_ledmap[0][20] = 	20;
			my_ledmap[0][21] = 	21;
			my_ledmap[0][22] = 	22;
			my_ledmap[0][23] = 	23;
			my_ledmap[0][24] = 	24;
			my_ledmap[0][25] = 	25;
			my_ledmap[0][26] = 	26;
			my_ledmap[0][27] = 	27;
			my_ledmap[0][28] = 	28;
			my_ledmap[0][29] = 	29;
	
			my_ledmap[0][30] = 	30;
			my_ledmap[0][31] = 	31;
			my_ledmap[0][32] = 	32;
			my_ledmap[0][33] = 	33;
			my_ledmap[0][34] = 	34;
		
		
		
		
		
			my_ledmap[1][0] =  	35;
			my_ledmap[1][1] =  	36;
			my_ledmap[1][2] =  	37;
			my_ledmap[1][3] =  	38;
			my_ledmap[1][4] =  	39;
			my_ledmap[1][5] =  	40; 
			my_ledmap[1][6] =  	41;
			my_ledmap[1][7] =  	42;
			my_ledmap[1][8] =  	43;
			my_ledmap[1][9] =  	44;
	                               
			my_ledmap[1][10] =  45;
			my_ledmap[1][11] =  46;
			my_ledmap[1][12] =  47;
			my_ledmap[1][13] =  48;
			my_ledmap[1][14] =  49;
			my_ledmap[1][15] =  50;
			my_ledmap[1][16] =  51;
			my_ledmap[1][17] =  52;
			my_ledmap[1][18] =  53;
			my_ledmap[1][19] =  54;
	                            
			my_ledmap[1][20] =  55; 
			my_ledmap[1][21] =  56; 
			my_ledmap[1][22] =  57;  
			my_ledmap[1][23] =  58; 
			my_ledmap[1][24] =  59; 
			my_ledmap[1][25] =  60; 
			my_ledmap[1][26] =  61; 
			my_ledmap[1][27] =  62; 
			my_ledmap[1][28] =  63; 
			my_ledmap[1][29] =  64; 
                                
			my_ledmap[1][30] =  65; 
			my_ledmap[1][31] =  66;
			my_ledmap[1][32] =  67;
			my_ledmap[1][33] =  68;
			my_ledmap[1][34] =  69;
	
			                                                  
          
			 my_ledmap[2][0] =  70;
			 my_ledmap[2][1] =  71;
			 my_ledmap[2][2] =  72;
			 my_ledmap[2][3] =  73;
			 my_ledmap[2][4] =  74;
			 my_ledmap[2][5] =  75;
			 my_ledmap[2][6] =  76;
			 my_ledmap[2][7] =  77;
			 my_ledmap[2][8] =  78;
			 my_ledmap[2][9] =  79;
	                            
			my_ledmap[2][10] =  80;
			my_ledmap[2][11] =  81;
			my_ledmap[2][12] =  82;
			my_ledmap[2][13] =  83;
			my_ledmap[2][14] =  84;
			my_ledmap[2][15] =  85;
			my_ledmap[2][16] =  86;
			my_ledmap[2][17] =  87;
			my_ledmap[2][18] =  88;
			my_ledmap[2][19] =  89;  
	                            
			my_ledmap[2][20] =  90;	
			my_ledmap[2][21] =  91;
			my_ledmap[2][22] =  92;
			my_ledmap[2][23] =  93;
			my_ledmap[2][24] =  94;
			my_ledmap[2][25] =  95;
			my_ledmap[2][26] =  96;
			my_ledmap[2][27] =  97;
			my_ledmap[2][28] =  98;
			my_ledmap[2][29] =  99;
                                
			my_ledmap[2][30] =  100;
			my_ledmap[2][31] =  101;
			my_ledmap[2][32] =  102;
			my_ledmap[2][33] =  103;
			my_ledmap[2][34] =  104;
		                               
		}
