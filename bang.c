/*
    Name: William Bolduc
Student#: 0851313

fileName: bang.c
	This program loads in collision information from a file (normally NCDB_1999_to_2012.csv)
	and allows you to some basic queries about the data

	This program uses the PILOT library in order to provide parallelism

	There are no other files
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pilot.h"

#define RECORDWIDTH 63
#define HEADERSIZE 147

// UU
#define UNKNOWN -1

// XX
#define RESTRICTED -2

// NN
#define NOTAPPLICABLE -3

//QQ
#define UNLISTED -4

int maximumpeoplecount = 0;
int maximumcarcount = 0;

//C_YEAR, C_MNTH, C_WDAY, C_HOUR, C_SEV, C_VEHS, C_CONF, C_RCFG, C_WTHR, C_RSUR, C_RALN, C_TRAF, V_ID, V_TYPE, V_YEAR, P_ID, P_SEX, P_AGE, P_PSN, P_ISEV, P_SAFE, P_USER
//	1999,     01,      1,     20,     2,     02,     34,     UU,      1,      5,      3,     03,   01,     06,   1990,   01,     M,    41,    11,      1,     UU,      1
// 1999,01,1,20,2,02,34,UU,1,5,3,03,01,06,1990,01,M,41,11,1,UU,1

typedef struct PERSON{
	int P_ID;
	char P_SEX;
	int P_AGE;
	int P_POS;
	int P_TREATMENTREQ;
	int P_SAFTEYDEVICE;
	int P_USERCLASS;
}PERSON;

typedef struct VEHICLE{
	int V_ID;
	int V_TYPE;
	int V_MODELYEAR;

	int PERSONcount;

	PERSON *PERSONlist;
}VEHICLE;

typedef struct COLLISION{
	int C_YEAR;
	int C_MONTH;
	int C_WEEKDAY;
	int C_HOUR;

	int C_SEVERITY;
	int C_VEHICLECOUNT;
	int C_COLLISIONCONFIG;
	int C_ROADWAYCONFIG;
	int C_WEATHER;
	int C_ROADSURFACE;
	int C_ROADALIGNMENT;
	int C_TRAFFICCONTROL;

	int totalPERSONcount;
	int totalVEHICLEcount;

	VEHICLE *VEHICLElist;
	PERSON 	**totalPERSONlist;
}COLLISION;

typedef struct RECORD{
	int C_YEAR;
	int C_MONTH;
	int C_WEEKDAY;
	int C_HOUR;
	int C_SEVERITY;
	int C_VEHICLECOUNT;
	int C_COLLISIONCONFIG;
	int C_ROADWAYCONFIG;
	int C_WEATHER;
	int C_ROADSURFACE;
	int C_ROADALIGNMENT;
	int C_TRAFFICCONTROL;

	int V_ID;
	int V_TYPE;
	int V_MODELYEAR;

	int P_ID;
	char P_SEX;
	int P_AGE;
	int P_POS;
	int P_TREATMENTREQ;
	int P_SAFTEYDEVICE;
	int P_USERCLASS;
}RECORD;


int W;								//number of workers
PI_PROCESS **Worker;				//array of process pointers
PI_CHANNEL **toWorker;				//array of channel pointers to Workers
PI_CHANNEL **result;				//array of channel pointers from Workers
PI_BUNDLE *toAllWorkers, *allResults;

void printRecord(RECORD record)
{
	printf("%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %c, %d, %d, %d, %d, %d\n", record.C_YEAR, record.C_MONTH, record.C_WEEKDAY, record.C_HOUR, record.C_SEVERITY, record.C_VEHICLECOUNT, record.C_COLLISIONCONFIG, record.C_ROADWAYCONFIG, record.C_WEATHER, record.C_ROADSURFACE, record.C_ROADALIGNMENT, record.C_TRAFFICCONTROL, record.V_ID, record.V_TYPE, record.V_MODELYEAR, record.P_ID, record.P_SEX, record.P_AGE, record.P_POS, record.P_TREATMENTREQ, record.P_SAFTEYDEVICE, record.P_USERCLASS);
}

int parse1Code(char *token)
{
	if (token[0] == 'U')
	{
		return UNKNOWN;
	}
	else if (token[0] == 'X')
	{
		return RESTRICTED;
	}
	else if (token[0] == 'N')
	{
		return NOTAPPLICABLE;
	}
	else if (token[0] == 'Q')
	{
		return UNLISTED;
	}
	else
	{
		return token[0] - 48;
	}
}

int parse2Code(char *token)			//TODO: COULD BE SWITCH
{
	if (token[0] == 'U')
	{
		return UNKNOWN;
	}
	else if (token[0] == 'X')
	{
		return RESTRICTED;
	}
	else if (token[0] == 'N')
	{
		return NOTAPPLICABLE;
	}
	else if (token[0] == 'Q')
	{
		return UNLISTED;
	}
	else
	{
		return atoi(token);
	}
}

RECORD readRecord(FILE *recordBlock)
{
	char line[RECORDWIDTH];
	char *token;
	RECORD record;

	if(fread(line, sizeof(char), RECORDWIDTH, recordBlock) < RECORDWIDTH)//read one line from the recordBlock
	{
		record.C_YEAR = 0;
		return record;
	}

	record.C_YEAR 				= parse2Code(strtok(line,","));
	record.C_MONTH 				= parse2Code(strtok(NULL, ","));
	record.C_WEEKDAY 			= parse1Code(strtok(NULL, ","));
	record.C_HOUR 				= parse2Code(strtok(NULL, ","));
	record.C_SEVERITY 			= parse1Code(strtok(NULL, ","));
	record.C_VEHICLECOUNT 		= parse2Code(strtok(NULL, ","));
	record.C_COLLISIONCONFIG 	= parse2Code(strtok(NULL, ","));
	record.C_ROADWAYCONFIG 		= parse2Code(strtok(NULL, ","));
	record.C_WEATHER 			= parse1Code(strtok(NULL, ","));
	record.C_ROADSURFACE		= parse1Code(strtok(NULL, ","));
	record.C_ROADALIGNMENT 		= parse1Code(strtok(NULL, ","));
	record.C_TRAFFICCONTROL 	= parse2Code(strtok(NULL, ","));

	record.V_ID 				= parse2Code(strtok(NULL, ","));
	record.V_TYPE 				= parse2Code(strtok(NULL, ","));
	record.V_MODELYEAR 			= parse2Code(strtok(NULL, ","));

	record.P_ID 				= parse2Code(strtok(NULL, ","));
	record.P_SEX 				= strtok(NULL, ",")[0];
	record.P_AGE 				= parse2Code(strtok(NULL, ","));
	record.P_POS 				= parse2Code(strtok(NULL, ","));
	record.P_TREATMENTREQ 		= parse1Code(strtok(NULL, ","));
	record.P_SAFTEYDEVICE 		= parse2Code(strtok(NULL, ","));
	record.P_USERCLASS 			= parse1Code(strtok(NULL, ","));

	return record;
}

int recordsAreFromSameCollision(RECORD record1, RECORD record2)
{
	//Any of the C_ fields have changed;
	if (record1.C_YEAR != record2.C_YEAR ||
		record1.C_MONTH != record2.C_MONTH ||
		record1.C_WEEKDAY != record2.C_WEEKDAY ||
		record1.C_HOUR != record2.C_HOUR ||
		record1.C_VEHICLECOUNT != record2.C_VEHICLECOUNT ||
		record1.C_COLLISIONCONFIG != record2.C_COLLISIONCONFIG ||
		record1.C_ROADWAYCONFIG != record2.C_ROADWAYCONFIG ||
		record1.C_WEATHER != record2.C_WEATHER ||
		record1.C_ROADSURFACE != record2.C_ROADSURFACE ||
		record1.C_ROADALIGNMENT != record2.C_ROADALIGNMENT ||
		record1.C_TRAFFICCONTROL != record2.C_TRAFFICCONTROL)
	{
		return 0;
	}

	//The C_ fields are the same, but V_ID changed to 1;
	if (record1.V_ID != 1 && record2.V_ID == 1)
	{
		return 0;
	}

	//The C_ fields are the same and V_ID stayed as 1, but V_TYPE or V_YEAR changed
	if (record1.V_ID == record2.V_ID  && (record1.V_TYPE != record2.V_TYPE || record1.V_MODELYEAR != record2.V_MODELYEAR))
	{
		return 0;
	}

	return 1;
}


int shiftToNextCollision(FILE *workBlock)	/*I don't know what happens if the work chunks are too small or if the whole work block is the same collision*/
{//shifts the file pointer to the beginning of the following record
	RECORD r1, r2;//TODO: fix posibility of EOF errors
	int recordsSkipped = 1;

	//move through records until you arrive to a pair of records that are not from the same collision
	r1 = readRecord(workBlock);
	r2 = readRecord(workBlock);

	while(recordsAreFromSameCollision(r1, r2))
	{
		r1 = r2;
		r2 = readRecord(workBlock);
		recordsSkipped++;
	}

	fseek(workBlock, -RECORDWIDTH, SEEK_CUR);
	return recordsSkipped - 1;
}

