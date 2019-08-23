/*
*	Reservation system project
*	--------------------------
*	- Few notes on the program structure:
*		- Single reservation : Appointment (struct)
*		- Stored in Array of Appointments (:= calendar)
*		- Last element of the calendar is marked by Appointment description (:= desc): '\0'
*		- Calendar handling functions are preferably of form:
*		  	- f(calendar) = new calendar
*
*	- Program (memory) fails when:
*	 	- Commandline input length > INPUT_SIZE-1
*		- Appointment desc. length > DESC_SIZE-1 (ie. in command: 'A desc 1 1 1')
*		- Filename length > FILENAME_SIZE-1 (ie. in commands 'W filename' & 'O filename')
*		- Action char (ie. 'A'/'D'...) is given as string (incorrectly) and the string length > DESC_SIZE-1
*
*	- Otherwise the program should run smoothly =D
*
*	- Jaakko Wallenius
*/

#define INPUT_SIZE 81 		// max length of commandline
#define DESC_SIZE 21 		// max length of Appointment description
#define FILENAME_SIZE 21 	// max length of filename

typedef struct 
{
	char desc[DESC_SIZE];
	int day;
	int month;
	int time;
} Appointment;

// Counts number whitespaces in a string
int count_whitespace(char *str);

// Check if given date is of valid form
int check_date(int day, int month, int time);

// Counts appoinments in calendar (exl. the last '\0' -Appointment)
int count_calendar(Appointment *calendar);

// Checks if time is free (in given calendar)
int is_free(Appointment *calendar, int day, int month, int time);

// Helper function for sort_calendar (qsort comparison func.)
int compare_appointments(const void *A, const void *B);

// Sorts the calendar (qsort implementation)
void sort_calendar(Appointment *calendar);

/*
*	FUNCTIONALITIES
*/

void print_calendar(Appointment *calendar);

Appointment *add_appointment(Appointment *calendar, char *desc, int day, int month, int time);

Appointment *delete_appointment(Appointment *calendar, int day, int month, int time);

// returns 1 if writing was succesfull
int write_calendar(Appointment *calendar, char *filename);

Appointment *read_calendar(Appointment *calendar, char *filename);

/*
*	INPUT HANDLING
*/

Appointment *parse_input(Appointment *calendar, char *input);
