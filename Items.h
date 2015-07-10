#include <iostream>
#include <string>
using namespace std;
/*
иерархия:
Item
 |->Resource
 |->WorkTool
 |   |->Modification
 |   |->Equipment не съм сигурен дали следващите ще са нужни, почти идентични са според работата си, за сега ги няма в кода
 |       |->Weapon
 |       |->Tool
 |       |->Armor
 |->Person
     |->Reasearcher
     |->Worker
*/

class Item
{
private:
protected:
	double price;//това са парите които трябва да се платят
public:
	double basePrice;//оригинална/складна цена
	char type;
	char tType;//каква версия на нещото, един вид type of type
	Item ()
	{
		basePrice = -1;
		type = 'n';
		calcPrice(1, 1);
	}
	Item (int basePrice, char type)
	{
		this->basePrice = basePrice;
		this->type = type;
		calcPrice(1, 1);
	}
	void calcPrice (int repIndex, int valueIndex) //модифицира price- нека не минава през този процес прекалено много
	{
		price = basePrice +
			valueIndex / repIndex +
			basePrice / repIndex;//там нещо си за пресмятане
	}
	double getPrice ()
	{
		return price;
	}
	virtual void printInfo ()
	{
		cout<<"Item: ";
		if (type == 'r')
			cout<<"resource\n";
		else if (type == 'm')
			cout<<"modification\n";
		else if (type == 'p')
			cout<<"person\n";
		else
			cout<<"something...\n";
		cout<<"Price: "<<price<<endl;
		cout<<"Base price: "<<basePrice<<endl;
	}
};
class Resource : public Item
{
private:
	double totalPrice;
public:
	//char rType; //resource type- на негово място е универсалното tType
	int quantity;
	Resource ()
	{
		setStuff(' ', 0, 0);
	}
	Resource (char tType, int quantity, int basePrice)
	{
		setStuff(tType, quantity, basePrice);
	}
	void setStuff (char tType, int quantity, int basePrice)
	{//наложи се да го прибавя
		type = 'r';
		this->tType = tType;
		this->quantity = quantity;
		this->basePrice = basePrice;
		calcPrice(1, 1);
		calcTotalPrice();
	}
	void calcTotalPrice ()
	{
		totalPrice = price * quantity;
	}
	double getTotalPrice ()
	{
		calcTotalPrice();
		return totalPrice;
	}
	void printInfo ()
	{
		cout<<"Quantity: "<<quantity<<endl;
		cout<<"Item: resource\n";
		cout<<"Resource Type: ";
		if (tType == 'f')
			cout<<"food\n";
		else if (tType == 'w')
			cout<<"water\n";
		else if (tType == 'i')
			cout<<"iron\n";
		else
			cout<<"something...\n";
		cout<<"Price per unit: "<<price<<endl;
		cout<<"Base price per unit: "<<basePrice<<endl;
		cout<<"Total: "<<totalPrice<<endl;
	}
};
class WorkTool : public Item
{//Equipment и Modification иамха променливи с еднакви роли
private:
protected:
	double efficiency;//с колко поцента ще подобрява- това се дели на 100 при смятане
public:
	bool equiped;
};
class Modification : public WorkTool
{
private:
	//int efficiency;
public:
	//ModAlgorithm modAlgorithm [];//в бъдеще ще реша как да работи това
	//char mType; //modification type- на негово място е универсалното tType
	//double modEfficiency;//efficiency от WorkTools
	//bool equiped;
	Modification ()
	{
		type = 'm';
		tType = ' ';
		basePrice = -1;
		efficiency = 0;
		equiped = false;
		calcPrice(1, 1);
	}
	Modification (int basePrice, char tType)
	{
		type = 'm';
		this->tType = tType;
		this->basePrice = basePrice;
		equiped = false;
		efficiency = 2;
		calcPrice(1, 1);
	}
	double calcEffMod (double equipmentEff)
	{
		return equipmentEff * (efficiency / 100);
	}
	void calcPrice (int repIndex, int valueIndex)
	{
		price = basePrice + basePrice * (efficiency / 100);
		Item::calcPrice(repIndex, valueIndex);
	}
	void printInfo ()
	{
		cout<<"Item: modification\n";
		cout<<"Modification for: ";
		if (tType == 'w')
			cout<<"weapon\n";
		else if (tType == 'a')
			cout<<"armor\n";
		else if (tType == 't')
			cout<<"tool\n";
		else
			cout<<"something...";
		cout<<"Modifies by "<<efficiency<<"%\n";
		cout<<"Price: "<<price<<endl;
		cout<<"Base price: "<<basePrice<<endl;
	}
};
class Equipment : public WorkTool
{
	//някъде в далечното бъдеще ще се разбере дали ще е подходящо да ги ползвам
	//int condition;
	//int durability;
	//void lowerCondition (...) {...}
	//int repairPrice () {...}
private:
	//double efficiency;//efficiency от WorkTools
	double modEfficiency;
	int modCount;
	Modification* modification [3];//нека за сега да има до 3 модификации
public:
	//char eType; //Equipment type: w - weapon, a - armor, t - tool - на негово място е универсалното tType
	//bool equiped;
	Equipment ()
	{
		basePrice = -1;
		type = 'e';
		tType = ' ';
		efficiency = 0;
		modCount = 0;
		equiped = false;
		//когато ще се работи с модификация,
		//ВИНАГИ трябва да се провери дали
		//има такава в дадения слот
		modification[0] = NULL;
		modification[1] = NULL;
		modification[2] = NULL;
		calcModEff ();
		calcPrice (1, 1);
	}
	Equipment (int basePrice, int efficiency, char tType)
	{
		this->basePrice = basePrice;
		this->tType = tType;
		type = 'e';
		this->efficiency = efficiency;
		modCount = 0;
		equiped = false;
		//когато ще се работи с модификация,
		//ВИНАГИ трябва да се провери дали
		//има такава в дадения слот
		modification[0] = NULL;
		modification[1] = NULL;
		modification[2] = NULL;
		calcModEff ();
		calcPrice (1, 1);
	}
	~Equipment ()
	{
		delete modification[0];
		delete modification[1];
		delete modification[2];
	}
	void addMod (Modification* mod, int modSlot)//модификацията се слага в желания слот
	{
		if (mod->tType == tType)
		{
			if (modification[modSlot] == NULL)
			{
				modification[modSlot] = mod;
				modification[modSlot]->equiped = true;
				modCount++;
			}
			else
				cout<<"Slot occupied\n";
		}
		else
			cout<<"This mod is for a different type of equipment\n";
	}
	void removeMod (int modSlot)
	{
		modification[modSlot]->equiped = false;//преди указателя да започне да сочи в нищото, да кажем на модификацията, че я махаме от мястото си
		modification[modSlot] = NULL;//винаги има един указател в инвентара, които сочи към модификацията, ако тук се изтрие обекта, ще стават грешки
		modCount--;
	}
	virtual void calcModEff () //virtual?
	{
		modEfficiency = efficiency;
		if (modification[0] != NULL)
			modEfficiency += modification[0]->calcEffMod(efficiency);
		if (modification[1] != NULL)
			modEfficiency += modification[1]->calcEffMod(efficiency);
		if (modification[2] != NULL)
			modEfficiency += modification[2]->calcEffMod(efficiency);
	}
	virtual void calcPrice (int repIndex, int valueIndex)
	{
		price = basePrice + basePrice * (modEfficiency + efficiency) / 100;
		Item::calcPrice(repIndex, valueIndex);
	}
	void setModCount (int count)//временно ще го ползвам
	{
		modCount = count;
	}
	void printInfo ()
	{
		cout<<"Item: equipment\n";
		cout<<"Type: ";
		if (tType == 'w')
			cout<<"weapon\n";
		else if (tType == 'a')
			cout<<"armor\n";
		else if (tType == 't')
			cout<<"tool\n";
		else
			cout<<"something...\n";
		printShortInfo();
		cout<<"Price: "<<price<<endl;
		cout<<"Base price: "<<basePrice<<endl;
	}
	void printShortInfo ()
	{
		cout<<"\tModifications: "<<modCount<<endl;
		cout<<"\tBase efficiency: "<<efficiency<<endl;
		cout<<"\tModified efficiency: "<<modEfficiency<<endl;
		if (equiped)
			cout<<"Equiped\n";
	}
};
class Person : public Item //как не ме срам да описвам човек като предмет...
{
public:
	string name;
	//char job;// на негово място е универсалното tType
	virtual void calcPrice (int repIndex, int valueIndex) = 0;
	virtual void printInfo () = 0;
};
class Researcher : public Person
{
private:
	int reasearchExp;
	//researched [];
public:
	Researcher ()
	{
		reasearchExp = 1;
		type = 'p';
		tType = 'r';
		basePrice = -1;
		name = "none";
		calcPrice(1, 1);
	}
	Researcher (int basePrice, int reasearchExp, string name)
	{
		this->basePrice = basePrice;
		this->reasearchExp = reasearchExp;
		this->name = name;
		type = 'p';
		tType = 'r';
		calcPrice(1, 1);
	}
	void calcPrice (int repIndex, int valueIndex)
	{
		price = basePrice + reasearchExp;
		Item::calcPrice(repIndex, valueIndex);
	}
	void printInfo ()
	{
		cout<<"Item: "<<"person\n";
		cout<<"Name: "<<name<<endl;
		if (tType == 'r')
			cout<<"Job: "<<"researcher\n";
		else
			cout<<"Researcher object with invalid tType!\n";
		cout<<"reasearch experience: "<<reasearchExp<<endl;
		cout<<"Research history: PLACEHOLDER\n";
		cout<<"Price: "<<price<<endl;
		cout<<"Base price: "<<basePrice<<endl;
	}
	//forReasearch [];
	//research () {}
};
class Worker : public Person
{
private:
	//в бъдеще ще се ползват за изчисляване на шансове
	int combatExp;
	int miningExp;
public:
	Equipment* weapon;//реално, екипировката не се копира, а се мести от едно място на друго
	Equipment* tool;
	Equipment* armor;
	Worker ()
	{
		combatExp = 1;
		miningExp = 1;
		type = 'p';
		tType = ' ';
		basePrice = -1;
		name = "none";
		//екипировка==
		weapon = new Equipment;
		weapon->tType = 'w';
		tool = new Equipment;
		tool->tType = 't';
		armor = new Equipment;
		armor->tType = 'a';
		//============
		calcPrice(1, 1);
	}
	Worker (int basePrice, int combatExp, int miningExp, string name, char tType)
	{
		this->basePrice = basePrice;
		this->combatExp = combatExp;
		this->miningExp = miningExp;
		this->name = name;
		this->tType = tType;//m - миньор, a - въоръжен (armed)
		type = 'p';
		//екипировка==
		weapon = new Equipment;
		weapon->tType = 'w';
		tool = new Equipment;
		tool->tType = 't';
		armor = new Equipment;
		armor->tType = 'a';
		//============
		calcPrice(1, 1);
	}
	void equipWeapon (Equipment* weapon)
	{
		this->weapon = weapon;
		this->weapon->equiped = true;
	}
	void equipTool (Equipment* tool)
	{
		this->tool = tool;
		this->tool->equiped = true;
	}
	void equipArmor (Equipment* armor)
	{
		this->armor = armor;
		this->armor->equiped = true;
	}
	void calcPrice (int repIndex, int valueIndex)
	{
		if (tType == 'm')
			price = basePrice + combatExp / 2 + miningExp;
		else
			price = basePrice + combatExp;
		Item::calcPrice(repIndex, valueIndex);
		price = price + weapon->getPrice();
		price = price + tool->getPrice();
		price = price + armor->getPrice();
	}
	void printInfo ()
	{
		cout<<"Item: "<<"person\n";
		cout<<"Name: "<<name<<endl;
		cout<<"Job: ";
		if (tType == 'm')
			cout<<"Miner\n";
		else if (tType == 'a')
			cout<<"Armed\n";
		else
			cout<<"Worker object with invalid tType!\n";
		cout<<"Mining experience: "<<miningExp<<endl;
		cout<<"Combat experience: "<<combatExp<<endl;
		cout<<"Price: "<<price<<endl;
		cout<<"Base price: "<<basePrice<<endl;
		cout<<"Equipment: \n";
		cout<<"     weapon:\n";
		weapon->printShortInfo();
		cout<<"     tool:\n";
		tool->printShortInfo();
		cout<<"     armor:\n";
		armor->printShortInfo();
	}
};
