/* Include guards */
#ifndef DATASTRUCTURES_H
#define DATASTRUCTURES_H


/*Defines a few usefull datastructures */


struct dynamicArrayf
{
	/* Holds the number of elements inside, initially set to 0 */
	unsigned int size;

	/*Hold the internal size which must allways be strictly > size */
	unsigned int internalSize;

	/* Pointer to the first data member */
	float* data;
};
typedef struct dynamicArrayf Arrayf;

struct dynamicArrayByte
{
	/* Holds the number of elements inside, initially set to 0 */
	unsigned int size;

	/*Hold the internal size which must allways be strictly > size */
	unsigned int internalSize;

	/* Pointer to the first data member */
	unsigned char* data;
};
typedef struct dynamicArrayByte ArrayB;



/* Initializes and empty array */
Arrayf* initArrayf(void)
{
	Arrayf* result = NULL;
	result = malloc(sizeof(Arrayf));
	
	result->data = NULL;
	result->data = malloc(sizeof(float)*128);
	result->size = 0;
	result->internalSize = 128;

	return result;
}




/* Initializes and empty array */
ArrayB* initArrayB(void)
{
	ArrayB* result = NULL;
	result = malloc(sizeof(ArrayB));
	
	result->data = NULL;
	result->data = malloc(sizeof(unsigned char)*128);
	result->size = 0;
	result->internalSize = 128;

	return result;
}

void destroyArrayf(Arrayf* target)
{
	free(target->data);
	free(target);
}

void destroyArrayB(ArrayB* target)
{
	free(target->data);
	free(target);
}


/* Adds value to the end of the array and reallocates memory if it is not sufficient */
void pushArrayf(Arrayf* dest,float value)
{
	dest->size = dest->size + 1;
	if(dest->size >= dest->internalSize)
	{
		dest->data[dest->size -1] = value;
		dest->data = realloc(dest->data, (sizeof(float)) * (dest->internalSize) * 2);
		dest->internalSize = dest->internalSize *2;
	}
	else
	{
		dest->data[dest->size -1] = value;
	}
}

/* Adds value to the end of the array and reallocates memory if it is not sufficient */
void pushArrayB(ArrayB* dest,unsigned char value)
{
	dest->size = dest->size + 1;
	if(dest->size >= dest->internalSize)
	{
		dest->data[dest->size -1] = value;
		dest->data = realloc(dest->data, (sizeof(unsigned char)) * (dest->internalSize) * 2);
		dest->internalSize = dest->internalSize *2;
	}
	else
	{
		dest->data[dest->size -1] = value;
	}
}


/* Treat array as stack and remove and return topmost member
, note that this DOES NOT deallocate memory 
and hence the old values remain in place only the size (i.e. stack pointer) is altered
 */
float popArrayf(Arrayf* dest)
{
	if(dest->size > 0)
	{
		dest->size = dest->size - 1;
		return dest->data[dest->size];
	}
	return 0;
}

unsigned char popArrayB(ArrayB* dest)
{
	if(dest->size > 0)
	{
		dest->size = dest->size - 1;
		return dest->data[dest->size];
	}
	return 0;
}

/*Insert one value to the desired index
if the index < size the old value will be overwritten.
else if the index = size pushArrayf will be called
else the behaviour is undefined

Index must not be greater than size (to avoid discontinuity)
New memory will be reserved when needed */
void insertArrayf(Arrayf* dest, float value, unsigned int index)
{

	if(index < dest->size)
	{
		dest->data[index] = value;
	}
	else if(index == dest->size)
	{
		pushArrayf(dest, value);
	}
}

void insertArrayB(ArrayB* dest, unsigned char value, unsigned int index)
{

	if(index < dest->size)
	{
		dest->data[index] = value;
	}
	else if(index == dest->size)
	{
		pushArrayB(dest, value);
	}
}

/* Copies size values to the array pointed by dest, by calling insertArrayf repeatedly*/
void copyToArrayf(Arrayf* dest, float* values, unsigned int index, unsigned int size)
{
	int i = 0;
	for(i = 0; i < size; i++)
	{
		insertArrayf(dest, values[i], index+i);
	}
}

void copyToArrayB(ArrayB* dest, unsigned char* values, unsigned int index, unsigned int size)
{
	int i = 0;
	for(i = 0; i < size; i++)
	{
		insertArrayB(dest, values[i], index+i);
	}
}

/* Set all values up to size (not including size) to 0.0 */
void set0Arrayf(Arrayf* target)
{
	int i = 0;
	for(i = 0; i < target->size; i++)
	{
		target->data[i] = 0.0;
	}
}

void set0ArrayB(ArrayB* target)
{
	int i = 0;
	for(i = 0; i < target->size; i++)
	{
		target->data[i] = 0;
	}
}

/* Get indexed value */
float getArrayf(Arrayf* dest, unsigned int index)
{
	if(index < dest->size)
	{
		return dest->data[index];
	}
	return 0;
}

unsigned char getArrayB(ArrayB* dest, unsigned int index)
{
	if(index < dest->size)
	{
		return dest->data[index];
	}
	return 0;
}

unsigned int sizeArrayf(Arrayf* target)
{
	return target->size;
}

unsigned int sizeArrayB(ArrayB* target)
{
	return target->size;
}



/* End of include guard DATASTRUCTURES_H*/
#endif

