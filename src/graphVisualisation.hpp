/**
* @file graphVisualisation.hpp
* @author Ondrej
* @brief Class that visualises the graph
*
*/

#pragma once

#include <SFML/Graphics.hpp>
#include <array>
#include <map>
#include <vector>

#include "graph.hpp"


struct RGB 
{
    int r;
    int g;
    int b;
    /* Alpha - opacity */
    int a;
};

/** Color scheme for visualisation */
struct ColorScheme 
{
    RGB wall;
    RGB empty;
    RGB step;
    RGB opened;
    RGB path;
    RGB background;
    RGB startEnd;
};

/** Handeling input */
struct InputData 
{
    bool finished = false;
    bool paused = false;
    int state;
    bool loop = false;
    size_t batchSize = 1;
    int visualStyle = 0;
    std::array<ColorScheme, 2> colorSchemes;
};


class GraphVisualisation
{
public:
    GraphVisualisation(Graph &graph, sf::RenderWindow &window, size_t visualisationSpeed)
        : m_graph(graph),
          m_window(window),
          m_visualisationSpeed(visualisationSpeed),
          m_visitedProgress(0),
          m_pathProgress(0)
    {
        this->init();
    };

    /** Sets the fps and color schemes */
    void init();

    /** Resets the screen */
    void resetAll();

    /** Processes all user input */
    void processInput(sf::Event &event);

    /** Main window loop */
    void windowLoop();

    /** Shows visualisation of the graph */
    bool showGraph(void);

    /** Displays individual step of algorithm */
    bool showVisitedStep(sf::VertexArray &tiles, size_t &vertexIndex);

    /** How many cells win render, render type is either visited cells (0) or path (1) */
    bool showBatch(size_t batchSize, bool renderType);

    /* Show individual step of path */
    bool showPathStep(sf::VertexArray &tiles, size_t &vertexIndex);

    /** Updates title based on algorithm type */
    void updateTitle(void);

    /** Resets visualisation - shows the steps of the path finding algorithm from the beginning */
    void reset(void);

    /** Changes visualisation speed */
    void changeSpeed(int speed);

    /** Calculates the size of individual tile based on screen width and height and number of tiles */
    double tileSize(void);

    /** Calculates the size of outline based on screenSize and tile Size */
    double outlineSize(void);

private:
    Graph m_graph;

    std::string m_screenTitle;

    /* Ended up not using this in the Graph class for now */
    size_t m_visualisationSpeed;

    sf::RenderWindow &m_window;

    size_t m_visitedProgress = 0;

    size_t m_pathProgress = 0;

    InputData m_gameData;
};
