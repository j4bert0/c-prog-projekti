#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "projekti.h"

int count_whitespace(char *str)
{
	int count = 0;
	while (*str != '\0') {
		if (*str == ' ')
			count += 1;
		str++;
	}
	return count;
}

int check_date(int day, int month, int time)
{
	if ((0 < month && month <= 12) && (0 < day && day <= 31) &&
		(0 <= time && time < 24))
		return 1;
	else
		return 0;
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

int is_free(Appointment *calendar, int day, int month, int time)
{
	int count = count_calendar(calendar);
	int i;
	for (i = 0; i < count; i++) {
		if (calendar[i].day == day && 
			calendar[i].month == month && 
			calendar[i].time == time)
			return 0;
	}
	return 1;
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
		if (a->month < b->month)
			return -1;
		else if (a->month > b->month)
			return 1;
		else {
			if (a->day < b->day)
				return -1;
			else if (a->day > b->day)
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
	size_t count = count_calendar(calendar) + 1; // '\0' appointment
	qsort(calendar, count, sizeof(Appointment), compare_appointments);
}

void print_calendar(Appointment *calendar)
{
	sort_calendar(calendar);
	if (calendar->desc[0] == '\0')
		printf("Kalenteri on tyhja.\n");
	else {
		while (calendar->desc[0] != '\0') {
			printf("%-20s %02d.%02d. klo %02d\n", 
				calendar->desc,
				calendar->day,
				calendar->month,
				calendar->time
				);
			calendar++;
		}
	}
}

Appointment *add_appointment(Appointment *calendar, char *desc, int day, int month, int time)
{
	if (is_free(calendar, day, month, time) && check_date(day, month, time)) {	
		int i = count_calendar(calendar);
		calendar = realloc(calendar, (i+2)*sizeof(Appointment));

		strcpy(calendar[i].desc, desc);
		calendar[i].day = day;
		calendar[i].month = month;
		calendar[i].time = time;

		calendar[i+1].desc[0] = '\0';
		printf("Tapaaminen lisatty. Nyt kalenterissa on yhteensa %d tapaamista.\n", i+1);

	} else if (!check_date(day, month, time))
		printf("Lisays epaonnistui. Annettu aika/paivamaara on vaarassa muodossa.\n");
	else
		printf("Lisays epaonnistui. Kyseinen ajankohta on varattu.\n");
	return calendar;
}

Appointment *delete_appointment(Appointment *calendar, int day, int month, int time)
{
	if (is_free(calendar, day, month, time)) {
		printf("Poistaminen epaonnistui. Valittu aika oli vapaa\n");
		return calendar;

	} else {
		int count = count_calendar(calendar);
		Appointment *new_calendar = malloc((count)*sizeof(Appointment));
		new_calendar[count-1].desc[0] = '\0';

		int i;
		int j = 0;
		for (i = 0; i < count; i++) {
			if (calendar[i].day == day &&
				calendar[i].month == month &&
				calendar[i].time == time) {
			} else {
				memcpy(&new_calendar[j], &calendar[i], sizeof(Appointment));
				j++;
			}
		}
		
		printf("Tapahtuman poistaminen onnistui.\n");
		free(calendar);
		return new_calendar;
	}
}

int write_calendar(Appointment *calendar, char *filename)
{
	FILE *f;
	f = fopen(filename, "w");
	if (f == NULL) {
		return 0;
	}

	int count = count_calendar(calendar);
	int i;
	for (i = 0; i < count; i++) {
		fprintf(f, "%s %d %d %d\n",
			calendar[i].desc,
			calendar[i].month,
			calendar[i].day,
			calendar[i].time);
	}
	fclose(f);
	return 1;
}

Appointment *read_calendar(Appointment *calendar, char *filename)
{
	FILE *f;
	f = fopen(filename, "r");

	// Handle non existent file
	if (f == NULL) {
		printf("Tiedostoa %s ei loytynyt.\n", filename);
		return calendar;
	}
	free(calendar);

	Appointment *new_calendar = malloc(sizeof(Appointment));
	new_calendar->desc[0] = '\0';
	
	char desc[21];
	int day, month, time;
	while (fscanf(f, "%s %d %d %d", desc, &month, &day, &time) == 4) {
		new_calendar = add_appointment(new_calendar, desc, day, month, time);
	}
	printf("Kalenteri luettiin tiedostosta.\n");
	fclose(f);
	return new_calendar;
}

Appointment *parse_input(Appointment *calendar, char *input)
{
	char action = *input;
	char c;
	int r, day, month, time;

	// Handle adding Appointment
	if (action == 'A') {
		char *desc = malloc(DESC_SIZE);
		r = sscanf(input, "%c %s %d %d %d", &c, desc, &month, &day, &time);
		if ((r != 5) | (count_whitespace(input) != 4))
			printf("Tapahtuman luominen epaonnistui. Syote oli vaarassa muodossa.\n");
		else {
			calendar = add_appointment(calendar, desc, day, month, time);
		}
		free(desc);
	
	// Handle deleting Appointment
	} else if (action == 'D') {
		r = sscanf(input, "%c %d %d %d", &c, &month, &day, &time);
		if ((r != 4) | (count_whitespace(input) != 3))
			printf("Tapahtuman poistaminen epaonnistui. Syote oli vaarassa muodossa.\n");
		else {
			calendar = delete_appointment(calendar, day, month, time);
		}

	// Handle printing calendar
	} else if (action == 'L') {
		if ((count_whitespace(input) > 0) | (strlen(input) > 2))
			printf("Printtaus komennon oikea muoto: 'L' (ilman valilyonteja).\n");
		else
			print_calendar(calendar);

	// Handle writing to file
	} else if (action == 'W') {
		char *filename = malloc(FILENAME_SIZE);
		r = sscanf(input, "%c %s", &c, filename);
		if ((r != 2) | (count_whitespace(input) != 1)) {
			printf("Syote 'W' tunnistettiin. Anna syote kuitenkin muodossa: 'W tiedostonimi'\n");
		} else {
			if (write_calendar(calendar, filename))
				printf("Tiedoston kirjoittaminen onnistui.\n");
			else
				printf("Tiedoston kirjoittaminen epaonnistui.\n");
		}
		free(filename);
	
	// Handle reading from file
	} else if (action == 'O') {
		char *filename = malloc(FILENAME_SIZE);
		r = sscanf(input, "%c %s", &c, filename);
		if ((r == 2) && (count_whitespace(input) == 1)) {
			calendar = read_calendar(calendar, filename);
		} else {
			printf("Syote 'O' tunnistettiin. Anna syote kuitenkin muodossa: 'O tiedostonimi'\n");
		}
		free(filename);
	
	// Handle unknown input	
	} else {
		printf("Syotetta ei tunnistettu.\n");
	}

	return calendar;
}

int main(void)
{
	char *input = malloc(INPUT_SIZE);
	int r = 1;

	Appointment *calendar = malloc(sizeof(Appointment));
	calendar->desc[0] = '\0';

	while (1) {
		
		// EOF check
		if (!fgets(input, INPUT_SIZE, stdin)) {
			free(calendar);
			free(input);
			printf("EOF. Ohjelman suoritus lopetetaan.\n");
			break;
		}

		if ((*input == 'Q') && (strlen(input) == 2)) {
			free(calendar);
			free(input);
			printf("Ohjelman suoritus lopetetaan.\n");
			break;
		} else if (r == 0) {
			printf("Anna syote\n");
		} else {
			calendar = parse_input(calendar, input);
		}
	}

	return 1;
}