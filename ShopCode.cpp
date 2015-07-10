#include<iostream>
#include<string>
#include<vector>
#include"items.h"
using namespace std;

class ItemForSale
{//тук няма да е нужно предифиниране на оператор '='
private:
	char type;
	char tType;
	int id;
public:
	char giveType ()
	{
		return type;
	}
	char giveID ()
	{
		return id;
	}
	char giveTType ()
	{
		return tType;
	}
	void prepareForSale (Item* item, int id)
	{
		bool isValid = false;
		if (item->type == 'm' || item->type == 'e')
		{
			if (item->tType == 'a' || item->tType == 't' || item->tType == 'w')
				isValid = true;
		}
		/*
		else if (item->type == 'e')
		{
			if (item->tType == 'a' || item->tType == 't' || item->tType == 'w')
				isValid = true;
		}
		*/
		else if (item->type == 'p')
		{
			if (item->tType == 'm' || item->tType == 'a' || item->tType == 'r')
				isValid = true;
		}
		if (isValid)
		{
			type = item->type;
			this->id = id;
			tType = item->tType;
		}
		else
			cout<<"[prepareForSale(Item, int)] Inavlid type or tType variable!\n";
	}
};
class Inventory
{
private:
	double totalSellPrice;//обща цена на всичко което е приготвено за продан, включително и подготвените пари
	void printCertainStuffInInventory(vector<Item*>& arr,/* int size,*/ string itemName, bool quickPrint)//името за победата
	{
		if (arr.size() == 0)
			cout<<"No "<<itemName<<"s in inventory...\n";
		else
			for (int i = 0; i < arr.size(); i++)
			{
				bool equiped = false;
				if (arr[i]->type == 'm' || arr[i]->type == 'e')
				{
					WorkTool* wt = dynamic_cast<WorkTool*>(arr[i]);
					if (wt->equiped)
						equiped = true;
				}
				if (isForSale((arr[i]), i) == false)
					if (equiped)
						cout<<"The "<<itemName<<" is equipped\n";
					else if (quickPrint)
						cout<<itemName<<"["<<i<<"], type-"<<arr[i]->type<<", tType- "<<arr[i]->tType<<" "<<arr[i]->getPrice()<<"\n";
					else
						arr[i]->printInfo();
			}
	}
	void _sellResource (Resource* resource, Resource* resForSale, int quantity, string type)
	{
		if (quantity <= resource->quantity)
		{
			resource->quantity = resource->quantity - quantity;
			resForSale->quantity = resForSale->quantity + quantity;
			totalSellPrice += resForSale->getTotalPrice();
		}
		else
			cout<<"Inventory has less "<<type<<" than that...\n";
	}
	void _returnResToInv (Resource* resource, Resource* resForSale, int quantity, string type)
	{
		if (quantity <= resForSale->quantity)
		{
			totalSellPrice -= resForSale->getTotalPrice();
			resource->quantity += quantity;
			resForSale->quantity -= quantity;
			totalSellPrice += resForSale->getTotalPrice();
		}
		else
			cout<<"Inventory is selling less "<<type<<" than that...\n";
	}
public:
	int funds;
	vector<Item*> modification;
	vector<Item*> weapon;
	vector<Item*> tool;
	vector<Item*> armor;
	vector<Item*> researcher;
	vector<Item*> worker;
	Resource food;
	Resource water;
	Resource metal;
	Inventory ()
	{
		funds = 0;
		food.setStuff('f', 0, 2);
		water.setStuff('w', 0, 5);
		metal.setStuff('m', 0, 3);
		payFunds = 0;
		foodForSale.setStuff('f', 0, 2);
		waterForSale.setStuff('w', 0, 5);
		metalForSale.setStuff('m', 0, 3);
		totalSellPrice = 0;
	}
	void addItem (Item* newItem)
	{
		if (newItem->type == 'm')
		{
			modification.push_back(newItem);
		}
		else if (newItem->type == 'e')
		{
			if (newItem->tType == 'w')
				weapon.push_back(newItem);
			else if (newItem->tType == 't')
				tool.push_back(newItem);
			else if (newItem->tType == 'a')
				armor.push_back(newItem);
			else
				cout<<"[addItem (Item*)]Equipment has invalid tType variable!\n";
		}
		else if (newItem->type == 'p')
		{
			if (newItem->tType == 'a' || newItem->tType == 'm')
			{
				worker.push_back(newItem);
			}
			else if (newItem->tType == 'r')
			{
				researcher.push_back(newItem);
			}
			else
				cout<<"[addItem (Item*)]Person has invalid tType variable!\n";
		}
		else
			cout<<"[addItem (Item*)]Item has invalid type variable\n";
	}	
	void printInventory (bool quickPrint)
	{
		printModsInfo(quickPrint);
		printWeaponsInfo(quickPrint);
		printToolsInfo(quickPrint);
		printArmorInfo(quickPrint);
		printResearchersInfo(quickPrint);
		printWorkersInfo(quickPrint);
		printResources();
		cout<<"Funds: "<<funds<<endl;
	}
	void printModsInfo (bool quickPrint)
	{
		printCertainStuffInInventory (modification, "modification", quickPrint);
	}
	void printWeaponsInfo (bool quickPrint)
	{
		printCertainStuffInInventory (weapon, "weapon", quickPrint);
	}
	void printToolsInfo (bool quickPrint)
	{
		printCertainStuffInInventory (tool, "tool", quickPrint);
	}
	void printArmorInfo (bool quickPrint)
	{
		printCertainStuffInInventory (armor, "armor", quickPrint);
	}
	void printResearchersInfo (bool quickPrint)
	{
		printCertainStuffInInventory (researcher, "researcher", quickPrint);
	}
	void printWorkersInfo (bool quickPrint)
	{
		printCertainStuffInInventory (worker, "worker", quickPrint);
	}
	void printResources ()
	{
		cout<<"Food: "<<food.quantity<<endl;
		cout<<"Water: "<<water.quantity<<endl;
		cout<<"Metal: "<<metal.quantity<<endl;
	}
	//за продан:
	vector<ItemForSale> itemForSale;
	Resource foodForSale;
	Resource waterForSale;
	Resource metalForSale;
	int payFunds;
	bool isForSale (Item* item, int id)
	{
		bool result = false;
		for (int i = 0; i < itemForSale.size(); i++)
		{
			if (item->type == itemForSale[i].giveType() &&
				item->tType == itemForSale[i].giveTType() &&
				id == itemForSale[i].giveID())
				result = true;
		}
		return result;
	}
	void addForSale (char type, char tType, int id)
	{//ако type == 'm' , стойността на tType няма значение, поне за сега
		ItemForSale addHelper;
		Item* item;
		if (type == 'm')
			item = modification[id];
		else if (type == 'e')
		{
			if (tType == 'w')
				item = weapon[id];
			else if (tType == 't')
				item = tool[id];
			else if (tType == 'a')
				item = armor[id];
			else
				cout<<"[addForSale(char, char, int)]Invalid tType argument!\n";
		}
		else if (type == 'p')
		{
			if (tType == 'r')
				item = researcher[id];
			else if (tType == 'a' || tType == 'm')
				item = worker[id];
			else
				cout<<"[addForSale(char, char, int)]Invalid tType argument!\n";
		}
		else
			cout<<"[addForSale(char, char, int)]Invalid type argument!\n";
		if (isForSale(item, id) == false)
		{
			totalSellPrice += item->getPrice();
			addHelper.prepareForSale(item, id);
			itemForSale.push_back(addHelper);
		}
		else
			cout<<"[addForSale(char, char, int)]Item is already prepared for sale.\n";
	}
	void returnToInv (int _id)
	{
		char type = itemForSale[_id].giveType();
		char tType = itemForSale[_id].giveTType();
		int id = itemForSale[_id].giveID();
		if (type == 'm')
			totalSellPrice -= modification[id]->getPrice();
		else if (type == 'e')
		{
			if (tType == 'w')
				totalSellPrice -= weapon[id]->getPrice();
			else if (tType == 't')
				totalSellPrice -= tool[id]->getPrice();
			else if (tType == 'a')
				totalSellPrice -= armor[id]->getPrice();
			else
				cout<<"[returnToInv(int)]Invalid tType argument!\n";
		}
		else if (type == 'p')
		{
			if (tType == 'r')
				totalSellPrice -= researcher[id]->getPrice();
			else if (tType == 'a' || tType == 'm')
				totalSellPrice -= worker[id]->getPrice();
			else
				cout<<"[returnToInv(int)]Invalid tType argument!\n";
		}
		else
			cout<<"[returnToInv(int)]Invalid type argument!\n";
		itemForSale.erase(itemForSale.begin() + _id);
	}
	void sellResource (char type, int quantity)
	{
		if (type == 'f')
			_sellResource(&food, &foodForSale, quantity, "food");
		else if (type == 'w')
			_sellResource(&water, &waterForSale, quantity, "water");
		else if (type == 'm')
			_sellResource(&metal, &metalForSale, quantity, "metal");
		else
			cout<<"[sellResource(char, int)]Invalid resource type, or not yet in game!\n";
	}
	void returnResToInv (char type, int quantity)
	{
		if (type == 'f')
			_returnResToInv(&food, &foodForSale, quantity, "food");
		else if (type == 'w')
			_returnResToInv(&water, &waterForSale, quantity, "water");
		else if (type == 'm')
			_returnResToInv(&metal, &metalForSale, quantity, "metal");
		else
			cout<<"[returnResToInv(char, int)]Invalid resource type, or not yet in game!\n";
	}
	void addPayFunds (int pay)
	{
		if (pay <= funds)
		{
			funds = funds - pay;
			payFunds = payFunds + pay;
			totalSellPrice += pay;
		}
		else
			cout<<"Inventory has less funds than that...\n";
	}
	void returnFundsToInv (int ret)
	{
		if (ret <= payFunds)
		{
			funds += ret;
			payFunds -= ret;
			totalSellPrice -= ret;
		}
		else
			cout<<"Less funds are prepared for trading...\n";
	}
	double totalPrice ()
	{
		return totalSellPrice;
	}
	void cancelDeal ()
	{
		for (int i = itemForSale.size() - 1; i >= 0; i--)
			itemForSale.erase(itemForSale.begin() + i);
		funds = funds + payFunds;
		payFunds = 0;
		food.quantity = food.quantity + foodForSale.quantity;
		foodForSale.quantity = 0;
		water.quantity = water.quantity + waterForSale.quantity;
		waterForSale.quantity = 0;
		metal.quantity = metal.quantity + metalForSale.quantity;
		metalForSale.quantity = 0;
		totalSellPrice = 0;
	}
	void printForSale (bool quickPrint)
	{
		for (int i = 0; i < itemForSale.size(); i++)
		{
			char type = itemForSale[i].giveType();//малко да опростя нещата и да махна непрестанното връщане на едни и същи данни
			char tType = itemForSale[i].giveTType();
			int id = itemForSale[i].giveID();
			cout<<"Item["<<i<<"]: ";
			if (type == 'm')
			{
				if (quickPrint)
					cout<<"mod["<<id<<"]\n";
				else
					modification[id]->printInfo();
			}
			else if (type == 'e')
			{
				if (tType == 'w')
					if (quickPrint)
						cout<<"weapon["<<id<<"]\n";
					else
						weapon[id]->printInfo();
				else if (tType == 't')
					if (quickPrint)
						cout<<"tool["<<id<<"]\n";
					else
						tool[id]->printInfo();
				else if (tType == 'a')
					if (quickPrint)
						cout<<"armor["<<id<<"]\n";
					else
						armor[id]->printInfo();
				else
					cout<<"[printForSale (bool)]Equipment object with invalid tType variable!\n";
			}
			else if (type == 'p')
			{
				if (tType == 'm' || tType == 'a')
					if (quickPrint)
						cout<<"worker["<<id<<"]\n";
					else
						worker[id]->printInfo();
				else if (tType == 'r')
					if (quickPrint)
						cout<<"researcher["<<id<<"]\n";
					else
						researcher[id]->printInfo();
				else
					cout<<"[printForSale (bool)]Person object with invalid tType variable!\n";
			}
			else
				cout<<"[printForSale (bool)]Item object with invalid type variable!\n";
		}
		printResourcesForSale();
		cout<<"Funds: "<<payFunds<<endl;
	}
	void printResourcesForSale ()
	{
		cout<<"Food: "<<foodForSale.quantity<<endl;
		cout<<"Water: "<<waterForSale.quantity<<endl;
		cout<<"Metal: "<<metalForSale.quantity<<endl;
	}
	//============
	void acceptOffer (Inventory* offerInv)
	{
		for (int i = offerInv->itemForSale.size() - 1; i >= 0; i--)
		{
			Item* item;
			char type = offerInv->itemForSale[i].giveType();
			char tType = offerInv->itemForSale[i].giveTType();
			int id = offerInv->itemForSale[i].giveID();
			if (type == 'm' && modification.size() > 0)
			{
				item = offerInv->modification[id];
				offerInv->modification.erase(offerInv->modification.begin() + id);
			}
			else if (type == 'e')
			{
				if (tType == 'w' && weapon.size() > 0)
				{
					item = offerInv->weapon[id];
					offerInv->weapon.erase(offerInv->weapon.begin() + id);
				}
				else if (tType == 't' && tool.size() > 0)
				{
					item = offerInv->tool[id];
					offerInv->tool.erase(offerInv->tool.begin() + id);
				}
				else if (tType == 'a' && armor.size() > 0)
				{
					item = offerInv->armor[id];
					offerInv->armor.erase(offerInv->armor.begin() + id);
				}
				else
					cout<<"[MAIN]Invalid tType!\n";
			}
			else if (type == 'p')
			{
				if (tType == 'r' && researcher.size() > 0)
				{
					item = offerInv->researcher[id];
					offerInv->researcher.erase(offerInv->researcher.begin() + id);
				}
				else if ((tType == 'a' || tType == 'm') && worker.size() > 0)
				{
					item = offerInv->worker[id];
					offerInv->worker.erase(offerInv->worker.begin() + id);
				}
				else
					if (modification.size() > 0 &&
						weapon.size() > 0 &&
						tool.size() > 0 &&
						armor.size() > 0 &&
						researcher.size() > 0 &&
						worker.size() > 0)
					cout<<"[MAIN]Invalid tType!\n";
			}
			else
				if (modification.size() > 0 &&
					weapon.size() > 0 &&
					tool.size() > 0 &&
					armor.size() > 0 &&
					researcher.size() > 0 &&
					worker.size() > 0)
				cout<<"[MAIN]Invalid type!\n";
			if (item != NULL)
				addItem(item);
			offerInv->itemForSale.erase(offerInv->itemForSale.begin() + i);
		}
		food.quantity = food.quantity + offerInv->foodForSale.quantity;
		offerInv->foodForSale.quantity = 0;
		water.quantity = water.quantity + offerInv->waterForSale.quantity;
		offerInv->waterForSale.quantity = 0;
		metal.quantity = metal.quantity + offerInv->metalForSale.quantity;
		offerInv->metalForSale.quantity = 0;
		funds = funds + offerInv->payFunds;
		offerInv->payFunds = 0;
		totalSellPrice = 0;
	}
};
class Squad
{
private:
public:
	Worker* leader;
	Inventory inventory;
};

