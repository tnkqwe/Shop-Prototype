#include <iostream>
using namespace std;

template <typename T>
class DynamicArray
{
private:
	T* element;
	int currSize;
	int absSize;
	void resize ()
	{
		T *temp = new T[absSize * 2];
		for (int c = 0; c < currSize; c++)
		{
			temp[c] = element[c];
		}
		delete element;
		element = temp;
		absSize = absSize * 2;
		//cout<<"Resizing complete!\n";
	}
	void shorten ()
	{
		T* temp = new T[absSize / 2];
		for (int c = 0; c < currSize; c++)
		{
			temp[c] = element[c];
		}
		delete element;
		element = temp;
		absSize = absSize / 2;
		//cout<<"Shorteing complete!\n";
	}
	string name;
public:
	DynamicArray (int initialSize)//обикновен
	{
		currSize = 0;
		element = new T[initialSize];
		absSize = initialSize;
		name = "Not set!";
		//cout<<"Parameterized constructor complete!\n";
	}
	DynamicArray ()//подразбиране
	{
		currSize = 0;
		element = new T[2];
		absSize = 2;
		name = "Not set!";
		//cout<<"Default constructor complete!\n";
	}
	DynamicArray (DynamicArray<T>& arr)//копиране
	{
		currSize = arr.currSize;
		element = new T[arr.currSize];
		absSize = arr.currSize;
		for (int c = 0; c < currSize; c++)
		{
			element[c] = arr.element[c];
		}
		//cout<<"Copy constructor complete!\n";
	}
	~DynamicArray ()//деструктор
	{
		delete element;
		//cout<<"Destructor complete!\n";
	}
	void setName (string name)
	{
		this->name = name;
	}
	void add(T n)//прибавяне на елемент в края
	{
		if (currSize >= absSize)
			resize();
		element[currSize] = n;
		currSize++;
	}
	void deleteEl(int id)//махане на елемент
	{
		for (int c = id; c < currSize - 1; c++)
		{
			element[c] = element[c + 1];
		}
		currSize--;
		if (currSize <= absSize / 4)
			shorten();
	}
	T top()//стойност на последен елемент
	{
		return element[currSize - 1];
	}
	int size()//дължина
	{
		return currSize;
	}
	int absoluteSize()//дължина
	{
		return absSize;
	}
	T& operator [] (int id)
	{
		if (id < currSize)
			return element[id];
		else
		{
			cout<<"["<<name<<"]: Index out of range or negative! Returning element[0]\n";
			return element[0];
		}
	}
	T returnElement(int id)
	{
		if (id < currSize)
			return element[id];
		else
		{
			cout<<"Index out of range or negative! Returning element[0]\n";
			return element[0];
		}
	}
	void changeElementOnIndex (int id, T newVal)
	{
		if (id <= currSize)
			element[id] = newVal;
		else
			cout<<"Index out of range!\n";
	}
	void print()//принтиране
	{
		for (int c = 0; c < currSize; c++)
			cout<<element[c]<<" ";
		cout<<endl;
	}
	bool isEmpty()//празен ли е
	{
		if (currSize == 0)
			return true;
		else
			return false;
	}
};
