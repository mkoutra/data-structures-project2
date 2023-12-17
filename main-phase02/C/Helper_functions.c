#include <stdio.h>
#include <stdlib.h>

#include "Movie.h"

new_movie_t *new_releases = NULL;
movie_t* guard;
movieCategory_t *categoryArray[6];	/* The categories array (pinakas kathgoriwn)*/

/*
 ******************************************************************************
 ******************************** ADD NEW MOVIE *******************************
 ******************************************************************************
*/

/*
 * Insert a new movie in the new_releases tree.
 * Returns 0 on success, otherwise -1
 */
int NewReleasesInsert(int movieID, int category, int year) {
    new_movie_t* prev = NULL;
    new_movie_t* tmp = new_releases;
    
    /* Search if movie is already inside the tree. */
    while (tmp != NULL) {
        if (movieID == tmp->movieID) {
            fprintf(stderr, "Movie %d is already inside\n", movieID);
            return -1;
        }
        prev = tmp;

        /* Find the right path */
        (movieID < tmp->movieID) ? (tmp = tmp->lc) : (tmp = tmp->rc);
    }

    /* Create and initialize new film */
    new_movie_t* new_film = (new_movie_t*)malloc(sizeof(new_movie_t));
    new_film->movieID = movieID;
    new_film->category =category;
    new_film->year = year;
    new_film->watchedCounter = 0;
    new_film->sumScore = 0;
    new_film->lc = new_film->rc = NULL;

    /* Tree is empty. */
    if (new_releases == NULL) {
        new_releases = new_film;
        return 0;
    }

    /* Check if the new node is the left or the right child of its parent.*/
    (movieID < prev->movieID) ? (prev->lc = new_film) : (prev->rc = new_film);

	return 0;
}

/* Print the nodes of the new_releases tree in inorder traversal */
void printNewReleases(new_movie_t* root) {
    if (root == NULL) return ;
    printNewReleases(root->lc);
    printf("<%d>, ", root->movieID);
    printNewReleases(root->rc);
}

int add_new_movie(int movieID, int category, int year){
    int code = NewReleasesInsert(movieID, category, year);
    if (code == -1) return 0;

    printf("A <%d> <%d> <%d>\n", movieID, category, year);
    printf("New releases Tree:\n");
    printf("    <new_releases>: ");
    printNewReleases(new_releases);
    printf("\nDONE\n");

    return 1;
}

/* Deallocate new movies BST tree */
void DeleteNewReleasesTree(new_movie_t* root) {
    if (root == NULL) return;
    DeleteNewReleasesTree(root->lc);
    DeleteNewReleasesTree(root->rc);
    free(root);
}

/*
 ******************************************************************************
 ******************************** DISTRIBUTION ********************************
 ******************************************************************************
*/

/*
 * Count the number of movies on each category of the new_releases tree.
 * Each element of array cat_counter represents the number of movies
 * for the corresponding category.
*/
void count_nodes(new_movie_t* root, int cat_counter[6]) {
    if (root == NULL) return;
    cat_counter[root->category]++;
    count_nodes(root->lc, cat_counter);
    count_nodes(root->rc, cat_counter);
    return;
}

/*
 * Fill the array given (arr) with the pointers of new movie tree nodes.
 * Each row's size == number of movies for this category.
 * Uses In-order traversal.
 * - arr is the array to fill. Contains arrays of pointers.
 * - idx contains the next position that a node should be
 *   insterted into array arr for each category.
*/
void fill_array(new_movie_t* root, new_movie_t*** arr, int idx[6]) {
    int cat, pos;
    if (root == NULL) return ;
    
    fill_array(root->lc, arr, idx);
    
    cat = root->category;
    pos = idx[cat];

    arr[cat][pos] = root;
    idx[cat]++;

    fill_array(root->rc, arr, idx);
}

/* 
 * Given an array of pointers to new_movie_t nodes subArr[p..q],
 * creates a movie_tree and return its root. Also, deallocates the nodes
 * from the new_releases tree.
 * Here subArr[] is a row of the array filled with fill_array().
*/
movie_t* arr_to_CategoryTree(new_movie_t* subArr[], int p, int q) {
    if (p > q) return guard;

    /* Middle element */
    int mid = (p + q) / 2;

    /* New node */
    movie_t* new_node = (movie_t*)malloc(sizeof(movie_t));
    new_node->movieID = subArr[mid]->movieID;
    new_node->year = subArr[mid]->year;
    new_node->watchedCounter = subArr[mid]->watchedCounter;
    new_node->sumScore = subArr[mid]->sumScore;

    new_node->lc = new_node->rc = guard;

    /* Deallocate node from NewReleases tree */
    free(subArr[mid]);

    /* Find LC and RC */ 
    new_node->lc = arr_to_CategoryTree(subArr, p, mid - 1);
    new_node->rc = arr_to_CategoryTree(subArr, mid + 1, q);

    return new_node;
}

