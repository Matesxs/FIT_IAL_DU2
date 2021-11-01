/*
 * Binárny vyhľadávací strom — iteratívna varianta
 *
 * S využitím dátových typov zo súboru btree.h, zásobníkov zo súborov stack.h a
 * stack.c a pripravených kostier funkcií implementujte binárny vyhľadávací
 * strom bez použitia rekurzie.
 */

#include "../btree.h"
#include "stack.h"
#include <stdio.h>
#include <stdlib.h>

/*
 * Inicializácia stromu.
 *
 * Užívateľ musí zaistiť, že incializácia sa nebude opakovane volať nad
 * inicializovaným stromom. V opačnom prípade môže dôjsť k úniku pamäte (memory
 * leak). Keďže neinicializovaný ukazovateľ má nedefinovanú hodnotu, nie je
 * možné toto detegovať vo funkcii.
 */
void bst_init(bst_node_t **tree) 
{
  if (tree == NULL) return;
  *tree = NULL;
}

/*
 * Nájdenie uzlu v strome.
 *
 * V prípade úspechu vráti funkcia hodnotu true a do premennej value zapíše
 * hodnotu daného uzlu. V opačnom prípade funckia vráti hodnotu false a premenná
 * value ostáva nezmenená.
 *
 * Funkciu implementujte iteratívne bez použitia vlastných pomocných funkcií.
 */
bool bst_search(bst_node_t *tree, char key, int *value) 
{
  bst_node_t *current = tree;

  while (current != NULL)
  {
    // If key is found return its value
    if (current->key == key)
    {
      *value = current->value;
      return true;
    }

    if (key < current->key)
    {
      // Go left
      current = current->left;
    }
    else
    {
      // Go right
      current = current->right;
    }
  }
  return false;
}

/*
 * Vloženie uzlu do stromu.
 *
 * Pokiaľ uzol so zadaným kľúčom v strome už existuje, nahraďte jeho hodnotu.
 * Inak vložte nový listový uzol.
 *
 * Výsledný strom musí spĺňať podmienku vyhľadávacieho stromu — ľavý podstrom
 * uzlu obsahuje iba menšie kľúče, pravý väčšie.
 *
 * Funkciu implementujte iteratívne bez použitia vlastných pomocných funkcií.
 */
void bst_insert(bst_node_t **tree, char key, int value) 
{
  if (tree == NULL) return;
  
  if (*tree == NULL)
  {
    // Tree is empty so create first node
    *tree = (bst_node_t*)malloc(sizeof(bst_node_t));
    if (*tree == NULL) return;

    (*tree)->key = key;
    (*tree)->value = value;
    (*tree)->left = NULL;
    (*tree)->right = NULL;
  }
  else
  {
    bst_node_t *current = *tree;

    while (true)
    {
      // Node with this key already exist
      if (key == current->key)
      {
        current->value = value;
        return;
      }

      if (key < current->key)
      {
        // Go left
        if (current->left != NULL)
          current = current->left;
        else break;
      }
      else 
      {
        // Go right
        if (current->right != NULL)
          current = current->right;
        else break;
      }
    }

    if (key < current->key)
    {
      // Insert left
      current->left = (bst_node_t*)malloc(sizeof(bst_node_t));
      if (current->left == NULL) return;

      current->left->key = key;
      current->left->value = value;

      current->left->left = NULL;
      current->left->right = NULL;
    }
    else 
    {
      // Insert right
      current->right = (bst_node_t*)malloc(sizeof(bst_node_t));
      if (current->right == NULL) return;

      current->right->key = key;
      current->right->value = value;

      current->right->left = NULL;
      current->right->right = NULL;
    }
  }
}

/*
 * Pomocná funkcia ktorá nahradí uzol najpravejším potomkom.
 *
 * Kľúč a hodnota uzlu target budú nahradené kľúčom a hodnotou najpravejšieho
 * uzlu podstromu tree. Najpravejší potomok bude odstránený. Funkcia korektne
 * uvoľní všetky alokované zdroje odstráneného uzlu.
 *
 * Funkcia predpokladá že hodnota tree nie je NULL.
 *
 * Táto pomocná funkcia bude využitá pri implementácii funkcie bst_delete.
 *
 * Funkciu implementujte iteratívne bez použitia vlastných pomocných funkcií.
 */
void bst_replace_by_rightmost(bst_node_t *target, bst_node_t **tree) 
{
  if (tree == NULL) return;
  if (*tree == NULL) return;
  if (target == NULL) return;

  bst_node_t *prev = NULL;
  bst_node_t *current = *tree;
  while (current->right != NULL)
  {
    prev = current;
    current = current->right;
  }

  if (prev != NULL)
    prev->right = current->left;

  target->key = current->key;
  target->value = current->value;
  free(current);
}

