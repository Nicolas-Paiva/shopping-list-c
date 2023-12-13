#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

const int MAXITEMS = 50;
const int MAXSTR = 255;

// IMPORTANT:
// All CamelCase variables are strcuts

// Shopping Item (contains the name and the amount/number)
typedef struct
{
  char *name;
  int amount;
} ShopItem;

////////////////////////////////////////////////////////////////////////////////////////////////////////////
// FUNCTION PROTOTYPES

// Initialising all shopping items (pointers and amount)
void initPointers(ShopItem *shoppingList, int numItems);

// Creates a shopping item
void addShopItem(ShopItem *shoppingList, int shopListIndex);

void createShoppingList(ShopItem *shoppingList);

// Confirms whether the item should be added to the shopping list or not:
bool confirmItemToList(ShopItem *Item);

void showShoppingList(ShopItem *shoppingList);

////////////////////////////////////////////////////////////////////////////////////////////////////////////

int main(void)
{
  ShopItem shoppingListArr[MAXITEMS];
  int itemNum = 0;
  int add = true;

  initPointers(shoppingListArr, MAXITEMS);

  while (itemNum < 50 && add == true)
  {
    char addMoreItems;
    addShopItem(shoppingListArr, itemNum);

    printf("Do you want to add more items? ('y' / 'n') ");
    scanf(" %c", &addMoreItems);
    getchar();

    if (addMoreItems == 'n')
    {
      break;
    }
    itemNum++;
  }

  showShoppingList(shoppingListArr);

  return 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
// FUNCTION DEFINITIONS

void addShopItem(ShopItem *shoppingList, int shopListIndex)
{
  ShopItem ListItem;
  char itemName[MAXSTR];

  // Adds a name to the item
  printf("Please, introduce item number %d: ", shopListIndex + 1);
  fgets(itemName, MAXSTR, stdin);
  itemName[strlen(itemName) - 1] = '\0';

  // Ensures that the item has a name
  while (strlen(itemName) == 0)
  {
    printf("The product's name cannot be empty.\n");
    printf("Please, introduce item number %d: ", shopListIndex + 1);
    fgets(itemName, MAXSTR, stdin);
    itemName[strlen(itemName) - 1] = '\0';
  }

  ListItem.name = malloc(strlen(itemName) + 1);

  if (ListItem.name == NULL)
  {
    fprintf(stderr, "Error. Memory not allocated");
    exit(1);
  }

  strcpy(ListItem.name, itemName);

  // Adding the amount:
  printf("Amount: ");
  scanf("%d", &ListItem.amount);
  putchar('\n');

  if (confirmItemToList(&ListItem) == false)
  {
    return addShopItem(shoppingList, shopListIndex);
  }
  else
  {
    shoppingList[shopListIndex] = ListItem;
  }
  return;
}

////////////////////////

void initPointers(ShopItem *shoppingList, int numItems)
{
  for (int i = 0; i < numItems; i++)
  {
    shoppingList[i].name = NULL;
    shoppingList[i].amount = 0;
  }
  return;
}

////////////////////////

bool confirmItemToList(ShopItem *Item)
{
  char confirmation;
  printf("Confirm item:\n");
  printf("Product: %s\n", Item->name);
  printf("Amount: %d\n", Item->amount);
  printf("Press return to confirm, \"n\" to cancel ");

  getchar(); // Consumes the '\n' left in the buffer
  scanf("%c", &confirmation);

  if (confirmation == 'n')
  {
    getchar();
    putchar('\n');
    return false;
  }
  return true;
}

////////////////////////

void showShoppingList(ShopItem *shoppingList)
{
  int i = 0;
  printf("Current Shopping List:\n");
  while (shoppingList[i].name != NULL)
  {
    printf("------------------\n");
    printf("Product %d: %s\n", i + 1, shoppingList[i].name);
    printf("Amount: %d\n", shoppingList[i].amount);
    printf("------------------");
    i++;
  }
}