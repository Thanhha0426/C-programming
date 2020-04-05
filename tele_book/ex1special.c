#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<ctype.h>
#define MAX 100

//char str[MAX];

typedef struct PNODE
{
    char name[MAX];
    char phone[MAX];
    int ht;
    struct PNODE *pleft, *pright;
} Pnode;
Pnode * root = NULL;

typedef struct aNODE
{
	Pnode * data;// nen la kieu du lieu phuc tap
	struct aNODE *pNext;
} NODE;
NODE *rear,*front;
int count;
//===============================================
void initQueue()
{
	rear = NULL;
	front = NULL;
	count = 0;
}
    //===========================================
void enqueue(Pnode *data)
{
	NODE *newNode = (NODE*) malloc(sizeof(NODE));
	newNode->pNext = NULL;
	newNode->data = data;
	if(count==0)
	{
		front = newNode;
		rear = newNode;
		count++;
	}
	else	// TH2. Khac rong
	{
		rear->pNext = newNode;
		rear = newNode;
		count++;
	}
}
    //===========================================
int size()
{
	return count;
}
    //===========================================
NODE *dequeue()
{
	// TH1. queue rong --> tra ve NULL
	if(count == 0){
        //printf("hang doi rong");
        return NULL;
	}
	// TH2. queue con 1 phan tu --> sau khi lau ra 		// thi gan rear = NULL
    if(count == 1){
            NODE *p = front;
            front = NULL;
            rear = NULL;
            count --;
            return p;
        }
            //return front;
	// TH3. Queue >1 phan tu
	NODE *p = front;
    front=front->pNext;
    count--;
    return p;
    //printf("so phan tu trong hang:%d\n", count);
}
//===============================================

int height( Pnode *);
Pnode *rotateright(Pnode *);// quay trái
Pnode *rotateleft(Pnode *);// quay phải
Pnode *RR(Pnode *);
Pnode *LL(Pnode *);
Pnode *LR(Pnode *);
Pnode *RL(Pnode *);
int BF(Pnode *);
//===============================================
int menu()
{
    int option;
    printf("========================================\n");
    printf("MENU!\n");
    printf("1.read contacts.\n");
    printf("2. search (phone).\n");
    printf("3. delete contact (phone).\n");
    printf("4. add contact.\n");
    printf("5. print contacts.\n");
    printf("6. print file.\n");
    printf("7. Exit.\n");
    printf("Enter your choice:");
    do{
        scanf("%d", &option);
    }while (option > 7 || option < 1);

    return option;
}
//===============================================
int height(Pnode *root)
{
    int lh,rh;
    if(root==NULL)
        return(0);

    if(root->pleft==NULL)
        lh=0;
    else
        lh=1+root->pleft->ht;

    if(root->pright==NULL)
        rh=0;
    else
        rh=1+root->pright->ht;

    if(lh>rh)
        return(lh);

    return(rh);
}
 //xoay cây sang phải===========================
Pnode * rotateright(Pnode *x)
{
    Pnode *y;
    y=x->pleft;
    x->pleft=y->pright;
    y->pright=x;
    x->ht=height(x);
    y->ht=height(y);
    return(y);
}
// xoay cây sang trái ==========================
Pnode * rotateleft(Pnode *x)
{
    Pnode *y;
    y=x->pright;
    x->pright=y->pleft;
    y->pleft=x;
    x->ht=height(x);
    y->ht=height(y);

    return(y);
}
 // cây phải phải ==============================
