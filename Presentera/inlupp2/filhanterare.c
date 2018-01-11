#include <stdio.h>
#include <stdlib.h>
#include "lager.h"



typedef struct item item_t;
typedef struct shelf shelf_t;

struct shelf
{
  char *name;
  int amount; 
};

struct item

{
  char *name;
  char *description;
  int price;
  list_t *storage;
};

  

//extern item_t *make_item(char *name, char *description, int price, char *shelf_name, int amount); 

bool save_item_to_file(tree_key_t key, elem_t elem, void *file_pointer)//Sparar en vara till fil. 
{
  
  if(! file_pointer)//Om file_pointern inte pekar på något,så finns inte filen. 
    {
      puts("Filen finns ej");
      return false;
    }
  FILE *file = file_pointer;
  item_t *item = elem.p;
  int length =list_length(item->storage);
  elem_t shelfelem;
  shelf_t *tmpshelf;

  fputs(item->name, file);//Skriver en string till fil, men inkluderar inte null-tecknet. Varunamn.   
  fputc('\n', file);//
  fputs(item->description, file);//Varubeskrivning. 
  fputc('\n', file);
  fprintf(file, "%d", item->price);//Pris. 
  fputc('\n', file);
  fprintf(file, "%d", length);//Hur många Hyllor. 
  fputc('\n', file);

  for(int i = 0; i < length; i++)
    {
      list_get(item->storage, i, &shelfelem);
      tmpshelf = shelfelem.p;
      fputs(tmpshelf->name, file);
      fputc('\n', file);
      fprintf(file, "%d", tmpshelf->amount);
      fputc('\n', file); 
    }
  return true; 

  
}

bool write_file(tree_t *tree, FILE *fptr)
{
  return tree_apply(tree, 0, save_item_to_file, fptr);
}

tree_t *read_file(tree_t *tree, FILE *fptr)
{
  char *name = NULL;
  char *desc = NULL;
  char *intstring = NULL; 
  int price;
  int numofShelf; //Hur många hyllor som ett item har.
  char *shelfname = NULL;
  int amount;

  size_t len = 0;
  ssize_t read = 0;
  //End of file
  while(read != -1){
    read = getline(&name, &len, fptr);

    //Om read == -1 så är det en ogiltigt inläsning, dvs. läser i slutet av filen. Retunerar tree. 

    if(read == -1)
      {
        free(name);
        free(desc);
        free(intstring);
        free(shelfname);
        return tree; 
  
      }

    //Läser in resterande 
    getline(&desc, &len, fptr);
    getline(&intstring, &len, fptr);
    price = atoi(intstring);//Atoi =converts string to an integer.  
    getline(&intstring, &len, fptr);
    numofShelf = atoi(intstring);

    name[strcspn(name, "\n")] = '\0'; //strcspn retunerar längden av det segment av den första strängen som inte finns i den andra strängen. 
    desc[strcspn(desc, "\n")] = '\0';


    list_t *shelves = list_new(NULL,NULL,NULL); // Vill skapa en ny tom hylla! 


    for(int i = 0; i < numofShelf; i++)// Lägger in varje hylla i listan. 
      {
        getline(&shelfname, &len, fptr);
        getline(&intstring, &len, fptr);
        shelfname[strcspn(shelfname, "\n")] = '\0'; 
        amount = atoi(intstring);
        
        shelf_t *shelf = calloc(1, sizeof(shelf_t));
        shelf->name = strdup(shelfname);
        shelf->amount = amount;
        elem_t element = {.p = shelf};
        list_append(shelves, element);
        free(shelf->name);
        free(shelf);
      }
  
    item_t *item = make_item(strdup(name), strdup(desc), price, strdup(shelfname), amount); //Skapar en ny vara med all indata.   
    elem_t key = {.p = item->name};
    elem_t elem = {.p =item};

    tree_insert(tree, key, elem);
  }
  return tree;
}



tree_t *load_tree(tree_t *tree, const char *filename)//Laddar databas(tree)
{
  FILE *fptr;//Filepointer
  fptr = fopen (filename, "r");

  if (fptr == NULL)
    {
      printf("Kunde inte hitta fil med det namnet %s.\n"
             "Skapar en ny tom databas.\n", filename);
      fptr = fopen(filename, "w");
      fclose(fptr);
      return tree; 
    }

  //Om filen finns. Så läser vi in varorna som finns i databasen. 
  tree = read_file(tree, fptr);
  fclose(fptr);
  return tree; 
}

void save_tree(tree_t *tree, const char *filename)
{
  FILE *fptr = fopen(filename, "w"); 
  if(write_file(tree, fptr))
    {
      printf("\nSparar %d varor i %s\n\n", tree_size(tree), filename); 
    }
  fclose(fptr); 
}