COLLISION *loadRecordBlock(FILE *workBlock, int workBlockSize, int *size, int shift) //size is the size of the colList
{//shift is there to tell loadRecordBlock if it must shift
	int pos = 0;
	int recordsInCollision;
	int carsInCollision;
	int i,j,k;
	int lastCarid;
	int currentCollision = 0;

	COLLISION *colList;

	RECORD collisionRecords[128];
	int carIndex[128];

	RECORD r1,r2;

	if(!(colList = malloc(sizeof(COLLISION)*workBlockSize)))
	{
		printf("No Space for the collision list\n");
		return NULL;
	}

	//move past the first collision if needed
	if(shift != 0)
	{
		pos = shiftToNextCollision(workBlock);
	}
	r1 = readRecord(workBlock);

	while(pos < workBlockSize && r1.C_YEAR != 0)
	{
		//load in the first records data
		colList[currentCollision].C_YEAR			= r1.C_YEAR;
		colList[currentCollision].C_MONTH			= r1.C_MONTH;
		colList[currentCollision].C_WEEKDAY			= r1.C_WEEKDAY;
		colList[currentCollision].C_HOUR			= r1.C_HOUR;
		colList[currentCollision].C_SEVERITY		= r1.C_SEVERITY;
		colList[currentCollision].C_VEHICLECOUNT	= r1.C_VEHICLECOUNT;
		colList[currentCollision].C_COLLISIONCONFIG	= r1.C_COLLISIONCONFIG;
		colList[currentCollision].C_ROADWAYCONFIG   = r1.C_ROADWAYCONFIG;
		colList[currentCollision].C_WEATHER			= r1.C_WEATHER;
		colList[currentCollision].C_ROADSURFACE		= r1.C_ROADSURFACE;
		colList[currentCollision].C_ROADALIGNMENT	= r1.C_ROADALIGNMENT;
		colList[currentCollision].C_TRAFFICCONTROL	= r1.C_TRAFFICCONTROL;

		//load records which are from the same collision into collisionRecords
		recordsInCollision = 0;
		do
		{
			collisionRecords[recordsInCollision] = r1;
			recordsInCollision++;
			pos++;
			r2 = r1;
			r1 = readRecord(workBlock);
			if (r1.C_YEAR == 0) //end of records exit loop
			{
				break;
			}
		}while(recordsAreFromSameCollision(r2, r1));

		//No more records pertaining to that collision
		carsInCollision = 0;
		lastCarid = 0;
		j = 0;
		for (i = 0; i < recordsInCollision; i++)
		{
			if (lastCarid != collisionRecords[i].V_ID)
			{
				carsInCollision++;

				if (collisionRecords[i].V_ID != 99) //vehicle is not a group of pedestrians
				{
					maximumcarcount++;
				}

				lastCarid = collisionRecords[i].V_ID;
				carIndex[j++] = i;
			}
		}
		carIndex[j] = recordsInCollision;

////////////////////////////////////////////////////////////

		colList[currentCollision].totalVEHICLEcount = carsInCollision;
		if(!(colList[currentCollision].VEHICLElist = malloc(sizeof(VEHICLE) * carsInCollision)))
		{
			printf("Ran out of space for cars\n");
			return colList;
		}
		if(!(colList[currentCollision].totalPERSONlist = malloc(sizeof(PERSON*) * carsInCollision)))
		{
			printf("Ran out of space for totalPERSONlist\n");
			return colList;
		}

		colList[currentCollision].totalPERSONcount = 0;

		//add cars
		for(i = 0; i < carsInCollision; i++)
		{
			//add car data
			colList[currentCollision].VEHICLElist[i].V_ID        = collisionRecords[carIndex[i]].V_ID;
			colList[currentCollision].VEHICLElist[i].V_TYPE      = collisionRecords[carIndex[i]].V_TYPE;
			colList[currentCollision].VEHICLElist[i].V_MODELYEAR = collisionRecords[carIndex[i]].V_MODELYEAR;

			//make space for the people in the car
			if(!(colList[currentCollision].VEHICLElist[i].PERSONlist = malloc(sizeof(PERSON)*(carIndex[i+1]-carIndex[i]))))
			{
				printf("No more space for people\n");
				return colList;
			}

			colList[currentCollision].VEHICLElist[i].PERSONcount = carIndex[i+1]-carIndex[i];
			for(j=carIndex[i], k = 0; j<carIndex[i+1]; k++, j++)
			{
				if(collisionRecords[j].P_ID != UNKNOWN && collisionRecords[j].P_ID != NOTAPPLICABLE)
				{
					colList[currentCollision].VEHICLElist[i].PERSONlist[k].P_ID 			= collisionRecords[j].P_ID;
					colList[currentCollision].VEHICLElist[i].PERSONlist[k].P_SEX			= collisionRecords[j].P_SEX;
					colList[currentCollision].VEHICLElist[i].PERSONlist[k].P_AGE 			= collisionRecords[j].P_AGE;
					colList[currentCollision].VEHICLElist[i].PERSONlist[k].P_POS 			= collisionRecords[j].P_POS;
					colList[currentCollision].VEHICLElist[i].PERSONlist[k].P_TREATMENTREQ 	= collisionRecords[j].P_TREATMENTREQ;
					colList[currentCollision].VEHICLElist[i].PERSONlist[k].P_SAFTEYDEVICE	= collisionRecords[j].P_SAFTEYDEVICE;
					colList[currentCollision].VEHICLElist[i].PERSONlist[k].P_USERCLASS		= collisionRecords[j].P_USERCLASS;

					//colList[currentCollision].totalPERSONlist[colList[currentCollision].totalPERSONcount] = &(colList[currentCollision].VEHICLElist[i].PERSONlist[k]);
					(colList[currentCollision].totalPERSONcount)++;
				}
			}
		}
		//maximumpeoplecount += colList[currentCollision].totalPERSONcount;
		currentCollision++;
	}

	if (currentCollision > 0)
	{
		colList = realloc(colList,sizeof(COLLISION) * currentCollision);
	}
	else
	{
		colList = NULL;
	}
	*size = currentCollision;
	return colList;
}

