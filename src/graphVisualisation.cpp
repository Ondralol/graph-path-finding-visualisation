/**
* @file graphVisualisation.hpp
* @author Ondrej
* @brief Class that visualises the graph
*
*/

#include "graph.hpp"
#include "graphVisualisation.hpp"
#include <iostream>

/* Decided not to use this for now */
#define OUTLINE_MULTIPLIER 7
#define speedMultiplier 100

/** Sets the fps and color schemes */
void GraphVisualisation::init(void)
{
    /* Setting the frame */
    m_window.setFramerateLimit(static_cast<int>(0.15 * m_visualisationSpeed * speedMultiplier));


    /* No way in hell this is optional, but considering I am only storing */
    /* 2 Color schemes for now, I will do this 														*/
    m_gameData.colorSchemes[1] = (ColorScheme{RGB{18, 171, 226, 255}, RGB{225, 255, 255, 255}, RGB{0, 230, 255, 255}, RGB{0, 153, 76, 25},
                                              RGB{255, 255, 0, 255}, RGB{255, 255, 255, 255}, RGB{255, 0, 0, 255}});
    m_gameData.colorSchemes[0] = (ColorScheme{RGB{0, 0, 0, 255}, RGB{126, 126, 126, 255}, RGB{219, 41, 22, 255}, RGB{27, 101, 19, 255},
                                              RGB{255, 204, 0, 255}, RGB{32, 32, 32, 255}, RGB{0, 0, 255, 255}});
}

/** Resets the screen*/
void GraphVisualisation::resetAll(void)
{
    m_graph.reset();
    m_graph.setUp(m_gameData.state);
    m_gameData.finished = false;
    this->reset();
}

/** Processes all user input */
void GraphVisualisation::processInput(sf::Event &event)
{
    /* Close window if window is closed */
    if (event.type == sf::Event::Closed)
        m_window.close();

    /* Display whole window again if screen resized */
    else if (event.type == sf::Event::Resized) {
        this->reset();
        m_gameData.finished = false;
    }

    /* Close window if ESC is pressed */
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
        m_window.close();

    /* Play/Pause if Space is pressed */
    else if (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::Space) {
        m_gameData.paused = !m_gameData.paused;
        if (m_gameData.paused)
            m_window.setTitle(m_screenTitle + " -  (PAUSED)");
        else
            m_window.setTitle(m_screenTitle);
    }

    /* Change speed (Faster) */
    else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::D) {
        m_visualisationSpeed = std::min(m_visualisationSpeed * 2, static_cast<size_t>(10000));
        this->changeSpeed(m_visualisationSpeed);
    }

    /* Change speed (Slower) */
    else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::A) {
        m_visualisationSpeed = std::max(m_visualisationSpeed / 2, static_cast<size_t>(1));
        this->changeSpeed(m_visualisationSpeed);
    }

    /* Change batchSize (Higher) */
    else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::E) {
        m_gameData.batchSize = std::min(m_gameData.batchSize * 2, static_cast<size_t>(10000));
    }

    /* Change batchSize (Lower) */
    else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Q) {

        if (m_gameData.batchSize > 1)
            m_gameData.batchSize = std::max(m_gameData.batchSize / 2, static_cast<size_t>(1));
        else
            m_gameData.batchSize = 1;
    }

    /* Change ColorScheme */
    else if (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::C) {
        m_gameData.visualStyle = (m_gameData.visualStyle + 1) % 2;
        this->resetAll();
    }

    /* Loop */
    else if (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::L) {
        m_gameData.loop = !m_gameData.loop;
        if (m_gameData.loop) {
            m_window.setTitle(m_screenTitle + " -  (LOOP)");
        } else
            m_window.setTitle(m_screenTitle);
    }

    /* Show just the shortes path*/
    else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::F) {
        this->resetAll();
        m_visitedProgress = m_graph.m_visitedInOrder.size() + 1;
    }

    /* Change Algorithm */
    else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::S) {
        m_gameData.state = (m_gameData.state + 1) % 5;
        this->resetAll();
        m_gameData.paused = false;
    }

    /* If R pressed, reset animation */
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::R)) {
        this->resetAll();
    }
}

