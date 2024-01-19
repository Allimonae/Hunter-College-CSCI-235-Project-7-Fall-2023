/**
CSCI 235 Fall 2023
Project 7
Created by Allison Lee on December 1, 2023
Inventory.cpp declares the Item class and implements its private and public members
*/

#include "Inventory.hpp"

// Default constructor
Inventory::Inventory() : BinarySearchTree<Item*>() {}

// Constructor with items
Inventory::Inventory(const std::vector<Item*>& items) : BinarySearchTree<Item*>() {}

// Destructor
Inventory::~Inventory() {
    // The BinarySearchTree destructor is called automatically
}
/**
    * @param : A Pointer to the Item object to be added to the inventory.
    * @post  : The Item is added to the inventory, preserving the BST structure. The BST property is based on (ascending) alphabetical order of the item's name.
                If the item is type UNKNOWN, WEAPON, or ARMOR and is already in the inventory, it is not added.
                However, if another instance of an Item of type CONSUMABLE is being added (an item with the same name), 
                its quantity is updated to the sum of the quantities of the two objects, 
                and the time_picked_up_ variable is updated to that of the latest instance of the item being added.
    * @return true if the item was added to the inventory or updated, false otherwise.
*/
bool Inventory::addItem(Item* const nw_itm){
    if (nw_itm == nullptr)
    {
        return false;
    }
    /**
        If the item is type UNKNOWN, WEAPON, or ARMOR and is already in the inventory, 
        it is not added.
    */
    if (nw_itm->getType() == "UNKNOWN" || nw_itm->getType() == "WEAPON" || nw_itm->getType() == "ARMOR")
    {
        if (contains(nw_itm))
        { 
            return false;
        }
    }
    /**
        However, if another instance of an Item of type CONSUMABLE is being added (an item with the same name), 
        its quantity is updated to the sum of the quantities of the two objects, 
        and the time_picked_up_ variable is updated to that of the latest instance of the item being added.
    */
    if (nw_itm->getType() == "CONSUMABLE") // item type is CONSUMABLE
    {
        auto same_item = getPointerTo(nw_itm);
        if (same_item != nullptr) // item is found
        {
            same_item->getItem()->setQuantity(same_item->getItem()->getQuantity() + nw_itm->getQuantity());
            same_item->getItem()->setTimePickedUp(nw_itm->getTimePickedUp());
            return true;
        }
    }
    add(nw_itm);
    return true;
}

/**
 * @param   : A reference to string name of the item to be removed from the inventory.
 * @return  : True if the item was found and removed or updated successfully, false otherwise.
 * @post    : If the item is found in the inventory, it is removed while preserving the BST structure.
            If a CONSUMABLE is removed, its quantity is decremented by one, but its time_picked_up_ remains the same. 
            However, if it is the last item (it's quantity is 1 and is being removed), the item should be removed. 
            Non-CONSUMABLE items should always be removed when they are found.
*/
bool Inventory::removeItem(const std::string& item_name){
    // Create a temporary Item with the specified name
    Item temp_item(item_name);
    // Find the node with the item in the inventory
    std::shared_ptr<BinaryNode<Item*>> temp_node = getPointerTo(&temp_item);

    if (temp_node != nullptr) {
        Item* item = temp_node->getItem();
        // If the item is CONSUMABLE and its quantity is more than 1, decrement quantity
        if (item->getType() == "CONSUMABLE" && item->getQuantity() > 1) 
        {
            item->setQuantity(item->getQuantity() - 1);
        } 
        else 
        {
            // Remove the item from the inventory if it's the last one or not a CONSUMABLE
            remove(item);
        }
        return true;
    }
    return false;
}

/**
 * @param   : A reference to string name of the item to be found.
 * @return  : An Item pointer to the found item, or nullptr if the item is not in the inventory.
 */
Item* Inventory::findItem(const std::string& item_name)
{
    std::shared_ptr<BinaryNode<Item*>> curr = getRoot();
    while(curr != nullptr){
        if (curr->getItem()->getName() == item_name){
            return curr->getItem();
        }
        else if (curr->getItem()->getName() < item_name){
            curr = curr->getLeftChildPtr();
        }
        else if (curr->getItem()->getName() > item_name){
            curr = curr->getRightChildPtr();
        }
    }
    return nullptr;
}

/**
 * @return    : The total sum of gold values of all items in the inventory.
 */
int Inventory::getTotalGoldValue() const
{
    int count = 0;
    std::vector<Item*> items;
    inorderVector(items, getRoot());
    for (int i = 0; i < items.size(); i++)
    {
        if (items[i]->getType() == "CONSUMABLE")
        {
            count += items[i]->getQuantity() * items[i]->getGoldValue();
        }
        else
        {
            count += items[i]->getGoldValue();
        }
    }
    return count;
}

