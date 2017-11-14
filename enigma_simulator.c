#include<stdio.h>
#include<stdlib.h>
#include<string.h>

struct enigma
{
	char* static_wheel; //for rotor3
	char* wheel3;
	char* wheel2;
	char* wheel1; //for reflector
	char* rotor3;
	char* rotor2;
	char* rotor1;
	char* key3; //for rotor3
	char* key2;
	char* key1;
	int nreflector[26]; //Reflector B
	char* key;
	char* plugboard;//for the plugboard
};
typedef struct enigma enigma;

struct node
{
	int before;
	int after;
	struct node* next; //right child
	struct node* prev; //left child
};
typedef struct node node;

struct tree
{
	node* root;
};
typedef struct tree tree;

void initialize_Rotors(enigma*);
void set_Enigma(enigma*, char*); //no character
void shift_Rotor(char*, int);
int find_Character(char*,char);
char character_Encode(char*, char, char*);
char* reflector_String(char*, int*);
char encrypt(enigma*, char); 
void initialize(tree*);
char* plugboard_encrypt(char*,tree*,char*);
char* plugboard_decrypt(char*,tree*,char*);
void insert(tree* , char,char);
node* recursive_Insert(node* root, node* newnode);
node* recursive_Search(node* root, char ch);
char search(tree*, char);
void error();

void main()
{
	int i=0,choice;
	char str[10],key[3];
	printf("Enter the key new:::");
	scanf("%s",&key);

    printf("Enter a word:");
	scanf("%s",&str);
	

    char *pb=(char*)malloc(10*sizeof(char));
    *pb=str;
    tree t;
	initialize(&t);

	printf("1.Encrypt 2.Decrypt\n");
	scanf("%d",&choice);
	switch(choice)
	{
		case 1: pb=plugboard_encrypt(str,&t,key);
		          printf("--------------\n");
				  printf("Encrypted word: %s\n",pb);
				  printf("--------------\n");
				  break;
		case 2:	pb=plugboard_decrypt(str,&t,key);
				  printf("--------------\n");
				  printf("Decrypted word: %s\n",pb);
				  printf("--------------\n");
				  break;

	}	
}
	
	
void initialize(tree* ptr_tree)
{
	ptr_tree -> root = NULL;
}
//initialize the rotors
void initialize_Rotors(enigma* e)
{	
	
	e->static_wheel= (char*)malloc(26*sizeof(char));
	strcpy(e->static_wheel, "ABCDEFGHIJKLMNOPQRSTUVWXYZ");
	
	e->wheel3= (char*)malloc(26*sizeof(char));
	strcpy(e->wheel3, "ABCDEFGHIJKLMNOPQRSTUVWXYZ");
	
	e->wheel2= (char*)malloc(26*sizeof(char));
	strcpy(e->wheel2, "ABCDEFGHIJKLMNOPQRSTUVWXYZ");
	
	e->wheel1= (char*)malloc(26*sizeof(char));
	strcpy(e->wheel1, "ABCDEFGHIJKLMNOPQRSTUVWXYZ");
		
	e->rotor3= (char*)malloc(26*sizeof(char));
	strcpy(e->rotor3, "BDFHJLCPRTXVZNYEIWGAKMUSQO");
	
	e->rotor2= (char*)malloc(26*sizeof(char));
	strcpy(e->rotor2, "AJDKSIRUXBLHWTMCQGZNPYFVOE");
	
	e->rotor1= (char*)malloc(26*sizeof(char));
	strcpy(e->rotor1, "EKMFLGDQVZNTOWYHXUSPAIBRCJ");
	
	e->key3= (char*)malloc(1*sizeof(char));
	e->key3='V';
	//strcpy(e->key3, "Q");
	
	e->key2= (char*)malloc(1*sizeof(char));
	e->key2='E';
	//strcpy(e->key2, "E");
	
	e->key1= (char*)malloc(1*sizeof(char));
	e->key1='Q';
	//strcpy(e->key1, "V");
	
	const int arr[26]={24,17,20,7,16,18,11,3,15,23,13,6,14,10,12,8,4,1,5,25,2,22,21,9,0,19};
	memcpy(e->nreflector, arr, sizeof(e->nreflector)+1);
}