/** Main window loop */
void GraphVisualisation::windowLoop(void)
{
    /** If graph cannot be displayed*/
    m_graph.setUp(-1);
    if (!this->showGraph()) {
        m_graph.pathInfo();
        return;
    }

    m_gameData.state = static_cast<int>(m_graph.m_algoType);

    /* Run until window is closed */
    while (m_window.isOpen()) {
        /* Check for keyboard events and window closed events */
        sf::Event event;
        while (m_window.pollEvent(event)) {
            this->processInput(event);
        }

        /* Display the visualisation*/
        if (!m_gameData.paused && !m_gameData.finished && !this->showBatch(m_gameData.batchSize, 0)) {
            if (!this->showBatch(m_gameData.batchSize, 1)) {
                m_graph.pathInfo();
                m_gameData.finished = true;
            }
        }

        /* Restart if loop*/
        if (m_gameData.finished && m_gameData.loop)
            this->resetAll();
    }
}

/** Updates title based on algorithm type */
void GraphVisualisation::updateTitle(void)
{
    switch (m_graph.m_algoType) {
        case SearchAlgorithmType::BFS:
            m_screenTitle = "Graph Visualisation - BFS";
            break;
        case SearchAlgorithmType::DFS:
            m_screenTitle = "Graph Visualisation - DFS";
            break;
        case SearchAlgorithmType::RandomSearch:
            m_screenTitle = "Graph Visualisation - RandomSearch";
            break;
        case SearchAlgorithmType::GreedySearch:
            m_screenTitle = "Graph Visualisation - GreedySearch";
            break;
        case SearchAlgorithmType::AStar:
            m_screenTitle = "Graph Visualisation - A*";
            break;
    }

    if (m_gameData.loop)
        m_window.setTitle(m_screenTitle + "- (LOOP)");
    else
        m_window.setTitle(m_screenTitle);
}

/** Resets visualisation - shows the steps of the path finding algorithm from the beginning */
void GraphVisualisation::reset(void)
{
    m_visitedProgress = 0;
    m_pathProgress = 0;
    this->updateTitle();
    this->showGraph();
}

/** Changes visualisation speed */
void GraphVisualisation::changeSpeed(int speed)
{
    m_window.setFramerateLimit(speed);
}

/** Draws visualisation in batch, returns true if there is anything left to display, returns false otherwise
   (All steps of paht displayed 																																					*/
bool GraphVisualisation::showBatch(size_t batchSize, bool renderType)
{
    bool returnValue = false;


    sf::VertexArray tiles(sf::Quads, batchSize * 4 * 4);
    size_t vertexIndex = 0;

    for (int i = 0; i < batchSize; i++) {
        /* Displays visited steps */
        if (renderType == false)
            returnValue = this->showVisitedStep(tiles, vertexIndex);
        /* Displays path */
        else
            returnValue = this->showPathStep(tiles, vertexIndex);
    }

    /* If there is anything to display */
    if (vertexIndex != 0) {
        m_window.draw(tiles);
        m_window.display();
    }
    return returnValue;
}

