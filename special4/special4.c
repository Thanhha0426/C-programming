#define _XOPEN_SOURCE 500 /* Enable certain library functions (strdup) on linux.  See feature_test_macros(7) */

#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <string.h>
#include <ctype.h>

typedef struct
{
    int tanxuat;
    int cau[100];
    int socau;
} data;
struct entry_s {
	char *key;
	data *value;
	struct entry_s *next;
};

typedef struct entry_s entry_t;

struct hashtable_s {
	int size;
	struct entry_s **table;
};

typedef struct hashtable_s hashtable_t;


/* tao bang bam. */
hashtable_t *ht_create( int size ) {

	hashtable_t *hashtable = NULL;
	int i;

	if( size < 1 ) return NULL;

	/* Allocate the table itself. */
	if( ( hashtable = malloc( sizeof( hashtable_t ) ) ) == NULL ) {
		return NULL;
	}

	/* Allocate pointers to the head nodes. */
	if( ( hashtable->table = malloc( sizeof( entry_t * ) * size ) ) == NULL ) {
		return NULL;
	}
	for( i = 0; i < size; i++ ) {
		hashtable->table[i] = NULL;
	}

	hashtable->size = size;

	return hashtable;
}

/* ham bam chuyen ki tu cua van ban thanh key. */
int ht_hash( hashtable_t *hashtable, char *key ) {

	unsigned long int hashval;
	int i = 0;

	/* Convert our string to an integer */
	while( hashval < ULONG_MAX && i < strlen( key ) ) {
		hashval = hashval << 8;
		hashval += key[ i ];
		i++;
	}

	return hashval % hashtable->size;
}

/* tao 1 cap key- value. */
entry_t *ht_newpair(char *key, int cau) {
	entry_t *newpair;

	if( ( newpair = malloc( sizeof( entry_t ) ) ) == NULL ) {
		return NULL;
	}

	if( ( newpair->key = strdup( key ) ) == NULL ) {
		return NULL;
	}
    if( ( newpair->value = malloc( sizeof( data ) ) ) == NULL )
		return NULL;
	newpair->value->tanxuat = 1;
	newpair->value->socau = 1;
	newpair->value->cau[0]=cau;

	newpair->next = NULL;

	return newpair;
}

/* Insert a key-value pair into a hash table. */
void ht_set( hashtable_t *hashtable, char *key, int cau) {
	int bin = 0;
	entry_t *newpair = NULL;
	entry_t *next = NULL;// cac key trung nhau cua cac ki tu
	entry_t *last = NULL;// luu lai lan truoc xuat hien

	bin = ht_hash( hashtable, key );// bam tu kkhoa

	next = hashtable->table[ bin ];

	while( next != NULL && next->key != NULL && strcmp( key, next->key ) > 0 ) {
		last = next;
		next = next->next;
	}

	/* There's already a pair.  Let's replace that string. */
	if( next != NULL && next->key != NULL && strcmp( key, next->key ) == 0 ) {

		next->value->tanxuat += 1;
		if(next->value->cau[next->value->socau-1]!=cau)
        {
            next->value->cau[next->value->socau]=cau;
            next->value->socau+=1;
        }
    //====================================================================
	/* neu key ma chua co thi tao key moi(lan dau). */
	} else {
		newpair = ht_newpair( key, cau );

		/* We're at the start of the linked list in this bin. */
		if( next == hashtable->table[ bin ] ) {
			newpair->next = next;
			hashtable->table[ bin ] = newpair;

		/* We're at the end of the linked list in this bin. */
		} else if ( next == NULL ) {
			last->next = newpair;

		/* We're in the middle of the list. */
		} else  {
			newpair->next = next;
			last->next = newpair;
		}
	}
}

/* tim value bang key da bam. */
data *ht_get( hashtable_t *hashtable, char *key ) {
	int bin = 0;
	entry_t *pair;

	bin = ht_hash( hashtable, key );

	/* Step through the bin, looking for our value. */
	pair = hashtable->table[ bin ];
	while( pair != NULL && pair->key != NULL && strcmp( key, pair->key ) > 0 ) {
		pair = pair->next;
	}

	/* Did we actually find anything? */
	if( pair == NULL || pair->key == NULL || strcmp( key, pair->key ) != 0 ) {
		return NULL;

	} else {
		return pair->value;
	}

}
int main()
{
    hashtable_t *hashtable = ht_create( 65536 );// tao 1 bang gom n so phan tu
    FILE *file;
    data *temp;
    char word[30];
    char c;
    int cau=1;
    int i = 0;
    char target[30];
	file = fopen("file.txt","r");
	while(!feof(file))
    {
        c = fgetc(file);
        if(c == ' ' || c == '\n' || c == '.' || c == ',' || c == '(' || c ==')')
        {
            if(i>0)
            {
                word[i] = '\0';
                ht_set(hashtable,word,cau);//  vua doc file vua chen + bam cac ki tu thanh key
                i = 0;
                if(c == '\n' || c == '.')
                {
                    cau ++;
                }
            }
        }
        // bien tat ca cac ki tu viet hoa thanh thuong
        else
        {
            c = tolower(c);
            word[i] = c;
            i++;
        }
    }
    fclose(file);
    printf("Nhap file xong!\n\n");
    printf("Nhap vao tu can kiem tra: ");
    scanf("%s",&target);
    fflush(stdin);
    temp = ht_get(hashtable,target);
    if(temp!=NULL)
    {
        printf("Tan xuat: %d\n",temp->tanxuat);
        printf("So cau xuat hien: %d\n",temp->socau);
        printf("Cac cau xuat hien: ");
        int j;
        for(j=0;j<temp->socau;j++)
        {
            printf("%d, ",temp->cau[j]);
        }
        printf("\n");
    }
    else
        printf("Tu ko co\n");
    return 0;
}