void printCollisionList(COLLISION *colList, int size, int index)
{
	int i,j,k;

	for (i = 0; i < size; i++)
	{
		printf("%d^Year       = %d\n", index, colList[i].C_YEAR);
		printf("%d^Month      = %d\n", index, colList[i].C_MONTH);
		printf("%d^Weekday    = %d\n", index, colList[i].C_WEEKDAY);
		printf("%d^Hour       = %d\n", index, colList[i].C_HOUR);

		printf("%d^Severity   = %d\n", index, colList[i].C_SEVERITY);
		printf("%d^Vehicle #  = %d\n", index, colList[i].C_VEHICLECOUNT);
		printf("%d^Config     = %d\n", index, colList[i].C_COLLISIONCONFIG);
		printf("%d^RoadConfig = %d\n", index, colList[i].C_ROADWAYCONFIG);
		printf("%d^Weather    = %d\n", index, colList[i].C_WEATHER);
		printf("%d^R Surface  = %d\n", index, colList[i].C_ROADSURFACE);
		printf("%d^R Align    = %d\n", index, colList[i].C_ROADALIGNMENT);
		printf("%d^Traffic C  = %d\n", index, colList[i].C_TRAFFICCONTROL);

		printf("%d^PERSONcount  = %d\n", index, colList[i].totalPERSONcount);
		printf("%d^VEHICLEcount = %d\n", index, colList[i].totalVEHICLEcount);

		for (j = 0; j < colList[i].totalVEHICLEcount; j++)
		{
			printf("%d^    |V_ID        = %d\n", index, colList[i].VEHICLElist[j].V_ID);
			printf("%d^    |V_TYPE      = %d\n", index, colList[i].VEHICLElist[j].V_TYPE);
			printf("%d^    |V_MODELYEAR = %d\n", index, colList[i].VEHICLElist[j].V_MODELYEAR);
			printf("%d^    |peopleCount = %d\n", index, colList[i].VEHICLElist[j].PERSONcount);

			for (k = 0; k < colList[i].VEHICLElist[j].PERSONcount; k++)
			{
				printf("%d^    |    |P_ID           = %d\n", index, colList[i].VEHICLElist[j].PERSONlist[k].P_ID);
				printf("%d^    |    |P_SEX          = %c\n", index, colList[i].VEHICLElist[j].PERSONlist[k].P_SEX);
				printf("%d^    |    |P_AGE          = %d\n", index, colList[i].VEHICLElist[j].PERSONlist[k].P_AGE);
				printf("%d^    |    |P_POS          = %d\n", index, colList[i].VEHICLElist[j].PERSONlist[k].P_POS);
				printf("%d^    |    |P_TREATMENTREQ = %d\n", index, colList[i].VEHICLElist[j].PERSONlist[k].P_TREATMENTREQ);
				printf("%d^    |    |P_SAFTEYDEVICE = %d\n", index, colList[i].VEHICLElist[j].PERSONlist[k].P_SAFTEYDEVICE);
				printf("%d^    |    |P_USERCLASS    = %d\n", index, colList[i].VEHICLElist[j].PERSONlist[k].P_USERCLASS);
				printf("%d^    |    -----------------------\n", index);
			}
			printf("%d^    -----------------------------\n", index);
		}
		printf("%d^+++++++++++++++++++++++++++++++++++++++++++++++++\n", index);
	}
}


