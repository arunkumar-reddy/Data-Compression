#include <bits/stdc++.h>
#include <Magick++.h>
using namespace std;
using namespace Magick;
int height,width;
struct node
{
    int alphabet;
    int frequency;
    struct node *left;
    struct node *right;
};
struct node* newnode(int a,int b)
{
    struct node* temp = (struct node*)malloc(sizeof(struct node));
    temp->alphabet = a;
    temp->frequency = b;
    temp->left = NULL;
    temp->right = NULL;
    return temp;
}
class compare
{
	public:
    	bool operator()(struct node* n1,struct node* n2) {
        	return n1->frequency>n2->frequency;
    	}
};
priority_queue<struct node*,vector<struct node* >,compare> q;
inline string getinput(string file)
{
    string input;
    input.clear();
    ifstream in(file.c_str());
    char c;
    while(in.get(c))
    {
        input += c;
    }
    string bits = "";
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
                bits = bits+'1';
            }
            else
            {
                bits = bits+'0';
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
            count--;
        }
        bool bit = (input[last] >> j)&1;
        if(bit)
        {
            bits = bits+'1';
        }
        else
        {
            bits = bits+'0';
        }
        count--;
    }
    return bits;
}
void form_codes(struct node *root,string code,map<string,int> &codes)
{
    if(root->left!=NULL)
    {
        form_codes(root->left,code+'0',codes);
    }
    if(root->right!=NULL)
    {
        form_codes(root->right,code+'1',codes);
    }
    if(root->left==NULL&&root->right==NULL)
    {
        codes.insert(make_pair(code,root->alphabet));
    }
}
inline vector<int> decodeAC(string file)
{
    vector<int> AC;
    AC.clear();
    string input = getinput(file+"ac.txt");
    string tree = file+"actree.txt";
    ifstream in(tree.c_str());
    map<string,int> codes;
    map<int,int> frequencies;
    codes.clear();
    frequencies.clear();
    char c;
    string freq = "";
    while(in.get(c))
    {
        freq += c;
    }
    in.close();
    int index = 0;
    while(index<freq.length())
    {
        string a = "",b = "";
        while(freq[index]!=' ')
        {
            a += freq[index];
            index++;
        }
        index++;
        while(freq[index]!=' ')
        {
            b += freq[index];
            index++;
        }
        index++;
        int alpha = atoi(a.c_str());
        int fr = atoi(b.c_str());
        frequencies.insert(make_pair(alpha,fr));
    }
    map<int,int>::iterator it;
    for(it=frequencies.begin();it!=frequencies.end();it++)
    {
        struct node* temp = newnode(it->first,it->second);
        q.push(temp);
    }
    while(q.size()!=1)
    {
        struct node *l = q.top();
        q.pop();
        struct node *r = q.top();
        q.pop();
        int sum = l->frequency + r->frequency;
        struct node *temp = newnode(INT_MAX,sum);
        temp->left = l;
        temp->right = r;
        q.push(temp);
    }
    struct node *root = q.top();
    q.pop();
    string code = "";
    form_codes(root,code,codes);
    index = 0;
    code = "";
    while(index<input.length())
    {
        code += input[index];
        index++;
        map<string,int>::iterator it = codes.find(code);
        if(it!=codes.end())
        {
            AC.push_back(it->second);
            code = "";
        }
    }
    return AC;
}
inline vector<int> decodeDC(string file)
{
    vector<int> DC;
    DC.clear();
    string input = getinput(file+"dc.txt");
    string tree = file+"dctree.txt";
    ifstream in(tree.c_str());
    map<string,int> codes;
    map<int,int> frequencies;
    codes.clear();
    frequencies.clear();
    char c;
    string freq = "";
    while(in.get(c))
    {
        freq += c;
    }
    in.close();
    int index = 0;
    while(index<freq.length())
    {
        string a = "",b = "";
        while(freq[index]!=' ')
        {
            a += freq[index];
            index++;
        }
        index++;
        while(freq[index]!=' ')
        {
            b += freq[index];
            index++;
        }
        index++;
        int alpha = atoi(a.c_str());
        int fr = atoi(b.c_str());
        frequencies.insert(make_pair(alpha,fr));
    }
    map<int,int>::iterator it;
    for(it=frequencies.begin();it!=frequencies.end();it++)
    {
        struct node* temp = newnode(it->first,it->second);
        q.push(temp);
    }
    while(q.size()!=1)
    {
        struct node *l = q.top();
        q.pop();
        struct node *r = q.top();
        q.pop();
        int sum = l->frequency + r->frequency;
        struct node *temp = newnode(INT_MAX,sum);
        temp->left = l;
        temp->right = r;
        q.push(temp);
    }
    struct node *root = q.top();
    q.pop();
    string code = "";
    form_codes(root,code,codes);
    index = 0;
    code = "";
    int prev = 0;
    while(index<input.length())
    {
        code += input[index];
        index++;
        map<string,int>::iterator it = codes.find(code);
        if(it!=codes.end())
        {
            DC.push_back(it->second+prev);
            prev = it->second;
            code = "";
        }
    }
    return DC;
}
inline vector<vector<int> > decode(int width,int height,string file)
{
    vector<vector<int> > result(height,vector<int> (width));
    vector<int> AC = decodeAC(file);
    vector<int> DC = decodeDC(file);
    int index1 = 0;
    int index2 = 0;
    for(int i1=0;i1<height;i1=i1+8)
    {
        for(int j1=0;j1<width;j1=j1+8)
        {
            vector<vector<int> > block(8,vector<int> (8));
            block[0][0] = DC[index2];
            index2++;
            int i2 = 0,i = 0;
            int j2 = 0,j = 0;
            int flag = 0;
            while(i2<8&&j2<8)
            {
                for(i=i2,j=j2;i>=0&&j<8;--i,++j)
                {
                    if(AC[index1]==INT_MAX)
                    {
                        flag = 1;
                    }
                    if(flag!=1)
                    {
                        block[i][j] = AC[index1];
                        index1++;
                    }
                    else
                    {
                        block[i][j] = 0;
                    }   
                }
                if(j>=8)
                {
                    i2=i+2;
                    j2=j-1;
                }
                else
                {
                    i2=i+1;
                    j2=j;
                }
                for(i=i2,j=j2;i<8&&j>=0;++i,--j)
                {
                    if(AC[index1]==INT_MAX)
                    {
                        flag = 1;
                    }
                    if(flag!=1)
                    {
                        block[i][j] = AC[index1];
                        index1++;
                    }
                    else
                    {
                        block[i][j] = 0;
                    }
                }
                if(i>=8)
                {
                    i2=i-1;
                    j2=j+2;
                }
                else
                {
                    i2=i;
                    j2=j+1;
                }
            }
            index1++;
            for(int i=i1;i<i1+8;i++)
            {
                for(int j=j1;j<j1+8;j++)
                {
                    result[i][j] = block[i-i1][j-j1] ;
                }
            }
        }
    }
    return result;
}
int quantize[8][8]  =   {{16,11,10,16,24,40,51,61},
                    {12,12,14,19,26,58,60,65},
                    {14,13,16,24,40,57,69,56},
                    {14,17,22,29,51,87,80,62},
                    {18,22,37,56,68,109,103,77},
                    {24,35,55,64,81,104,113,92},
                    {49,64,78,87,103,121,120,101},
                    {72,92,95,98,112,100,103,99}};
