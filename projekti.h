#define FILENAME "reservations.txt" // IO file
#define INPUT_SIZE 81 // max length of command
#define DESC_SIZE 21 // max length of Appointment desc

typedef struct 
{
	char desc[21];
	int day;
	int month;
	int time;
} Appointment;


// Check if given date is of valid form
int check_date(int day, int month, int time);

// Counts appoinments in calendar
int count_calendar(Appointment *calendar);

// Checks if time is free (in given calendar)
int is_free(Appointment *calendar, int day, int month, int time);

// Helper function for sort_calendar
int compare_appointments(const void *A, const void *B);

// Sorts the calendar (qsort implementation)
void sort_calendar(Appointment *calendar);

/*
*	FUNCTIONALITIES
*/

void print_calendar(Appointment *calendar);

Appointment *add_appointment(Appointment *calendar, char *desc, int day, int month, int time);

Appointment *delete_appointment(Appointment *calendar, int day, int month, int time);

int write_calendar(Appointment *calendar, char *filename);

Appointment *read_calendar(Appointment *calendar, char *filename);

/*
*	INPUT HANDLING
*/

Appointment *parse_input(Appointment *calendar, char *input);
