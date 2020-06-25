#include<iostream>
#include<string>
#include<fstream>
#include<vector>
#include<bitset>
#include <cstdio> 
#include<sstream>
#include<string>
#define COUNT 10
using namespace std;
struct HuffMan_number {
	char character;
	int number;
	char code[128]={'\0'};
};
struct HNode {
	//Sinh viên đề xuất phần lưu trữ dữ liệu của nút trong cây
	char data[128];
	int number;
	//Gợi ý:
	//unsigned char character; //Lưu trữ ký tự cần lưu
	//int frequency; //Lưu trữ tần số xuất hiện của ký tự
	HNode* left; //Con trỏ chỉ sang nút trái của cây
	HNode* right; //Con trỏ chi sang nút phải của cây
};

void inputNumber_of_char(vector<HuffMan_number>& number, char* sample);
int exist(vector<HuffMan_number> array, char x);
int exist(char* str, char x);
void print_numberlist(vector<HuffMan_number> ar);
HNode* CreateHNode(vector<HuffMan_number> array);
void insertHNode(vector<HNode*> &array,HNode*x);
void print2D(HNode* root);
void print2DUtil(HNode* root, int space);
void parent_data(HNode* parent, HNode* a, HNode* b);
void get_code(vector<HuffMan_number>& array);
void print_HuffManCode(vector<HuffMan_number> ar);
void write_code(string FileName, vector<HuffMan_number> array,char* source);
int find_code(char x, vector<HuffMan_number> array);
void OnBit(unsigned char& byte, int position);
void OffBit(unsigned char& byte, int position);
void writeHuffManTree(string FileName,vector<HuffMan_number> array);
void DeCode_HuffManFile(string inputFile, string outputFile);