void freeCollisionList(COLLISION *colList, int size)
{//TODO: maybe write more scalable code whydontcha
	int i,j;
	for (i = 0; i < size; i++)
	{
		for (j = 0; j < colList[i].totalVEHICLEcount; j++)
		{
			free(colList[i].VEHICLElist[j].PERSONlist);
		}
		free(colList[i].VEHICLElist);
		free(colList[i].totalPERSONlist);
	}
	free(colList);
}

void tallyMW(COLLISION *colList, int size, int* Km, int* Kw)
{
	int i,j,k;

	for (i=0;i<size;i++)
	{
		for (j = 0; j < colList[i].totalVEHICLEcount; j++)
		{
			for (k = 0; k < colList[i].VEHICLElist[j].PERSONcount; k++)
			{
				if (colList[i].VEHICLElist[j].PERSONlist[k].P_TREATMENTREQ == 3)
				{
					if(colList[i].VEHICLElist[j].PERSONlist[k].P_SEX == 'M')
					{
						(*Km)++;
					}
					else if (colList[i].VEHICLElist[j].PERSONlist[k].P_SEX == 'F')
					{
						(*Kw)++;
					}
				}
			}
		}
	}
}



void tallyCollisionLocations(COLLISION *colList, int size, int *collisionLocation)
{
	int i;
	for (i = 0; i < size; i++)
	{
		if (colList[i].C_ROADWAYCONFIG > 0)
		{
			collisionLocation[colList[i].C_ROADWAYCONFIG]++;
		}
		else if (colList[i].C_ROADWAYCONFIG == UNKNOWN)
		{
			collisionLocation[0]++;
		}
	}
}

