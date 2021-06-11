#include "include.h"
#define CW 1
#define CCW 0

//
//void fill_scen_struct(frame this_frame)
//{
//
//		switch(this_frame.instruction)
//		{
//			case LINEAR_COLOR_CHANGE:
//			{
//				memcpy(&this_linear_process, this_frame.buffer, sizeof(linear_process));
//			}
//			default: break;
//		}
//}

leds get_led_state_from_frame(uint8_t scen[], uint16_t x, uint16_t y)
{
	leds this_led;
	uint16_t *format = (uint16_t *)scen; // � ������ �������� ����� � 16 ������ �������.
	switch (*format)
	{
	case ABSOLUTE_COLOR:
	{
		absolute_color_scen *this_scen = (absolute_color_scen *)scen; //
		return this_scen->led_state;
	}
	case INDIVIDUAL_X_COLOR:
	{
		individual_x_color_scen *this_scen = (individual_x_color_scen *)scen; //
		return this_scen->led_state[x];
	}
	default:
	{
		leds this_led = {0};
		return this_led;
	}
	}
	return this_led;
}
//======================================================================TEST!!!!!!!!!!!!!!!!!
/*
�������� ��������� ������������� � ������������ �� �������� � ���������� ���������� �� Y . ���������� �����������;
fade - ������� ����������� ���������� � ������� ��������. ����� ���� ������ ��� ����������� � �����
*/
uint8_t get_linear_progress_y_TEST(uint8_t crosfade, uint16_t SIZE_LED_STACK, uint16_t led_index, uint8_t cw, uint16_t fade)
{
	//float coef_crosfade = (float)crosfade/(255-(float)fade);

	//����� ������� � ������� ��������� �������
	uint16_t front_pixel = ((float)crosfade / 255) * (SIZE_LED_STACK + fade);
	if (cw)
	{
		uint16_t fade_component = (255 / (float)fade) * (front_pixel - (led_index));
		/*
		*/
		if (fade_component > 255)
			fade_component = 255;
		if (led_index > front_pixel)
			fade_component = 0;
		return fade_component;
	}
	else
	{
		uint16_t fade_component = (255 / (float)fade) * (front_pixel - (SIZE_LED_STACK - led_index));
		if (fade_component > 255)
			fade_component = 255;
		if ((SIZE_LED_STACK - led_index) > front_pixel)
			fade_component = 0; // ����� ���������� ����� ������������ ���������� ����� ������ ����� ����� - 0
		return fade_component;
	}
}
//================================================================================
void linear_color_change_handler(frame instruction, uint16_t x, uint16_t y, leds *this_led, uint8_t crosfade)
{
	linear_process *this_linear_process = ((linear_process *)FRAME.buffer);				 // ����� �� ������������� ����� ���������� ������ ���. ������ ������� ��������� ���������, ������� ��������� �� ������ ������
	linear_process *previous_linear_process = ((linear_process *)PREVIOUS_FRAME.buffer); // ����� �� ������������� ����� ���������� ������ ���. ������ ������� ��������� ���������, ������� ��������� �� ������ ������

	leds next_led_state = get_led_state_from_frame(this_linear_process->scen, x, y);
	leds previous_led_state = get_led_state_from_frame(previous_linear_process->scen, x, y);

	float led_state;
	if (this_linear_process->route > 255)
	{
		led_state = (float)get_linear_progress_y_TEST(crosfade, X_MATRIX_SIZE, x, this_linear_process->route, this_linear_process->fade) / 255; // ���������� ��������� ����������
	}
	else
	{
		led_state = (float)get_linear_progress_y_TEST(crosfade, Y_MATRIX_SIZE, y, this_linear_process->route, this_linear_process->fade) / 255; // ���������� ��������� ����������
	}

	this_led->red = previous_led_state.red + ((float)next_led_state.red - (float)previous_led_state.red) * (led_state);
	this_led->green = previous_led_state.green + ((float)next_led_state.green - (float)previous_led_state.green) * (led_state);
	this_led->blue = previous_led_state.blue + ((float)next_led_state.blue - (float)previous_led_state.blue) * (led_state);
}
//================================================================================
void handler_definition(frame this_frame, uint16_t x, uint16_t y, leds *this_led, uint8_t crosfade)
{
	switch (this_frame.instruction)
	{
	case LINEAR_COLOR_CHANGE:
		linear_color_change_handler(this_frame, x, y, this_led, crosfade); //����������� ��������� �������.
		break;

	default:
		break;
	}
}