//set the enigma to the key provided
void set_Enigma(enigma* e, char* key)
{
	//allocating memory for the key
	e->key=(char*)malloc(3*sizeof(char));
	strcpy(e->key, key); 
	
	//rotor3
	shift_Rotor(e->rotor3, key[2]-'A');
	
	//rotor2
	shift_Rotor(e->wheel3, key[2]-'A');
	shift_Rotor(e->rotor2, key[1]-'A');
	 
	//rotor1
	shift_Rotor(e->wheel2, key[1]-'A');
	shift_Rotor(e->rotor1, key[0]-'A');

	//reflector
	shift_Rotor(e->wheel1, key[0]-'A');

	printf("wheel 1 :%s\n",e->wheel1);
	printf("wheel 2 :%s\n",e->wheel2);
	printf("wheel 3 :%s\n",e->wheel3);
	printf("rotor 1 :%s\n",e->rotor1);
	printf("rotor 2 :%s\n",e->rotor2);
	printf("rotor 3 :%s\n\n\n",e->rotor3);

}

char encrypt(enigma* e, char ch)
{
	if(e->wheel3[0]==e->key3)
	{
		if(e->wheel2[0]==e->key2)
		{
			shift_Rotor(e->wheel1,1);
			shift_Rotor(e->rotor1,1);
		}
		shift_Rotor(e->wheel2,1);
		shift_Rotor(e->rotor2,1);
	}
	printf("Wheel :  %s\n",e->static_wheel);
	shift_Rotor(e->rotor3,1);
	printf("Rotor 3: %s\n",e->rotor3);
	ch=character_Encode(e->rotor3, ch, e->static_wheel);
	
	
	shift_Rotor(e->wheel3,1);
	printf("Wheel 3: %s\n",e->wheel3);
	printf("Rotor 2: %s\n",e->rotor2);
	ch=character_Encode(e->rotor2, ch, e->wheel3);
	
	printf("Wheel 2: %s\n",e->wheel2);
	printf("Rotor 1: %s\n",e->rotor1);
	ch=character_Encode(e->rotor1, ch, e->wheel2);
	
	printf("Wheel 1: %s\n",e->wheel1);
	printf("Reflect: ");
	ch=character_Encode(reflector_String(e->wheel1, e->nreflector), ch, e->wheel1);
	
	ch=character_Encode(e->wheel2, ch, e->rotor1);
	
	ch=character_Encode(e->wheel3, ch, e->rotor2);
	
	ch=character_Encode(e->static_wheel, ch, e->rotor3);
	return ch;
}

char* reflector_String(char* str, int* nreflector)
{
	int i;
	char* reflector=(char*)malloc(26*sizeof(char));	
	for(i=0;i<26;i++)
	{
		reflector[i]=str[nreflector[i]];
		printf("%c",reflector[i]);
	}
	printf("\n");
	return reflector;
}

char character_Encode(char* rotor, char ch, char* wheel)
{
	//printf("HI");
	int pos;
	pos=find_Character(wheel,ch);
	ch=rotor[pos];
	printf("%c\n\n",ch);
	return ch;	
}

//Shifts string n times to the left. Pass the string and n. Send string as enigma->rotor
void shift_Rotor(char* rotor, int n)
{
	int i, j;
	for(i=1; i<=n; i++)
	{
		char temp=rotor[0];
		for(j=0;j<25;j++)
		{
			rotor[j]=rotor[j+1];
		}
		rotor[j]=temp;
	} 
	//printf("%s\n",rotor);
}

int find_Character(char* str, char ch)
{
	int i;
	for(i=0;i<26;i++)
	{
		if(str[i]==ch)
			return i;
	}
}