/* Pre the category tree in In-order traversal */
void InOrderCatTree(movie_t* root) {
    if (root == guard) return ;
    InOrderCatTree(root->lc);
    printf("<%d>, ", root->movieID);
    InOrderCatTree(root->rc);
}

void PreOrderCatTree(movie_t* root) {
    if (root == guard) return ;
    printf("<%d>, ", root->movieID);
    PreOrderCatTree(root->lc);
    PreOrderCatTree(root->rc);
}

/* Initialize Category array. Returns 0 on success, -1 on failure */
int InitializeCatArray(void) {
    /* Allocate global guard node */
    guard = (movie_t*)malloc(sizeof(movie_t));
    guard->movieID = -1;
    guard->year = guard->sumScore = guard->watchedCounter = 0;
    guard->lc = guard->rc = NULL;

    /* Allocate memory for categoryArray*/
    for (int i = 0; i < 6; ++i) {
        categoryArray[i] = (movieCategory_t*) malloc(sizeof(movieCategory_t));
        if (categoryArray[i] == NULL) {
            fprintf(stderr, "Malloc error InitializeCatArr\n");
            return -1;
        }
        categoryArray[i]->movie = guard;
        categoryArray[i]->sentinel = guard;
    }
    return 0;
}

/**
 * @brief Distribute the movies from new release binary tree to the array of categories.
 *
 * @return 1 on success
 *         0 on failure
*/
int distribute_movies(void) {
    /* Save the number of movies for each category */
    int mov_cat_counters[6] = {0, 0, 0, 0, 0, 0};
    /* Array needed to fill arr_cat with fill_array() */
    int indices[6] = {0, 0, 0, 0, 0, 0};
    /* Array with 6 rows and each column equal to the number of movies for that category */
    new_movie_t*** arr_cat;

    int n_movies, i = 0;
    
    /* Count the movies of each category */
    count_nodes(new_releases, mov_cat_counters);

    /* Create array of arrays of pointers */
    arr_cat = (new_movie_t***) malloc(6 * sizeof(new_movie_t**));
    if (arr_cat == NULL) {
        fprintf(stderr, "Malloc error distribute arr_cat\n");
        return 0;
    }

    /* Allocate sufficient memory for the movies of category i */
    for (i = 0; i < 6; ++i) {
        n_movies = mov_cat_counters[i];
        arr_cat[i] = (new_movie_t**) malloc(n_movies * sizeof(new_movie_t*));
        if (arr_cat[i] == NULL) {
            fprintf(stderr, "Malloc error distribute arr_cat[i]\n");
            return 0;
        }
    }

    /* Fill arr_cat with the pointers of each new movie.*/
    fill_array(new_releases, arr_cat, indices);

    /*For each row of arr_cat create a Category Tree and place
      it to the corresponding category of categoryArray*/
    for (i = 0; i < 6; ++i) {
        categoryArray[i]->movie = arr_to_CategoryTree(arr_cat[i], 0, mov_cat_counters[i] - 1);
    }
    new_releases = NULL;

    /* Printing */
    printf("D\nMovie Category Array:\n");
    for (i = 0; i < 6; ++i) {
        printf("    <Category %d>: ", i);
        InOrderCatTree(categoryArray[i]->movie);
        putchar('\n');
    }
    printf("DONE\n");
    return 1;
}

/*
 ******************************************************************************
 ******************************* SEARCH MOVIE *********************************
 ******************************************************************************
*/

/**
 * @brief Search for a movie with identification movieID in a specific category.
 *
 * @param movieID The identifier of the movie
 * @param category The category of the movie
 *
 * @return 1 on success
 *         0 on failure
*/
int search_movie(int movieID, int category) {
    if (category >= 6 || category < 0) {
        fprintf(stderr, "Invalid category given to search_movie()\n");
        return 0;
    }
    movie_t* tmp = categoryArray[category]->movie;
    movie_t* sent_node = categoryArray[category]->sentinel;

    /* Place the movieID given to the guard node*/
    sent_node->movieID = movieID;
    
    while(tmp->movieID != movieID) {
        (movieID < tmp->movieID) ? (tmp = tmp->lc) : (tmp = tmp->rc);
    }

    /* Restore sentinel movieID */
    sent_node->movieID = -1;

    if (tmp != sent_node) {
        printf("I <%d> <%d> <%d>\nDONE\n", tmp->movieID, category, tmp->year);
    }
    else {
        printf("Movie <%d> was not found in category <%d>.\n", movieID, category);
        return 0;
    }

    return 1;
}

/*
 ******************************************************************************
 ******************************* PRINT MOVIES *********************************
 ******************************************************************************
*/

