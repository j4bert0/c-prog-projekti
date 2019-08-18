#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "reservation.h"

/*
	Utility functions
*/

int check_date(Date date)
{
	if ((0 < date.month && date.month <= 12)
		&& (0 < date.day && date.day <= 31))
		return 1;
	else
		return 0;
}

/*
	Calendar handling functions
*/

Appointment * init_calendar(void)
{
	Appointment *calendar = malloc(sizeof(Appointment));
	calendar->desc[0] = '\0';
	return calendar;
}

int count_calendar(Appointment *calendar)
{
	int count = 0;
	while (calendar->desc[0] != '\0') {
		count++;
		calendar++;
	}
	return count;
}

void print_calendar(Appointment *calendar)
{
	sort_calendar(calendar);
	if (calendar->desc[0] == '\0')
		printf("Kalenteri on tyhja.\n");
	else {
		while (calendar->desc[0] != '\0') {
			printf("%s %02d.%02d. klo %02d\n", 
				calendar->desc,
				calendar->date.day,
				calendar->date.month,
				calendar->time
				);
			calendar++;
		}
	}
}

void add_appointment(Appointment *calendar, char *desc, Date date, int time)
{
	if (is_free(calendar, date, time) && check_date(date)) {
		int count = count_calendar(calendar);
		calendar = realloc(calendar, (count+2)*sizeof(Appointment));
		calendar += count;

		strcpy(calendar->desc, desc);
		calendar->date = date;
		calendar->time = time;

		calendar++;
		calendar->desc[0] = '\0';
	
		printf("Tapaaminen lisatty. Nyt kalenterissa on yhteensa %d tapaamista.\n", count+1);
	} else if (!check_date(date)) {
		printf("Paivamaara oli vaarassa muodossa.\n");
	} else {
		printf("Lisays epaonnistui. Kyseinen ajankohta on varattu.\n");
	}
}

Appointment *delete_appointment(Appointment *calendar, Date date, int time)
{
	if (is_free(calendar, date, time)) {
		printf("Poistaminen epaonnistui. Valittu aika oli vapaa\n");
		return calendar;

	} else {
		int count = count_calendar(calendar);
		Appointment *new_calender = malloc(count*sizeof(Appointment));

		int i;
		int j = 0;
		for (i = 0; i < count; i++) {
			if (calendar[i].date.day == date.day &&
				calendar[i].date.month == date.month &&
				calendar[i].time == time) {
			} else {
				memcpy(&new_calender[j], &calendar[i], sizeof(Appointment));
				j++;
			}
		}
		calendar = realloc(new_calender, count*sizeof(Appointment));
		printf("Tapahtuman poistaminen onnistui.\n");

		free(calendar);
		return new_calender;
	}
}

int compare_appointments(const void *A, const void *B)
{
	const Appointment *a = A;
	const Appointment *b = B;

	if (a->desc[0] == '\0')
		return 1;
	else if (b->desc[0] == '\0')
		return -1;
	else {
		if (a->date.month < b->date.month)
			return -1;
		else if (a->date.month > b->date.month)
			return 1;
		else {
			if (a->date.day < b->date.day)
				return -1;
			else if (a->date.day > b->date.day)
				return 1;
			else {
				if (a->time < b->time)
					return -1;
				else if (a->time > b->time)
					return 1;
				else
					return 0;
			}
		}
	}
}

void sort_calendar(Appointment *calendar)
{
	size_t count = count_calendar(calendar) + 1; // +1 for the '\0' appointment
	qsort(calendar, count, sizeof(Appointment), compare_appointments);
}

int write_calendar(Appointment *calendar, char *filename)
{
	FILE *f;
	f = fopen(filename, "w");
	if (f == NULL)
		return 0;

	int count = count_calendar(calendar);
	int i;
	for (i = 0; i < count; i++) {
		fprintf(f, "%s %d %d %d\n",
			calendar[i].desc,
			calendar[i].date.month,
			calendar[i].date.day,
			calendar[i].time);
	}
	fclose(f);
	return 1;
}

Appointment *read_calendar(Appointment *calendar, char *filename)
{
	free_calendar(calendar); // HUOM!!

	FILE *f;
	f = fopen(filename, "r");

	if (f == NULL)
		return NULL;

	Appointment *new_calendar = init_calendar();
	
	char desc[21];
	Date date;
	int time;
	while (fscanf(f, "%s %d %d %d", desc, &date.month, &date.day, &time) == 4) {
		add_appointment(new_calendar, desc, date, time);
	}
	return new_calendar;
}

void free_calendar(Appointment *calendar)
{
	free(calendar);
}


int is_free(Appointment *calendar, Date date, int time)
{
	int count = count_calendar(calendar);
	int i;
	for (i = 0; i < count; i++) {
		if (calendar[i].date.day == date.day && 
			calendar[i].date.month == date.month && 
			calendar[i].time == time)
			return 0;
	}
	return 1;
}

void parse_input(Appointment *calendar, char *input)
{
	char action = *input;
	char tmp;
	int r, day, month, time;

	if (action == 'A') {
		char *desc = malloc(50);
		r = sscanf(input, "%c %s %d %d %d", &tmp, desc, &month, &day, &time);
		if (r < 5)
			printf("Tapahtuman luominen epaonnistui. Syote oli vaarassa muodossa.\n");
		else {
			Date date = {month, day};
			add_appointment(calendar, desc, date, time);
		}
	} else if (action == 'D') {
		r = sscanf(input, "%c %d %d %d", &tmp, &month, &day, &time);
		if (r < 4)
			printf("Tapahtuman poistaminen epaonnistui. Syote oli vaarassa muodossa.\n");
		else {
			Date date = {month, day};
			calendar = delete_appointment(calendar, date, time);
		}
	} else if (action == 'L') {
		print_calendar(calendar);

	} else if (action == 'W') {
		if (write_calendar(calendar, "varaukset.txt"))
			printf("Tiedoston kirjoittaminen onnistui.\n");
		else
			printf("Tiedoston kirjoittaminen epaonnistui.\n");
	
	} else if (action == 'O') {
		calendar = read_calendar(calendar, "varaukset.txt");
	} else {
		printf("Syotetta ei tunnistettu.\n");
	}
}

int main(void)
{
	char *input = malloc(400);
	int r = 1;

	Appointment *calendar = init_calendar();

	while (1) {
		fgets(input, 400, stdin);

		if (input[0] == 'Q') {
			free_calendar(calendar);
			printf("Ohjelman suoritus lopetetaan.\n");
			break;
		} else if (r == 0) {
			printf("Anna syote\n");
		} else {
			parse_input(calendar, input);
		}
	}

	return 1;
}