Squad squadA;
Squad squadB;
Squad squadC;
Squad squadD;
Inventory* inventoryA;//с това си съкращавам "малко" if-else-ове
Inventory* inventoryB;//чрез двете само маркирам инвентарите които искам, за да мога директно да работя с тях

//ако бях сложил функции "selectInvА" и "selectInvB", в телата им щеше да има един и същ код
//чрез аргументът се избира с кой указател искам да работя
void selectInv (char inv)
{//маркиране на инвентар, след това чрез inventoryA и inventoryВ се достъпва до избраните инвентари
	if (inv != 'a' && inv != 'b')
	{
		cout<<"[select_inv (char)]Wrong argument passed!\n";
		return;
	}
	cout<<"Choose inventory: \n";
	cout<<"A - squadA\n";
	cout<<"B - squadB\n";
	cout<<"C - squadC\n";
	cout<<"D - squadD\n";
	char chosen;
	Inventory* temp;
	for (bool done = false; done == false;)
	{
		cin>>chosen;
		if (chosen == 'A')
		{
			temp = &squadA.inventory;
			done = true;
		}
		else if (chosen == 'B')
		{
			temp = &squadB.inventory;
			done = true;
		}
		else if (chosen == 'C')
		{
			temp = &squadC.inventory;
			done = true;
		}
		else if (chosen == 'D')
		{
			temp = &squadD.inventory;
			done = true;
		}
		else
			cout<<"There is no such squad- A, B, C or D only.";
	}
	if (inv == 'a')
		inventoryA = temp;
	else
		inventoryB = temp;
}
void printInv (Inventory* inv)
{//принтиране
	cout<<"======IN INVENTORY======\n";
	inv->printInventory(true);
	cout<<"========FOR SALE========\n";
	inv->printForSale(true);
	cout<<"Total price: "<<inv->totalPrice()<<endl;
}
void printInv (char inv)
{//принтиране след даване на команда от "играчът"
	if (inv != 'a' && inv != 'b')
	{
		cout<<"[print_inv (char)]Wrong argument passed!\n";
		return;
	}
	Inventory* temp;
	if (inv == 'a')
		temp = inventoryA;
	else
		temp = inventoryB;
	if (temp == NULL)
	{
		cout<<"The inventory is not yet selected!\n";
		return;
	}
	printInv(temp);
}
void sellItem (char inv)
{//приготвяне на предмети за продажба
	if (inv != 'a' && inv != 'b')
	{
		cout<<"[sell_item (char)]Wrong argument passed!\n";
		return;
	}
	Inventory* temp;
	if (inv == 'a')
		temp = inventoryA;
	else
		temp = inventoryB;
	if (temp == NULL)
	{
		cout<<"The inventory is not yet selected!\n";
		return;
	}
	for (bool done = false; done == false;)
	{
		printInv(temp);
		char type;
		char tType;
		int id;
		cout<<"[type] [tType] [id] (0 0 0 - if you are done selecting)\n";
		cin>>type;
		cin>>tType;
		cin>>id;
		//=====
		if (type == 'm')
			temp->addForSale(temp->modification[id]->type, temp->modification[id]->tType, id);
			else if (type == 'e')
			{
				if (tType == 'w')
					temp->addForSale(temp->weapon[id]->type, temp->weapon[id]->tType, id);
				else if (tType == 't')
					temp->addForSale(temp->tool[id]->type, temp->tool[id]->tType, id);
				else if (tType == 'a')
					temp->addForSale(temp->armor[id]->type, temp->armor[id]->tType, id);
				else
					cout<<"Invalid equipment tType!\n";
			}
			else if (type == 'p')
			{
				if (tType == 'r')
					temp->addForSale(temp->researcher[id]->type, temp->researcher[id]->tType, id);
				else if (tType == 'a' || tType == 'm')
					temp->addForSale(temp->worker[id]->type, temp->worker[id]->tType, id);
				else
					cout<<"Invalid person tType!\n";
			}
			else if (type == '0' && tType == '0' && id == 0)
				done = true;
			else
				cout<<"Invalid type!\n";
	}
}
void returnItem (char inv)
{//връщане на приготвен за продажба предмет в инвентара
	if (inv != 'a' && inv != 'b')
	{
		cout<<"[return_item (char)]Wrong argument passed!\n";
		return;
	}
	Inventory* temp;
	if (inv == 'a')
		temp = inventoryA;
	else
		temp = inventoryB;
	if (temp == NULL)
	{
		cout<<"The inventory is not yet selected!\n";
		return;
	}
	int id;
	for (bool done = false; done == false;)
	{
		temp->printForSale(true);
		cout<<"[ID] (-1 or less- cancel)\n";
		cin>>id;
		if (id < temp->itemForSale.size() && id >= 0)
			temp->itemForSale.erase(temp->itemForSale.begin() + id);
		else if (id < 0)
			done = true;
		else
			cout<<"ID is out of range!\n";
	}
}
void sellResource (char inv)
{//приготвяне на ресурси
	if (inv != 'a' && inv != 'b')
	{
		cout<<"[sell_resource (char)]Wrong argument passed!\n";
		return;
	}
	Inventory* temp;
	if (inv == 'a')
		temp = inventoryA;
	else
		temp = inventoryB;
	if (temp == NULL)
	{
		cout<<"The inventory is not yet selected!\n";
		return;
	}
	char type;
	int quantity;
	for (bool done = false; done == false;)
	{
		cout<<"In inventory:\n";
		temp->printResources();
		cout<<"For sale:\n";
		temp->printResourcesForSale();
		cout<<"[type] [quantity] (0 0 - if you are done selecting)\n";
		cin>>type;
		cin>>quantity;
		if (type == '0' && quantity == 0)
			done = true;
		else if (type == 'f' || type == 'w' || type == 'm')
			temp->sellResource(type, quantity);
		else
			cout<<"Invalid input.\n";
	}
}
void returnResource (char inv)
{//връщане на ресурси
	if (inv != 'a' && inv != 'b')
	{
		cout<<"[return_resource (char)]Wrong argument passed!\n";
		return;
	}
	Inventory* temp;
	if (inv == 'a')
		temp = inventoryA;
	else
		temp = inventoryB;
	if (temp == NULL)
	{
		cout<<"The inventory is not yet selected!\n";
		return;
	}
	char type;
	int quantity;
	for (bool done = false; done == false;)
	{
		cout<<"In inventory:\n";
		temp->printResources();
		cout<<"For sale:\n";
		temp->printResourcesForSale();
		cout<<"[type] [quantity] (0 0 - if you are done selecting)\n";
		cin>>type;
		cin>>quantity;
		if (type == '0' && quantity == 0)
			done = true;
		else if (type == 'f' || type == 'w' || type == 'm')
			temp->returnResToInv(type, quantity);
		else
			cout<<"Invalid input.\n";
	}
}
void giveFunds (char inv)
{//приготвяне на пари- най-вероятно само за изравняване на сметките
	if (inv != 'a' && inv != 'b')
	{
		cout<<"[sell_resource (char)]Wrong argument passed!\n";
		return;
	}
	Inventory* temp;
	if (inv == 'a')
		temp = inventoryA;
	else
		temp = inventoryB;
	if (temp == NULL)
	{
		cout<<"The inventory is not yet selected!\n";
		return;
	}
	int funds;
	for (bool done = false; done == false;)
	{
		cout<<"In inventory: "<<temp->funds<<endl;
		cout<<"For buying: "<<temp->totalPrice()<<endl;
		cout<<"How much? (-1 or lower - cancel)";
		cin>>funds;
		if (funds >= 0)
			temp->addPayFunds(funds);
		else
			done = true;
	}
}
void returnFunds (char inv)
{//връщане на приготвени пари
	if (inv != 'a' && inv != 'b')
	{
		cout<<"[sell_resource (char)]Wrong argument passed!\n";
		return;
	}
	Inventory* temp;
	if (inv == 'a')
		temp = inventoryA;
	else
		temp = inventoryB;
	if (temp == NULL)
	{
		cout<<"The inventory is not yet selected!\n";
		return;
	}
	int funds;
	for (bool done = false; done == false;)
	{
		cout<<"In inventory: "<<temp->funds<<endl;
		cout<<"For buying: "<<temp->totalPrice()<<endl;
		cout<<"How much? (-1 or lower - cancel) ";
		cin>>funds;
		if (funds >= 0)
			temp->returnFundsToInv(funds);
		else
			done = true;
	}
}
void cancelDeal (char inv)
{//отказване на зделката- връща всичко в инвентара!
	if (inv != 'a' && inv != 'b')
	{
		cout<<"[cancelDeal (char)]Wrong argument passed!\n";
		return;
	}
	Inventory* temp;
	if (inv == 'a')
		temp = inventoryA;
	else
		temp = inventoryB;
	if (temp == NULL)
	{
		cout<<"The inventory is not yet selected!\n";
		return;
	}
	temp->cancelDeal();
}
void acceptDeal ()
{//приемане на зделка
	if (inventoryA == NULL || inventoryB == NULL)
	{
		cout<<"[print_inv (char)]Only one or NO inventoies are selected! Canceling...\n";
		return;
	}
	if (inventoryA->totalPrice() != inventoryB->totalPrice())
		cout<<"One of squads' offer costs less than the other one's offer.\n";
	inventoryA->acceptOffer(inventoryB);
	inventoryB->acceptOffer(inventoryA);
	inventoryA->funds = inventoryA->funds + inventoryB->payFunds;
	inventoryB->payFunds = 0;
	inventoryB->funds = inventoryB->funds + inventoryA->payFunds;
	inventoryA->payFunds = 0;
}

