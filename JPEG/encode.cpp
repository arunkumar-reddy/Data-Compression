#include <bits/stdc++.h>
#include <Magick++.h>
using namespace std;
using namespace Magick;
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
void form_codes(struct node *root,string code,map<int,string> &codes)
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
        codes.insert(make_pair(root->alphabet,code));
    }
}
inline vector<int> getAC(vector<vector<int> > &block)
{
    vector<int> AC;
    int zero = 0;
    int i1 = 0,i = 0;
    int j1 = 0,j = 0;
    while(i1<8&&j1<8)
    {
        for(i=i1,j=j1;i>=0&&j<8;--i,++j)
        {
            if(block[i][j]==0)
            {
                zero++;
            }
            else
            {
                for(int k=0;k<zero;k++)
                {
                    AC.push_back(0);
                }
                zero = 0;
                AC.push_back(block[i][j]);
            }
        }
        if(j>=8)
        {
            i1=i+2;
            j1=j-1;
        }
        else
        {
            i1=i+1;
            j1=j;
        }
        for(i=i1,j=j1;i<8&&j>=0;++i,--j)
        {
            if(block[i][j]==0)
            {
                zero++;
            }
            else
            {
                for(int k=0;k<zero;k++)
                {
                    AC.push_back(0);
                }
                zero = 0;
                AC.push_back(block[i][j]);
            }   
        }
        if(i>=8)
        {
            i1=i-1;
            j1=j+2;
        }
        else
        {
            i1=i;
            j1=j+1;
        }
    }
    return AC;
}
inline void encodeDC(vector<int> &DC,string file)
{
    int dc_bitcount = 1;
    char dc_byte = 0b00100000;
    int prev = 0;
    string tree = file+"dctree.txt";
    file += "dc.txt";
    fstream freq(tree.c_str(),fstream::out);
    fstream out(file.c_str(),fstream::out);
    map<int,string> codes;
    map<int,int> frequencies;
    codes.clear();
    frequencies.clear();
    for(int i=0;i<DC.size();i++)
    {
        int diff = DC[i] - prev;
        prev = DC[i];
        if(frequencies.find(diff)==frequencies.end())
        {
            frequencies.insert(make_pair(diff,1));
        }
        else
        {
            frequencies.find(diff)->second += 1;
        }
    }
    map<int,int>::iterator it;
    for(it=frequencies.begin();it!=frequencies.end();it++)
    {
        freq << it->first << " " << it->second << " ";
        struct node* temp = newnode(it->first,it->second);
        q.push(temp);
    }
    freq.close();
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
    string output = "";
    prev = 0;
    for(int i=0;i<DC.size();i++)
    {
        output += codes.find(DC[i]-prev)->second;
        prev = DC[i];
    }
    for(int j=0;j<output.length();j++)
    {
        bool bit = false;
        if(output[j]=='1')
        {
            bit = true;
        }
        if(dc_bitcount==2)
        {
            dc_byte = dc_byte << 1 | true;
            dc_bitcount++;
        }
        dc_byte = dc_byte << 1 | bit;
        dc_bitcount++;  
        if(dc_bitcount==8)
        {
            out << dc_byte;
            dc_bitcount = 1;
            dc_byte = 0b00100000;
        }
    }
    out << dc_byte;
    out << dc_bitcount;
    out.close();
}
inline void encodeAC(vector<int> &AC,string file)
{
    int ac_bitcount = 1;
    char ac_byte = 0b00100000;
    string tree = file+"actree.txt";
    file += "ac.txt"; 
    fstream out(file.c_str(),fstream::out);
    fstream freq(tree.c_str(),fstream::out);
    map<int,string> codes;
    map<int,int> frequencies;
    codes.clear();
    frequencies.clear();
    for(int i=0;i<AC.size();i++)
    {
        if(frequencies.find(AC[i])==frequencies.end())
        {
            frequencies.insert(make_pair(AC[i],1));
        }
        else
       {
            frequencies.find(AC[i])->second += 1;
        }
    }
    map<int,int>::iterator it;
    for(it=frequencies.begin();it!=frequencies.end();it++)
    {
        freq << it->first << " " << it->second << " ";
        struct node* temp = newnode(it->first,it->second);
        q.push(temp);
    }
    freq.close();
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
    string output = "";
    for(int i=0;i<AC.size();i++)
    {
        output += codes.find(AC[i])->second;
    }
    for(int j=0;j<output.length();j++)
    {
        bool bit = false;
        if(output[j]=='1')
        {
            bit = true;
        }
        if(ac_bitcount==2)
        {
            ac_byte = ac_byte << 1 | true;
            ac_bitcount++;
        }
        ac_byte = ac_byte << 1 | bit;
        ac_bitcount++;  
        if(ac_bitcount==8)
        {
            out << ac_byte;
            ac_bitcount = 1;
            ac_byte = 0b00100000;
        }
    }
    out << ac_byte;
    out << ac_bitcount;
    out.close();
}
inline void encode(vector<vector<int> > &matrix,int width,int height,string file)
{
    vector<int> DC;
    vector<int> AC;
    for(int i1=0;i1<height;i1=i1+8)
    {
        for(int j1=0;j1<width;j1=j1+8)
        {
            DC.push_back(matrix[i1][j1]);
            vector<vector<int> > block(8,vector<int> (8));
            for(int i=i1;i<i1+8;i++)
            {
                for(int j=j1;j<j1+8;j++)
                {
                    block[i-i1][j-j1] = matrix[i][j];
                }
            }
            vector<int> temp = getAC(block);
            for(int i=0;i<temp.size();i++)
            {
                AC.push_back(temp[i]);
            }
            AC.push_back(INT_MAX);
        }
    }
    encodeAC(AC,file);
    encodeDC(DC,file);
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
inline vector<vector<int> > dct(vector<vector<int > > &block)
{
    vector<vector<int> > result(8,vector<int>(8));
    for(int i=0;i<8;i++)
    {
        for(int j=0;j<8;j++)
        {
            block[i][j] -= 128;
        }
    }
    for(int i=0;i<8;i++)
    {
        for(int j=0;j<8;j++)
        {
            double temp = 0;
            double alpha1 = 1.0,alpha2 = 1.0;
            if(i==0)
            {
                alpha1 = 1.0/sqrt(2);
            }
            if(j==0)
            {
                alpha2 = 1.0/sqrt(2);
            }
            for(int x=0;x<8;x++)
            {
                for(int y=0;y<8;y++)
                {
                    double c1 = (2*x+1)*i*M_PI;
                    double c2 = (2*y+1)*j*M_PI;
                    temp += cos((1.0*c1)/16)*cos((1.0*c2)/16)*block[x][y];
                }
            }
            temp *= 0.25*alpha1*alpha2;
            temp /= (quantize[i][j]);
            result[i][j] = round(temp);
        }
    }
    return result;
}
inline vector<vector<int> > compress(vector<vector<int> > &pixels,int width,int height)
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
            vector<vector<int> > transformed = dct(block);
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
        cout << "Please provide an image as input" << endl;
        return 0;
    }
    Image image;
    image.read(argv[1]);
    int width = image.columns();
    int height = image.rows();
    ofstream out("size.txt");
    out << width << " " << height;
    out.close();
    int extra_h = height%8,extra_w = width%8;
    vector<vector<int> > Y(height+extra_h,vector<int >(width+extra_w));
    vector<vector<int> > Cb(height+extra_h,vector<int >(width+extra_w));
    vector<vector<int> > Cr(height+extra_h,vector<int >(width+extra_w));
    PixelPacket *pixels = image.getPixels(0,0,width,height);
    for(int i=0;i<height;i++)
    {
        for(int j=0;j<width;j++)
        {
            int red = (int)((unsigned char)pixels[i*width+j].red);
            int green = (int)((unsigned char)pixels[i*width+j].green);
            int blue = (int)((unsigned char)pixels[i*width+j].blue);
            Y[i][j] = (0.299*red) + (0.587*green) + (0.114*blue);
            Cb[i][j] = 128 - (0.168736*red) - (0.331264*green) + (0.5*blue);
            Cr[i][j] = 128 + (0.5*red) - (0.418688*green) - (0.081312*blue);
        }
    }
    for(int i=0;i<height;i++)
    {
        for(int j=0;j<extra_w;j++)
        {
            Y[i][width+j] = Y[i][width-1];
            Cb[i][width+j] = Cb[i][width-1];
            Cr[i][width+j] = Cr[i][width-1];
        }
    }
    for(int i=0;i<width;i++)
    {
        for(int j=0;j<extra_h;j++)
        {
            Y[height+j][i] = Y[height-1][i];
            Cb[height+j][i] = Cb[height-1][i];
            Cr[height+j][i] = Cr[height-1][i];
        }
    }
    vector<vector<int> > Y_transform = compress(Y,width+extra_w,height+extra_h);
    encode(Y_transform,width+extra_w,height+extra_h,"y");
    vector<vector<int> > Cb_transform = compress(Cb,width+extra_w,height+extra_h);
    encode(Cb_transform,width+extra_w,height+extra_h,"Cb");
    vector<vector<int> > Cr_transform = compress(Cr,width+extra_w,height+extra_h);
    encode(Cr_transform,width+extra_w,height+extra_h,"Cr");
    clock_t end = clock();
    double elapsed = double(end-begin)/CLOCKS_PER_SEC;
    cout << "Compression time = " << elapsed << endl;
    return 0;
}