#include "Movie.h"

int main(void) {
    int movie_ids[5] = {9, 15, 5, 17, 4};
    int movie_years[5] = {1900, 1920, 1950, 2000, 2023};
    int movie_cats[5] = {1, 3, 1, 3, 4};

    for (int i = 0; i < 5; ++i) {
        add_new_movie(movie_ids[i], movie_cats[i], movie_years[i]);
    }

    InitializeCatArray();
    distribute_movies();

    search_movie(17, 3);

    print_movies();

    return 0;
}