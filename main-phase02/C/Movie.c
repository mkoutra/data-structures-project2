/*****************************************************
 * @file   Movie.c                                   *
 * @author Paterakis Giorgos <geopat@csd.uoc.gr>     *
 *                                                   *
 * @brief Implementation for Movie.h 				 *
 * Project: Winter 2023						         *
 *****************************************************/
#include "Movie.h"

/*
 ******************************************************************************
 ******************************** ADD NEW MOVIE ********************************
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

/*
 ******************************************************************************
 ******************************** SLL FUNCTIONS *******************************
 ******************************************************************************
*/

/*
 * Insert a new user with id user_id to the chain described by head.
 * Returns 0 on success, otherwise -1. O(1)
*/
int ChainInsertUser(user_t** head, int user_id) {
    /* Allocate and initialize the new node */
    user_t* new_user = (user_t*)malloc(sizeof(user_t));
    if (new_user == NULL) {
        fprintf(stderr, "Malloc problem, ChainInsert.\n");
        return -1;
    }
    new_user->userID = user_id;
    new_user->history = NULL;
    new_user->next = (*head);
    
    (*head) = new_user;
    return 0;
}

/*
 * Scan the chain described by head and return
 * the address of the node with the userID given.
 * Otherwise, returns NULL. O(N)
*/
user_t* ChainLookUpUser(user_t* head, int user_id) {
    user_t* tmp = head;
    /* Scan for user_id*/
    while ((tmp != NULL) && (tmp->userID != user_id)) {
        tmp = tmp->next;
    }

    return tmp;
}

/*
 * Deletes the user with the user_id given from the chain described by head.
 * Returns 0 on success, otherwise -1. O(N)
*/
int ChainDeleteUser(user_t** head, int user_id) {
    user_t* tmp = (*head);
    user_t* prev = NULL;

    /* Search for user_id*/
    while(tmp != NULL && (tmp->userID != user_id)) {
        prev = tmp;
        tmp = tmp->next;
    }

    /* User was not found*/
    if (tmp == NULL) return -1;

    /* tmp is the head*/
    if (tmp == (*head)) {
        (*head) = (*head)->next;
    }
    /* tmp is a regular node */
    else {
        prev->next = tmp->next;
    }

    free(tmp);
    return 0;
}

/* Print the users of a chain */
void print_chain(user_t* head) {
    user_t* tmp = head;
    while(tmp != NULL) {
        printf("    <%d>\n", tmp->userID);
        tmp = tmp->next;
    }
}

/*
 ******************************************************************************
 ***************************** HASH TABLE FUNCTIONS ***************************
 ******************************************************************************
*/

/* Returns the next greater prime number to K, using the primes_g array.*/
int FindPrime(int K) {
    int i = 0;
    while (i < 170 && (primes_g[i] <= K)) i++;

    /*if i == 170 we must take the largest element. */
    return (i < 170) ? primes_g[i] : primes_g[169];
}

/*
 * - Allocates the user_hashtable_p and updates hashtable_size.
 * - Initializes the global variables hash_a, hash_b and hash_p
 *   needed for HashFunc().
 * - Returns 0 on success, -1 otherwise.
 * 
 * load_f: Load factor needed to specify hashtable_size.
*/
int UsersHashTableInit(int load_f) {
    int i = 0;
    /* We want the load factor = max_users/hashtable_size to be constant. */
    hashtable_size = (max_users / load_f);

    /* Allocate and Initialize user's hashtable */
    user_hashtable_p = (user_t**) malloc(hashtable_size * sizeof(user_t*));
    if (user_hashtable_p == NULL) {
        fprintf(stderr, "Malloc error in hash table\n");
        return -1;
    }
    for (i = 0; i < hashtable_size; ++i) {
        user_hashtable_p[i] = NULL;
    }

    /* Initialize global variables needed for hash function */
    srand(100);
    hash_p = FindPrime(max_id);
    hash_a = 1 + rand() % hash_p; /* Random number in [1, p-1]*/
    hash_b = rand() % hash_p; /* Random number in [0, p]*/ 

    printf("p = %d, a = %d, b = %d\n", hash_p, hash_a, hash_b);

    return 0;
}

/*
 * Hash function using Universal hashing:
 * h(x) = ((ax + b) % p) % size)
 * p: prime number larger that max_id
 * a: element of [1, p-1]
 * b: element of [0, p-1]
 * size: Size of hash table
*/
int HashFunc(int x, int a, int b, int p, int size) {
    return (((a*x + b) % p) % size);
}

