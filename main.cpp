#include<iostream>
#include<vector>
#include<fstream>
#include<string>
using namespace std;
class Node {
public:
	int frequency;
	Node* left;
	Node* right;
	char data;	Node(){
		data = 0;
		left = NULL;
		right = NULL;
		frequency = 0;
	}
	Node(int freq, Node* left, Node* right, char data){
		frequency = freq;
		this->left = left;
		this->right = right;
		this->data = data;
	}
};
template <typename T>
class minHeap{
	vector<T> arr;
	void Heapify(int i){
		int l = left(i);
		int r = right(i);
		int smallest = i;
		if (l < arr.size() && arr[l]->frequency < arr[i]->frequency)
			smallest = l;
		if (r < arr.size() && arr[r]->frequency < arr[smallest]->frequency)
			smallest = r;
		if (smallest != i){
			swap(arr[i], arr[smallest]);
			Heapify(smallest);
		}
	}
public:
	void Print(){
		cout << "Ch\tFreq" << endl;
		for (int i = 0; i < arr.size(); i++){
			cout << char(arr[i]->data) << "        " << arr[i]->frequency << endl;
		}
	}
	void buildMinHeap(int i) {
		while (i > 0){
			if (arr[parent(i)]->frequency > arr[i]->frequency)
                swap(arr[parent(i)], arr[i]);
			i = parent(i);
		}
	}
	int parent(int i){
		return (i) / 2;
	}
	int left(int i){
		return (2 * i);
	}
	int right(int i){
		return (2 * i + 1);
	}
	void swap(T& x, T& y){
		T temp = x;
		x = y;
		y = temp;
	}
	void insert(T value){
		arr.push_back(value);
		buildMinHeap(arr.size() - 1);
	}
	T getMin(){
		return arr[0];
	}
	T deleteMin(){
		if (arr.empty())
			cout << "Array is Empty" << endl;
		else if (arr.size() == 0)
			return NULL;
		else{
			T root = arr[0];
			arr[0] = arr[arr.size() - 1];
			arr.resize(arr.size() - 1);
			Heapify(0);
			return root;
		}
	}
	T build_Huffman(){
		while (arr.size() != 1){
			Node* left = deleteMin();
			Node* right = deleteMin();
			if (left != NULL && right != NULL){
				Node* parent = new Node(left->frequency + right->frequency, left, right, '$');
				insert(parent);
			}
		}
		Node* point = getMin();
		return point;
	}
	void R_InOrder(Node* curr){
		if (curr == nullptr)
			return;
		else{
			R_InOrder(curr->left);
			cout << curr->data << " " << curr->frequency << endl;
			R_InOrder(curr->right);
		}
	}
};
vector<Node*> calculateFrequency(string str){
	vector<Node*> frequency;
	for (int i = 0; i < str.length(); i++){
		bool flag = false;
		for (int j = 0; j < frequency.size(); j++){
			if (frequency[j]->data == str[i]){
				frequency[j]->frequency++;
				flag = true;
				break;
			}
		}
		if (!flag){
			Node* node = new Node();
			node->data = str[i];
			node->frequency = 1;
			frequency.push_back(node);
		}
	}
	return frequency;
}
bool ValidityCheck(string str1, string str2){
	if (str1 == str2)
		return true;
	return false;
}
void Character_Finder(char data, Node* curr, Node* prev, vector<int>& vect, bool& flag){
	if (curr){
		if (curr->data == data)
			flag = true;
		if (!flag)
			Character_Finder(data, curr->left, curr, vect, flag);
		if (!flag)
			Character_Finder(data, curr->right, curr, vect, flag);
	}
	if (flag){
		if (curr == prev->left)
			vect.push_back(0);
		else if (curr == prev->right)
			vect.push_back(1);
	}
}
int main(){
	ifstream file("huf.txt");
	string file_array;
	string append;
	int path = 0;
	minHeap<Node*> obj;
	while (!file.eof()){
		getline(file, append);
		file_array += append		;
	}
	vector<Node*> frequency = calculateFrequency(file_array);
	cout << "Ch\tFreq" << endl;
	for (int i = 0; i < frequency.size(); i++){
		cout << char(frequency[i]->data) << "       " << frequency[i]->frequency << endl;
	}
	cout << "----Min_Heap Creation----" << endl;
	for (int i = 0; i < frequency.size(); i++)
		obj.insert(frequency[i]);
	obj.Print();
	cout << "----Huffman_Tree Creation----" << endl;
	obj.build_Huffman();
	vector<int> new_vector;
	Node* root = obj.build_Huffman();
	obj.R_InOrder(root);
	ofstream binary;
	binary.open("encode.bin", ios::binary | ios::out);
	for (int i = 0; i < file_array.size(); i++){
		bool flag = false;
		new_vector.resize(0);
		Character_Finder(file_array[i], root, root,new_vector, flag);
		for (int j = new_vector.size() - 1; j >= 0; j--){
			cout << new_vector[j] << " ";
			binary.write((char*)&new_vector[j], sizeof(new_vector[j]));
		}
		cout << endl;
	}
	binary.close();
	Node* curr = root;
	ifstream files("encode.bin", ios::binary);
	ofstream out;
	out.open("./decode.txt");
	while (!files.eof()){
		files.read((char*)&path, sizeof(int));
		if (path == 0)
			curr = curr->left;
		else if (path == 1)
			curr = curr->right;
		if (curr->data != '$'){
			cout << curr->data;
			out << curr->data;
			curr = root;
		}
	}
	string array = file_array;
	files.close();
	bool flag = ValidityCheck(file_array, array);
	if (flag)
		cout << "\nBoth are the Same" << endl;
	else
		cout << "File Are not same " << endl;
	system("pause");
	return 0;
}
