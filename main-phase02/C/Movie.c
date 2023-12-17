/*****************************************************
 * @file   Movie.c                                   *
 * @author Paterakis Giorgos <geopat@csd.uoc.gr>     *
 *                                                   *
 * @brief Implementation for Movie.h 				 *
 * Project: Winter 2023						         *
 *****************************************************/
#include "Movie.h"

/******************************* ADD NEW MOVIE *****************************/

/*
 * Insert a new movie in the new_releases tree.
 * Returns 0 on success, otherwise -1
 */
int NewReleasesInsert(int movieID, int category, int year) {
    new_movie_t* prev = NULL;
    new_movie_t* tmp = new_releases;
    
    // Search if movie is already inside the tree.
    while (tmp != NULL) {
        if (movieID == tmp->movieID) {
            fprintf(stderr, "Movie %d is already inside\n", movieID);
            return -1;
        }
        prev = tmp;

        // Find the right path
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

    // Tree is empty.
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

/********************************** DISTRIBUTION *****************************/


/******************************* EVENT FUNCTIONS *****************************/

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
	 return 1;
 }
 
 /**
 * @brief Prints the movies in movies categories array.
 * @return 1 on success
 *         0 on failure
 */

 int print_movies(void){
	 return 1;
 }
 
  /**
 * @brief Prints the users hashtable.
 * @return 1 on success
 *         0 on failure
 */

 int print_users(void){
	 return 1;
 }
 
