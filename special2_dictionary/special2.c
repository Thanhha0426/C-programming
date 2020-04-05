#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define ARRAY_SIZE(a) sizeof(a)/sizeof(a[0])

// Alphabet size (# of symbols)
#define ALPHABET_SIZE (62)

// Converts key current character into index
// use only 'a' through 'z' and lower case
#define LIMIT 5
char code[]={'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o',
            'p','q','r','s','t','u','v','w','x','y','z','A','B','C','D',
            'E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S',
            'T','U','V','W','X','Y','Z','0','1','2','3','4','5','6','7','8','9'};

//char code2 = "012345......62"
//=====================================
// cách tạo cây trie
// cách giới hạn số từ gợi ý ra màn hình
//=====================================
// trie node
int INDEX(char c)
{
    int i;
    for(i=0; i<62; i++)
        if(code[i]==c) return i;
}

struct TrieNode
{
    struct TrieNode *children [ALPHABET_SIZE];
    //struct TrieNode *children = (struct TrieNode *)malloc(sizeof (struct TrieNode));
    bool isEndOfWord;
};

// Returns new trie node (initialized to NULLs)
// tạo node mới
struct TrieNode *getNode(void)
{
    struct TrieNode *pNode = NULL;

    pNode = (struct TrieNode *)malloc(sizeof(struct TrieNode));

    if (pNode)
    {
        int i;

        pNode->isEndOfWord = false;

        for (i = 0; i < ALPHABET_SIZE; i++)
            pNode->children[i] = NULL;
    }

    return pNode;
}
// node queue
struct QNode
{
    struct TrieNode *data;
	char currWord[30];
    struct QNode *next;
};

// The queue, front stores the front node of LL and rear stores ths
// last node of LL
struct Queue
{
    struct QNode *front, *rear;
	int count;
};

// A utility function to create a new linked list node.
struct QNode* newNode(struct TrieNode *data, char *currWord)
{
    struct QNode *temp = (struct QNode*)malloc(sizeof(struct QNode));
    temp->data = data;
	strcpy(temp->currWord,currWord);
    temp->next = NULL;
    return temp;
}

// A utility function to create an empty queue
// tạo 1 hàng đợi trống
struct Queue *createQueue()
{
    struct Queue *q = (struct Queue*)malloc(sizeof(struct Queue));
    q->front = q->rear = NULL;
	q->count = 0;
    return q;
}
// đưa những từ gợi ý vafp hàng đợi
// The function to add a key k to q
void enQueue(struct Queue *q, struct TrieNode *data, char *currWord)
{
    // Create a new LL node
    struct QNode *temp = newNode(data,currWord);

    // If queue is empty, then new node is front and rear both
    if (q->rear == NULL)
    {
       q->front = q->rear = temp;
       q->count += 1;
       return;
    }

    // Add the new node at the end of queue and change rear
    q->rear->next = temp;
    q->rear = temp;
	q->count += 1;
}

// Function to remove a key from given queue q
struct QNode *deQueue(struct Queue *q)
{
    // If queue is empty, return NULL.
    if (q->front == NULL)
       return NULL;

    // Store previous front and move front one node ahead
    struct QNode *temp = q->front;
    q->front = q->front->next;

    // If front becomes NULL, then change rear also as NULL
    if (q->front == NULL)
       q->rear = NULL;
    q->count -= 1;
    return temp;
}

// If not present, inserts key into trie
// If the key is prefix of trie node, just marks leaf node
void insert(struct TrieNode *root, const char *key)
{
    int level,i;
    int length = strlen(key);
    int index;

    struct TrieNode *pCrawl = root;
    // duyet tu chu cai dau tien cua tu den chu cai cuoi cung
    for (level = 0; level < length; level++)
    {
        // kiem tra xem ki hieu la thuong hay hoa hay chu so
        index = INDEX(key[level]);

        if (!pCrawl->children[index])
            pCrawl->children[index] = getNode();

        pCrawl = pCrawl->children[index];
    }
    // mark last node as leaf
    pCrawl->isEndOfWord = true;// danh dau ket thuc 1 tu
}

// Returns true if key presents in trie, else false
bool search(struct TrieNode *root, const char *key)
{
    int level,i;
    int length = strlen(key);
    int index;
    struct TrieNode *pCrawl = root;

    for (level = 0; level < length; level++)
    {
        index=INDEX(key[level]);

        if (pCrawl->children[index]==NULL)
            return false;

        pCrawl = pCrawl->children[index];
    }

    return (pCrawl != NULL && pCrawl->isEndOfWord);
}

bool isLastNode(struct TrieNode*root)
{
    int i;
	for( i=0;i<62;i++)
	{
		if(root->children[i]!=NULL)
			return 0;
	}
	return 1;
}

void suggestionRec(struct TrieNode *data,char *currWord)
{
    int countanswer = 0;
	struct Queue *q = createQueue();
	struct QNode* tempQNode;
	char tempWord[30];
	enQueue(q,data,currWord);
	char c[2];
	c[1] = '\0';
	int i;
	while(q->count>0)
	{
        tempQNode = deQueue(q);
        if(tempQNode->data->isEndOfWord) //tìm được từ thỏa mãn thì in ra mh
        {
            printf("%s\n",tempQNode->currWord);
            countanswer += 1;
            if(countanswer >= LIMIT)
                break;
        }
        for(i = 0; i < ALPHABET_SIZE; i++)
        {
            if(tempQNode->data->children[i]!=NULL)
			{
				c[0] = code[i]; // dịch ngược từ số về chữ
				strcpy(tempWord,tempQNode->currWord);// loi phan tu trong queue ra ngoai
				strcat(tempWord,c);
				enQueue(q,tempQNode->data->children[i],tempWord);
			}
        }
        free(tempQNode);
	}
	if(countanswer==0) printf("Khong co tu %s trong van ban\n",currWord);
	else printf("%d\n",countanswer);
}

void printAutoSuggestion(struct TrieNode *root,char *target)
{
	struct TrieNode *temp;
	temp = root;
	int level, i;
	int index;
	int n = strlen(target);
	for(level = 0;level < n; level++)
	{
		index = INDEX(target[level]);
		if(!temp->children[index])
		{
			printf("Invalid Input!!!\n");
			return;
		}
		temp = temp->children[index];
	}
	suggestionRec(temp,target);
}

// Driver
int main()
{
    char Pattern[50],Vocab[50];
    FILE *f;
    int op;
    struct TrieNode *root = getNode();
    do
    {
        printf("------------------------------------------");
        printf("\n1)Read File:");
        printf("\n2)Search:");
        printf("\n3)Exit:\n");
        printf("Enter Your Choice: ");
        scanf("%d",&op);
        if (op<1 || op>3) printf("Try Again\n");
        switch(op)
        {
            case 1: f = fopen("dictionary.txt","r");
                    while(!feof(f))
                    {
                        fscanf(f,"%s",Vocab);
                        insert(root,Vocab);
                    }
                    printf("DONE!!!\n");
                    fclose(f);
                    break;
            case 2: printf("Nhap vao tu muon tim: ");
                    scanf("%s",Pattern);
                    printAutoSuggestion(root,Pattern);
                    break;
        }
    }while(op!=3);
	// Input keys (use only 'a' through 'z' and lower case)
	return 0;
}
