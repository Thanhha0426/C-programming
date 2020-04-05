#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

struct list {
    struct list *next;
    char string[20];
    float TeF;// tan so xuat hien cua 1 tu trong text
    float DoF;//so luong cau chua tu / tong so cau
    int dscau;// danh so cau
    int SoCauChua;
    float DoQuanTrong;
    };
// dslk cua bang bam
struct entry_s {
	char *key;
	float value;
	struct entry_s *next;
};

struct hashtable_s {
	int size;
	struct entry_s **table;
};

struct list *head = NULL;
typedef struct entry_s entry_t;
typedef struct hashtable_s hashtable_t;
float SoLuongTu;
int SoCau = 0;
int count = 0;
float count1 = 0;
float count2 = 0;
float count3 = 0;


hashtable_t *ht_create( int );
int ht_hash( hashtable_t *, char * );
entry_t *ht_newpair( char *, float );
void ht_set( hashtable_t *hashtable, char *, float  );
float ht_get( hashtable_t *, char * );
unsigned long int size_of_file(FILE *);
void ReadFile();
void TolowerFile();
void printList();

/* Create a new hashtable. */
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

/* Hash a string for a particular hash table. */
int ht_hash( hashtable_t *hashtable, char *key ) {

	unsigned long int hashval;
	int i = 0;

	/* Convert our string to an integer */
	while( hashval < ULONG_MAX && i <= strlen( key ) ) {
		hashval = hashval << 8;
		hashval += key[ i ];
		i++;
	}

	return hashval % hashtable->size;
}

/* Create a key-value pair. */
entry_t *ht_newpair( char *key, float value ) {
	entry_t *newpair;

	if( ( newpair = malloc( sizeof( entry_t ) ) ) == NULL ) {
		return NULL;
	}

	if( ( newpair->key = strdup( key ) ) == NULL ) {
		return NULL;
	}

	if( ( newpair->value = value ) < 0 ) {
		return NULL;
	}

	newpair->next = NULL;

	return newpair;
}