int UserHashTableInsert(int user_id) {
    int pos = HashFunc(user_id, hash_a, hash_b, hash_p, hashtable_size);
    ChainInsertUser(&user_hashtable_p[pos], user_id);
}

/*
 ******************************************************************************
 ******************************* EVENT FUNCTIONS ******************************
 ******************************************************************************
*/

/**
 * @brief Creates a new user.
 * Creates a new user with userID as its identification.
 *
 * @param userID The new user's identification
 *
 * @return 1 on success
 *         0 on failure
 */

 int register_user(int userID){
    int pos = HashFunc(userID, hash_a, hash_b, hash_p, hashtable_size);
    
    int code = ChainInsertUser(&user_hashtable_p[pos], userID);
    
    if (code == -1) {
        fprintf(stderr, "Problem inserting user\n");
        return 0;
    }

    printf("Chain <%d> of Users:\n", pos);
    print_chain(user_hashtable_p[pos]);
    printf("DONE\n");
    return 1;
 }
 
/**
 * @brief Deletes a user.
 * Deletes a user with userID from the system, along with users' history tree.
 *
 * @param userID The new user's identification
 *
 * @return 1 on success
 *         0 on failure
 */

 int unregister_user(int userID){
    int pos = HashFunc(userID, hash_a, hash_b, hash_p, hashtable_size);

    /* TODO: Delete the watch history tree */

    int code = ChainDeleteUser(&user_hashtable_p[pos], userID);
    if (code == -1) {
        fprintf(stderr, "User <%d> does not exist.\n", userID);
        return 0;
    }

    printf("U <%d>\n", userID);
    printf("Chain <%d> of Users:\n", pos);
    print_chain(user_hashtable_p[pos]);
    printf("DONE\n");
    return 1;
 }
 
 /**
 * @brief Add new movie to new release binary tree.
 * Create a node movie and insert it in 'new release' binary tree.
 *
 * @param movieID The new movie identifier
 * @param category The category of the movie
 * @param year The year movie released
 *
 * @return 1 on success
 *         0 on failure
 */

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
 
 /**
 * @brief Distribute the movies from new release binary tree to the array of categories.
 *
 * @return 0 on success
 *         1 on failure
 */

 int distribute_movies(void){
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
 
 /**
 * @brief User rates the movie with identification movieID with score
 *
 * @param userID The identifier of the user
 * @param category The Category of the movie
 * @param movieID The identifier of the movie
 * @param score The score that user rates the movie with id movieID
 *
 * @return 1 on success
 *         0 on failure
 */

 int watch_movie(int userID,int category, int movieID, int score){
	 return 1;
 }
 
/**
 * @brief Identify the best rating score movie and cluster all the movies of a category.
 *
 * @param userID The identifier of the user
 * @param score The minimum score of a movie
 *
 * @return 1 on success
 *         0 on failure
 */

 int filter_movies(int userID, int score){
	 return 1;
 }
 
/**
 * @brief Find movies from categories withn median_score >= score t
 *
 * @param userID The identifier of the user
 * @param category Array with the categories to search.
 * @param score The minimum score the movies we want to have
 *
 * @return 1 on success
 *         0 on failure
 */

 int user_stats(int userID){
	 return 1;
 }
 
/**
 * @brief Search for a movie with identification movieID in a specific category.
 *
 * @param movieID The identifier of the movie
 * @param category The category of the movie
 *
 * @return 1 on success
 *         0 on failure
 */

 int search_movie(int movieID, int category){
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
 
 /**
 * @brief Prints the movies in movies categories array.
 * @return 1 on success
 *         0 on failure
 */

 int print_movies(void){
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
 
  /**
 * @brief Prints the users hashtable.
 * @return 1 on success
 *         0 on failure
 */

 int print_users(void){
    int i = 0;
    printf("P\n");
    for (i = 0; i < hashtable_size; ++i) {
        printf("Chain <%d> of Users:\n", i);
        
        user_t* tmp = user_hashtable_p[i];
        
        while (tmp != NULL) {
            printf("      <%d>\n", tmp->userID);
            printf("      History Tree:\n");
            /*print history tree*/
            if (tmp->history == NULL) printf("          NULL\n");
            tmp = tmp->next;
        }
    }
    printf("DONE\n");
 }
 
