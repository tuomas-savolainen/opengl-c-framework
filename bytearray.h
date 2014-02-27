/* Include guards */
#ifndef BYTEARRAY_H
#define BYTEARRAY_H

/* How it works in case you forget:

The struct byteArray has member data that stores arbitrary amount of data in bytes
Data is of type unsigned char in order to store anything to the byteArray (hence the name byteArray)

You need to do three things in order to use this header

1. 

Use explicit typecasting in your code like this:
printf("%d\n", *(int*)getByteArray(testInt, i));
So before referencing the void pointer you have to cast it to some type 
(void can be understood as no type). In general to get value from the array use the following syntax when using explicit cast
*(datatype*)getByteArray(arrayName, index);

***OR***

Have a cast function for your datatype (struct), like this:

dataType casterFunction(void* value)
{
	dataType temp;
	memcpy(&temp, value, sizeof(dataType));
	return temp;
}
example of cast function named castInt is included in this header




2. Correctly initialize 
byteArray* myArray = initByteArray(sizeof(dataType));


3. Remember to destruct your byteArray:
when you don't need your container any more call:
destroyByteArray(myArray);

Ok, how it works in short (if you need to change this code)

The data pointer points to continuous memory, which looks something like this:

Byte 0, 	Byte 1, 	Byte 2, 	Byte 3, 	Byte 4, 	Byte 5, 	Byte 6, 	Byte 7, 	Byte 8, ...
10100111 	10010001 	00001111 	00001111 	11110000 	00001111 	11110000	11011101	11110001

The data pointer doesn't really care what this represents to it this just sequence of unsigned chars (i.e. bytes)

To access the data correctly you need to tell the byteArray how much memory your datatype takes up in bytes,
standard librarys sizeof() function tells you that. This information is stored in byteSize member variable.

If for example byteSize = 4
then the array would represent the data in the following manner:

Byte 0, 	Byte 1, 	Byte 2, 	Byte 3, 	Byte 4, 	Byte 5, 	Byte 6, 	Byte 7, 	Byte 8, ...
10100111 	10010001 	00001111 	00001111 	11110000 	00001111 	11110000	11011101	11110001

Element0,										Element1,										Element2, ...
data[0],	data[1],	data[2],	data[3],	data[4],	data[5],	data[6],	data[7],	data[8], ...

But since the data is stored as unsigned chars the getter functions can't just return one data member,
for example if you want to return element1 you have to make a copy of data[4], data[5], data[6], data[7]
and return it as a void*. That is also the reason why there is (*target->internalSize) everywhere.

 This is just a convienience datastructure if you can't or haven't time to use better arrays,
do not change byteSize or any other member variables directly (valgrind will not like the results)! */




struct byteArray
{
	/* Holds the number of elements inside, initially set to 0 */
	unsigned int size;

	/*Hold the internal size which must allways be strictly > size */
	unsigned int internalSize;

	/*Holds the size in bytes (octets) of individual elements, use sizeof() when setting this number*/
	unsigned int byteSize;

	/* Pointer to the first data member */
	unsigned char* data;

	/* Return value, eliminates the need of explicit caster funtions */
	void* element;

};

typedef struct byteArray byteArray;



/* some forward declarations */
void pushByteArray(byteArray* dest,void* value);


/* Initializes and empty array */
byteArray* initByteArray(unsigned int byteSize)
{
	byteArray* result = NULL;
	result = malloc(sizeof(byteArray));

	result->data = NULL;
	result->data = malloc(byteSize*128);
	result->element = NULL;
	result->element = malloc(byteSize);

	result->size = 0;       
	result->internalSize = 128;
	result->byteSize = byteSize;


	return result;
}

void destroyByteArray(byteArray* target)
{
	free(target->element);
	free(target->data);
	free(target);
}


/* Adds value to the end of the array and reallocates memory if it is not sufficient */
void pushByteArray(byteArray* dest,void* value)
{
	dest->size = dest->size + 1;
	if(dest->size >= dest->internalSize)
	{
		memcpy(&(dest->data[(dest->size -1)*dest->byteSize]), value, dest->byteSize);

		dest->data = realloc(dest->data, (dest->byteSize * (dest->internalSize) * 2));
		dest->internalSize = dest->internalSize *2;
	}
	else
	{
		memcpy(&(dest->data[(dest->size -1)*dest->byteSize]), value, dest->byteSize);
	}
}

/* Treat array as stack and remove and return topmost member
, note that this DOES NOT deallocate memory 
and hence the old values remain in place only the size (i.e. stack pointer) is altered
, return NULL if target->size equals 0 i.e. there is no values to pop
 */
void* popByteArray(byteArray* target)
{
	if(target->size > 0)
	{
		target->size = target->size - 1;
		/* Allocate bytes for the return value, these must be freed in the cast function */
		/*
		unsigned char* value = NULL;
		value = malloc(target->byteSize);
		*/
		memcpy(target->element, &(target->data[(target->size)*target->byteSize]),  target->byteSize);
		return target->element;
	}
	return NULL;
}


/*Insert one value to the desired index
if the index < size the old value will be overwritten.
else if the index = size pushByteArray will be called
else the behaviour is undefined

Index must not be greater than size (to avoid discontinuity)
New memory will be reserved when needed */

void insertByteArray(byteArray* dest, void* value, unsigned int index)
{

	if(index < dest->size)
	{
		memcpy(&(dest->data[(index)*dest->byteSize]), value, dest->byteSize);
	}
	else if(index == dest->size)
	{
		pushByteArray(dest, value);
	}
}



void* getByteArray(byteArray* dest, unsigned int index)
{
	if(index < dest->size)
	{
		/* Allocate bytes for the return value, these must be freed in the cast function */
		/*
		unsigned char* value = NULL;
		value = malloc(dest->byteSize);
		*/
		memcpy(dest->element, &(dest->data[(index)*dest->byteSize]),  dest->byteSize);

		return dest->element;
	}
	return NULL;
}

unsigned int sizeByteArray(byteArray* target)
{
	return target->size;
}


int castInt(void* value)
{
	int result = 0;
	memcpy(&result, value,  sizeof(int));
	
	return result;
}




/* End of include guard BYTEARRAY_H*/
#endif