#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_INGREDIENTS 10
#define MAX_RECIPES 100
#define TABLE_SIZE 101

// Recipe structure
typedef struct {
    char name[50];
    char ingredients[MAX_INGREDIENTS][50];
    int numIngredients;
} Recipe;

// Recipe book structure
typedef struct {
    Recipe recipes[MAX_RECIPES];
    int numRecipes;
} RecipeBook;

// Hash Table Node
typedef struct {
    char key[50];
    int recipeIndex;
} HashNode;

// Hash Table
HashNode hashTable[TABLE_SIZE];

// Hash function
int hash(char *key) {
    int hashValue = 0;
    while (*key) {
        hashValue = (hashValue << 5) + *key++;
    }
    return hashValue % TABLE_SIZE;
}

// Function to add a recipe to the recipe book and hash table
void addRecipe(RecipeBook *book, Recipe newRecipe) {
    if (book->numRecipes < MAX_RECIPES) {
        book->recipes[book->numRecipes++] = newRecipe;

        // Add to the hash table
        int index = hash(newRecipe.name);
        while (hashTable[index].recipeIndex != -1) {
            index = (index + 1) % TABLE_SIZE;
        }
        strcpy(hashTable[index].key, newRecipe.name);
        hashTable[index].recipeIndex = book->numRecipes - 1;

        printf("Recipe added successfully!\n");
    } else {
        printf("Recipe book is full. Cannot add more recipes.\n");
    }
}

// Function to search for a recipe in the recipe book
void searchRecipe(RecipeBook *book, char *recipeName) {
    int index = hash(recipeName);
    while (hashTable[index].recipeIndex != -1) {
        if (strcmp(hashTable[index].key, recipeName) == 0) {
            int recipeIndex = hashTable[index].recipeIndex;
            printf("Recipe found:\n");
            printf("Name: %s\n", book->recipes[recipeIndex].name);
            printf("Ingredients:\n");
            for (int i = 0; i < book->recipes[recipeIndex].numIngredients; i++) {
                printf("- %s\n", book->recipes[recipeIndex].ingredients[i]);
            }
            return;
        }
        index = (index + 1) % TABLE_SIZE;
    }
    printf("Recipe not found.\n");
}

// Function to generate a shopping list based on selected recipes
void generateShoppingList(RecipeBook *book, char **selectedRecipes, int numSelected) {
    printf("Shopping List:\n");
    for (int i = 0; i < numSelected; i++) {
        searchRecipe(book, selectedRecipes[i]);
    }
}

// Function to display the main menu
void displayMenu() {
    printf("\nDigital Recipe Book\n");
    printf("1. Add Recipe\n");
    printf("2. Search Recipe\n");
    printf("3. Generate Shopping List\n");
    printf("4. Exit\n");
    printf("Choose an option: ");
}

int main() {
    RecipeBook recipeBook;
    recipeBook.numRecipes = 0;

    // Initialize hash table
    for (int i = 0; i < TABLE_SIZE; i++) {
        hashTable[i].recipeIndex = -1;
    }

    int choice;
    do {
        displayMenu();
        scanf("%d", &choice);

        switch (choice) {
            case 1: // Add Recipe
                {
                    Recipe newRecipe;
                    printf("Enter recipe name: ");
                    scanf("%s", newRecipe.name);

                    printf("Enter number of ingredients: ");
                    scanf("%d", &newRecipe.numIngredients);

                    printf("Enter ingredients:\n");
                    for (int i = 0; i < newRecipe.numIngredients; i++) {
                        printf("  Ingredient %d: ", i + 1);
                        scanf("%s", newRecipe.ingredients[i]);
                    }

                    addRecipe(&recipeBook, newRecipe);
                }
                break;

            case 2: // Search Recipe
                {
                    char searchName[50];
                    printf("Enter the recipe name to search: ");
                    scanf("%s", searchName);
                    searchRecipe(&recipeBook, searchName);
                }
                break;

            case 3: // Generate Shopping List
                {
                    int numSelected;
                    printf("Enter the number of recipes for the shopping list: ");
                    scanf("%d", &numSelected);

                    // Dynamic allocation for selectedRecipes
                    char **selectedRecipes = malloc(numSelected * sizeof(char *));
                    if (selectedRecipes == NULL) {
                        printf("Memory allocation failed.\n");
                        return 1;
                    }

                    for (int i = 0; i < numSelected; i++) {
                        selectedRecipes[i] = malloc(50 * sizeof(char));
                        if (selectedRecipes[i] == NULL) {
                            printf("Memory allocation failed.\n");

                            // Free allocated memory before returning
                            for (int j = 0; j < i; j++) {
                                free(selectedRecipes[j]);
                            }
                            free(selectedRecipes);

                            return 1;
                        }
                    }

                    printf("Enter recipe names for the shopping list:\n");
                    for (int i = 0; i < numSelected; i++) {
                        printf("  Recipe %d: ", i + 1);
                        scanf("%s", selectedRecipes[i]);
                    }

                    generateShoppingList(&recipeBook, selectedRecipes, numSelected);

                    // Free the allocated memory
                    for (int i = 0; i < numSelected; i++) {
                        free(selectedRecipes[i]);
                    }
                    free(selectedRecipes);
                }
                break;

            case 4: // Exit
                printf("Exiting the Digital Recipe Book. Goodbye!\n");
                break;

            default:
                printf("Invalid choice. Please choose a valid option.\n");
        }

    } while (choice != 4);

    return 0;
}