int findBiggestCollision(COLLISION *colList, int size)
{
	int i, mostVehicles=0, index = 0;

	for (i = 0; i < size; i++)
	{
		if (colList[i].totalVEHICLEcount > mostVehicles)
		{
			mostVehicles = colList[i].totalVEHICLEcount;
			index = i;
		}
	}

	return ((colList[index].totalVEHICLEcount * 10000000) + (colList[index].C_YEAR * 1000) + (colList[index].C_MONTH * 10) + colList[index].C_WEEKDAY);//ccYYYYMMD
}

void averageAges(COLLISION *colList, int size,int *wrecks, int *ageSum, int *carCount)
{
	int i,j;

	for(i = 0; i < size; i++)
	{
		for (j = 0; j < colList[i].totalVEHICLEcount; j++)
		{
			if (colList[i].VEHICLElist[j].V_ID != 99 && colList[i].VEHICLElist[j].V_MODELYEAR > 0) // if not a pedestrian grouping and not undefined modelYear
			{
				if (colList[i].VEHICLElist[j].V_MODELYEAR >= colList[i].C_YEAR)
				{
					(*wrecks)++;
				}
				//printf("age = %d\n", colList[i].C_YEAR - colList[i].VEHICLElist[j].V_MODELYEAR + 1);
				(*ageSum) += colList[i].C_YEAR - colList[i].VEHICLElist[j].V_MODELYEAR + 1;
				(*carCount)++;
			}
		}
	}
}

void printMonths(int badMonths[14][12][2])
{
	int i,j;
	printf("           Jan       Feb       Mar       Apr       May       Jun       Jul       Aug       Sep       Oct       Nov       Dec\n");
	for (i = 0; i < 14; i++)
	{
		printf("Coll  ");
		for (j = 0; j < 11; j++)
		{
			printf("%8d, ", badMonths[i][j][0]);
		}
		printf("%8d\nFatal ", badMonths[i][11][0]);

		for (j = 0; j < 11; j++)
		{
			printf("%8d, ", badMonths[i][j][1]);
		}
		printf("%8d\n\n", badMonths[i][11][1]);
	}
}

void tallyBadMonths(COLLISION *colList, int size, int badMonths[14][12][2])
{
	int i, j, k;
	int yearIndex;
	int monthIndex;


	for (i = 0; i < size; i++)
	{
		if (colList[i].C_MONTH <= 0)	//Month unavailible, skipping month
		{
			continue;
		}

		yearIndex  = colList[i].C_YEAR - 1999; //assumes the data starts at 1999
		monthIndex = colList[i].C_MONTH - 1;

		//increment collisons for the month
		badMonths[yearIndex][monthIndex][0]++;

		for (j = 0; j < colList[i].totalVEHICLEcount; j++)
		{
			for (k = 0; k < colList[i].VEHICLElist[j].PERSONcount; k++)
			{
				if (colList[i].VEHICLElist[j].PERSONlist[k].P_TREATMENTREQ == 3)
				{
					badMonths[yearIndex][monthIndex][1]++;	//increment fatalities for the month
				}
			}
		}
	}
}

