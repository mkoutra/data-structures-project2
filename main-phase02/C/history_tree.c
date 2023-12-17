#include <stdio.h>
#include <stdlib.h>

#include "Movie.h"

/******************************************************************************
*************************** DOUBLY-LINKED LEAF-ORIENTED BST *******************
******************************************************************************/


/* Check if a node is leaf. */
int IsLeaf(userMovie_t* v) {
    return ((v->lc == NULL) && (v->rc == NULL));
}

/* Insert a node in a doubly linked leaf-oriented BST */
void InsertHistoryLeaf(userMovie_t** root, int movie_id, int cat, int rating) {
    userMovie_t* tmp = (*root);
    userMovie_t* prev = NULL;

    /* Node containing the info of the new node*/
    userMovie_t* new_watched_film = (userMovie_t*)malloc(sizeof(userMovie_t));
    new_watched_film->movieID = movie_id;
    new_watched_film->category = cat;
    new_watched_film->score = rating;
    new_watched_film->parent = NULL;
    new_watched_film->lc = NULL;
    new_watched_film->rc = NULL;

    /* Initially Empty tree */
    if (*root == NULL) {
        (*root) = new_watched_film;
        return;
    }

    /* Find v' */
    while (tmp != NULL) {
        prev = tmp;

        if (movie_id < tmp->movieID) {
            tmp = tmp->lc;
        }
        else if (movie_id > tmp->movieID) {
            tmp = tmp-> rc;
        }
        else if (movie_id == tmp->movieID) {
            if (IsLeaf(tmp)) {
                /* Key already inside a leaf */
                fprintf(stderr, "Key already inside the tree.\n");
                return;
            }
            else {
                tmp = tmp->rc;
            }
        }
    }

    /* Create the 3-node tree */
    userMovie_t* q = (userMovie_t*)malloc(sizeof(userMovie_t));
    /* Copy prev to q */
    q->movieID = prev->movieID;
    q->category = prev->category;
    q->score = prev->score;
    q->lc = NULL;
    q->rc = NULL;

    /* Prev does not change position.*/
    new_watched_film->parent = prev;
    q->parent = prev;

    /*
     * Here both q and new_watched_film have prev as their parent.
     * But we have not specified yet who is the left and right 
     * child of prev.
    */

    /* Set prev's fields to invalid values. (Not necessary) */
    prev->category = -1;
    prev->score = -1;

    /*prev's key does need to change.*/
    if (movie_id < prev->movieID) {
        prev->lc = new_watched_film;
        prev->rc = q;
    }
    /*prev's key must change to movie_id (largest)*/
    else { /* movie_id >= prev->movieID */
        prev->rc = new_watched_film;
        prev->lc = q;
        prev->movieID = movie_id;
    }
}

void InOrder(userMovie_t* root) {
    if (root == NULL) return ;
    InOrder(root->lc);
    if (root->parent != NULL) {
        printf("Child %d, parent %d\n", root->movieID, root->parent->movieID);
    }
    else printf("Child %d, parent NULL\n", root->movieID);
    InOrder(root->rc);
}

void PreOrder(userMovie_t* root) {
    if (root == NULL) return ;
    
    if (root->parent != NULL) {
        printf("Child %d, parent %d\n", root->movieID, root->parent->movieID);
    }
    else printf("Child %d, parent NULL\n", root->movieID);
    
    PreOrder(root->lc);
    PreOrder(root->rc);
}

/******************************************************************************
******************* DOUBLY-LINKED LEAF-ORIENTED BST SUCCESSOR ****************
******************************************************************************/

/* Returns the leftmost node of a BST with root x. */
userMovie_t* LeftMost(userMovie_t* x) {
    if (x == NULL) return NULL;
    
    if (x->lc == NULL) return x;
    
    while(x->lc != NULL) x = x->lc;
    
    return x;
}

/*
 * Given a leaf v of a doubly-linked leaf-oriented BST with root root,
 * Returns a pointer to the successor leaf in in-order traversal.
*/
userMovie_t* Successor(userMovie_t* v, userMovie_t* root) {
    userMovie_t* tmp = v;
    userMovie_t* prev = NULL;
    
    // Find v
    // while (tmp != NULL) {
    //     prev = tmp;

    //     if (k < tmp->key) {
    //         tmp = tmp->LC;
    //     }
    //     else if (k > tmp->key) {
    //         tmp = tmp-> RC;
    //     }
    //     else if (k == tmp->key) {
    //         if (IsLeaf(tmp)) {
    //             break;
    //         }
    //         else {
    //             tmp = tmp->LC;
    //         }
    //     }
    // }

    if (tmp == NULL) return NULL;

    if (tmp == root) return NULL;   /* v is the root of single node tree */

    if (tmp == tmp->parent->lc) {   /* v is left child */
        /* Leftmost node of uncle. */
        return LeftMost(tmp->parent->rc);
    }
    else {                          /* v is right child */
        userMovie_t* y = tmp->parent;
        
        while (y != NULL && tmp == y->rc) {
            tmp = y;
            y = y->parent;
        }
        if (y == NULL) return NULL; /* No successor found */
        return LeftMost(y->rc);
    }
}

/*
 * Deallocate all nodes of the watch history 
 * leaf-oriented BST using post order traversal.
*/
void DeleteWatchHistoryTree(userMovie_t* root) {
    if (root == NULL) return ;
    DeleteWatchHistoryTree(root->lc);
    DeleteWatchHistoryTree(root->rc);
    free(root);
}

/*
 * Print the leaves of a leaf-oriented BST with root root.
 * in InOrder traversal.*/
void PrintLeavesInOrder(userMovie_t* root) {
    userMovie_t* lleft = LeftMost(root); /* Leftmost leaf */
    while(lleft != NULL) {
        printf("       <%d, %d>\n", lleft->movieID, lleft->score);
        lleft = Successor(lleft, root);
    }
}

/* Return the average rating of the watch history tree given.*/
float AverageRating(userMovie_t* root) {
    float ScoreSum = 0;
    int counter = 0;

    userMovie_t* lleft = LeftMost(root); /* Leftmost leaf */
    while(lleft != NULL) {
        counter++;
        ScoreSum += lleft->score;
        lleft = Successor(lleft, root);
    }

    return (ScoreSum / counter);
}

int main(void) {
    userMovie_t* root = NULL;
    int mids[] = {5, 10, 4, 15, 3, 1};
    int cats[] = {0, 1, 2, 3, 4, 5};
    int scores[] = {10, 50, 70, 24, 86, 47};

    //int keys[] = {5, 8, 2, 4, 3, 1};
    int n = 6;

    for (int i = 0; i < 2; ++i) {
        InsertHistoryLeaf(&root, mids[i], cats[i], scores[i]);
    }

    printf("In Order: ");
    InOrder(root);
    putchar('\n');

    printf("Pre Order: ");
    PreOrder(root);
    putchar('\n');

    userMovie_t* lleft = LeftMost(root);
    userMovie_t* suc = Successor(lleft, root);
    printf("LeftMost %d\n", lleft->movieID);
    //printf("Successor %d\n", suc->movieID);

    // printf("Successor's parent: id = %d, cat = %d, score = %d\n",
    //         suc->parent->movieID, suc->parent->category, suc->parent->score);
    //printf("Successor's: cat = %d, score = %d\n", suc->category, suc->score);

    PrintLeavesInOrder(root);
    float aver = AverageRating(root);
    printf("Average rating = %.3f\n", aver);
    // Node* suc = Successor(4, root);
    // if (suc == NULL) printf("Succesor is null\n");
    // else printf("Succesor is %d\n", suc->key);

    return 0;
}