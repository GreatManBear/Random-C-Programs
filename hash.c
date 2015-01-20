/* 
 * File:   hash.c
 * Author: kiarash
 *
 * Created on January 3, 2015, 11:25 AM
 * An alphanumerically based hashtable storage system. 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_VALUE_LEN 800
#define MAX_KEY_LEN 20

/**
 * @brief Trims spaces before and after a string
 *
 * @param toTrim the string to trim spaces for 
 * @return Returns the trimmed string
 * 
 */
char * trimSpaces(char *toTrim)
{
    if (toTrim == NULL)
        return NULL;
    int length = strlen(toTrim);
    char *trimmed;
    
    while(*toTrim && isspace(*toTrim))
    {
        toTrim++; 
        length--;
    }
    
    while(isspace(toTrim[length - 1]))
    {
        length--;
    }

    trimmed = strndup(toTrim, length);
    
    return trimmed;
}

typedef struct Entry//for linked list
{ 
    char value[MAX_VALUE_LEN]; // value of entry e.g 2500000, 1600000, 8400000
    char key[MAX_KEY_LEN]; // name of entry e.g Toronto, Montreal, New York
    struct Entry *next;
} Entry;

typedef struct Hashtable 
{
    int numberOfEntries;
    struct Entry * table[62]; //basically an array of heads
} Hashtable;

/**
 * @brief Custom function to generate a hash table
 */
Hashtable * generateHashtable() 
{
    Hashtable *hashtable = NULL;


    if((hashtable = malloc(sizeof(Hashtable)))==NULL)
            return NULL;

    int i;
    for(i = 0; i < 62; i++) //sets all heads to NULL
            hashtable->table[i] = NULL;

    hashtable->numberOfEntries = 0;
    return hashtable;	
}

/**
 * @brief Custom function to generate an entry for the hashtable
 * @param key Key for entry
 * @param value Value for entry
 */
Entry * generateEntry(char *key, char *value) 
{
    key = trimSpaces(key);
    value = trimSpaces(value);

    Entry *newEntry; 

    if( ( newEntry = malloc( sizeof( Entry ) ) ) == NULL ) //check if there is enough space for the linked list
    {
            return NULL;
    }

    strcpy(newEntry->key, key);
    strcpy(newEntry->value,value);
    newEntry->next = NULL;

    return newEntry;
}

/**
 * @brief Custom function to take a string and find the index for the hashtable
 * @param key The key we need to find the index for
 */
int getIndex(char *key) 
{
    key = trimSpaces(key);

    int keyInInt = (int)key[0];// convert to int 
    int hashIndex;
    if ((keyInInt>47)&&(keyInInt<58)) //between 0 and 9
            hashIndex= keyInInt - 48; //shift down from 48 to 0
    else if ((keyInInt>64)&&(keyInInt<91)) //between A - Z
            hashIndex= keyInInt - 65 + 10;
    else if ((keyInInt>96)&&(keyInInt<123)) //between a - z
            hashIndex= keyInInt - 97 + 36;
    else //bad key
            return -1;

    return hashIndex;
}

/**
 * @brief Custom function to add a new entry to the hashtable
 * @param hashtable The hashtable we would be adding an entry to
 * @param key Key of the entry
 * @param value Value of the entry
 * @return Returns 1 if successful, -1 if bad param or unable to delete 
 */
int set(Hashtable *hashtable, char *key, char *value) 
{
    key = trimSpaces(key);
    value = trimSpaces(value);
    
    int index = getIndex(key);

    if (index==-1) 
    {    
        printf("Bad param");\
        return -1;
    }
    if (value==NULL) 
    {
        if(!deleteEntry(hashtable,key))
        {
                printf("ERROR. Couldn't delete %s. Key doesn't exist",key);
                return -1;

        }
    }
    else 
    {
        Entry *next = NULL;
        Entry *last = NULL;

        next = hashtable->table[index];//start location 

        if (next == NULL)
        {
            Entry * newEntry = generateEntry(key, value);    
            hashtable->table[index] = newEntry;

            return;
        }
        else
        {
            while(next != NULL) 
            {
                if ( strcmp(key, next->key) == 0 ) // Replace value 
                {
                    strcpy(next->value, value);

                    return;
                }
                last = next;
                next = next->next;
            }
            // List not empty, key doesn't already exist, insert new entry in the front
            Entry * newEntry = generateEntry(key, value);    
            Entry * temp = hashtable->table[index];
            hashtable->table[index] = newEntry;
            newEntry->next = temp;
        }
    }
}

/**
 * @brief Custom function to get a value of a given key
 * @param hashtable The hashtable we would be checking
 * @param key The key we need to find in the hashtable
 */
char * getValue( Hashtable *hashtable, char *key ) 
{
    key = trimSpaces(key);
    Entry *entry;

    int index = getIndex(key);
    
    if (index == -1)
        return "notfound";
    /* Step through the bin, looking for our value. */ 
    entry = hashtable -> table[index];
    /* Not end of linked list and in order i.e value/key is less than current place*/
    while(entry != NULL) 
    {
        if (strcmp(key, entry->key) == 0)
            return (entry->value);               
        entry = entry->next;
    }
    //end of linked list and nothing was found
    return "notfound";
}

/**
 * @brief Custom function to delete an entry from the hashtable
 * @param hashtable The hashtable we would be editing
 * @param key The key we need to delete in the hashtable
 */
int  deleteEntry( Hashtable *hashtable, char *key ) 
{
    key = trimSpaces(key);
    Entry *temp, *prev;

    int index=getIndex(key);
    temp = hashtable -> table[index];

    while (temp!=NULL)
    {
        if(strcmp(key, temp->key)==0) 
        {
            if(temp==(hashtable->table[index]))//head
            {
                hashtable->table[index]=temp->next;
                free (temp);
                return 1;
            }
            else
            {
                prev->next=temp->next;
                free (temp);
                return 1;
            }
        }
        else
        {
            prev = temp;
            temp = temp->next;
        }
    }
    return 0; //not found
}

int main(void)
{
        // TESTER
	Hashtable *table = generateHashtable();
	set(table, "Toronto", "10000" );
	set(table, "New York", "1300" );
	set(table, " Quebec City", "3414213" );
	set(table, "$test", "floyd" );	
	set(table, "T Land", "32312312" );
	set(table, "The Land", "123231" );  
        printf("#  0 %d  %d\n", getIndex("#"), getIndex("0"));
        printf("A  Z %d  %d\n", getIndex("A"), getIndex("Z"));
        printf("a  z %d  %d\n", getIndex("a"), getIndex("z"));

	printf("%s\n", getValue( table, "Toronto"));
	printf("%s\n", getValue( table, "T Land"));
        printf("%s\n", getValue( table, "The Land"));
	printf("%s\n", getValue( table, "  Quebec City    "));
	printf("%s\n", getValue( table, "$test"));
        printf("%s\n", getValue( table, "New York"));
        set(table, "T Land", NULL );
        printf("%s\n", getValue( table, "T Land"));

        
	return 0;
}