/** Displays individual step of algorithm */
bool GraphVisualisation::showVisitedStep(sf::VertexArray &tiles, size_t &vertexIndex)
{
    /* If every steps has been already visited */
    if (m_visitedProgress >= m_graph.m_visitedInOrder.size() - 1 || m_graph.m_visitedInOrder.size() == 0)
        return false;

    double tileSize = this->tileSize();
    double outlineSize = this->outlineSize();

    Position x = m_graph.m_visitedInOrder[m_visitedProgress++];

    float X;
    float Y;
    float size;
    sf::Color color(255, 255, 255, 255);

    /* If not start position */
    if (x != m_graph.m_startPos) {

        int r = m_gameData.colorSchemes[m_gameData.visualStyle].step.r;
        int g = m_gameData.colorSchemes[m_gameData.visualStyle].step.g;
        int b = m_gameData.colorSchemes[m_gameData.visualStyle].step.b;

        X = 15 + x.first * (outlineSize + tileSize);
        Y = 15 + x.second * (outlineSize + tileSize);
        size = tileSize + outlineSize;
        color = sf::Color(r, g, b, 255);

        tiles[vertexIndex++] = (sf::Vertex(sf::Vector2f(X, Y), color));
        tiles[vertexIndex++] = (sf::Vertex(sf::Vector2f(X + size, Y), color));
        tiles[vertexIndex++] = (sf::Vertex(sf::Vector2f(X + size, Y + size), color));
        tiles[vertexIndex++] = (sf::Vertex(sf::Vector2f(X, Y + size), color));
    }

    /* If position opened any vertices */
    if (m_graph.m_opened.find(x) != m_graph.m_opened.end()) {
        /* For every vertex that x opened */
        for (const auto &y: m_graph.m_opened[x]) {
            int r = m_gameData.colorSchemes[m_gameData.visualStyle].opened.r;
            int g = m_gameData.colorSchemes[m_gameData.visualStyle].opened.g;
            int b = m_gameData.colorSchemes[m_gameData.visualStyle].opened.b;

            X = 15 + y.first * (outlineSize + tileSize);
            Y = 15 + y.second * (outlineSize + tileSize);
            size = tileSize + outlineSize;
            color = sf::Color(r, g, b, 255);

            tiles[vertexIndex++] = (sf::Vertex(sf::Vector2f(X, Y), color));
            tiles[vertexIndex++] = (sf::Vertex(sf::Vector2f(X + size, Y), color));
            tiles[vertexIndex++] = (sf::Vertex(sf::Vector2f(X + size, Y + size), color));
            tiles[vertexIndex++] = (sf::Vertex(sf::Vector2f(X, Y + size), color));
        }
    }

    return true;
}

/* Shows individual step of path */
bool GraphVisualisation::showPathStep(sf::VertexArray &tiles, size_t &vertexIndex)
{
    double tileSize = this->tileSize();
    double outlineSize = this->outlineSize();

    if (m_pathProgress >= m_graph.m_path.size() - 1 || m_graph.m_path.size() == 0)
        return false;

    float X;
    float Y;
    float size;
    sf::Color color(255, 255, 255, 255);

    Position x = m_graph.m_path[m_pathProgress++];

    /* If not start position */
    if (x != m_graph.m_startPos) {
        int r = m_gameData.colorSchemes[m_gameData.visualStyle].path.r;
        int g = m_gameData.colorSchemes[m_gameData.visualStyle].path.g;
        int b = m_gameData.colorSchemes[m_gameData.visualStyle].path.b;

        X = 15 + x.first * (outlineSize + tileSize);
        Y = 15 + x.second * (outlineSize + tileSize);
        size = tileSize + outlineSize;
        color = sf::Color(r, g, b, 255);

        tiles[vertexIndex++] = (sf::Vertex(sf::Vector2f(X, Y), color));
        tiles[vertexIndex++] = (sf::Vertex(sf::Vector2f(X + size, Y), color));
        tiles[vertexIndex++] = (sf::Vertex(sf::Vector2f(X + size, Y + size), color));
        tiles[vertexIndex++] = (sf::Vertex(sf::Vector2f(X, Y + size), color));
    }

    return true;
}

/** Calculates tile size based on screensize and number of tiles*/
double GraphVisualisation::tileSize(void)
{
    if (m_graph.m_graph.empty())
        return 0.0;

    size_t rows = m_graph.m_graph.size();
    size_t cols = m_graph.m_graph[0].size();

    unsigned screenWidth = sf::VideoMode::getDesktopMode().width;
    unsigned screenHeight = sf::VideoMode::getDesktopMode().height;

    //size_t rectCount = cols;i
    double tileSize;
    if (screenWidth / cols > screenHeight / rows)
        tileSize = (1.0 * screenHeight - cols * this->outlineSize() - 150) / rows;
    else
        tileSize = (1.0 * screenWidth - cols * this->outlineSize() - 150) / cols;

    if (tileSize > 1000)
        tileSize = 20;

    return tileSize;
}


/** Calculates the size of outline based on screenSize and tile Size */
double GraphVisualisation::outlineSize(void)
{
    return 0.0;

    if (m_graph.m_graph.empty())
        return 0.0;

    unsigned screenWidth = sf::VideoMode::getDesktopMode().width;
    unsigned screenHeight = sf::VideoMode::getDesktopMode().height;

    size_t rows = m_graph.m_graph.size();
    size_t cols = m_graph.m_graph[0].size();

    return std::min((std::min(screenWidth, screenHeight) * 1.0 / std::max(rows, cols)) * 1.0 / OUTLINE_MULTIPLIER, 10.0);
}

