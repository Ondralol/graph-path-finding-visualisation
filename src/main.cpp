/**
* @file main.pp
* @author Ondrej
* @brief Main function that parses input arguments and calls other functions/metods
*/

#include "conversion.hpp"
#include "graph.hpp"
#include "graphVisualisation.hpp"

#include <iostream>
#include <map>

/**
* @brief Manages whole program
* - Argument 1: Algorithm type (bfs/dfs/astar/random/greedy)
* - Argument 2: File path (relative)
* - Argument 3: (Optional) Visualisation speed (1-100), default value is 50
*
*/
int main(int argc, char **argv)
{
    SearchAlgorithmType algorithmType;
    std::string filePath;

    /** Default visualisation speed set to 50 */
    size_t visualisationSpeed = 50;

    /* If not enough arguments */
    if (argc < 3 || argc > 4)
        return EXIT_FAILURE;

    /* Correct amount of arguments, but no Visualisation Speed argument (optional) */
    if (argc >= 3) {
        /* Converts arguments to correct format, exits if conversion fails, likely wrong input */
        if (!strToAlgoType(argv[1], algorithmType))
            return EXIT_FAILURE;
        filePath = argv[2];
    }

    /* Correct amount of arguments, even the Visualisation Speed argument */
    if (argc == 4) {
        /* Converts the input, exits if conversion fails */
        if (!strToNum(argv[3], visualisationSpeed))
            return EXIT_FAILURE;
    }

    /* Creates an instance of Graph */
    Graph maze(algorithmType, filePath);

    unsigned screenWidth = sf::VideoMode::getDesktopMode().width;
    unsigned screenHeight = sf::VideoMode::getDesktopMode().height;

    sf::RenderWindow window(sf::VideoMode({screenWidth, screenHeight}), "Graph Visualisation");

    /* Creates an instance of Graph Visualisation */
    GraphVisualisation visualisation(maze, window, visualisationSpeed);

    visualisation.windowLoop();
    window.close();

    return EXIT_SUCCESS;
}
