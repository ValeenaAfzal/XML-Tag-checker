#include<iostream>
#include<fstream>
#include<string>
#include<cstring>
#include<iomanip>

using namespace std;

template <class T>
class Stack;

template <class T>
class Node
{
public:
	T data;
	Node<T>* Next;
	Node()
	{
		data = {};
		Next = NULL;
	}
};

template <class T>
class Stack
{
private:
	Node<T>* Top;
public:
	Stack()
	{
		Top = NULL;
	}

	~Stack()
	{
		if (!IsEmpty())// not empty
		{
			Node<T>* temp = Top;

			while (Top)
			{
				Node<T>* t = Top->Next;
				Top->Next = NULL;
				delete Top;
				Top = t;
			}
		}
	}

	Stack(Stack& rhs)// copy constructor
	{
		Node<T>* temp = new Node<T>();// new poinetr
		Top = temp;
		Top->Next = temp->Next = NULL;
		Node<T>* temp2 = rhs.Top;

		for (; temp2 != NULL; temp = temp->Next, temp2 = temp2->Next)// deep copy
		{
			temp->data = temp2->data;
			if (temp2->Next != NULL)// if end reached donot allocate more memory
				temp->Next = new Node<T>();
		}
		temp = NULL;
	}

	bool Push(const T&val)
	{
		Node<T>* temp = new Node<T>();
		temp->data = val;
		temp->Next = Top;
		Top = temp;
		return true;
	}

	bool Pop()
	{
		if (!IsEmpty())// not empty
		{
			Node<T>* temp = Top;
			Top = Top->Next;
			temp->Next = NULL;
			if(temp)
				delete temp;
			return true;
		}
		return false;
	}

	bool IsEmpty()
	{
		if (Top)
			return false;
		else
			return true;
	}

	T top()
	{
		if (!IsEmpty())
			return Top->data;
	}
	

	void Print()
	{
		if (!IsEmpty())
		{
			cout << "Stack" << endl << "____________________________" << endl << endl;
			Node<T>* temp = Top;
			for (; temp != NULL; temp = temp->Next)
				cout << temp->data << endl;
			cout<< "____________________________" << endl << endl;
		}
	}
	

};

class XMlData
{
private:
	string tag;
	int LineNumber;
	string tagData;
	Stack<int> Sline;
public:
	XMlData()
	{
		tag = {};
		tagData = {};
		LineNumber = 0;
	}

	void Prolong(string line)
	{
		int size = line.length();
		if (LineNumber == 1)
		{
			int count = 0;
			int proend = tag.find("?>");
			if (proend == -1)// if no end
			{
				cout << "\n		--ERROR-- \n Prolong Is Missing: ?> \n";
				cout << "Line Number:	" << LineNumber << endl;
				cout << "_______________________________________________\n\n";
			}
			while (size > 2)
			{
				int start = line.find('"');
				if (start > -1 && start < line.length())// if quotes found counter ++
					count++;
				line = line.substr(start + 1, size);
				size = line.length();
			}
			if (count % 2 != 0)// if quotes pair is not complete
			{
				cout << "\n		--ERROR-- \n Missing Quotation marks in Prolong \n";
				cout << "Line Number:	" << LineNumber << endl;
				cout << "_______________________________________________\n\n";
			}
		}
		else
			cout << "\n		--ERROR-- \n  Prolong is not in Line NUmber One \n",
			cout<<"LineNumber:	"<<LineNumber<<endl,
			cout << "_______________________________________________\n\n";
	}

	bool Comments(string line, ifstream& File)
	{
		int start = line.find("-->");
		if (start > -1 && start < line.length())
			return true;
		else
		{
			while (File.eof() == false)// keep searchin for end of cooment till file ends
			{
				line = {};
				getline(File, line, '\n');
				if (line.length())
					LineNumber++;
				int start = line.find("-->");
				if (start > -1 && start < line.length())
					return true;
			}
		}
		return false;
	}

