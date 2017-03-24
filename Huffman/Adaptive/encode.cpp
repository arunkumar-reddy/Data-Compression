#include <bits/stdc++.h>
using namespace std;
struct node
{
    char symbol;
    int value;
    int number;
    struct node *left;
    struct node *right;
    struct node *parent;
};
int original = 0,compressed = 0;
int n = 7,node_number = 0;
struct node *nyt,*root;
map<char,string> prefix;
map<int,struct node* > getnode;
map<char,int> present;
struct node* newnode(char a)
{
    struct node* temp = (struct node*)malloc(sizeof(struct node));
	temp->symbol = a;
	temp->value = 0;
	temp->number = 0;
	temp->left = NULL;
	temp->right = NULL;
	temp->parent = NULL;	    
    return temp;
}
string getbinary(int i)
{
	string code = "";
	int j = 6;	
	while(j>=0)
	{
		char c = i%2+'0';
		code = c + code;
		j--;
		i = i/2;
	}
	return code;
}
void setprefixes()
{
	for(int i=0;i<126;i++)
	{
		prefix.insert(make_pair((char)i,getbinary(i)));
	}
}
int add(char c)
{
	struct node* merge = newnode('*');
	merge->value = 0;
	merge->number = node_number;
	node_number++;
    struct node* symbol = newnode(c);
	symbol->value = 0;
	symbol->number = node_number;
	node_number++;
	if(root==nyt)
	{
		root = merge;
	}
	else
	{
		merge->parent = nyt->parent;
		nyt->parent->left = merge;	
	}
	int temp = merge->number;
	merge->number = nyt->number;
	nyt->number = temp;	
	merge->left = nyt;
	nyt->parent = merge;
	merge->right = symbol;
	symbol->parent = merge; 
	getnode.find(merge->number)->second = merge;
	getnode.insert(make_pair(nyt->number,nyt));
	getnode.insert(make_pair(symbol->number,symbol));
	return symbol->number;
}
string getcode(struct node* curr)
{
	if(curr==root)
	{
		return "";
	}
	struct node* temp = curr->parent;
	if(temp->left==curr)
	{
		return getcode(temp)+'0';
	}
	else
	{
		return getcode(temp)+'1';
	}
}
void swap(struct node* curr,struct node* temp)
{
	getnode.find(curr->number)->second = temp;
	getnode.find(temp->number)->second = curr;
	char z1 = curr->symbol;
	char z2 = temp->symbol;
	int num = curr->number;
	curr->number = temp->number;
	temp->number = num;
	if(z1!='*')
	{
		present.find(z1)->second = curr->number;
	}
	if(z2!='*')
	{
		present.find(z2)->second = temp->number;
	}
	struct node* temp1 = curr->parent;	
	struct node* temp2 = temp->parent;
	if(temp1==temp2)
	{
		temp1->left = temp;
		temp1->right = curr;
	}
	else
	{
		if(temp1->left==curr)
		{
			temp1->left = temp;
			temp->parent = temp1;
		}
		else
		{
			temp1->right = temp;
			temp->parent = temp1;
		}
		if(temp2->left==temp)
		{
			temp2->left = curr;
			curr->parent = temp2;
		}
		else
		{
			temp2->right = curr;
			curr->parent = temp2;
		}
	}
}
void update(struct node* curr)
{
	if(root==curr)
	{
		curr->value += 1;
		return;
	}
	struct node* temp = NULL;
	int lim;
	if(curr->parent->left==curr)
	{
		lim = curr->number+1;
	}
	else
	{
		lim = curr->parent->number;
	}
	for(int i=0;i<=lim;i++)
	{
		struct node *check = getnode.find(i)->second;
		if(check!=curr&&check->value==curr->value)
		{
			temp = getnode.find(i)->second;
			break;
		}
	}
	if(temp==NULL)
	{
		curr->value += 1;                         
		update(curr->parent);
	}
	else
	{
		if(temp==curr->parent)
		{
			curr->value += 1;
			update(curr->parent);
		}
		else
		{
			swap(curr,temp);
			curr->value += 1;
			update(curr->parent);									
		}	
	}			
}
int main(int argc,char *argv[])
{
	if(argc==1)
    {
        cout << "Please provide input file followed by the output file" << endl;
        return 0;
    }
    present.clear();
    prefix.clear();
    getnode.clear();
    char c;
    nyt = newnode('*');
	nyt->value = 0;
	nyt->number = node_number;
	node_number++;
	root = nyt;
	setprefixes();
    ifstream in(argv[1]);
    ofstream out(argv[2]);
    string line;
    int compressed_length = 0;
    int bitcount = 1;
    char byte = 0b00100000;
    clock_t start = clock();
    while(getline(in,line))
    {
    	line += '\n';
		for(int i=0;i<line.length();i++)
		{
			c = line[i];
			original += 8;
			string code;
			if(present.find(c)==present.end())
			{
				code = getcode(nyt);			
				code += prefix.find(c)->second;
				int num = add(c);
				present.insert(make_pair(c,num));
				struct node* temp = getnode.find(num)->second;
				update(temp);
			}
			else
			{ 
				int num = present.find(c)->second;
				struct node* temp = getnode.find(num)->second;
				code = getcode(temp);
				update(temp);
			}
			compressed_length += 8;
			for(int j=0;j<code.length();j++)
			{
				bool bit;
				if(code[j]=='1')
				{
					bit = true;
				}
				else 
				{
					bit = false;
				}
				if(bitcount==2)
				{
					byte = byte << 1 | true;
					bitcount++;
				}
				byte = byte << 1 | bit;
				bitcount++;  
				if(bitcount==8)
				{
					out << byte;
					compressed += 8;
					bitcount = 1;
					byte = 0b00100000;
				}
			}
		}
     }
     out << byte;
     out << bitcount;
     compressed += 16;
     clock_t end = clock();
     in.close();
     out.close();
     double time = double(end-start);
     printf("The Runtime is %0.2f\n",time);
     printf("The original file size is %d bytes\n",original/8);
     printf("The compressed file size is %d bytes\n",compressed/8);
     double ratio = (1.0)*compressed/original;
     printf("The compression ratio is %lf\n",(1-ratio)*100);
     return 0;
}