/* Insert a key-value pair into a hash table. */
void ht_set( hashtable_t *hashtable, char *key, float value ) {
	int bin = 0;
	entry_t *newpair = NULL;
	entry_t *next = NULL;
	entry_t *last = NULL;

	bin = ht_hash( hashtable, key );

	next = hashtable->table[ bin ];

	while( next != NULL && next->key != NULL && strcmp( key, next->key ) > 0 ) {
		last = next;
		next = next->next;
	}

	/* There's already a pair.  Let's replace that string. */
	if( next != NULL && next->key != NULL && strcmp( key, next->key ) == 0 ) {

		next->value =  value ;

	/* Nope, could't find it.  Time to grow a pair. */
	} else {
		newpair = ht_newpair( key, value );

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

/* Retrieve a key-value pair from a hash table. */
float ht_get( hashtable_t *hashtable, char *key ) {
	int bin = 0;
	entry_t *pair;

	bin = ht_hash( hashtable, key );

	/* Step through the bin, looking for our value. */
	pair = hashtable->table[ bin ];
	while( pair != NULL && pair->key != NULL && strcmp( key, pair->key ) > 0 ) {
		pair = pair->next;
	}

	/* Did we actually find aweathernything? */
	if( pair == NULL || pair->key == NULL || strcmp( key, pair->key ) != 0 ) {
		return -1;

	} else {
		return pair->value;
	}

}

unsigned long int size_of_file(FILE *fp)
{
	fseek(fp, 0, SEEK_END);
	unsigned long int size = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	return size;
}

void ReadFile(){
    FILE *fp;
    char file[] = "a++.txt";
     /* in C you CANNOT omit the "struct keyword" from a declaration or definition. */
    struct list **pp= &head;
    int ch; /* getc() returns an int */
    size_t len ;
    char buff[30];
    int dscau = 1;

    TolowerFile();
    fp = fopen(file, "r");
    if (!fp) {
        fprintf(stderr, "\nError opening file!");
        exit(EXIT_FAILURE);
    }
    //printf("\nFile has been opened for reading.\n");

    for (len=0; len < sizeof buff;   ) {
        ch = fgetc(fp);
        if (ch == EOF && !len) break;
        if(ch == '.' ){
            if ((ch < 39) || (ch > 39 && ch < 48) || (ch > 57 && ch < 65) || (ch > 90 && ch < 97) || (ch > 122)) {
            if (!len) continue;
            buff[len] = '\0';
            *pp = malloc(sizeof **pp);
            if (!*pp) break;
            strcpy((*pp)->string, buff);
            (*pp)->dscau = dscau;
                     dscau++;
            (*pp)->next = NULL;
            pp = &(*pp)->next ;
            len=0; continue;
            }
        buff[len++] = ch;
        }
        else{
            if ((ch < 39) || (ch > 39 && ch < 48) || (ch > 57 && ch < 65) || (ch > 90 && ch < 97) || (ch > 122))  {
                if (!len) continue;
                buff[len] = '\0';
                *pp = malloc(sizeof **pp);
                if (!*pp) break;
                strcpy((*pp)->string, buff);
                (*pp)->dscau = dscau;
                (*pp)->next = NULL;
                pp = &(*pp)->next ;
                len=0; continue;
                }

            buff[len++] = ch;
        }
    }

    if (len) {
        fprintf(stderr, "\nWord was too large, or out of memory\n");
        }
    if (fclose(fp) ==  EOF) {
        fprintf(stderr, "\nError closing file!\n");
        exit(EXIT_FAILURE);
    }
    //printf("\nFile has been closed.\n");

}

void TolowerFile() {
   FILE *fp1, *fp2;
   int a;

   fp1 = fopen("a.txt", "r");
   fp2 = fopen("a++.txt", "w");

   while ( (a = fgetc(fp1)) != EOF )
   {
      a = tolower(a);
      fputc(a, fp2);
   }

   fclose(fp1);
   fclose(fp2);
}

void printList() {
   struct list *ptr = head;
   printf("\n[ ");
   //start from the beginning
   while(ptr != NULL) {
      printf("(%s,%d) ",ptr->string,ptr->dscau);
      ptr = ptr->next;
   }

   printf(" ]");
}

void CountTF(){

    hashtable_t *hashtable = ht_create( 65536 );
    hashtable_t *hashtable2 = ht_create( 65536 );
    hashtable_t *hashtable3 = ht_create( 65536 );
    float count = 0;
    struct list *ptr = head;

    while(ptr != NULL) {// tim tong so cau
        ht_set(hashtable, ptr->string, 0);
        if(ptr->dscau > SoCau) SoCau = ptr->dscau;
        ptr = ptr->next;
    }    ptr = head;

    while(ptr != NULL) {// tinh so luong tu
        count = 0;
            count = ht_get(hashtable, ptr->string) + 1;
            ht_set(hashtable, ptr->string, count);
        ptr = ptr->next;
        SoLuongTu++;
    }    ptr = head;

    while(ptr != NULL){// tinh tan so xuat hien cua 1 tu trong text
        ptr->TeF = ht_get(hashtable, ptr->string)/SoLuongTu;
        ptr = ptr->next;
    }   ptr = head;

    while(ptr != NULL){// dua vao bang bam 1, reset lai value de tinh tiep DF va IDF
        ht_set(hashtable,ptr->string,0);
        ptr = ptr->next;
    }    ptr = head;
    int i;
    for(i = 1;i < SoCau+1;i++){
        while(ptr != NULL){
            ht_set(hashtable2,ptr->string,0);
            ptr = ptr->next;
        }   ptr = head;
        while(ptr != NULL){
                if(ptr->dscau == i){// xet tung cau
                    if(ht_get(hashtable2, ptr->string) == 0){
                        ht_set(hashtable, ptr->string, ht_get(hashtable, ptr->string)+1);
                        ht_set(hashtable2,ptr->string,1);
                    }
                }
                ptr = ptr->next;
        }   ptr = head;
    }
    while(ptr != NULL){
        ptr->DoF = ht_get(hashtable, ptr->string)/SoCau;
        ptr->DoQuanTrong = ptr->TeF * (1/ptr->DoF);
        printf("%s : %15f\n",ptr->string,ptr->DoQuanTrong);
        ptr = ptr->next;
    }   ptr =head;

    float *sentences = malloc(sizeof(float)*SoCau);
    for(i = 1;i<SoCau+1;i++){
        count1 = 0;// tong do quan trong cua 1 cau
        count2 = 0;// so luong tu trong 1 cau
        count3 = 0;// do quan trong cua cau quan trong nhat
        while(ptr != NULL){
            if(ptr->dscau == i){
                count1 += ptr->DoQuanTrong;
                count2 ++;
                sentences[i-1] = count1 / count2;
            }
            ptr = ptr->next;
        }   ptr = head;
    }
    for(i = 0;i < SoCau;i++){
        printf("Do quan trong cua cau thu %d :%f\n",i+1,sentences[i]);
        if (count3 < sentences[i]){
                count3 = sentences[i];
                count = i+1;
        }
    }
    printf("Vay cau quan trong nhat la cau thu %g (voi do quan trong %g): ",count,count3);

    while(ptr != NULL){
        if(ptr->dscau == count) printf(" %s",ptr->string);
        ptr = ptr->next;
    }   ptr = head;
    printf(".\n");
}
int main(){
    ReadFile();
    CountTF();
    return 0;
}