/**
 * @post    : The names of all items in the Inventory are printed in ascending order.
 *            This function performs an in-order traversal of the binary search tree and prints the details of each item in the following format. 
            NOTE: QUANTITY ONLY NEEDS TO BE DISPLAYED FOR CONSUMABLE ITEMS.

            [NAME] ([TYPE])
            Level: [LEVEL]
            Value: [VALUE]
            Quantity: [QUANTITY]

            Example:
            TIRED GAUNTLETS (ARMOR)
            Level: 3
            Value: 25
        
            SMALL HEALTH POTION (CONSUMABLE)
            Level: 1
            Value: 10
            Quantity: 2
* 
*/
void Inventory::printInventoryInOrder() const{
    std::vector<Item*> items;
    inorderVector(items, getRoot());
    for (int i = 0; i < items.size(); i++)
    {
        std::cout << items[i]->getName() << " (" << items[i]->getType() << ")\nLevel: " << items[i]->getLevel() <<
            "\nValue: " << items[i]->getGoldValue();
        if (items[i]->getType() == "CONSUMABLE") 
        {
            std::cout << "\nQuantity: " << items[i]->getQuantity();
        }
        std::cout << "\n\n";
    }
}

void Inventory::inorderVector(std::vector<Item*>& items, std::shared_ptr<BinaryNode<Item*>> root) const{
    if (root != nullptr)
    {
        inorderVector(items, root->getLeftChildPtr());
        items.push_back(root->getItem());
        inorderVector(items, root->getRightChildPtr());
    }
}

/**
 * @param   : a reference to bool if the items are to be printed in ascending order. 
 * @param   : a reference to string attribute which defines the order by which the items are to be printed. 
 *          You may assume that the given parameter will be in one of the following forms: ["NAME"/"TYPE"/"LEVEL"/"VALUE"/"TIME"]
 * @post    : All items in the Inventory are printed in the order specified by the parameter.
 *          Example usage: inventory.printInventory(false, "LEVEL");
            NOTE: QUANTITY ONLY NEEDS TO BE DISPLAYED FOR CONSUMABLE ITEMS:

            FEATHER DUSTER (WEAPON)
            Level: 5
            Value: 100

            TIRED GAUNTLETS (ARMOR)
            Level: 3
            Value: 50

            SMALL HEALTH POTION (CONSUMABLE)
            Level: 1
            Value: 10
            Quantity: 2

            NOODLES (CONSUMABLE)
            Level: 0
            Value: 120
            Quantity: 5
*/
void Inventory::printInventory(bool ascending, const std::string& attribute) const{
    std::vector<Item*> items;
    inorderVector(items, getRoot());
    if (ascending && attribute == "NAME"){
        std::sort(items.begin(), items.end(),[](Item* const item1, Item* const item2)
            {
                return item1->getName() < item2->getName();  
            });
    }
    else if (!ascending && attribute == "NAME"){
        std::sort(items.begin(), items.end(),[](Item* const item1, Item* const item2)
            {
                return item1->getName() > item2->getName();  
            });
    }
    else if (ascending && attribute == "TYPE"){
        std::sort(items.begin(), items.end(),[](Item* const item1, Item* const item2)
            {
                return item1->getType() < item2->getType();  
            });
    }
    else if (!ascending && attribute == "TYPE"){
        std::sort(items.begin(), items.end(),[](Item* const item1, Item* const item2)
            {
                return item1->getType() > item2->getType();  
            });
    }
    else if (ascending && attribute == "LEVEL"){
        std::sort(items.begin(), items.end(),[](Item* const item1, Item* const item2)
            {
                return item1->getLevel() < item2->getLevel();  
            });
    }
    else if (!ascending && attribute == "LEVEL"){
        std::sort(items.begin(), items.end(),[](Item* const item1, Item* const item2)
            {
                return item1->getLevel() > item2->getLevel();  
            });
    }
    else if (ascending && attribute == "VALUE"){
        std::sort(items.begin(), items.end(),[](Item* const item1, Item* const item2)
            {
                return item1->getGoldValue() < item2->getGoldValue();  
            });
    }
    else if (!ascending && attribute == "VALUE"){
        std::sort(items.begin(), items.end(),[](Item* const item1, Item* const item2)
            {
                return item1->getGoldValue() > item2->getGoldValue();  
            });
    }
    else if (ascending && attribute == "TIME"){
        std::sort(items.begin(), items.end(),[](Item* const item1, Item* const item2)
            {
                return item1->getTimePickedUp() < item2->getTimePickedUp();  
            });
    }
    else if (!ascending && attribute == "TIME"){
        std::sort(items.begin(), items.end(),[](Item* const item1, Item* const item2)
            {
                return item1->getTimePickedUp() > item2->getTimePickedUp();  
            });
    }
    for (int i = 0; i < items.size(); i++)
    {
        std::cout << items[i]->getName() << " (" << items[i]->getType() << ")\nLevel: " << items[i]->getLevel() <<
            "\nValue: " << items[i]->getGoldValue();
        if (items[i]->getType() == "CONSUMABLE") 
        {
            std::cout << "\nQuantity: " << items[i]->getQuantity();
        }
        std::cout << "\n\n";
    }
}