Pnode * RR (Pnode *root)
{
    root=rotateleft(root);
    return(root);
}
// cây trasi trái ==============================
Pnode * LL(Pnode *root)
{
    root=rotateright(root);
    return(root);
}
// cây trái phải================================
Pnode * LR(Pnode *root)
{
    root->pleft=rotateleft(root->pleft);
    root=rotateright(root);

    return(root);
}
//cây phải trái=================================
Pnode * RL(Pnode *root)
{
    root->pright=rotateright(root->pright);
    root=rotateleft(root);
    return(root);
}
// chỉ số cân bằng của cây =====================
int BF( Pnode *root)
{
    int lh,rh;
    if(root==NULL)
        return(0);

    if(root->pleft==NULL)
        lh=0;
    else
        lh=1+root->pleft->ht;

    if(root->pright==NULL)
        rh=0;
    else
        rh=1+root->pright->ht;

    return(lh-rh);
}
//===============================================*/
void print(FILE *f, int *n)
{
    char str[MAX];
    fscanf(f, "%d" , n);
    printf("%d",*n);
    while (!(feof(f))){
        fgets(str,MAX,f);
        if(feof(f)) break;

        printf("%s", str);
    }
}
//===============================================
Pnode * insert(Pnode *root, char name1[], char phone1[])
{
    if(root==NULL)
    {
        root=(Pnode*)malloc(sizeof(Pnode));
        strcpy(root->name, name1);
        strcpy(root->phone, phone1);
        root->pleft=NULL;
        root->pright=NULL;
    }
    else
        if(strcmp(phone1,root->phone) > 0)        // insert in right subtree
        {
            root->pright=insert(root->pright,name1,phone1);
            if(BF(root)==-2){
                if(strcmp(phone1, root->pright->phone) > 0)
                    root=RR(root);
                else
                    root=RL(root);
                    }
        }
        else
            if(strcmp(phone1,root->phone) < 0)
            {
                root->pleft=insert(root->pleft,name1,phone1);
                if(BF(root)==2)
                    if(strcmp(phone1, root->pleft->phone) < 0)
                        root=LL(root);
                    else
                        root=LR(root);
            }

        root->ht=height(root);

        return(root);
}
//==============================================
Pnode* search (Pnode * root, char phone1[MAX])
{
    if(root == NULL) return NULL;
    if(strcmp(phone1, root->phone) == 0) return root;
    else if(strcmp(phone1, root->phone) > 0)
            return search(root->pright, phone1);
    else return search(root->pleft, phone1);
}
//===============================================
void preorder(Pnode *T)
{
    if(T!=NULL)
    {
        printf("%s(Bf=%d)%s\n",T->name,BF(T), T->phone);
        preorder(T->pleft);
        preorder(T->pright);
    }
}
//===============================================
Pnode * Delete(Pnode *T, char phone1[])
{
    Pnode *p;

    if(T==NULL) return NULL;
    else
    {
        if(strcmp(phone1,T->phone)>0)        // insert in right subtree
        {
            T->pright=Delete(T->pright,phone1);
            if(BF(T)==2){
                if(BF(T->pleft)>=0)
                    T=LL(T);
                else
                    T=LR(T);
            }
        }
        else
            if(strcmp(phone1,T->phone)<0)
            {
                T->pleft=Delete(T->pleft,phone1);
                if(BF(T)==-2)    //Rebalance during windup
                {
                    if(BF(T->pright)<=0)
                        T=RR(T);
                    else
                        T=RL(T);
                }
            }
            else
            {
                //data to be deleted is found
                if(T->pright!=NULL)
                {    //delete its inorder succesor
                    p=T->pright;

                    while(p->pleft!=NULL)
                        p=p->pleft;

                    strcpy(T->name,p->name);
                    strcpy(T->phone,p->phone);
                    T->pright=Delete(T->pright,p->phone);

                    if(BF(T)==2)//Rebalance during windup
                    {
                        if(BF(T->pleft)>=0)
                            T=LL(T);
                        else
                            T=LR(T);
                    }
                }
                else return(T->pleft);
            }
        //n-=1;
    }
    T->ht=height(T);
    return(T);
}
//===============================================
void ghifile(int number)
{
    FILE *f = fopen("contacts.txt", "w");
    int dieukien = 1;
    enqueue(root);
    do{
        if (dieukien == 1)
        {
            fprintf(f, "%d\n", number);
            dieukien = 0;
        }
        NODE *p = dequeue();
        fprintf(f,"%s\n%s\n", p->data->name, p->data->phone);
        if (p->data->pleft != NULL)
            enqueue(p->data->pleft);
        if(p->data->pright != NULL)
            enqueue(p->data->pright);
    }while (size() > 0);
    fclose(f);
}
//===============================================
void readdata(char name1[], char phone1[])
{
    root = NULL;
    FILE *f;
    int i;
    char temp;
    int anumber;
    //int *anumber= (int*)malloc(sizeof(int));
    f = fopen ("contacts.txt", "r");
    if(f == NULL){
        printf("file not exit!");
        exit(0);
    }
    fscanf(f, "%d", &anumber);
    printf("A number of contacts = %d\n", anumber);
    fscanf(f, "%c", &temp);
    for (i = 0; i< anumber; i++){
        fscanf(f, "%[^\n]", name1); printf("%s\n", name1);
        fscanf(f, "%s", phone1); printf("%s\n", phone1);
        fscanf(f, "%c", &temp);
        root = insert(root, name1, phone1);
    }
    fclose(f);
}
//===============================================
int main ()
{
    char name1[MAX], phone1[MAX];
    char s;
    //readdata();
    int anumber;
    FILE *f = fopen("contacts.txt", "r");
    fscanf(f, "%d", &anumber );
    fclose(f);
    //int i;
    int a;
    do{
        a=menu();
        switch(a)
        {
        case 1:
            {
                readdata(name1, phone1);
                break;
            }
        case 2:
            {
                printf("\nSearch Data:");
                printf("\nEnter the phone:");
                scanf("%s",&phone1);
                Pnode *temp = search(root, phone1);
						  if(NULL==temp) printf("%s is not EXIST!\n",phone1);
						  else {
                                printf("%s is EXIST!\n", phone1);
						        printf("%s\n", temp->name);
						  }
                break;
            }
        case 3:
            {
                printf("\nEnter the phone:");
                scanf("%s",phone1);
                Pnode *tmp = search(root, phone1);
                if (NULL == tmp)
                    printf("%s not exit.\n", phone1);
                else{
                    root = Delete(root, phone1);
                    printf("phone is deleted.\n");
                    anumber =anumber -1;
                }
                break;
            }
        case 4:
            {
                printf("Enter the name:");
                scanf("%c", &s);
                scanf("%[^\n]c", name1);
                printf("Enter the phone:");
                scanf("%s", phone1);
                Pnode *tmp=search(root, phone1);
                if(NULL != tmp){
                    printf("%s is EXIT\n");
                    printf("%s\n", tmp->name);
                }
                else {
                    root = insert(root, name1, phone1);
                    anumber +=1;
                }
                break;
            }
        case 5:
            {
                printf("A number of contacts = %d\n", anumber);
                printf("Preoder:\n");
                preorder(root);
                break;
            }
        case 6:
            {
                ghifile(anumber);
                break;
            }
        }
    }while (a != 7);
    return 0;
}
