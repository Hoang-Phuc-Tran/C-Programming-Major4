/*
 * Project:	    Major 4 Assignment
 * By:	    	Hoang Phuc Tran
 * Student ID:  8789102
 * Date:		April 11, 2022
 * Description: a program will generate sport team win-loss records from game results in files.
 */



#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#pragma warning(disable: 4996)



#define MAXVALUE 100


 // Prototype
int processGame(char filename[]);
int parseLine(char gameResult[], char nameOfOpponent[], int* scorePrimaryTeam, int* scoreOpponentTeam);



int main()
{
	FILE* fp = NULL;
	int valueReturn = 0;
	char teams[MAXVALUE] = { 0 };

	// Open myTextFile.txt for reading
	fp = fopen("teams.txt", "r");

	// Check if any error during opening file 
	if (fp == NULL)
	{
		printf("Can't open teams.txt\n");
		return -1;
	}
	// If file is opened sucessfully
	else
	{
		// Use while loop to read the name of other files.
		while (fgets(teams, sizeof(teams), fp) != 0)
		{
			// Get red of the newline character
			teams[strcspn(teams, "\n")] = 0;

			valueReturn = processGame(teams);
		}
	}

	// Check closing the file.
	if (fclose(fp) != 0)
	{
		// If it fails to close file, displaying error message.
		printf("Error closing input file\n");
		return -2;
	}

	return 0;
}

/*
* Function: processGame()
* Description:  this function will display total all wins, losses, and ties for that team and then
				display a final winning percentage
* Parameters: an array of charaters.
* Returns: int: 0 if the parsing of the game result work perfectly, negative number otherwise
*/
int processGame(char filename[])
{
	FILE* fp = NULL;
	char* stringPrimaryTeam = NULL;
	char gameResult[MAXVALUE] = { 0 };
	char nameOfOpponent[MAXVALUE] = { 0 };
	char nameOfPrimary[MAXVALUE] = { 0 };
	int scorePrimaryTeam = 0;
	int scoreOpponentTeam = 0;
	int returnValue = 2;

	// Open filename contains the game result string
	fp = fopen(filename, "r");

	//Check if any error during opening file
	if (fp == NULL)
	{
		printf("Can't open %s - Skipping team\n\n", filename);
		return -2;
	}
	else
	{
		int win = 0;
		int lose = 0;
		int tie = 0;
		float result = 0;
		size_t length = 0;
		size_t lengthStringPrimary = 0;
		size_t stringFinal = 0;

		// Get rid of .txt 
		if (strchr(filename, '.') == NULL)
		{
			// if filename does not contain '.', copy all filename to nameOfPrimary
			strcpy(nameOfPrimary, filename);
		}
		else
		{
			// Calculate the length of filename
			length = strlen(filename);

			stringPrimaryTeam = strrchr(filename, '.');

			// Take the length of the string
			lengthStringPrimary = strlen(stringPrimaryTeam);

			// Calculate the length to find location of the string name of the primary team
			stringFinal = length - lengthStringPrimary;

			//Copy the name to the string called nameOfPrimary
			strncpy(nameOfPrimary, filename, stringFinal);

		}

		printf("Processing %s\n", filename);

		// Loop the game result string
		while (fgets(gameResult, sizeof gameResult, fp) != 0)
		{
			gameResult[strcspn(gameResult, "\n")] = 0;

			// Call ParseLine function
			returnValue = parseLine(gameResult, nameOfOpponent, &scorePrimaryTeam, &scoreOpponentTeam);
			// Check if the string is parsed ok or not
			if (returnValue == -4)
			{
				printf("Warning: malformed line. Skipping game.\n");
			}
			else
			{
				// the primary team wins
				if (scorePrimaryTeam > scoreOpponentTeam)
				{
					printf("\t%s beat %s %d-%d\n", nameOfPrimary, nameOfOpponent, scorePrimaryTeam, scoreOpponentTeam);
					win++;
				}
				// the primary team loses
				else if (scorePrimaryTeam < scoreOpponentTeam)
				{
					printf("\t%s lost to %s %d-%d\n", nameOfPrimary, nameOfOpponent, scoreOpponentTeam, scorePrimaryTeam);
					lose++;
				}
				// // the primary team ties
				else
				{
					printf("\t%s and %s tied at %d\n", nameOfPrimary, nameOfOpponent, scoreOpponentTeam);
					tie++;
				}
			}

			// Check errors 
			if (ferror(fp))
			{
				printf("Error reading line from file");
				// If any errors occur, close the file
				if (fclose(fp) != 0)
				{
					printf("Error closing the file\n");
					return -5;
				}
			}
		}
		// Calculate the results and displays
		result = ((2 * win) + tie) / (float)(2 * (win + lose + tie));
		printf("Season result for %s: %.3f (%d-%d-%d)\n\n", nameOfPrimary, result, win, lose, tie);
	}
	return 0;
}

/*
* Function: parseLine()
* Description: This function will parse the array that contains the game result string, then store the name
of opponent, the score of the primary team, and the score of the opponent team to other parameters.
* Parameters:  four parameters - the array containing the game result string - an array that
you will fill in with the opponent's name - a pointer to the score for the primary team - a pointer
to the score for the opponent team.
* Returns: int: -4 if the parsing of the game result failed, 0 otherwise
*/
int parseLine(char gameResult[], char nameOfOpponent[], int* scorePrimaryTeam, int* scoreOpponentTeam)
{
	char name[MAXVALUE] = { 0 };
	char stringResult[MAXVALUE] = { 0 };
	size_t total = 0;
	size_t lengthOfOpponent = 0;
	size_t lengthIndex = 0;
	size_t lengthScore = 0;
	char* stringName = NULL;
	char* stringContainScoreOpponent = NULL;

	memset(nameOfOpponent, '\0', sizeof(nameOfOpponent));
	// Copy the game result to an array called stringResult
	strcpy(stringResult, gameResult);


	// Check if the string is not contain '-' or ',' then return -4
	if (strchr(stringResult, '-') == NULL || strchr(stringResult, ',') == NULL)
	{
		return -4;
	}
	else
	{

		total = strlen(stringResult);
		// Locate last occurrence of ',' character in string
		stringName = strrchr(stringResult, ',');
		
		// Take the length of the string
		lengthIndex = strlen(stringName);

		// Calculate the length to find location of the string name of the opponent team
		lengthOfOpponent = total - lengthIndex;

		//Copy the name to the string called name
		strncpy(nameOfOpponent, stringResult, lengthOfOpponent);

		// Take the score of the primary team
		*scorePrimaryTeam = atoi(++stringName);

		// Find the location the score of the opponent team
		stringContainScoreOpponent = strrchr(stringResult, '-');

		// Convert the string to number
		*scoreOpponentTeam = atoi(++stringContainScoreOpponent);

	}

	return 0;
}