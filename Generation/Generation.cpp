#include <SFML/Graphics.hpp>
#include "FastNoiseLite.h"
#include <iostream>
#include <random>

using namespace sf;

const int WIDTH = 700;
const int HEIGHT = 700;
const int TILE_SIZE = 64;
const int TILES_PER_SUPER_TILE = 5;
const int SUPER_TILE_SIZE = TILES_PER_SUPER_TILE * TILE_SIZE;
const int NUM_SUPER_TILES = 3; // 3x3 grid as shown in the image
const int INITIAL_MAP_SIZE = NUM_SUPER_TILES * TILES_PER_SUPER_TILE * TILE_SIZE;
const int BORDER_DISTANCE = 100;

void generateTilemap(std::vector<std::vector<int>>& tilemap, int startX, int startY, int size)
{
    FastNoiseLite noise;
    noise.SetNoiseType(FastNoiseLite::NoiseType_Perlin);
    noise.SetSeed(42);
    noise.SetFrequency(0.05f);

    for (int y = startY; y < startY + size; ++y) {
        for (int x = startX; x < startX + size; ++x) {
            double noiseValue = noise.GetNoise((float)x, (float)y);

            if (noiseValue < -0.35f) {
                tilemap[y][x] = 1;  // Water
            }
            else if (noiseValue < -0.25) {
                tilemap[y][x] = 2;  // Sand
            }
            else if (noiseValue < -0.15) {
                tilemap[y][x] = 0;  // Grass
            }
            else {

                std::random_device rd;
                std::mt19937 gen(rd());
                std::uniform_int_distribution<> dis(1, 10);
                int randomNumber = dis(gen);

                if (randomNumber < 2)
                {
                    tilemap[y][x] = 3;
                }
                else if (randomNumber < 3)
                {
                    tilemap[y][x] = 4;
                }
                else
                {
                    tilemap[y][x] = 0;
                }
                
            }
        }
    }
}

void generateNewTiles(std::vector<std::vector<int>>& tilemap, int playerX, int playerY)
{

    if (playerX + WIDTH > tilemap.size() * TILE_SIZE - BORDER_DISTANCE) {
        int newTilesStartX = tilemap.size() * TILE_SIZE;
        int newTilesStartY = playerY - HEIGHT / 2;
        std::cout << "New tiles start at: (" << newTilesStartX << ", " << newTilesStartY << ")" << std::endl;
        generateTilemap(tilemap, newTilesStartX, newTilesStartY, HEIGHT);
    }

    if (playerX < BORDER_DISTANCE) {
        int newTilesStartX = 0;
        int newTilesStartY = playerY - HEIGHT / 2;
        std::cout << "New tiles start at: (" << newTilesStartX << ", " << newTilesStartY << ")" << std::endl;
        generateTilemap(tilemap, newTilesStartX, newTilesStartY, HEIGHT);
    }
}

int main()
{
    RenderWindow window(VideoMode(WIDTH, HEIGHT), "Tilemap");

    int mapSize = INITIAL_MAP_SIZE;
    int numTiles = mapSize / TILE_SIZE;
    int playerX = mapSize / 2;
    int playerY = mapSize / 2;

    std::vector<std::vector<int>> tilemap(mapSize, std::vector<int>(mapSize, 0));
    generateTilemap(tilemap, 0, 0, mapSize);

    Texture grassTexture;
    if (!grassTexture.loadFromFile("grass.png")) {
        std::cerr << "Failed to load grass.png" << std::endl;
        return 1;
    }

    Texture waterTexture;
    if (!waterTexture.loadFromFile("water.png")) {
        std::cerr << "Failed to load water.png" << std::endl;
        return 1;
    }

    Texture sandTexture;
    if (!sandTexture.loadFromFile("sand.png")) {
        std::cerr << "Failed to load sand.png" << std::endl;
        return 1;
    }

    Texture treeTexture;
    if (!treeTexture.loadFromFile("trees.png")) {
        std::cerr << "Failed to load trees.png" << std::endl;
        return 1;
    }
    Texture treeStoneTexture;
    if (!treeStoneTexture.loadFromFile("treesStone.png")) {
        std::cerr << "Failed to load treesStone.png" << std::endl;
        return 1;
    }

    Sprite tileSprite;
    tileSprite.setTexture(grassTexture);

    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) {
                window.close();
            }
        }

        if (Keyboard::isKeyPressed(Keyboard::W)) {
            playerY -= 1;
        }
        else if (Keyboard::isKeyPressed(Keyboard::S)) {
            playerY += 1;
        }
        else if (Keyboard::isKeyPressed(Keyboard::A)) {
            playerX -= 1;
        }
        else if (Keyboard::isKeyPressed(Keyboard::D)) {
            playerX += 1;
        }

        // Generate new tiles if the player is approaching the right edge
        generateNewTiles(tilemap, playerX, playerY);

        window.clear();

        for (int y = 0; y < HEIGHT; y += TILE_SIZE) {
            for (int x = 0; x < WIDTH; x += TILE_SIZE) {
                int tileX = (x + playerX) / TILE_SIZE;
                int tileY = (y + playerY) / TILE_SIZE;
                if (tileX < tilemap.size() && tileY < tilemap.size()) {
                    int tileType = tilemap[tileY][tileX];

                    switch (tileType) {
                    case 0:
                        tileSprite.setTexture(grassTexture);
                        break;
                    case 1:
                        tileSprite.setTexture(waterTexture);
                        break;
                    case 2:
                        tileSprite.setTexture(sandTexture);
                        break;
                    
                    case 3:
                        tileSprite.setTexture(treeTexture);
                        break;
                    case 4:
                        tileSprite.setTexture(treeStoneTexture);
                        break;
                    }

                    tileSprite.setPosition(x, y);
                    window.draw(tileSprite);
                }
            }
        }

        window.display();
    }

    return 0;
}
