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
int t = 0;
int n = 7,node_number = 0;
string line,input;
struct node *nyt,*root;
map<string,char> prefix;
map<int,struct node* > bind;
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
};
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
		prefix.insert(make_pair(getbinary(i),(char)i));
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
	bind.find(merge->number)->second = merge;
	bind.insert(make_pair(nyt->number,nyt));
	bind.insert(make_pair(symbol->number,symbol));
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
int isnyt(struct node* curr,string code)
{
	if(curr==nyt)
	{
		return 1;
	}
	if(code[t]=='0')
	{
		if(curr->left==NULL)
		{
			return 0;
		}
		t++;
		return isnyt(curr->left,code);
	}
	else if(code[t]=='1')
	{
		if(curr->right==NULL)
		{
			return 0;
		}
		t++;
		return isnyt(curr->right,code);
	}
}
char search(struct node* curr,string code)
{
	if(curr->symbol!='*')
	{
		return curr->symbol;
	}
	if(code[t]=='0')
	{
		t++;
		return search(curr->left,code);	
	}
	else if(code[t]=='1')
	{
		t++;
		return search(curr->right,code);
	}
}
void swap(struct node* curr,struct node* temp)
{
	bind.find(curr->number)->second = temp;
	bind.find(temp->number)->second = curr;
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
		struct node *check = bind.find(i)->second;
		if(check!=curr&&check->value==curr->value)
		{
			temp = bind.find(i)->second;
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
    bind.clear();
    char c;
    nyt = newnode('*');
	nyt->value = 0;
	nyt->number = node_number;
	node_number++;
	root = nyt;
	setprefixes();
    ifstream in(argv[1]);
    ofstream out(argv[2]);
    while(getline(in,input))
    {
    	for(int i=0;i<input.length()-2;i++)
    	{
    		for(int j=6;j>=0;j--)
    		{
    			if(j==5)
    			{
    				j--;
    			}
    			bool bit = (input[i] >> j)&1;
    			if(bit)
    			{
    				line = line+'1';
    			}
    			else
    			{
    				line = line+'0';
    			}
    		} 
    	}
    	int last = input.length()-1;
    	int count = (int)input[last]-'0';
    	last--;
    	count--;
    	for(int j=6;j>=0;j++)
    	{
    		if(count==0)
    		{
    			break;
    		}
    		if(j==5)
    		{
    			j--;
    		}
    		bool bit = (input[last] >> j)&1;
    		if(bit)
    		{
    			line = line+'1';
    		}
    		else
    		{
    			line = line+'0';
    		}
    		count--;
    	}
    	while(t<line.length()-1)
		{
			int k = t;
			char ans;
			if(isnyt(root,line))
			{
				string code = "";
				for(int i=0;i<7;i++)
				{
					code += line[t];
					t++;
				}
				ans = prefix.find(code)->second;
				int num = add(ans);
				present.insert(make_pair(ans,num));
				struct node* temp = bind.find(num)->second;
				update(temp);
			}
			else
			{	
				t = k;
				ans = search(root,line); 
				int num = present.find(ans)->second;
				struct node* temp = bind.find(num)->second;
				update(temp);
			}
			out << ans;
		}
     }
     return 0;
}