int workerFunc(int index, void* arg2)
{
	int workBlockStart, workBlockSize, collisionCount,i,j,Km,Kw;
	char* fileName;
	char query;
	FILE *dataFile;
	COLLISION *colList;

	int collisionLocation[13];
	int worstMonths[14][12][2];
	int wrecks, ageSum, carCount;

	PI_Read( toWorker[index], "%d %d", &workBlockStart, &workBlockSize);


	//printf("I am worker %d, I get records %d to %d\n", index, workBlockStart, workBlockStart + workBlockSize);

	//open file
	dataFile = fopen(arg2, "r");
	fseek(dataFile, (workBlockStart * RECORDWIDTH) + HEADERSIZE, SEEK_SET);

	//load collisions
	colList = loadRecordBlock(dataFile, workBlockSize, &collisionCount, index);
	//return results of the load
	PI_Write(result[index], "%d %d", collisionCount, maximumcarcount);


	//begin solving queries
	PI_Read(toWorker[index], "%c", &query);
	while(query != '0')
	{
		switch (query) {
			case '1':	//what is the worst month of the year for collisions
				for (i = 0; i < 14; i++)
				{
					for (j = 0; j < 12; j++)
					{
						worstMonths[i][j][0] = 0;
						worstMonths[i][j][1] = 0;
					}
				}
				tallyBadMonths(colList,collisionCount,worstMonths);
				PI_Write(result[index], "%*d", 336, worstMonths);
				break;

			case '2':	//who is more likely to die M/F
				Km = 0;
				Kw = 0;
				tallyMW(colList, collisionCount, &Km, &Kw);
				PI_Write(result[index],"%d %d", Km, Kw);
				break;

			case '3':	//What was the most number of vehicles ever involved in a single collision during the 14 years, and when did it take place?
				PI_Write(result[index],"%d", findBiggestCollision(colList, collisionCount));
				break;

			case '4':	//In a year on average, how many people typically wreck their new car (defined as vehicle year >= collision year), and how old was the average vehicle in a collision over the 14 years?
				wrecks = 0;
				ageSum = 0;
				carCount = 0;
				averageAges(colList,collisionCount,&wrecks, &ageSum, &carCount);
				PI_Write(result[index], "%d %d %d", wrecks, ageSum, carCount);
				break;

			case '5':	//what is the most likely place to have a collision
				for (i = 0; i<13; i++)
				{
					collisionLocation[i]=0;
				}	//set collisionLocations to 0
				tallyCollisionLocations(colList, collisionCount, collisionLocation);

				PI_Write(result[index],"%*d", 13, collisionLocation);
				break;
		}
		PI_Read(toWorker[index], "%c", &query);
	}


	freeCollisionList(colList, collisionCount);
	fclose(dataFile);
	return 0;
}

void Pquery1(COLLISION *colList, int size)
{
	int i, j,k;
	int worstMonths[14][12][2];
	int badMonths[14][12][2];
	int finalMonths[12][2];

	int worstCollisions=0;
	int worstFatalities=0;
	int wcIndex=0;
	int wfIndex=0;

	//initialize values
	for (i = 0; i < 12; i++)
	{
		for (j = 0; j < 14; j++)
		{
			worstMonths[j][i][0] = 0;
			worstMonths[j][i][1] = 0;
		}
		finalMonths[i][0] = 0;
		finalMonths[i][1] = 0;
	}

	if (W > 0)
	{
		//signal query
		for(i=0; i<W; i++)
		{
			PI_Write(toWorker[i],"%c",'1');
		}

		//combine results
		for (i = 0; i<W; i++)
		{
			j = PI_Select(allResults);
			PI_Read(result[j], "%*d", 336, &badMonths);

			//convert single array to 3d
			for (j = 0; j < 14; j++)
			{
				for (k = 0; k < 12; k++)
				{
					worstMonths[j][k][0] += badMonths[j][k][0];
					worstMonths[j][k][1] += badMonths[j][k][1];
				}
			}
		}
	}
	else
	{
		tallyBadMonths(colList, size, worstMonths);
	}


	//print worst for each year
	for (i = 0; i<14; i++)
	{
		worstCollisions = 0;
		worstFatalities = 0;
		for (j=0;j<12;j++)
		{
			if(worstMonths[i][j][0] > worstCollisions)
			{
				worstCollisions = worstMonths[i][j][0];
				wcIndex = j;
			}
			if(worstMonths[i][j][1] > worstFatalities)
			{
				worstFatalities = worstMonths[i][j][1];
				wfIndex = j;
			}

			//total to find worst month ever
			finalMonths[j][0] += worstMonths[i][j][0];
			finalMonths[j][1] += worstMonths[i][j][1];
		}
		printf("$Q1,%d,%d,%d\n", 1999+i, wcIndex+1, wfIndex+1);
	}

	//print worst month of all time
	worstCollisions = 0;
	worstFatalities = 0;
	for (i = 0; i < 12; i++)
	{
		if(finalMonths[i][0] > worstCollisions)
		{
			worstCollisions = finalMonths[i][0];
			wcIndex = i;
		}
		if(finalMonths[i][1] > worstFatalities)
		{
			worstFatalities = finalMonths[i][1];
			wfIndex = i;
		}
	}
	printf("$Q1,9999,%d,%d\n", wcIndex+1, wfIndex+1);
}

void Pquery2(COLLISION *colList, int size)
{
	int i, j, Km = 0, Kw= 0, menK, womK;
	float totalPeople;

	if (W > 0)
	{
		//send out query
		for(i=0; i<W; i++)
		{
			PI_Write(toWorker[i],"%c",'2');
		}

		//combine results
		for (i = 0; i<W; i++)
		{
			j = PI_Select(allResults);
			PI_Read(result[j], "%d %d", &menK, &womK);
			Km += menK;
			Kw += womK;
		}
	}
	else
	{
		//one core, do it solo
		tallyMW(colList, size, &Km, &Kw);
	}

	totalPeople = Kw + Km;
	printf("$Q2,%d,%d,%.2f,%.2f\n",Km,Kw,(float)Km/totalPeople,(float)Kw/totalPeople);
}