/*
 * Odstránenie uzlu v strome.
 *
 * Pokiaľ uzol so zadaným kľúčom neexistuje, funkcia nič nerobí.
 * Pokiaľ má odstránený uzol jeden podstrom, zdedí ho otec odstráneného uzla.
 * Pokiaľ má odstránený uzol oba podstromy, je nahradený najpravejším uzlom
 * ľavého podstromu. Najpravejší uzol nemusí byť listom!
 * Funkcia korektne uvoľní všetky alokované zdroje odstráneného uzlu.
 *
 * Funkciu implementujte iteratívne pomocou bst_replace_by_rightmost a bez
 * použitia vlastných pomocných funkcií.
 */
void bst_delete(bst_node_t **tree, char key) 
{
  if (tree == NULL) return;
  if (*tree == NULL) return;

  bst_node_t *prev = NULL;
  bool prevIsRight = false;
  bst_node_t *current = *tree;
  while (current != NULL)
  {
    if (current->key == key)
    {
      break;
    }

    prev = current;
    if (key < current->key)
    {
      prevIsRight = false;
      current = current->left;
    }
    else
    {
      prevIsRight = true;
      current = current->right;
    }
  }

  if (current != NULL)
  {
    if (current->left == NULL && current->right == NULL)
    {
      // its leaf
      if (prev != NULL)
      {
        if (prevIsRight)
          // comming from right
          prev->right = NULL;
        else
          // comming from left
          prev->left = NULL;
      }
      else
        // its core node so replace it
        *tree = NULL;

      free(current);
    }
    else if (current->left != NULL && current->right == NULL)
    {
      // it have only left side
      if (prev != NULL)
      {
        if (prevIsRight)
          // comming from right
          prev->right = current->left;
        else
          // comming from left
          prev->left = current->left;
      }
      else
        // its core node so replace it
        *tree = current->left;

      free(current);
    }
    else if (current->left == NULL && current->right != NULL)
    {
      // it have only right side
      if (prev != NULL)
      {
        if (prevIsRight)
          // comming from right
          prev->right = current->right;
        else
          // comming from left
          prev->left = current->right;
      }
      else
        // its core node so replace it
        *tree = current->right;

      free(current);
    }
    else
    {
      // it have branches on both sides
      bst_replace_by_rightmost(current, &current->left);
    }
  }
}

/*
 * Zrušenie celého stromu.
 *
 * Po zrušení sa celý strom bude nachádzať v rovnakom stave ako po
 * inicializácii. Funkcia korektne uvoľní všetky alokované zdroje rušených
 * uzlov.
 *
 * Funkciu implementujte iteratívne pomocou zásobníku uzlov a bez použitia
 * vlastných pomocných funkcií.
 */
void bst_dispose(bst_node_t **tree) 
{
  if (tree == NULL) return;
  if (*tree == NULL) return;

  // stack of nodes need to be go thru for dispose
  stack_bst_t toDisposeStack;
  stack_bst_init(&toDisposeStack);

  // push core node to stack
  stack_bst_push(&toDisposeStack, *tree);

  // iterate until stack is empty
  while (!stack_bst_empty(&toDisposeStack))
  {
    // get top of the stack
    bst_node_t *current = stack_bst_pop(&toDisposeStack);

    // if current node have left or right branch then add it to stack
    if (current->left != NULL)
      stack_bst_push(&toDisposeStack, current->left);

    if (current->right != NULL)
      stack_bst_push(&toDisposeStack, current->right);

    // dispose current node
    free(current);
  }

  // clear pointer to core node
  *tree = NULL;
}

/*
 * Pomocná funkcia pre iteratívny preorder.
 *
 * Prechádza po ľavej vetve k najľavejšiemu uzlu podstromu.
 * Nad spracovanými uzlami zavola bst_print_node a uloží ich do zásobníku uzlov.
 *
 * Funkciu implementujte iteratívne pomocou zásobníku uzlov a bez použitia
 * vlastných pomocných funkcií.
 */
void bst_leftmost_preorder(bst_node_t *tree, stack_bst_t *to_visit) 
{
  if (tree == NULL) return;

  bst_node_t *current = tree;
  while (current != NULL)
  {
    // Go thru whole left branch and print all its nodes
    bst_print_node(current);

    // if node have right branch add it to processing later
    if (current->right != NULL)
      stack_bst_push(to_visit, current->right);

    // go to next left node
    current = current->left;
  }
}

/*
 * Preorder prechod stromom.
 *
 * Pre aktuálne spracovávaný uzol nad ním zavolajte funkciu bst_print_node.
 *
 * Funkciu implementujte iteratívne pomocou funkcie bst_leftmost_preorder a
 * zásobníku uzlov bez použitia vlastných pomocných funkcií.
 */
void bst_preorder(bst_node_t *tree) 
{
  if (tree == NULL) return;

  // Init stack of nodes that need to be visited
  stack_bst_t toVisitStack;
  stack_bst_init(&toVisitStack);

  // Add core node to nodes to visit
  stack_bst_push(&toVisitStack, tree);

  // Iterate until stack of nodes to visit is empty
  while (!stack_bst_empty(&toVisitStack))
  {
    // go thru each node on stack and go thru it from left
    bst_leftmost_preorder(stack_bst_pop(&toVisitStack), &toVisitStack);
  }
}