char* plugboard_encrypt(char *str,tree* ptr_tree,char* key)
{
	for(int m=0;m<strlen(str);m++)
	{
		if((int)str[m]>=97 && (int)str[m]<=122)
		{
			int temp = (int)str[m];
			temp=temp-32;
			str[m]=(char)temp;
		}
		if((int)str[m]>=65 && (int)str[m]<=90)
		{
			continue;
		}
		else
		{
			error();
			break;
		}
	}
	//printf("\n%s",str);
    char* a=(char*)malloc(13*sizeof(char));
	char* b=(char*)malloc(13*sizeof(char));
	int i, j, num;
	printf("Enter the number of plugboard settings you wish to give: ");
	scanf("%d",&num);
	printf("Enter plugboard settings: \n");
	for(i=0; i<num; i++)
	{
		scanf(" %c,%c",&a[i],&b[i]);//space before %c ignores previous whitespaces
		insert(ptr_tree,a[i],b[i]);
		printf("inserted\n");
	}
	for(i=0; i<num;i++)
	{
		printf("%d %c %c\n",i,a[i],b[i]);
	}

    for(int k=0;k<strlen(str);k++)
    {
		str[k] = search(ptr_tree,str[k]);
	}
    printf("The new string after plugboard is swapped is %s \n",str);
    printf("----------\n");
    enigma e;
	initialize_Rotors(&e);
	set_Enigma(&e, key);
	for(i=0;i<strlen(str);i++)
	{
		str[i]=encrypt(&e,str[i]);
	printf("----------\n");
	}
	printf("before plugboard switch back is %s\n",str);

	for(i=0; i<strlen(str);i++)
	{
		for(j=0;j<strlen(b);j++)
		{
			if(str[i]==b[j])
			{
				str[i]=a[j];	
				break;
			}
		}
	}
	return str;
}

char* plugboard_decrypt(char *str,tree* ptr_tree,char* key)
{
	for(int m=0;m<strlen(str);m++)
	{
		if((int)str[m]>=97 && (int)str[m]<=122)
		{
			int temp = (int)str[m];
			temp=temp-32;
			str[m]=(char)temp;
		}
		if((int)str[m]>=65 && (int)str[m]<=90)
		{
			continue;
		}
		else
		{
			error();
			break;
		}
	}
	//printf("\n%s",str);
    char* a=(char*)malloc(13*sizeof(char));
	char* b=(char*)malloc(13*sizeof(char));
	int i, j, num;
	printf("Enter the number of plugboard settings you wish to give: ");
	scanf("%d",&num);
	printf("Enter plugboard settings: \n");
	for(i=0; i<num; i++)
	{
		scanf(" %c,%c",&a[i],&b[i]);//space before %c ignores previous whitespaces
		printf("inserted\n");
	}
	for(i=0; i<num;i++)
	{
		printf("%d %c %c\n",i,a[i],b[i]);
	}
    printf("The new string after plugboard is swapped is %s \n",str);
    printf("----------\n");
    enigma e;
	initialize_Rotors(&e);
	set_Enigma(&e, key);
	for(i=0;i<strlen(str);i++)
	{
		str[i]=encrypt(&e,str[i]);
	printf("----------\n");
	}
	printf("before plugboard switch back is %s\n",str);

	for(i=0; i<strlen(str);i++)
	{
		for(j=0;j<strlen(b);j++)
		{
			if(str[i]==b[j])
			{
				str[i]=a[j];	
				break;
			}
		}
	}
	return str;
}
node* create_Node(char d1,char d2)
{
	node* newnode = (node*)malloc(sizeof(node));
	newnode -> before = d1;
	newnode -> after=d2;
	newnode->prev=NULL;
	newnode -> next = NULL;
	return newnode;
}

node* recursive_Insert(node* root, node* newnode)
{
	if(root == NULL)
	{
		root = newnode;
	}
	else if(root ->before > newnode -> before)
	{
		root -> prev = recursive_Insert(root -> prev, newnode);
	}
	else
	{
		root -> next = recursive_Insert(root -> next, newnode);
	}
	return root;
	
}
void insert(tree* ptr_tree, char d1,char d2)
{
	node* newnode =create_Node(d1,d2);
	ptr_tree -> root = recursive_Insert(ptr_tree -> root, newnode);
}


node* recursive_Search(node* root, char ch)
{
	if(root == NULL)
	{
		node* newnode =create_Node('0','0');
		return newnode;
	}
	else if(root -> before == ch)
	{
		return root;
	}
	else if(root -> before > ch)
	{
		return recursive_Search(root -> prev, ch);
	}
	else
	{
		return recursive_Search(root -> next, ch);
	}
}


char search(tree* ptr_tree, char data)
{
	node* result = recursive_Search(ptr_tree -> root, data);
	if(result->before=='0')
	{
		return data;
	}
	else
	{
		data = result->after;
		return(data);
	}
}


void error()
{
	printf("ERROR! please enter the correct data");
	exit(1);
}