int exist(vector<HuffMan_number> array, char x)
{
	for (int i = 0; i < array.size(); i++)
	{
		if (array[i].character == x)
			return i;
	}
	return -1;
}
int exist(char* str, char x) 
{
	for (int i = 0; i < strlen(str); i++)
	{
		if (x == str[i])
			return 1;
	}
	return 0;
}
//void inputNumber_of_char(vector<HuffMan_number>& number, char* sample)
//{
//	int i = 0;
//	while (sample[i] != '\0')
//	{
//		int pos;
//		if ((pos=exist(number, sample[i]))>=0)
//		{
//			number[pos].number++;
//		}
//		else
//		{
//			HuffMan_number temp;
//			temp.character = sample[i];
//			temp.number = 1;
//			number.push_back(temp);
//		}
//		i++;
//	}
//}
void input_array(vector<HuffMan_number>& number, string FileName)
{
	fstream input;
	input.open(FileName, ios::in);
		char ch;
	while (input.get(ch))
	{
		int pos;
		if ((pos = exist(number, ch))>=0)
		{
			number[pos].number++;
		}
		else
		{
			HuffMan_number temp;
			temp.character = ch;
			temp.number = 1;
			number.push_back(temp);
		}
	}
	input.close();
}
void print_numberlist(vector<HuffMan_number> ar)
{
	for (int i = 0; i < ar.size(); i++)
	{
		cout << ar[i].character << "	" << ar[i].number << endl;
	}
}
void sort(vector<HuffMan_number> &ar)
{
	for (int i = 0; i < ar.size()-1; i++)
	{
		for(int j = i+1; j<ar.size();j++)
			if (ar[i].number < ar[j].number)
			{
				HuffMan_number temp = ar[i];
				ar[i] = ar[j];
				ar[j] = temp;
			}
	}
}
HNode* CreateHNode(vector<HuffMan_number> data_array)
{
	vector<HNode*> node_array(data_array.size());
	for (int i = 0; i < data_array.size(); i++)
	{
		node_array[i] = new HNode();
		node_array[i]->data[0] = data_array[i].character;
		node_array[i]->number = data_array[i].number;
	}
	
	while (node_array.size()>1)
	{
		//Create parent node of 2 least appear node
		HNode* a = node_array.back();
		node_array.pop_back();
		HNode* b = node_array.back();
		node_array.pop_back();
		HNode* parent = new HNode();
		parent->left = a;
		parent->right = b;
		parent->number = a->number + b->number;
		parent_data(parent, a, b);
		insertHNode(node_array, parent);
	}
	return node_array[0];
}
void insertHNode(vector<HNode*>& array,HNode* x)
{
	array.push_back(x);
	int i = array.size() - 1;
	while (i!=0&&array[i]->number >= array[i - 1]->number)
	{
		HNode* temp = array[i];
		array[i] = array[i - 1];
		array[i - 1] = temp;
		i--;
	}
}
void print2D(HNode* root)
{
	// Pass initial space count as 0  
	print2DUtil(root, 0);
}
void print2DUtil(HNode* root, int space)
{
	// Base case  
	if (root == NULL)
		return;

	// Increase distance between levels  
	space += COUNT;

	// Process right child first  
	print2DUtil(root->right, space);

	// Print current node after space  
	// count  
	cout << endl;
	for (int i = COUNT; i < space; i++)
		cout << " ";
	cout << root->data <<"_"<<root->number<< "\n";

	// Process left child  
	print2DUtil(root->left, space);
}
void parent_data(HNode* parent, HNode* a, HNode* b)
{
	int i = 0;
	for (int j = 0; a->data[j] != '\0'; j++)
	{
		parent->data[i++] = a->data[j];
	}
	for (int j = 0; b->data[j] != '\0'; j++)
	{
		parent->data[i++] = b->data[j];
	}
}
void get_code(vector<HuffMan_number>& array)
{
	HNode* root = CreateHNode(array);
	for (int i = 0; i < array.size(); i++)
	{
		HNode *temp = root;
		while (strlen(temp->data)>1)
		{
			if (exist(temp->left->data, array[i].character)) {
				temp = temp->left;
				strcat(array[i].code, "0");
			}
			else
			{
				temp = temp->right;
				strcat(array[i].code, "1");
			}
		}
	}
}
void print_HuffManCode(vector<HuffMan_number> ar)
{
	for (int i = 0; i < ar.size(); i++)
	{
		cout << ar[i].character << "	" << ar[i].code << endl;
	}
}
int find_code(char x, vector<HuffMan_number> array)
{
	for (int i = 0; i < array.size(); i++)
	{
		if (array[i].character == x)
			return i;
	}
	cout << "find_code error !" << endl;
	return -1;
}
void write_code(string FileName_out, vector<HuffMan_number> array,string FileName_in)
{
	fstream input;
	fstream output;
	input.open(FileName_in, ios::in);
	output.open("Binary.txt", ios::out|ios::trunc|ios::binary);
	// copy binary string to a file
	int binary_length = 0;
	while(!input.eof())
	{	
		char ch;
		input.get(ch);
	
		int code_index = find_code(ch, array);
		output << array[code_index].code;
		binary_length += strlen(array[code_index].code);
	}
	if ((binary_length % 8) > 0) {
		for (int i = 8-(binary_length % 8); i > 0; i--)
		{
			output << '0';
			binary_length++;
		}
	}
	output.close();
	input.close();
	output.open(FileName_out, ios::out|ios::app|ios::binary);
	input.open("Binary.txt");
	unsigned char temp;
	for (long j = 0; j < binary_length; )
	{
		for (int i = 0; i < 8; i++) //get 8 bits
		{
			char bin = input.get();
			
			if (bin == '0')
			{
				OffBit(temp, 7-i);
			}
			else if (bin == '1')
			{
				OnBit(temp, 7-i);
			}
			j++;
		}
			output << temp;
	}
	input.close();
	output.close();
}
void OnBit(unsigned char& byte, int position)
{
	byte = byte | (1 << position);
}
void OffBit(unsigned char& byte, int position)
{
	byte = byte & ~(1 << position);
}
void writeHuffManTree(string FileName,vector<HuffMan_number> array)
{
	fstream output;
	output.open(FileName,ios::out|ios::trunc|ios::binary);
	output << array.size()<<char(16);
	for (int i = 0; i < array.size(); i++)
	{
		output << array[i].character << char(16) << array[i].number << char(16);
	}
	output.close();
}
void DeCode_HuffManFile(string inputFile, string outputFile)
{
	fstream input;
	fstream output;
	output.open(outputFile, ios::trunc|ios::out | ios::binary);
	input.open(inputFile,ios::in|ios::binary);
	int size = output.tellg();
	vector<HuffMan_number> array;
	int n;
	long number_of_char = 0;
	input >> n;
	input.ignore();
	array.resize(n);
	for (int i = 0; i < n; i++)
	{
		input.get( array[i].character);
		input.ignore();
		input >> array[i].number;
		input.ignore();
		number_of_char += array[i].number;
	}
	HNode* root = CreateHNode(array);
	HNode* temp = root;
	for (int i = 0;(number_of_char>0);)
	{
		char ch;
		input.get(ch);
		bitset<8> bin(ch);
		for (int j = 0; j < 8;)
		{
			while (!(temp->left == NULL && temp->right == NULL))
			{
				
				if (bin[7 - j] == 0) {
					temp = temp->left;
				}
				else {
					temp = temp->right;
				}
				j++;
				if (j >= 8)
					break;
			}
			if (j <= 7) // chua doc het day bit cua ch -> da doc duoc chu can output -> xuat ra file
			{
				cout << temp->data;
				output << temp->data;
				temp = root;
				number_of_char--;
				if (number_of_char == 0)
					break;
			}
		}
	}
		//test file binary
	/*	fstream binary;
		binary.open("Binary.txt",ios::in);
		for (int i = 0; (number_of_char > 0);)
		{
			char ch_1;
			while (!(temp->left == NULL && temp->right == NULL))
			{
				binary.get(ch_1);
				if (ch_1=='0')
					temp = temp->left;
				else
					temp = temp->right;
				
				
			}
				cout << temp->data;
				output << temp->data;
				temp = root;
				number_of_char--;
				if (number_of_char == 0)
					break;
		}*/
	input.close();
	output.close();
}
void main()
{
	cout<<"tuan anh be de"<<endl;
	string FileName = "nguoidep.txt";
	vector<HuffMan_number> number;
	input_array(number, FileName);
	sort(number);
	print_numberlist(number);
	HNode* root;
	root = CreateHNode(number);
	cout << "----------------------------------" << endl;
	get_code(number);
	string FileName_out = "nguoidep.huf";
	writeHuffManTree(FileName_out, number);
	write_code(FileName_out, number, FileName);
	DeCode_HuffManFile(FileName_out, "TrucAnh.txt");//decode
}