inline void print(vector<vector<int> > &block)
{
    for(int i=0;i<8;i++)
    {
        for(int j=0;j<8;j++)
        {
            cout << block[i][j] << " ";
        }
        cout << endl;
    }
    cout << endl;
}
inline vector<vector<int> > idct(vector<vector<int> > &block)
{
    vector<vector<int> > result(8,vector<int> (8));
    for(int i=0;i<8;i++)
    {
        for(int j=0;j<8;j++)
        {
            block[i][j] = block[i][j]*(quantize[i][j]);
        }
    }
    for(int i=0;i<8;i++)
    {
        for(int j=0;j<8;j++)
        {
            double temp = 0;
            for(int x=0;x<8;x++)
            {
                for(int y=0;y<8;y++)
                {
                    double alpha1 = 0.5,alpha2 = 0.5;
                    if(x==0)
                    {
                        alpha1 = 1.0/sqrt(8);
                    }
                    if(y==0)
                    {
                        alpha2 = 1.0/sqrt(8);
                    }
                    double c1 = (2*i+1)*x*M_PI;
                    double c2 = (2*j+1)*y*M_PI;
                    temp += alpha1*alpha2*cos((1.0*c1)/16)*cos((1.0*c2)/16)*block[x][y];
                }
            }
            result[i][j] = round(temp);
        }
    }
    for(int i=0;i<8;i++)
    {
        for(int j=0;j<8;j++)
        {
            result[i][j] += 128; 
        }
    }
    return result;
}
inline vector<vector<int> > decompress(int width,int height,vector<vector<int> > &pixels)
{
    vector<vector<int> > result(height,vector<int>(width));
    for(int i1=0;i1<height;i1=i1+8)
    {
        for(int j1=0;j1<width;j1=j1+8)
        {
            vector<vector<int> > block(8,vector<int>(8));
            for(int i=i1;i<i1+8;i++)
            {
                for(int j=j1;j<j1+8;j++)
                {
                    block[i-i1][j-j1] = pixels[i][j];
                }
            }
            vector<vector<int> > transformed = idct(block);
            for(int i=i1;i<i1+8;i++)
            {
                for(int j=j1;j<j1+8;j++)
                {
                    result[i][j] = transformed[i-i1][j-j1];
                }
            }
        }
    }
    return result;
}
int main(int argc,char *argv[])
{
    clock_t begin = clock();
    if(argc==1)
    {
        cout << "please specify the output file name as a .jpg image" << endl;
        return 0;
    }
    ifstream in("size.txt");
    in >> width >> height;
    in.close();
    int extra_h = height%8,extra_w = width%8;
    vector<vector<int> > Y_transform = decode(width+extra_w,height+extra_h,"y");
    vector<vector<int> > Y = decompress(width+extra_w,height+extra_h,Y_transform);
    vector<vector<int> > Cb_transform = decode(width+extra_w,height+extra_h,"Cb");
    vector<vector<int> > Cb = decompress(width+extra_w,height+extra_h,Cb_transform);
    vector<vector<int> > Cr_transform = decode(width+extra_w,height+extra_h,"Cr");
    vector<vector<int> > Cr = decompress(width+extra_w,height+extra_h,Cr_transform);
    Image image(Geometry(width,height),"white");
    PixelPacket *pixels = image.getPixels(0,0,width,height);
    for(int i=0;i<height;i++)
    {
        for(int j=0;j<width;j++)
        {
            int red = Y[i][j] + 1.402*(Cr[i][j]-128);
            int green = Y[i][j] - 0.344136*(Cb[i][j]-128) - 0.714136*(Cr[i][j]-128);
            int blue = Y[i][j] + 1.772*(Cb[i][j]-128);
            unsigned char r = (unsigned char)red;
            unsigned char g = (unsigned char)green;
            unsigned char b = (unsigned char)blue;
            pixels[i*width+j] = Color(256*r,256*g,256*b);
            image.syncPixels();
        }
    }
    image.write(argv[1]);
    clock_t end = clock();
    double elapsed = double(end-begin)/CLOCKS_PER_SEC;
    cout << "Decompression time = " << elapsed << endl;
    return 0;
}