void Pquery3(COLLISION *colList, int size)
{
	int i, j, carCount, yyyy, mm, d, bigCollision, biggestCollision = 0;
	//send out query
	if (W > 0)
	{
		for(i=0; i<W; i++)
		{
			PI_Write(toWorker[i],"%c",'3');
		}

		//combine results
		for (i = 0; i<W; i++)
		{
			j = PI_Select(allResults);
			PI_Read(result[j], "%d", &bigCollision);

			if (biggestCollision < bigCollision)
			{
				biggestCollision = bigCollision;
			}
		}
	}
	else
	{
		//one core, do it solo
		biggestCollision = findBiggestCollision(colList, size);
	}

	d = biggestCollision%10;
	biggestCollision/=10;
	mm = biggestCollision%100;
	biggestCollision/=100;
	yyyy = biggestCollision%10000;
	carCount = biggestCollision/10000;

	printf("$Q3,%d,%d,%d,%d\n",carCount,yyyy,mm,d);
}

void Pquery4(COLLISION *colList, int size)
{
	int i, j, wreckedNewCars = 0, combinedWreckAge = 0, totalCars = 0;
	int wrecks, ageSum, carCount;
	//send out query

	if (W > 0)
	{

		for(i=0; i<W; i++)
		{
			PI_Write(toWorker[i],"%c",'4');
		}

		//combine results
		for (i = 0; i<W; i++)
		{
			j = PI_Select(allResults);
			PI_Read(result[j], "%d %d %d", &wrecks, &ageSum, &carCount);

			wreckedNewCars += wrecks;
			combinedWreckAge += ageSum;
			totalCars += carCount;
			//printf("totalCars %d\n",totalCars);
		}
	}
	else //single process
	{
		averageAges(colList, size, &wreckedNewCars, &combinedWreckAge, &totalCars);
	}
	//printf("maximumcarcount %d\ntotalCars %d\nwreckedNewCars %d\ncombinedWreckAge %d\n", maximumcarcount, totalCars, wreckedNewCars, combinedWreckAge);
	printf("$Q4,%.1f,%.1f\n",(float)wreckedNewCars/14, (float)combinedWreckAge/totalCars);//sketchy is the dataset gets bigger than 14 years
}

void Pquery5(COLLISION *colList, int size)
{
	int i,j;
	int mostLikely=0;
	int mostCollisions = 0;

	int collisionLocation[13];
	int totalCollisionLocation[13];

	for (i = 0; i<13; i++){totalCollisionLocation[i]=0;}	//set totalCollisionLocations to 0

	if (W > 0)
	{
		for(i=0; i<W; i++)
		{
			PI_Write(toWorker[i],"%c",'5');
		}

		//combine results
		for (i = 0; i<W; i++)
		{
			j = PI_Select(allResults);
			PI_Read(result[j], "%*d", 13, collisionLocation);

			for (j = 0; j<13; j++)
			{
				totalCollisionLocation[j] += collisionLocation[j];
			}
		}
	}
	else
	{
		//do it solo
		tallyCollisionLocations(colList,size, totalCollisionLocation);
	}

	//find most likely
	for (i = 0; i<13; i++)
	{
		if (totalCollisionLocation[i] > mostCollisions)
		{
			mostLikely = i;
			mostCollisions = totalCollisionLocation[i];
		}
	}

	//output result
	printf("$Q5,%d",mostLikely);
	for (i = 1; i<13; i++)
	{
		printf(",%d",totalCollisionLocation[i]);
	}
	printf(",%d\n",totalCollisionLocation[0]);

}