/*
 * Pomocná funkcia pre iteratívny inorder.
 *
 * Prechádza po ľavej vetve k najľavejšiemu uzlu podstromu a ukladá uzly do
 * zásobníku uzlov.
 *
 * Funkciu implementujte iteratívne pomocou zásobníku uzlov a bez použitia
 * vlastných pomocných funkcií.
 */
void bst_leftmost_inorder(bst_node_t *tree, stack_bst_t *to_visit) 
{
  if (tree == NULL) return;

  bst_node_t *current = tree;
  while (current != NULL)
  {
    // Add all left nodes on current branch to stack
    stack_bst_push(to_visit, current);
    current = current->left;
  }
}

/*
 * Inorder prechod stromom.
 *
 * Pre aktuálne spracovávaný uzol nad ním zavolajte funkciu bst_print_node.
 *
 * Funkciu implementujte iteratívne pomocou funkcie bst_leftmost_inorder a
 * zásobníku uzlov bez použitia vlastných pomocných funkcií.
 */
void bst_inorder(bst_node_t *tree) 
{
  if (tree == NULL) return;

  stack_bst_t toVisitStack;
  stack_bst_init(&toVisitStack);

  // Add left branch of core node to stack of nodes to visit
  bst_leftmost_inorder(tree->left, &toVisitStack);

  while (!stack_bst_empty(&toVisitStack))
  {
    // Get node from stack
    bst_node_t *current = stack_bst_pop(&toVisitStack);

    // Print node
    bst_print_node(current);

    // If node have right branch then get all its left nodes to stack
    if (current->right != NULL)
    {
      bst_leftmost_inorder(current->right, &toVisitStack);
    }
  }

  // print core node
  bst_print_node(tree);

  // Add right branch of core node to stack of nodes to visit
  bst_leftmost_inorder(tree->right, &toVisitStack);

  // Same as the left branch
  while (!stack_bst_empty(&toVisitStack))
  {
    bst_node_t *current = stack_bst_pop(&toVisitStack);

    bst_print_node(current);

    if (current->right != NULL)
    {
      bst_leftmost_inorder(current->right, &toVisitStack);
    }
  }
}

/*
 * Pomocná funkcia pre iteratívny postorder.
 *
 * Prechádza po ľavej vetve k najľavejšiemu uzlu podstromu a ukladá uzly do
 * zásobníku uzlov. Do zásobníku bool hodnôt ukladá informáciu že uzol
 * bol navštívený prvý krát.
 *
 * Funkciu implementujte iteratívne pomocou zásobníkov uzlov a bool hodnôt a bez použitia
 * vlastných pomocných funkcií.
 */
void bst_leftmost_postorder(bst_node_t *tree, stack_bst_t *to_visit,
                            stack_bool_t *first_visit) 
{
  if (tree == NULL) return;

  bst_node_t *current = tree;
  while (current != NULL)
  {
    // Go thru most left branch, add its nodes to stack and mark it as first visited
    stack_bst_push(to_visit, current);
    stack_bool_push(first_visit, true);
    current = current->left;
  }
}

/*
 * Postorder prechod stromom.
 *
 * Pre aktuálne spracovávaný uzol nad ním zavolajte funkciu bst_print_node.
 *
 * Funkciu implementujte iteratívne pomocou funkcie bst_leftmost_postorder a
 * zásobníkov uzlov a bool hodnôt bez použitia vlastných pomocných funkcií.
 */
void bst_postorder(bst_node_t *tree) 
{
  if (tree == NULL) return;

  // Create stack for nodes to visit and stack with flags that marks nodes that were already visited
  stack_bst_t toVisitStack;
  stack_bst_init(&toVisitStack);
  stack_bool_t firstVisitFlagStack;
  stack_bool_init(&firstVisitFlagStack);

  // Get nodes from left branch (and center node)
  bst_leftmost_postorder(tree, &toVisitStack, &firstVisitFlagStack);

  while (!stack_bst_empty(&toVisitStack))
  {
    // get pointer to current node from nodes to visit
    bst_node_t *tmp = stack_bst_top(&toVisitStack);

    // Create bool that signalize that current node was already visited
    // If node was visited for the first time then it came from left
    bool visited = stack_bool_pop(&firstVisitFlagStack);

    if (visited)
    {
      // Mark current node as visited for the second time
      stack_bool_push(&firstVisitFlagStack, false);

      // And go thru all its left nodes on the right branch
      bst_leftmost_postorder(tmp->right, &toVisitStack, &firstVisitFlagStack);
    }
    else
    {
      // If node was already visited then we can print it
      bst_print_node(stack_bst_pop(&toVisitStack));
    }
  }
}
