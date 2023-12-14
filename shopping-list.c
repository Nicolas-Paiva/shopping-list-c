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
// ### MAIN
void newList(ShopItem *shoppingList, int MAXITEMS);

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

// Adds new items to the end of the list
void addNewItems(ShopItem *shoppingList);

//////////////////////////////////
// ### DISPLAY FUNCTIONS

// Shows the shopping list
void showShoppingList(ShopItem *shoppingList);

// Formats the item name for styling purposes
void formatItemName(char *itemName);

// Shows an item from the list
void showListItem(ShopItem *shoppingList, int index);

// Shows the initial menu so that the user can decide whether or not to create a new shopping list
void showInitialMenu();

// For display/organisation purposes
void horizontalBar();

// Once the user creates/loads a shopping list, they will have the options to display the list, select an item to display, modify an item, etc
void showMainMenu();

// Displays the main menu again
void menuReset(int *option);

void addWhiteSpace();

//////////////////////////////////
// ### FILE HANDLING

// TODO Learn how to preperly write to a file
void saveList(ShopItem *shoppingList);

// TODO Function that will read a shopping list from a file
void loadList();

////////////////////////////////////////////////////////////////////////////////////////////////////////////

int main(void)
{
  ShopItem shoppingListArr[MAXITEMS];
  int itemsTotal;
  int initOption;
  int mainMenuOption;

  // INITIAL MENU
  showInitialMenu();

  scanf("%d", &initOption);
  getchar();

  if (initOption == 1) // CREATES A NEW LIST
  {
    newList(shoppingListArr, MAXITEMS);
  }
  else if (initOption == 2) // EXIT
  {
    exit(1);
  }

  showMainMenu();

  scanf("%d", &mainMenuOption);
  getchar();

  while (mainMenuOption != 6)
  {
    // SHOWS THE LIST
    if (mainMenuOption == 1)
    {
      showShoppingList(shoppingListArr);
      itemsTotal = countShopListItems(shoppingListArr);
      printf("Total items: %d\n", itemsTotal);

      menuReset(&mainMenuOption);
    }

    // FINDS AND DISPLAY AN ITEM OF THE LIST (IF IT EXISTS)
    else if (mainMenuOption == 2)
    {
      char itemName[MAXSTR];

      printf("Item: ");
      fgets(itemName, MAXSTR, stdin);
      itemName[strlen(itemName) - 1] = '\0';

      int listIndex = findItem(shoppingListArr, itemName);

      if (listIndex >= 0)
      {
        showListItem(shoppingListArr, listIndex);
      }
      else
      {
        printf("Item does not exist\n");
      }
      menuReset(&mainMenuOption);
    }

    // ADDS MORE ITEMS TO THE LIST
    else if (mainMenuOption == 3)
    {
      addNewItems(shoppingListArr);
      showShoppingList(shoppingListArr);
      menuReset(&mainMenuOption);
    }
  }

  freeMemoryAndReset(shoppingListArr);

  return 0;
}

// FUNCTION DEFINITIONS
////////////////////////////////////////////////////////////////////////////////////////////////////////////

// ### MAIN

void newList(ShopItem *shoppingList, int MAXITEMS)
{
  initPointers(shoppingList, MAXITEMS);
  createShoppingList(shoppingList, MAXITEMS);
}

// ### LIST INITIALISATION/CREATION/MEMORY
void initPointers(ShopItem *shoppingList, int numItems)
{
  for (int i = 0; i < numItems; i++)
  {
    shoppingList[i].name = NULL;
    shoppingList[i].amount = 0;
  }
  return;
}

void addShopItem(ShopItem *shoppingList, int shopListIndex)
{
  ShopItem ListItem;
  char itemName[MAXSTR];

  // Adds a name to the item
  putchar('\n');
  printf("Item %d: ", shopListIndex + 1);
  fgets(itemName, MAXSTR, stdin);
  itemName[strlen(itemName) - 1] = '\0';

  // Ensures that the item has a name
  while (strlen(itemName) == 0)
  {
    printf("The name cannot be empty.\n");
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
    free(ListItem.name);
    return addShopItem(shoppingList, shopListIndex);
  }
  else
  {
    shoppingList[shopListIndex] = ListItem;
  }
  return;
}

