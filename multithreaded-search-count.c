/*
	200201056 - Tolgahan Tunç

	I could not write the right code that you want.
	The problem is on search part.
	My code searching just lowercase.

	I think, other parts run well as you want.

	Also you should compile this c file using this:
	gcc filename.c -o filename -lpthread
	./filename (thread number)
*/

#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stddef.h>

char *randomString = NULL; //to generate random strings.
char *keepRandomString = NULL; //to get together all character as a string.

char *fileName = "big.txt"; //name of our txt file.

char subStr1[] = "x";	//one of substrings to search.
char subStr2[] = "os";	//one of substrings to search.
char subStr3[] = "cpu";	//one of substrings to search.
char subStr4[] = "disk";	//one of substrings to search.
char subStr5[] = "cache";	//one of substrings to search.

int threadCount = 0;

int searchCount = 0; //to control search number.

typedef struct
{
	int start;
	int end;
	int threadNo;
}THREAD_PARAMETERS;

void *runner(void *param);

int main(int argc, char* argv[])
{
	keepRandomString = malloc(sizeof(char) * (2)); //default malloc settings for this variable.

	threadCount = atoi(argv[1]);
	pthread_t tid[threadCount];
	pthread_attr_t attr[threadCount];

	THREAD_PARAMETERS* lpParameter;


	lpParameter = malloc(sizeof(THREAD_PARAMETERS)* threadCount);

 	int  i=0;

	for(i=0; i<threadCount; i++)
	{
		lpParameter[i].start = 0;
		lpParameter[i].end = 100; //number of letters.
		lpParameter[i].threadNo = i + 1;

		pthread_attr_init(&attr[i]);
		pthread_create(&tid[i],&attr[i],runner,&lpParameter[i]);
		pthread_join(tid[i],NULL);

	}

	return 1;
}

//this function generates a random letter in size length.
char *randstring(size_t length) {

	/*
		these three lines written for adding the created letter to whole string.
	*/
	char *tempRandomString = NULL;
	tempRandomString = malloc(sizeof(char) * ((int) strlen(keepRandomString)) + 1);
	strcpy(tempRandomString, keepRandomString);

    static char alphabet[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
    if (length) {
        randomString = malloc(sizeof(char) * (length +1));

        if (randomString) {
            for (int n = 0;n < length;n++) {
                int key = rand() % (int)(sizeof(alphabet) -1);
                randomString[n] = alphabet[key];
            }
            randomString[length] = '\0';
        }
    }

	/*
		these three lines written for adding the created letter to whole string
		also these are in relation with above three lines.
	*/
		tempRandomString[strlen(tempRandomString)] = randomString[0];
		keepRandomString = malloc(sizeof(char) * ((int) strlen(tempRandomString)) + 1);
		strcpy(keepRandomString, tempRandomString);

		return randomString;
}

/*
	this function make search in file which name considired top of the codes
	via fileName variable.
*/
int findSubString(char* word)
{
	FILE *fp;
	int count = 0;
	int ch, len;

	if(NULL == ( fp = fopen(fileName, "r")))
		return -1; //this means error occured while opening file.

	len = strlen(word);

	for(;;)
	{
		int i;

		if(EOF == (ch = fgetc(fp)))
			break;
		if((char)ch != *word)
			continue;

		for(i = 1;i < len; ++i)
		{
			if(EOF == (ch = fgetc(fp)))
				goto end;
			if((char)ch != word[i])
			{
				fseek(fp, 1-i, SEEK_CUR);
				goto next;
			}
		}
		++count;
		next: ;
	}
	end:
		fclose(fp);
		return count;
}

void *runner(void *parameters)
{
	THREAD_PARAMETERS* param = (THREAD_PARAMETERS*)parameters;
	int i = 0;


	FILE *f = fopen(fileName, "w");
	if (f == NULL)
	{
			printf("Error opening file!\n");
			exit(1);
	}

	for (i = param->start; i < param->end; i++)
	{
		randstring(1);
		fprintf(f, "%s", randomString);
	}

	fclose(f);

/*
	this if print the result of the search one time.
*/
	if (searchCount == 0)
	{
		searchCount = 1;

		int totalCount = 0;

		totalCount = findSubString(subStr1);
		printf("'%s' finds %d times in the big.txt file.\n",subStr1,totalCount);
		totalCount = findSubString(subStr2);
		printf("'%s' finds %d times in the big.txt file.\n",subStr2,totalCount);
		totalCount = findSubString(subStr3);
		printf("'%s' finds %d times in the big.txt file.\n",subStr3,totalCount);
		totalCount = findSubString(subStr4);
		printf("'%s' finds %d times in the big.txt file.\n",subStr4,totalCount);
		totalCount = findSubString(subStr5);
		printf("'%s' finds %d times in the big.txt file.\n",subStr5,totalCount);
	}


	pthread_exit(0);
}