/**
 * @brief Prints the movies in movies categories array.
 * @return 1 on success
 *         0 on failure
*/
int print_movies(void) {
    int i = 0;
    /* Printing */
    printf("M\nMovie Category Array:\n");
    for (i = 0; i < 6; ++i) {
        printf("    <Category %d>: ", i);
        InOrderCatTree(categoryArray[i]->movie);
        putchar('\n');
    }
    printf("DONE\n");
    return 1;
}

int NodesAboveScore(movie_t* root, int score) {
    int s = 0;
    float av = 0.0;
    if (root == NULL) return 0;
    
    s += NodesAboveScore(root->lc, score);

    av = (float) root->year / root->movieID;
    if (av >= score) s++;
    
    s += NodesAboveScore(root->rc, score);
    
    return s;
}

void FillFilteringArray(movie_t* root, movie_t* filtering_arr[], int* idx, int score) {
    float av = 0.0;
    if (root == NULL) return ;
    
    FillFilteringArray(root->lc, filtering_arr, idx, score);

    av = (float) root->year / root->movieID;
    if (av >= score) {
        filtering_arr[*idx] = root;
        (*idx)++;
    }

    FillFilteringArray(root->rc, filtering_arr, idx, score);
}

/*
 ******************************************************************************
 ********************************** TESTING ***********************************
 ******************************************************************************
*/
int main(void) {
    int movie_ids[5] = {9, 15, 5, 17, 4};
    int movie_years[5] = {1900, 1920, 1950, 2000, 2023};
    int movie_cats[5] = {1, 1, 1, 1, 1};

    for (int i = 0; i < 5; ++i) {
        add_new_movie(movie_ids[i], movie_cats[i], movie_years[i]);
    }

    InitializeCatArray();
    distribute_movies();

    search_movie(17, 3);

    print_movies();

    int n = NodesAboveScore(categoryArray[1]->movie, 200);
    // int n = NodesAboveScore(NULL, 200);
    printf("n = %d\n", n);
    if (n > 0) {
        //movie_t* filt_arr[n]; /* Assuming n > 0*/
        movie_t** filt_arr = (movie_t**)malloc(2* n * sizeof(movie_t*));
        int idx = 0;
        FillFilteringArray(categoryArray[1]->movie, filt_arr, &idx, 200);
        FillFilteringArray(categoryArray[1]->movie, filt_arr, &idx, 200);
        printf("idx = %d\n", idx);
        for (int i = 0; i < 2*n; ++i)
            printf("%d, ", filt_arr[i]->movieID);
        putchar('\n');
    }

    return 0;
}

/* Old tests */

// int main(void) {
//     int movie_ids[5] = {5, 3, 9, 17, 4};
//     int movie_years[5] = {1900, 1920, 1950, 2000, 2023};
//     int movie_cats[5] = {2, 2, 2, 2, 2};

//     for (int i = 0; i < 5; ++i) {
//         add_new_movie(movie_ids[i], movie_cats[i], movie_years[i]);
//     }

//     int s[6] = {0, 0, 0, 0, 0, 0};
//     count_nodes(new_releases, s);
//     for(int i = 0; i < 6; ++i) printf("s[%d] = %d\n", i, s[i]);

//     /* Create array of arrays of pointers */
//     new_movie_t*** arr_cat = (new_movie_t***) malloc(6 * sizeof(new_movie_t**));

//     for (int i = 0; i < 6; ++i) {
//         int n_cat = s[i]; /* Number of movies for each category */

//         /* Sub array with pointers */
//         new_movie_t** sub_arr = (new_movie_t**) malloc(n_cat * sizeof(new_movie_t*));
//         arr_cat[i] = sub_arr;
//     }

//     int indices[6] = {0, 0, 0, 0, 0, 0};
//     fill_array(new_releases, arr_cat, indices);

//     for(int i = 0; i < 6; i++) {
//         printf("Cat: %d: ", i);
//         for (int j = 0; j < s[i]; ++j) {
//             printf("%d,", arr_cat[i][j]->movieID);
//         }
//         putchar('\n');
//     }

//     guard = (movie_t*)malloc(sizeof(movie_t));
//     guard->movieID = -1;
//     guard->year = guard->sumScore = guard->watchedCounter = 0;
//     guard->lc = guard->rc = NULL;

//     for (int i = 0; i < 6; ++i) {
//         categoryArray[i] = (movieCategory_t*) malloc(sizeof(movieCategory_t));
//     }

//     for (int i = 0; i < 6; ++i) {
//         categoryArray[i]->sentinel = guard;
//         categoryArray[i]->movie = guard;
//     }

//     for (int i = 0; i < 6; ++i) {
//         categoryArray[i]->movie = arr_to_BST(arr_cat[i], 0, s[i]-1);
//     }

//     for(int i = 0; i < 6; ++i) {
//         print_cat(categoryArray[i]->movie);
//         putchar('\n');
//         print_cat_PREORDER(categoryArray[i]->movie);
//         putchar('\n');
//     }

//     return 0;
// }