void createShoppingList(ShopItem *shoppingList, int MAXITEMS)
{
  int shopListIndex = 0;
  while (shopListIndex < MAXITEMS)
  {
    addShopItem(shoppingList, shopListIndex);

    printf("Add another item? Press return to continue, 'n' to exit\n");

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

bool confirmItemToList(ShopItem *Item)
{
  char confirmation;
  printf("Confirm item:\n");
  horizontalBar();
  printf("Product: %s\n", Item->name);
  printf("Amount: %d\n", Item->amount);
  horizontalBar();
  printf("Return to confirm, \"n\" to cancel\n");

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

void freeMemoryAndReset(ShopItem *shoppingList)
{
  int i = 0;
  while (i < MAXITEMS && shoppingList[i].name != NULL)
  {
    free(shoppingList[i].name);
    shoppingList[i].amount = 0;
    i++;
  }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ### LIST INTERACTION

int countShopListItems(ShopItem *shoppingList)
{
  int counter = 0;
  while (shoppingList[counter].name != NULL && counter < MAXITEMS)
  {
    counter++;
  }
  return counter;
}

int findItem(ShopItem *shoppingList, char *itemName)
{
  int i = 0;
  while (i < MAXITEMS && shoppingList[i].name != NULL)
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

void addNewItems(ShopItem *shoppingList)
{
  // First we get the index where the item will be placed:
  int index = countShopListItems(shoppingList);
  while (index < MAXITEMS)
  {
    char verify;
    addShopItem(shoppingList, index);
    printf("Add more items? (press 'n' to leave)");
    scanf("%c", &verify);

    if (verify == 'n')
    {
      getchar();
      break;
    }
    index++;
  }
  return;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ### LIST/ITEM DISPLAY

void showShoppingList(ShopItem *shoppingList)
{
  int i = 0;
  horizontalBar();
  printf("%sCurrent Shopping List:%s\n", YELLOW, RESET);
  while (shoppingList[i].name != NULL)
  {
    horizontalBar();
    printf("%sProduct %d:%s %s\n", BOLD, i + 1, RESET, shoppingList[i].name);
    printf("%sAmount:%s %d\n", BOLD, RESET, shoppingList[i].amount);
    i++;
  }
  putchar('\n');
  return;
}

void showListItem(ShopItem *shoppingList, int index)
{
  horizontalBar();
  printf("Item: %s\n", shoppingList[index].name);
  printf("Amount: %d\n", shoppingList[index].amount);
  horizontalBar();
}

void horizontalBar()
{
  printf("------------------\n");
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ### MENU

void showInitialMenu()
{
  printf("%s----------------------------------\n", BOLD);
  printf("%sWelcome to your shopping list!\n%s", BRED, RESET);
  printf("%s----------------------------------\n", BOLD);
  putchar('\n');
  horizontalBar();
  printf("Options:\n");
  printf("1- Create New List\n");
  printf("2- Quit\n%s", RESET);
  horizontalBar();
  putchar('\n');
}

void showMainMenu()
{
  putchar('\n');
  printf("%s----------------------------------\n", BOLD);
  printf("%sACTIONS:\n%s", BRED, RESET);
  printf("%s----------------------------------\n", BOLD);
  printf("1- Show List\n");
  printf("2- Find item\n");
  printf("3- Add more items\n");
  printf("4- Save list\n");
  printf("5- Create new list\n");
  printf("6- Quit\n%s", RESET);
}

void addWhiteSpace()
{
  for (int i = 0; i < 2; i++)
  {
    putchar('\n');
  }
  return;
}

void menuReset(int *option)
{
  showMainMenu();
  scanf("%d", option);
  getchar();
  return;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ### FILE HANDLING

void saveList(ShopItem *shoppingList)
{
  char fileName[MAXSTR];
  printf("Please choose the name of the file: ");
  fgets(fileName, MAXSTR, stdin);

  FILE *fp = fopen("%s.txt", "w");
}
