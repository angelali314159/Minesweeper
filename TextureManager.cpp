#include <unordered_map>
#include <SFML/Graphics.hpp>
#include "TextureManager.h"
using namespace std;

unordered_map<string, sf::Texture> TextureManager::textures;

sf::Texture &TextureManager::getTexture(string textureName) {
    auto result = textures.find(textureName);
    if (result == textures.end()){
        //texture does not already exist in the map, so need to get it
        sf::Texture newTexture;
        newTexture.loadFromFile("files/images/" + textureName + ".png");
        textures[textureName] = newTexture;
        return textures[textureName];
    }
    else {
        return result->second;
    }
}