	void Tags(string line, Stack<string> &stack)
	{
		Stack<string> TAG;
		int size = line.length();
		int end = line.find(">");
		if (end == -1)
		{
			cout<< "\n		--ERROR-- \nEnd of Tag Missing \n";
			cout << "Line Number:	" << LineNumber<<endl,
			cout << "_______________________________________________\n\n";
		}

		// create a stack and keep pushing < and if > found pop
		Stack<char> tag;
		for (int i = 0; i < size; i++)// check if <> are balanced
		{
			if (line[i] == '<')// ye check kr skty k har bar jab push hora ha to stack phly khali ho
				tag.Push('<');
			if (line[i] == '>')
				tag.Pop();
		}

		if (!tag.IsEmpty())
		{
			cout << "\n		--ERROR-- \nTags Not Balanced: \n";
			cout << "Line Number:	" << LineNumber << endl,
			cout << "_______________________________________________\n\n";
		}

		int quotescount = 0;// to count quotes attribute
		
		while(size>0)// find start end tags and push pop in stack
		{
			string copy = line;
			int startpos = line.find('<');
			int endpos = 0;

			int Space = line.find(' ');
			int Bracket = line.find('>');

			if (Space < Bracket && Space != -1 && startpos < Space)
				endpos = Space;
			else
				endpos = Bracket;


			tagData = line.substr(startpos+1, endpos-1);
			if (tagData[0] == '/')// if ending found and no start
			{
				if (tagData[tagData.length() - 1] != '>')
					tagData += '>';
				string end = tagData.substr(1,tagData.length()-2);

				if (stack.IsEmpty() == true)
					cout << "\n		--ERROR--\nStart of <" << tagData << "Missing\n" , 
					cout<<"LineNumber:	"<<LineNumber<<endl,
					cout << "_______________________________________________\n\n";
				else if (stack.IsEmpty() == false)
							if (stack.top() != end)
								cout << "\n		--ERROR--\nStart of <" << tagData << "Missing\n",
								cout << "LineNumber:	" << LineNumber << endl,
								cout << "_______________________________________________\n\n";
							else if (stack.IsEmpty() == false && end.compare(stack.top()) == 0)
								stack.Pop() , Sline.Pop();
			}
			if (Bracket == -1)
				Bracket = line.length() - 1;

			for (int i = 0; i <= Bracket; i++)
			{
				if (copy[i] == '"')
					quotescount++;
			}
			if (quotescount % 2 != 0)
				cout << "		--ERROR--\nMissing Quotes in Attribute\nLineNumber:	" << LineNumber << endl,
				cout << "_______________________________________________\n\n";


			// push start tags
			if(startpos != -1 && tagData[0] != '/')
				stack.Push(tagData) , Sline.Push(LineNumber);// if the stack is not empty initially then use other one
			line = line.substr(Bracket+1, size);
			size = line.length();// shorten and compare
		}
	}

	void ReadFile()
	{
		Stack<string> stack;
		string name ;
		cout << "Enter File Name:	";
		cin >> name;	
		ifstream File;
		File.open(name);

		if (File)// check if File exists
		{
			int comment = 0;
			while (!File.eof())// read till end
			{
				LineNumber++;
				getline(File, tag, '\n');
				cout << tag <<endl;

				string Tag = {};

				// finding starting tags in lines
				int start = tag.find("<!--");
				if (start > -1 && start < tag.length())// if  "<?" found
					Tag = "<!--";
				if (start == -1) // if "<?" not found
				{
					start = tag.find("<?");
					if (start > -1 && start < tag.length())//if "<!--" found
						Tag = "<?";
				}
				if (start == -1 )// if "<!--" not found
				{
					start = tag.find("<");
					if (start > -1 && start < tag.length())// '<' found
						Tag = "<";
				}
				
				// dependind upon start get into the respective check program
				if (Tag == "<?")
					Prolong(tag);
				else if (Tag == "<!--")// check whole doc for the end of comment if not found give error
				{
					if (!Comments(tag, File))
						cout << "\n		--ERROR-- \nEnd of File Reached End Of Comment Missing \n",
						cout << "_______________________________________________\n\n";
				}			
				else if (Tag == "<")
					Tags(tag,stack);
				
				// now check if there are only ending tags, ?> or --> in line but no starrt

				int end = tag.find(">") || tag.find("?>") || tag.find("-->");
				if (end != -1 && start == -1)
				{
					cout << "\n		--ERROR-- \nEnd of Tag Found and No start\n";
					cout << "Line Number:	" << LineNumber << endl;
					cout << "_______________________________________________\n\n";
				}
			}
			if (!stack.IsEmpty())
				while (stack.IsEmpty() != true)
				{
					cout << "		--ERROR--\n Ending tag of <" << stack.top() << "> Missing\n";
					cout << "\nLineNumber:	" << Sline.top() << endl;
					cout << "_______________________________________________\n\n";
					stack.Pop();
					Sline.Pop();
				}
		}
		File.close();
	}
};

int main()
{
	system("color 0B");
	XMlData obj;
	obj.ReadFile();
	system("pause");
	return 0;
}
