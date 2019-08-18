#include <stdio.h>
#include "reservation.h"
#include "reservation.c"

int main(void)
{
	/*
	Appointment *calendar = init_calendar();
	print_calendar(calendar);
	*/
	char *desc = "Barber";
	Date date1 = {1, 1};
	Date date2 = {5, 5};
	Date date3 = {5, 5};
	Date date4 = {1, 31};
	Date date5 = {2, 2};
	Date date6 = {12, 24};


	/*
	add_appointment(calendar, desc, date2, 21);
	add_appointment(calendar, desc, date3, 20);
	add_appointment(calendar, desc, date4, 2);

	print_calendar(calendar);

	calendar = delete_appointment(calendar, date4, 2);

	print_calendar(calendar);

	if (write_calendar(calendar, "testi.txt"))
		printf("Tiedoston kirjoittaminen onnistui.\n");
	else
		printf("Tiedoston kirjoittaminen epaonnistui.\n");
	*/

	Appointment *calendar = read_calendar(NULL, "testi.txt");

	print_calendar(calendar);

	add_appointment(calendar, desc, date5, 8);
	add_appointment(calendar, desc, date1, 7);
	add_appointment(calendar, desc, date1, 7);
	delete_appointment(calendar, date6, 1);

	print_calendar(calendar);

	/*	
	char *test_input1 = "A Hiustenleikkuu 3 26 14";
	char *test_input2 = "D 3 26 14";
	char *test_input3 = "P kalenteri";

	parse_input(test_input1);
	parse_input(test_input2);
	parse_input(test_input3);
	*/

	return 1;
}