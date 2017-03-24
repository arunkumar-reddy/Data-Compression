#include <bits/stdc++.h>
using namespace std;
struct node
{
    char alphabet;
    int frequency;;
    struct node *left;
    struct node *right;
};
int frequency[100];
char alphabet[100];
string huffman_codes[100];
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
map<char,int> present;
struct node* newnode(char a,int b)
{
    struct node* temp = (struct node*)malloc(sizeof(struct node));
    temp->alphabet = a;
    temp->frequency = b;
    temp->left = NULL;
    temp->right = NULL;
    return temp;
}
void form_codes(struct node *root,string code)
{
    if(root->left!=NULL)
    {
	form_codes(root->left,code+'0');
    }
    if(root->right!=NULL)
    {
	form_codes(root->right,code+'1');
    }
    if(root->left==NULL&&root->right==NULL)
    {
	int index = present.find(root->alphabet)->second;
	huffman_codes[index] = code;
    }
}
void print_codes(int n)
{
    for(int i=0;i<n-1;i++)
    {
	printf("%c %d ",alphabet[i],frequency[i]);
	cout << huffman_codes[i] << endl;
    }
}
int main()
{
	present.clear();
	FILE* fp = fopen("input.txt","r");
	char c;
	int n = 0;
	while(fscanf(fp,"%c",&c)!=EOF)
	{
		if(present.find(c)==present.end())
		{
			present.insert(make_pair(c,n));
			alphabet[n] = c;
			frequency[n] = 1;
			n++;
		}
		else
		{
			int temp = present.find(c)->second;
			frequency[temp]++;
		}
	}
	fclose(fp);
	for(int i=0;i<n-1;i++)
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
	string code = "";
	form_codes(root,code);
	fp = fopen("input.txt","r");
	FILE *out = fopen("minvariance_output.txt","w");
	while(fscanf(fp,"%c",&c)!=EOF)
	{
		int index = present.find(c)->second;
		for(int i=0;i<huffman_codes[index].length();i++)
	    {
			fprintf(out,"%c",huffman_codes[index][i]);
	    }
	}
	fclose(fp);
	fclose(out);
	int original = 0,compressed = 0;
	for(int i=0;i<n-1;i++)
	{
	    original += frequency[i]*8;
		compressed += frequency[i]*huffman_codes[i].length();
	}
	out = fopen("huffman_tree.txt","w");
	for(int i=0;i<n-1;i++)
	{
		fprintf(out,"%c ",alphabet[i]);
		fprintf(out,"%d\n",frequency[i]);
	}
	printf("The original file size is %d bytes\n",original/8);
	printf("The compressed file size is %d bytes\n",compressed/8);
	double ratio = (1.0)*compressed/original;
	printf("The compression ratio is %lf\n",(1-ratio)*100);
	return 0;
}