/* Displays the whole graph */
bool GraphVisualisation::showGraph(void)
{

    if (m_graph.m_graph.empty())
        return false;

    size_t rows = m_graph.m_graph.size();
    size_t cols = m_graph.m_graph[0].size();

    /* If graph doesn't fit the screen, the problem is, that calculation the exact value of graph that would not fit */
    /* is not that simple, so I put these constants here. Program should not crash if graph cannot be displayed			 */
    if (rows > 1000 || cols > 1000)
        return false;

    double tileSize = this->tileSize();
    double outlineSize = this->outlineSize();

    sf::RectangleShape tile;
    tile.setSize((sf::Vector2f(tileSize, tileSize)));
    tile.setOutlineColor(sf::Color(72, 72, 72, 255));
    tile.setOutlineThickness(outlineSize);

    int r = m_gameData.colorSchemes[m_gameData.visualStyle].background.r;
    int g = m_gameData.colorSchemes[m_gameData.visualStyle].background.g;
    int b = m_gameData.colorSchemes[m_gameData.visualStyle].background.b;

    m_window.clear(sf::Color(r, g, b, 255));

    sf::Vector2f tilePosition(15, 15);

    /* Creates array of vertices */
    sf::VertexArray tiles(sf::Quads, rows * cols * 4);
    float X = 15.0f;
    float Y = 15.0f;
    size_t vertexIndex = 0;

    /* Iterates through every vertex/position of graph and displays it accordingly */
    for (auto x: m_graph.m_graph) {
        for (auto y: x) {
            sf::Color color;
            if (y == true) {
                r = m_gameData.colorSchemes[m_gameData.visualStyle].empty.r;
                g = m_gameData.colorSchemes[m_gameData.visualStyle].empty.g;
                b = m_gameData.colorSchemes[m_gameData.visualStyle].empty.b;
                color = (sf::Color(r, g, b, 255));
            } else if (y == false) {
                r = m_gameData.colorSchemes[m_gameData.visualStyle].wall.r;
                g = m_gameData.colorSchemes[m_gameData.visualStyle].wall.g;
                b = m_gameData.colorSchemes[m_gameData.visualStyle].wall.b;
                color = (sf::Color(r, g, b, 255));
            } else {
                color = (sf::Color(102, 255, 102, 255));
            }

            /* Creates square */

            tiles[vertexIndex].color = color;
            tiles[vertexIndex].position = sf::Vector2f(X, Y);
            vertexIndex++;

            tiles[vertexIndex].color = color;
            tiles[vertexIndex].position = sf::Vector2f(X + tileSize, Y);
            vertexIndex++;

            tiles[vertexIndex].position = sf::Vector2f(X + tileSize, Y + tileSize);
            tiles[vertexIndex].color = color;
            vertexIndex++;

            tiles[vertexIndex].color = color;
            tiles[vertexIndex].position = sf::Vector2f(X, Y + tileSize);
            vertexIndex++;

            X += tileSize + outlineSize;
        }
        Y += tileSize + outlineSize;
        X = 15;
    }

    m_window.draw(tiles);

    /* Visualising starting position and ending position */
    r = m_gameData.colorSchemes[m_gameData.visualStyle].startEnd.r;
    g = m_gameData.colorSchemes[m_gameData.visualStyle].startEnd.g;
    b = m_gameData.colorSchemes[m_gameData.visualStyle].startEnd.b;

    tile.setFillColor(sf::Color(r, g, b, 255));

    /* Start Pos*/
    tilePosition = sf::Vector2f(15 + m_graph.m_startPos.first * (outlineSize + tileSize), 15 + m_graph.m_startPos.second * (outlineSize + tileSize));
    tile.setPosition(tilePosition);
    m_window.draw(tile);

    /* End Pos*/
    tilePosition = sf::Vector2f(15 + m_graph.m_endPos.first * (outlineSize + tileSize), 15 + m_graph.m_endPos.second * (outlineSize + tileSize));
    tile.setPosition(tilePosition);
    m_window.draw(tile);

    m_window.display();

    return true;
}
