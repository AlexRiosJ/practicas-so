struct DATE {
	int year;
	int month;
	int day;
	int hour;
	int min;
	int sec;
};

unsigned int datetoint(struct DATE date);
int inttodate(struct DATE *date,unsigned int val);
unsigned int currdatetimetoint();