int main(int argc, char *argv[])
{
	int i, j, R, workBlockSize, collisionCount = 0, colSubTotal, carSubTotal; //R for Record Count
	long byteCount;
	char query;

	//Timing report variables
	double initTime, queryTime;
	double queryTotalTime[5] = {0,0,0,0,0};
	int queryFreq[5] = {0,0,0,0,0};

	COLLISION *colList = NULL;

	FILE *dataFile;


	// Pilot configuration phase; return no. of processes available
	W = PI_Configure(&argc, &argv) - 1;
	PI_StartTime();

	// Create worker lists
	Worker   = malloc(sizeof(PI_PROCESS *) * W);
	toWorker = malloc(sizeof(PI_CHANNEL *) * W);
	result   = malloc(sizeof(PI_CHANNEL *) * W);

	// create Worker processes (with arg i) and to/from channels
	for (i = 0; i<W; i++)
	{
		Worker[i]   = PI_CreateProcess(workerFunc, i, argv[1]);
		toWorker[i] = PI_CreateChannel(PI_MAIN, Worker[i]);
		result[i]   = PI_CreateChannel(Worker[i], PI_MAIN);
	}

	// form "toworker" channels into bundle for broadcasting
	//toAllWorkers = PI_CreateBundle(PI_BROADCAST, toWorker, W);

	// form "result" channels into bundle for selecting
	if (W > 0)
	{
		allResults = PI_CreateBundle(PI_SELECT, result, W);
	}

	PI_StartAll();
	printf("#Number of workers = %d\n", W);
	//Get the input filename from argv[1] and try to open it.
	if(argv[1] == NULL)
	{
		printf("No data file given\n");
		goto EXIT;
	}
	if(!(dataFile = fopen(argv[1], "r")))
	{
		printf("Could not open file %s\n", argv[1]);
		goto EXIT;
	}

	//Find the amount of records
	fseek(dataFile, 0, SEEK_END);
	byteCount = ftell(dataFile);
	R = (byteCount/(long)sizeof(char) - HEADERSIZE) / RECORDWIDTH;
	printf("#the count of records is %d\n",R);


	//R=10; 	//NOTE: BIGASS DEBUG STATEMENT


	//divide work
	if(W == 0)
	{
		//load the collision List
		printf("master is the only core\n");
		//move to begining and load
		fseek(dataFile, HEADERSIZE, SEEK_SET);
		colList = loadRecordBlock(dataFile, R, &collisionCount, 0);
	}
	else
	{
		//divy up work
		workBlockSize = R / W;
		PI_Write(toWorker[0], "%d %d", 0, workBlockSize + (R % W) + 1);

		for (i = 1; i < W; i++)
		{
			PI_Write(toWorker[i], "%d %d", (workBlockSize*i + (R % W)), workBlockSize + 1);
		}


		//get totalCollisions and number of cars
		for (i = 0; i<W; i++)
		{
			j = PI_Select(allResults);
			PI_Read(result[j], "%d %d", &colSubTotal, &carSubTotal);
			collisionCount += colSubTotal;
			maximumcarcount += carSubTotal;
		}
	}
	initTime = PI_EndTime();
	printf("$T0,%d,%.1lf\n", W + 1, initTime);		//timing
	fclose(dataFile);
	printf("#collisionCount = %d\n", collisionCount);

	//printCollisionList(colList, collisionCount, 0);

	//loop through queries
	for(i = 2; argv[i]; i++)
	{
		query = argv[i][0];

		switch (query) {
			case '1':	//what is the worst month of the year for collisions
				queryTime = PI_EndTime();
				Pquery1(colList, collisionCount);
				queryTime = PI_EndTime() - queryTime;

				(queryFreq[0])++;
				(queryTotalTime[0])+= queryTime;

				printf("$T1,%d,%.1lf\n",W+1, queryTime);
				break;

			case '2':	//who is more likely to die M/F
				queryTime = PI_EndTime();
				Pquery2(colList, collisionCount);
				queryTime = PI_EndTime() - queryTime;

				(queryFreq[1])++;
				(queryTotalTime[1])+= queryTime;

				printf("$T2,%d,%.1lf\n",W+1, queryTime);
				break;


			case '3':	//What was the most number of vehicles ever involved in a single collision during the 14 years, and when did it take place?
				queryTime = PI_EndTime();
				Pquery3(colList, collisionCount);
				queryTime = PI_EndTime() - queryTime;

				(queryFreq[2])++;
				(queryTotalTime[2])+= queryTime;

				printf("$T3,%d,%.1lf\n",W+1, queryTime);
				break;

			case '4':	//In a year on average, how many people typically wreck their new car (defined as vehicle year >= collision year), and how old was the average vehicle in a collision over the 14 years?
				queryTime = PI_EndTime();
				Pquery4(colList, collisionCount);
				queryTime = PI_EndTime() - queryTime;

				(queryFreq[3])++;
				(queryTotalTime[3])+= queryTime;

				printf("$T4,%d,%.1lf\n",W+1, queryTime);
				break;

			case '5':	//what is the most likely place to have a collision
				queryTime = PI_EndTime();
				Pquery5(colList, collisionCount);
				queryTime = PI_EndTime() - queryTime;

				(queryFreq[4])++;
				(queryTotalTime[4])+= queryTime;
				printf("$T5,%d,%.1lf\n",W+1, queryTime);
				break;

			default:
				printf("invalid query \"%s\"\n",argv[i]);
				break;
		}
	}

	//release workers
	for(i=0; i<W; i++)
	{
		PI_Write(toWorker[i],"%c",'0');
	}

	printf("$T9,%d,%.1lf\n",W+1, PI_EndTime() - initTime);
	PI_StopMain(0);		// end program

	printf("# totalQueryTimes = %lf, %lf, %lf, %lf, %lf\n",queryTotalTime[0], queryTotalTime[1], queryTotalTime[2], queryTotalTime[3], queryTotalTime[4]);
	printf("#      queryFreqs = %8d, %8d, %8d, %8d, %8d\n", queryFreq[0], queryFreq[1], queryFreq[2], queryFreq[3], queryFreq[4]);
	printf("#  avg query time = %lf, %lf, %lf, %lf, %lf\n", queryTotalTime[0]/queryFreq[0], queryTotalTime[1]/queryFreq[1], queryTotalTime[2]/queryFreq[2], queryTotalTime[3]/queryFreq[3], queryTotalTime[4]/queryFreq[4]);

EXIT:	// Free channels
	free(Worker);
	free(toWorker);
	return 0;
}
