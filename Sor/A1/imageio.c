/************************************  Header Section  *********************************************
 --
 -- NAME       : Wang Zhibin
 -- Data       : June, 2012
 -- E-mail     : zhibinwang@yahoo.cn
 -- Address    : Department of Electrical Engineering, Jiangnan University

 -- Filename   : imageio.c
 -- Description: Read the pgm format image 
			     Save the pgm format image  
 -- 
*********************************** End Header Section  *********************************************/

/*Standard includes*/
#include <stdio.h> 	
#include <stdlib.h>	
#include <string.h>  

/* Our includes*/
#include "imageio.h"

#define LENGTH 80

/**************************************************************************************************
 *								Get Next String From the FILE
 **************************************************************************************************/
static void _getNextString(FILE *fp, char *line)
{
	int i;
	
	line[0] = '\0';

	while(line[0] == '\0')
	{
		fscanf(fp, "%s", line);
		i = -1;
		do{
			i++;
			if(line[i] == '#')
			{
				line[i] = '\0';
				while(fgetc(fp) != '\n');
			}
		}while(line[i] != '\0');
	}
}


/**************************************************************************************************
 *								pgm Read Header 
***************************************************************************************************/
void pgmReadHeader( FILE *fp, int *magic, int *ncols, int *nrows, int *maxval)
{
  	char line[LENGTH];
	
 	 /* Read magic number */
  	_getNextString(fp, line);
  	if (line[0] != 'P')
	{
    		printf("(pgmReadHeader) Magic number does not begin with 'P', ""but with a '%c'\n", line[0]);
	}
  	sscanf(line, "P%d", magic);
	if (*magic != 5)
  	{
    		printf("(pgmReadHeader) Magic number is not 'P5', but 'P%d' \n", *magic);
	}
	
  	/* Read size, skipping comments */
  	_getNextString(fp, line);
  	*ncols = atoi(line);
  	_getNextString(fp, line);
  	*nrows = atoi(line);
  	if (*ncols < 0 || *nrows < 0 || *ncols > 10000 || *nrows > 10000)
	{
    		printf("(pnmReadHeader) The dimensions %d x %d are unacceptable\n",*ncols, *nrows);
	}	
  	/* Read maxval, skipping comments */
  	_getNextString(fp, line);
  	*maxval = atoi(line);
  	fread(line, 1, 1, fp); /* Read newline which follows maxval */
	
  	if (*maxval != 255)
	{
    		printf("(pnmReadHeader) Max val is not 255, but %d \n", *maxval);
	}
}



/**************************************************************************************************
 *										pgm	Read Header	File
 **************************************************************************************************/
void pgmReadHeaderFile(char *fname, int *magic, int *ncols, int *nrows, int *maxval)
{
	FILE *fp;

  	/* Open file */
  	if ( (fp = fopen(fname, "rb")) == NULL)
	{
    		printf("(pgmReadHeaderFile) Can't open file named '%s' for reading\n", fname);
	}
  	/* Read header */
  	pgmReadHeader(fp, magic, ncols, nrows, maxval);

  	/* Close file */
  	fclose(fp);
}




/**************************************************************************************************
 * 						pgm	Read
 **************************************************************************************************/
unsigned char *pgmRead(FILE *fp,int *ncols, int *nrows)
{
	unsigned char *ptr;
  	int magic, maxval;

	/* Read header */
  	pgmReadHeader(fp, &magic, ncols, nrows, &maxval);

  	/* Allocate memory, if necessary, and set pointer */

	ptr = (unsigned char *)malloc(*nrows * *ncols * sizeof(unsigned char));
	if(!ptr)
	{
		perror("Cannot allocate memory for gray!\n");
		exit(EXIT_SUCCESS);
	}
	memset(ptr, 0, *nrows * *ncols );

	fread(ptr,1,*nrows * *ncols, fp);
	return ptr;
}


/**************************************************************************************************
 *									pgm Read File
 **************************************************************************************************/
unsigned char *pgmReadFile(char *fname, int *ncols, int *nrows)
{
	unsigned char *ptr;
  	FILE *fp;

  	/* Open file */
  	if ( (fp = fopen(fname, "rb")) == NULL)
	{
    		printf("(pgmReadFile) Can't open file named '%s' for reading\n", fname);
	}
  	/* Read file */
  	ptr = pgmRead(fp, ncols, nrows);

  	/* Close file */
  	fclose(fp);

  	return ptr;
}


/**************************************************************************************************
 * 									pgm Write
 **************************************************************************************************/
void pgmWrite(FILE *fp, unsigned char *img, int ncols, int nrows)
{
	//int i;

  	/* Write header */
  	fprintf(fp, "P5\n");
  	fprintf(fp, "%d %d\n", ncols, nrows);
  	fprintf(fp, "255\n");

  	/* Write binary data */
	fwrite(img, 1, ncols * nrows, fp);
}


/****************************************************************************************************
 * 			pgmWriteFile
 ****************************************************************************************************/
void pgmWriteFile(char *fname, unsigned char *img, int ncols, int nrows)
{
	FILE *fp;

  	/* Open file */
  	if ( (fp = fopen(fname, "wb")) == NULL)
	{
    		printf("(pgmWriteFile) Can't open file named '%s' for writing\n", fname);
	}
  	/* Write to file */
  	pgmWrite(fp, img, ncols, nrows);

  	/* Close file */
  	fclose(fp);
}
