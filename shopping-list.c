#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>

// TERMINAL COLOURS
#define RED "\033[0;31m"
#define GREEN "\033[0;32m"
#define YELLOW "\033[0;33m"
#define BRED "\033[1;31m"
#define RESET "\033[0m"
#define BOLD "\033[1m"

// CONSTANTS
const int MAXITEMS = 50;
const int MAXSTR = 255;

// IMPORTANT:
// CamelCase variables are strcuts

// Shopping Item (contains the name and the amount/number)
typedef struct
{
  char *name;
  int amount;
} ShopItem;

////////////////////////////////////////////////////////////////////////////////////////////////////////////
// FUNCTION PROTOTYPES

//////////////////////////////////
// ### INITIALISATION AND CREATION OF THE LIST

// Initialiss the items in the array
void initPointers(ShopItem *shoppingList, int numItems);

// Creates a shopping item
void addShopItem(ShopItem *shoppingList, int shopListIndex);

// Asks for more items to be added to the list:
void createShoppingList(ShopItem *shoppingList, int MAXITEMS);

// Confirms whether the item should be added to the shopping list or not:
bool confirmItemToList(ShopItem *Item);

// Frees the dynamically allocated memory in the items:
void freeMemoryAndReset(ShopItem *shoppingList);

//////////////////////////////////
// ### INTERACTION WITH THE LIST

// Returns the number of elements in the list
int countShopListItems(ShopItem *shoppingList);

// Returns the index of a certain item if it is in the list
int findItem(ShopItem *shoppingList, char *itemName);

//////////////////////////////////
// ### DISPLAY FUNCTIONS

// Shows the shopping list
void showShoppingList(ShopItem *shoppingList);

// Formats the item name for styling purposes
void formatItemName(char *itemName);

// Shows an item from the list
void showListItem(ShopItem *shoppingList, int index);

void showInitialMenu();

////////////////////////////////////////////////////////////////////////////////////////////////////////////

int main(void)
{
  ShopItem shoppingListArr[MAXITEMS];
  int itemsTotal;
  int option;

  showInitialMenu();

  scanf("%d", &option);
  getchar();

  if (option == 1)
  {
    initPointers(shoppingListArr, MAXITEMS);
    createShoppingList(shoppingListArr, MAXITEMS);
    showShoppingList(shoppingListArr);
    itemsTotal = countShopListItems(shoppingListArr);
    printf("Total items: %d\n", itemsTotal);
  }
  else if (option == 2)
  {
    abort();
  }

  putchar('\n');
  printf("-------------------");
  putchar('\n');

  printf("Product: ");

  char itemName[MAXSTR];
  fgets(itemName, MAXSTR, stdin);
  itemName[strlen(itemName) - 1] = '\0';

  int listIndex = findItem(shoppingListArr, itemName);

  showListItem(shoppingListArr, listIndex);

  freeMemoryAndReset(shoppingListArr);

  return 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
// FUNCTION DEFINITIONS

void addShopItem(ShopItem *shoppingList, int shopListIndex)
{
  ShopItem ListItem;
  char itemName[MAXSTR];

  // Adds a name to the item
  putchar('\n');
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

  formatItemName(itemName);

  strcpy(ListItem.name, itemName);

  // Adding the amount:
  printf("Amount: ");
  int itemAmount = scanf("%d", &ListItem.amount);

  // If user passes an invalid value such as a character or 0
  if (itemAmount == 0 || ListItem.amount == 0)
  {
    ListItem.amount = 1;
  }

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

void createShoppingList(ShopItem *shoppingList, int MAXITEMS)
{
  int shopListIndex = 0;
  while (shopListIndex < MAXITEMS)
  {
    addShopItem(shoppingList, shopListIndex);

    printf("Add another item? Press 'n' to exit\n");

    char addMore;
    scanf("%c", &addMore);

    if (addMore == 'n')
    {
      getchar(); // Remove o '\n' apos o 'n'
      break;
    }

    shopListIndex++;
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
  printf("Press return to confirm, \"n\" to cancel\n");

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
  putchar('\n');
  return;
}

////////////////////////

int countShopListItems(ShopItem *shoppingList)
{
  int counter = 0;
  while (shoppingList[counter].name != NULL && counter < MAXITEMS)
  {
    counter++;
  }
  return counter;
}

////////////////////////

int findItem(ShopItem *shoppingList, char *itemName)
{
  int i = 0;
  while (shoppingList[i].name != NULL && i < MAXITEMS)
  {
    int equalStr = strcasecmp(shoppingList[i].name, itemName);
    if (equalStr == 0)
    {
      return i;
    }
    i++;
  }
  return -1;
}

////////////////////////

void formatItemName(char *itemName)
{
  char firstLetter = toupper(itemName[0]);
  itemName[0] = firstLetter;

  for (int i = 1; i < strlen(itemName); i++)
  {
    char otherLetters = tolower(itemName[i]);
    itemName[i] = otherLetters;
  }
  return;
}

////////////////////////

void showListItem(ShopItem *shoppingList, int index)
{
  printf("Item: %s\n", shoppingList[index].name);
  printf("Amount: %d\n", shoppingList[index].amount);
}

////////////////////////

void freeMemoryAndReset(ShopItem *shoppingList)
{
  int i = 0;
  while (i < 50 && shoppingList[i].name != NULL)
  {
    free(shoppingList[i].name);
    shoppingList[i].amount = 0;
    i++;
  }
}

void showInitialMenu()
{
  printf("%s----------------------------------\n", BOLD);
  printf("%sWelcome to your shopping list!\n%s", BRED, RESET);
  printf("%s----------------------------------\n", BOLD);
  putchar('\n');
  printf("Options:\n%s", RESET);
  printf("1- Create New List\n");
  printf("2- Quit\n");
  putchar('\n');
}
