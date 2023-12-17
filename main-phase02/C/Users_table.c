#include <stdio.h>
#include <stdlib.h>

#include "Movie.h"

user_t **user_hashtable_p;	/* The users hashtable. This is an array of chains (pinakas katakermatismoy xrhstwn)*/
int hashtable_size;			/* The size of the users hashtable, parsed from the command line (>0)*/
int max_users;				/* The maximum number of registrations (users)*/			
int max_id;   			    /* The maximum account ID */

int hash_a, hash_b, hash_p; /* Used in hash function */

int primes_g[170] = {5,   7,  11,  13,  17,  19,  23,  29,  31,  37,
                    41,  43,  47,  53,  59,  61,  67,  71,  73,  79,
                    83,  89,  97, 101, 103, 107, 109, 113, 127, 131,
                    137, 139, 149, 151, 157, 163, 167, 173, 179, 181,
                    191, 193, 197, 199, 211, 223, 227, 229, 233, 239,
                    241, 251, 257, 263, 269, 271, 277, 281, 283, 293,
                    307, 311, 313, 317, 331, 337, 347, 349, 353, 359,
                    367, 373, 379, 383, 389, 397, 401, 409, 419, 421,
                    431, 433, 439, 443, 449, 457, 461, 463, 467, 479,
                    487, 491, 499, 503, 509, 521, 523, 541, 547, 557,
                    563, 569, 571, 577, 587, 593, 599, 601, 607, 613,
                    617, 619, 631, 641, 643, 647, 653, 659, 661, 673,
                    677, 683, 691, 701, 709, 719, 727, 733, 739, 743,
                    751, 757, 761, 769, 773, 787, 797, 809, 811, 821,
                    823, 827, 829, 839, 853, 857, 859, 863, 877, 881,
                    883, 887, 907, 911, 919, 929, 937, 941, 947, 953,
                    967, 971, 977, 983, 991, 997, 1009, 1013, 1019, 1021 };

/**************************** SLL FUNCTIONS ****************************/

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

/******************************************************************************
 **************************** HASH TABLE FUNCTIONS ****************************
 ******************************************************************************/

/*
 * Returns the next greater prime number to K, using the primes_g array. 
*/
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

/**
 * @brief Creates a new user.
 * Creates a new user with userID as its identification.
 *
 * @param userID The new user's identification
 *
 * @return 1 on success
 *         0 on failure
*/
int register_user(int userID) {
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
int unregister_user(int userID) {
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
 * @brief Prints the users hashtable.
 * @return 1 on success
 *         0 on failure
*/
int print_users(void) {
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

int main(void) {
    // int user_ids[5] = {10, 13, 5, 8, 100};
    // user_t* chain = NULL;
    // for (int i = 0; i < 5; ++i) {
    //     ChainInsertUser(&chain, user_ids[i]);
    // }
    // print_chain(chain);
    
    // user_t* user_found = ChainLookUpUser(chain, 13);
    // if(user_found == NULL) printf("NULL\n");
    // else printf("found = %d\n", user_found->userID);

    // ChainDeleteUser(&chain, 100);
    // ChainDeleteUser(&chain, 13);
    // ChainDeleteUser(&chain, 8);
    // ChainDeleteUser(&chain, 5);
    // int code = ChainDeleteUser(&chain, 10);
    // print_chain(chain);
    // printf("code = %d\n", code);

    // max_id = 3;
    // int p = FindPrime(max_id);
    // printf("p = %d\n", p);
    max_id = 100;
    max_users = 10;
    int load_factor = 2;
    int user_ids[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    // /* Initialize user_ids */
    // for (int i = 0; i < max_users; ++i) {
    //     user_ids[i] = 1 + rand() % max_id; /*range [1, max_id]*/
    // }
    UsersHashTableInit(load_factor);
    printf("hashtable_size = %d\n", hashtable_size);

    for(int i = 0; i < max_users; ++i) {
        register_user(user_ids[i]);
    }
    
    print_users();

    unregister_user(6);

    return 0;
}