int main ()
{
	Modification* mod [] = {
		new Modification(50, 'w'),//0
		new Modification(95, 'w'),//1
		new Modification(70, 'w'),//2
		new Modification(75, 'w'),//3
		new Modification(60, 'w'),//4
		new Modification(85, 'w'),//5
		new Modification(105, 'a'),//6
		new Modification(85, 'a'),//7
		new Modification(35, 'a'),//8
		new Modification(50, 'a'),//9
		new Modification(75, 'a'),//10
		new Modification(80, 't'),//11
		new Modification(95, 't'),//12
		new Modification(75, 't'),//13
		new Modification(55, 't'),//14
		new Modification(100, 't')//15
	};
	Equipment* weapon [] = {
		new Equipment(450, 30, 'w'),//0
		new Equipment(430, 20, 'w'),//1
		new Equipment(500, 25, 'w'),//2
		new Equipment(300, 15, 'w'),//3
		new Equipment(600, 30, 'w'),//4
		new Equipment(400, 25, 'w'),//5
		new Equipment(500, 30, 'w'),//6
		new Equipment(300, 15, 'w'),//7
		new Equipment(400, 40, 'w'),//8
		new Equipment(400, 35, 'w'),//9
		new Equipment(305, 15, 'w')//10
	};
	Equipment* tool [] = {
		new Equipment(605, 30, 't'),//0
		new Equipment(405, 20, 't'),//1
		new Equipment(500, 25, 't'),//2
		new Equipment(205, 20, 't'),//3
		new Equipment(405, 30, 't'),//4
		new Equipment(500, 20, 't'),//5
		new Equipment(450, 30, 't'),//6
		new Equipment(450, 25, 't'),//7
		new Equipment(205, 25, 't')//8
	};
	Equipment* armor [] = {
		new Equipment(35, 650, 'a'),//0
		new Equipment(25, 450, 'a'),//1
		new Equipment(30, 550, 'a'),//2
		new Equipment(25, 500, 'a'),//3
		new Equipment(25, 350, 'a'),//4
		new Equipment(30, 250, 'a')//5
	};
	Researcher* researcher [] = {
		new Researcher(100, 10, "Seim Mitolish"),//0
		new Researcher(140, 15, "Movzid Pekta"),//1
		new Researcher(120, 20, "Aklimik Quot"),//2
		new Researcher(200, 10, "Drak Miloka"),//3
		new Researcher(230, 15, "Heres Zadis"),//4
		new Researcher(130, 25, "Filepa Lyke")//5
	};
	Worker* worker [] = {
		new Worker(200, 30, 0, "Kehl Dovam", 'a'),//0
		new Worker(150, 35, 0, "Plax Kazove", 'a'),//1
		new Worker(150, 20, 0, "Fest Melay", 'a'),//2
		new Worker(210, 30, 0, "Betmo Dakarsa", 'a'),//3
		new Worker(120, 35, 0, "Losa Rossun", 'a'),//4
		new Worker(220, 10, 20, "Celfam Rokil", 'm'),//5
		new Worker(155, 15, 25, "Lot Limes", 'm'),//6
		new Worker(140, 10, 20, "Danke Shon", 'm'),//7
		new Worker(210, 15, 30, "Fur Shpilen", 'm'),//8
		new Worker(120, 10, 20, "Grast Tona", 'm')//9
	};
	//===================================
	squadA.inventory.funds = 10000;
	squadA.inventory.water.quantity = 400;
	squadA.inventory.food.quantity = 200;
	squadA.inventory.metal.quantity = 250;
	//squadA.leader = worker[0];//лявата част започва да сочи където сочи дясната, това ще се ползва в бъдеще
	squadA.inventory.addItem(mod[0]);
	squadA.inventory.addItem(mod[1]);
	squadA.inventory.addItem(mod[5]);
	squadA.inventory.addItem(mod[6]);
	squadA.inventory.addItem(mod[7]);
	squadA.inventory.addItem(weapon[0]);
	squadA.inventory.addItem(weapon[1]);
	squadB.inventory.addItem(weapon[4]);
	squadB.inventory.addItem(weapon[5]);
	squadA.inventory.addItem(tool[0]);
	squadA.inventory.addItem(tool[1]);
	squadA.inventory.addItem(tool[2]);
	squadA.inventory.addItem(armor[0]);
	squadA.inventory.addItem(armor[1]);
	squadA.inventory.addItem(researcher[0]);
	squadA.inventory.addItem(researcher[1]);
	squadA.inventory.addItem(worker[0]);
	squadA.inventory.addItem(worker[2]);
	squadA.inventory.addItem(worker[3]);
	Equipment* tempE = dynamic_cast<Equipment*>(squadA.inventory.weapon[0]);
	Modification* tempM = dynamic_cast<Modification*>(squadA.inventory.modification[0]); tempE->addMod(tempM, 0);
	tempM = dynamic_cast<Modification*>(squadA.inventory.modification[1]); tempE->addMod(tempM, 1);
	//===================================
	squadB.inventory.funds = 5000;
	squadB.inventory.water.quantity = 300;
	squadB.inventory.food.quantity = 400;
	squadB.inventory.metal.quantity = 350;
	squadB.inventory.addItem(mod[2]);
	squadB.inventory.addItem(mod[3]);
	squadB.inventory.addItem(mod[4]);
	squadB.inventory.addItem(mod[8]);
	squadB.inventory.addItem(mod[9]);
	squadB.inventory.addItem(mod[10]);
	squadB.inventory.addItem(mod[11]);
	squadB.inventory.addItem(weapon[2]);
	squadB.inventory.addItem(weapon[3]);
	squadB.inventory.addItem(tool[3]);
	squadB.inventory.addItem(tool[4]);
	squadB.inventory.addItem(tool[5]);
	squadB.inventory.addItem(armor[2]);
	squadB.inventory.addItem(armor[3]);
	squadB.inventory.addItem(researcher[2]);
	squadB.inventory.addItem(researcher[3]);
	squadB.inventory.addItem(worker[1]);
	squadB.inventory.addItem(worker[4]);
	squadB.inventory.addItem(worker[5]);
	squadB.inventory.addItem(worker[6]);
	//====================================
	squadC.inventory.funds = 8000;
	squadC.inventory.water.quantity = 350;
	squadC.inventory.food.quantity = 450;
	squadC.inventory.metal.quantity = 350;
	squadC.inventory.addItem(mod[12]);
	squadC.inventory.addItem(mod[13]);
	squadC.inventory.addItem(mod[14]);
	squadC.inventory.addItem(mod[15]);
	squadC.inventory.addItem(weapon[6]);
	squadC.inventory.addItem(weapon[7]);
	squadC.inventory.addItem(weapon[8]);
	squadC.inventory.addItem(weapon[9]);
	squadC.inventory.addItem(weapon[10]);
	squadC.inventory.addItem(tool[6]);
	squadC.inventory.addItem(tool[7]);
	squadC.inventory.addItem(tool[8]);
	squadC.inventory.addItem(armor[4]);
	squadC.inventory.addItem(armor[5]);
	squadC.inventory.addItem(researcher[4]);
	squadC.inventory.addItem(researcher[5]);
	squadC.inventory.addItem(worker[7]);
	squadC.inventory.addItem(worker[8]);
	squadC.inventory.addItem(worker[9]);
	Worker* tempW = dynamic_cast<Worker*>(squadC.inventory.worker[0]);
	tempE = dynamic_cast<Equipment*>(squadC.inventory.weapon[0]); tempW->equipWeapon(tempE);
	tempE = dynamic_cast<Equipment*>(squadC.inventory.armor[0]);  tempW->equipArmor(tempE);
	tempE = dynamic_cast<Equipment*>(squadC.inventory.tool[0]);   tempW->equipTool(tempE);
	//====================================
	squadD.inventory.funds = 80000;
	squadD.inventory.water.quantity = 3500;
	squadD.inventory.food.quantity = 4500;
	squadD.inventory.metal.quantity = 3500;
	//====================================
	cout<<"Commands:\n";
	cout<<"select_inv_A / select_inv_B\n";
	cout<<"print_inv_A / print_inv_B\n\n";
	//====
	cout<<"sell_item_A / sell_item_B\n";
	cout<<"sell_resource_A / sell_resource_B\n";
	cout<<"give_funds_A / give_funds_B\n\n";
	//====
	cout<<"return_item_A / return_item_B\n";
	cout<<"return_resource_A / return_resource_B\n";
	cout<<"return_funds_A / return_funds_B\n\n";
	//====
	cout<<"cancel_deal_A / cancel_deal_B WIP\n";
	cout<<"accept_deal\n";
	string command;
	for (bool cancel = false; cancel == false;)
	{
		cout<<"Command: ";
		cin>>command;
		if (command == "select_inv_A")
			selectInv('a');
		else if (command == "select_inv_B")
			selectInv('b');
		//======
		else if (command == "print_inv_A")
			printInv('a');
		else if (command == "print_inv_B")
			printInv('b');
		//======
		else if (command == "sell_item_A")
			sellItem('a');
		else if (command == "sell_item_B")
			sellItem('b');
		else if (command == "return_item_A")
			returnItem('a');
		else if (command == "return_item_B")
			returnItem('b');
		//======
		else if (command == "sell_resource_A")
			sellResource('a');
		else if (command == "sell_resource_B")
			sellResource('b');
		else if (command == "return_resource_A")
			returnResource('a');
		else if (command == "return_resource_B")
			returnResource('b');
		//======
		else if (command == "give_funds_A")
			giveFunds('a');
		else if (command == "give_funds_B")
			giveFunds('b');
		else if (command == "return_funds_A")
			returnFunds('a');
		else if (command == "return_funds_B")
			returnFunds('b');
		//======
		else if (command == "cancel_deal_A")
			cancelDeal('a');
		else if (command == "cancel_deal_B")
			cancelDeal('b');
		else if (command == "accept_deal")
			acceptDeal();
		else if (command == "cancel")
			cancel = true;
		else
			cout<<"No such command or not yet developed!\n";
	}
	system("pause");
	return 0;
}
