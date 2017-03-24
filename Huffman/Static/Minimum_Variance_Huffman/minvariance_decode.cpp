#include <bits/stdc++.h>
using namespace std;
struct node
{
    char alphabet;
    int frequency;
    struct node *left;
    struct node *right;
};
int t = 0;
int frequency[100];
char alphabet[100];
class compare
{
	public:
    	bool operator()(struct node* n1,struct node* n2) {
        	if(n1->frequency==n2->frequency)
			{
				return n1->alphabet<n2->alphabet;		
			}
			return n1->frequency>n2->frequency;
		}
};
priority_queue<struct node*,vector<struct node* >,compare> q;
struct node* newnode(char a,int b)
{
    struct node* temp = (struct node*)malloc(sizeof(struct node));
    temp->alphabet = a;
    temp->frequency = b;
    temp->left = NULL;
    temp->right = NULL;
    return temp;
}
char getcode(string line,struct node* root)
{
	if(root->alphabet!='*')
	{
		return root->alphabet; 
	}
	if(line[t]=='0')
	{
		t++;
		return getcode(line,root->left);
	}
	if(line[t]=='1')
	{
		t++;
		return getcode(line,root->right);
	} 
}
void printsymbols(string line,struct node* root)
{
	FILE *fp = fopen("minvariance_test.txt","w");
	while(t!=line.length())
	{
		char code = getcode(line,root);
		fprintf(fp,"%c",code);
	}
	fclose(fp);
}
int main()
{
	FILE *fp = fopen("huffman_tree.txt","r");
	char c;
	int n = 0;
	while(fscanf(fp,"%c",&c)!=EOF)
	{
		char ch;
		int k = 0;
		fscanf(fp,"%d\n",&k);
		alphabet[n] = c;
		frequency[n] = k;
		n++;
	}
	fclose(fp);
	for(int i=0;i<n;i++)
	{
	    struct node *temp = newnode(alphabet[i],frequency[i]);
		q.push(temp);
	}
	while(q.size()!=1)
	{
		struct node *l = q.top();
		q.pop();
		struct node *r = q.top();
		q.pop();
		int sum = l->frequency + r->frequency;
		struct node *temp = newnode('*',sum);
		temp->left = l;
		temp->right = r;
		q.push(temp);
	}
	struct node *root = q.top();
	q.pop();
	fp = fopen("minvariance_output.txt","r");
	string line = "";
	while(fscanf(fp,"%c",&c)!=EOF)
	{
		line += c;
	}	
	fclose(fp);
	printsymbols(line,root);
	return 0;
}
		
			
	
