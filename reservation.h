#include <stdio.h>

typedef struct
{
	int month;
	int day;
} Date;

typedef struct 
{
	char desc[21];
	Date date;
	int time;
} Appointment;


Appointment * init_calendar(void);

void print_calendar(Appointment *calendar);

void add_appointment(Appointment *calendar, char *desc, Date date, int time);

Appointment *delete_appointment(Appointment *calendar, Date date, int time);

int write_calendar(Appointment *calendar, char *filename);

Appointment *read_calendar(Appointment *calendar, char *filename);

void free_calendar(Appointment *calendar);

void sort_calendar(Appointment *calendar);

int compare_appointments(const void *a, const void *b);


/*
	Utility functions
*/

void parse_input(Appointment *calendar, char *input);

// returns 1 if date is appropriate
int check_date(Date date);

// Return the number of appointments in calendar (exl. end of calendar)
int count_calendar(Appointment *calendar);

// Returns 1 if time is free
int is_free(Appointment *calendar, Date date, int time);


