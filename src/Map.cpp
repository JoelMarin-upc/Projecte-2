
#include "Engine.h"
#include "Render.h"
#include "Textures.h"
#include "Map.h"
#include "Log.h"

#include <math.h>

Map::Map() : mapLoaded(false)
{

}

// Destructor
Map::~Map()
{}

// Called before render is available
bool Map::Awake()
{
    LOG("Loading Map Parser");

    return true;
}

bool Map::Start() {
    return true;
}

bool Map::Update(float dt)
{
    bool ret = true;

    if (mapLoaded) {
        Engine::GetInstance().render->DrawTexture(bgtexture,-Engine::GetInstance().render->camera.x,40);
        // L07 TODO 5: Prepare the loop to draw all tiles in a layer + DrawTexture()
        // iterate all tiles in a layer
        for (const auto& mapLayer : mapData.layers) {
            //L09 TODO 7: Check if the property Draw exist get the value, if it's true draw the lawyer
            if (mapLayer->properties.GetProperty("Draw") != NULL && mapLayer->properties.GetProperty("Draw")->value == true) {
                Vector2D camPosTile = GetCameraPositionInTiles();
                Vector2D limits = GetCameraLimitsInTiles(camPosTile);
                
                for (int i = camPosTile.getX(); i <= limits.getX(); i++) {
                    for (int j = camPosTile.getY(); j <= limits.getY(); j++) {
						// L07 TODO 9: Complete the draw function
                        //Get the gid from tile
                        int gid = mapLayer->Get(i, j);

                        //Check if the gid is different from 0 - some tiles are empty
                        if (gid != 0) {
                            //L09: TODO 3: Obtain the tile set using GetTilesetFromTileId
                            TileSet* tileSet = mapData.tilesets.front();
                            if (tileSet != nullptr) {
                                //Get the Rect from the tileSetTexture;
                                SDL_Rect tileRect = tileSet->GetRect(gid);
                                //Get the screen coordinates from the tile coordinates
                                Vector2D mapCoord = MapToWorld(i, j);
                                //Draw the texture
                                Engine::GetInstance().render->DrawTexture(tileSet->texture, (int)mapCoord.getX(), (int)mapCoord.getY(), &tileRect);
                            }
                        }
                    }
                }
            }
        }
    }

    return ret;
}

// L09: TODO 2: Implement function to the Tileset based on a tile id
TileSet* Map::GetTilesetFromTileId(int gid) const
{
    TileSet* set = nullptr;
    for (const auto& tileset : mapData.tilesets) {
        set = tileset;
        if (gid >= tileset->firstGid && gid < tileset->firstGid + tileset->tileCount) {
            break;
        }
    }
    return set;
}

// Called before quitting
bool Map::CleanUp()
{
    LOG("Unloading map");

    // L06: TODO 2: Make sure you clean up any memory allocated from tilesets/map
    for (const auto& tileset : mapData.tilesets) {
        delete tileset;
    }
    mapData.tilesets.clear();

    // L07 TODO 2: clean up all layer data
    for (const auto& layer : mapData.layers)
    {
        delete layer;
    }
    mapData.layers.clear();

    // L07 TODO 2: clean up all layer data
    for (const auto& object : mapData.objectlayers)
    {
        delete object;
    }
    mapData.objectlayers.clear();

    return true;
}

// Load new map
bool Map::Load(std::string path, std::string fileName)
{
    bgtexture = Engine::GetInstance().textures->Load("Assets/Maps/forest1.png");
    bool ret = false;

    // Assigns the name of the map file and the path
    mapFileName = fileName;
    mapPath = path;
    std::string mapPathName = mapPath + mapFileName;

    pugi::xml_document mapFileXML;
    pugi::xml_parse_result result = mapFileXML.load_file(mapPathName.c_str());

    if(result == NULL)
	{
		LOG("Could not load map xml file %s. pugi error: %s", mapPathName.c_str(), result.description());
		ret = false;
    }
    else {

        // L06: TODO 3: Implement LoadMap to load the map properties
        // retrieve the paremeters of the <map> node and store the into the mapData struct
        mapData.width = mapFileXML.child("map").attribute("width").as_int();
        mapData.height = mapFileXML.child("map").attribute("height").as_int();
        mapData.tileWidth = mapFileXML.child("map").attribute("tilewidth").as_int();
        mapData.tileHeight = mapFileXML.child("map").attribute("tileheight").as_int();

        // L06: TODO 4: Implement the LoadTileSet function to load the tileset properties
       
        //Iterate the Tileset
        for(pugi::xml_node tilesetNode = mapFileXML.child("map").child("tileset"); tilesetNode!=NULL; tilesetNode = tilesetNode.next_sibling("tileset"))
		{
            //Load Tileset attributes
			TileSet* tileSet = new TileSet();
            tileSet->firstGid = tilesetNode.attribute("firstgid").as_int();
            tileSet->name = tilesetNode.attribute("name").as_string();
            tileSet->tileWidth = tilesetNode.attribute("tilewidth").as_int();
            tileSet->tileHeight = tilesetNode.attribute("tileheight").as_int();
            tileSet->spacing = tilesetNode.attribute("spacing").as_int();
            tileSet->margin = tilesetNode.attribute("margin").as_int();
            tileSet->tileCount = tilesetNode.attribute("tilecount").as_int();
            tileSet->columns = tilesetNode.attribute("columns").as_int();

			//Load the tileset image
			std::string imgName = tilesetNode.child("image").attribute("source").as_string();
            tileSet->texture = Engine::GetInstance().textures->Load((mapPath+imgName).c_str());

			mapData.tilesets.push_back(tileSet);
		}

        // L07: TODO 3: Iterate all layers in the TMX and load each of them
        for (pugi::xml_node layerNode = mapFileXML.child("map").child("layer"); layerNode != NULL; layerNode = layerNode.next_sibling("layer")) {

            // L07: TODO 4: Implement the load of a single layer 
            //Load the attributes and saved in a new MapLayer
            MapLayer* mapLayer = new MapLayer();
            mapLayer->id = layerNode.attribute("id").as_int();
            mapLayer->name = layerNode.attribute("name").as_string();
            mapLayer->width = layerNode.attribute("width").as_int();
            mapLayer->height = layerNode.attribute("height").as_int();

            //L09: TODO 6 Call Load Layer Properties
            LoadProperties(layerNode, mapLayer->properties);

            //Iterate over all the tiles and assign the values in the data array
            for (pugi::xml_node tileNode = layerNode.child("data").child("tile"); tileNode != NULL; tileNode = tileNode.next_sibling("tile")) {
                mapLayer->tiles.push_back(tileNode.attribute("gid").as_int());
            }

            //add the layer to the map
            mapData.layers.push_back(mapLayer);
        }

        for (pugi::xml_node objectGroupNode = mapFileXML.child("map").child("objectgroup"); objectGroupNode != NULL; objectGroupNode = objectGroupNode.next_sibling("objectgroup")) {

            ObjectGroup* objectGroup = new ObjectGroup();
            objectGroup->id = objectGroupNode.attribute("id").as_int();
            objectGroup->name = objectGroupNode.attribute("name").as_string();

            for (pugi::xml_node objectNode = objectGroupNode.child("object"); objectNode != NULL; objectNode = objectNode.next_sibling("object")) {
                Object* object = new Object();
                object->id = objectNode.attribute("id").as_int();
                object->x = objectNode.attribute("x").as_int();
                object->y = objectNode.attribute("y").as_int();
                object->width = objectNode.attribute("width").as_int();
                object->height = objectNode.attribute("height").as_int();
                objectGroup->objects.push_back(object);
            }

            mapData.objectlayers.push_back(objectGroup);
        }

        // L08 TODO 3: Create colliders
        // L08 TODO 7: Assign collider type
        // Later you can create a function here to load and create the colliders from the map
        //Iterate the layer and create colliders
        for (const auto& mapLayer : mapData.layers) {
            if (mapLayer->name == "Logic") {
                for (int i = 0; i < mapData.width; i++) {
                    for (int j = 0; j < mapData.height; j++) {
                        int gid = mapLayer->Get(i, j);
                        Vector2D mapCoord = MapToWorld(i, j);            
                        if (gid == 626) {
                            Collider* c1 = Engine::GetInstance().physics->CreateRectangle(mapCoord.getX() + mapData.tileWidth / 2, mapCoord.getY() + mapData.tileHeight / 2, mapData.tileWidth, mapData.tileHeight, STATIC, 0x0002, 0X003);
                            c1->ctype = ColliderType::DEATHZONE;
                        }
                        else if (gid == 627) {
                            Collider* c1 = Engine::GetInstance().physics->CreateRectangleSensor(mapCoord.getX() + mapData.tileWidth / 2, mapCoord.getY() + mapData.tileHeight / 2, mapData.tileWidth, mapData.tileHeight, STATIC, 0x0002, 0X003);
                            c1->ctype = ColliderType::RESPAWNPOINT;
                        }
                        else if (gid == 628) {
                            Collider* c1 = Engine::GetInstance().physics->CreateRectangleSensor(mapCoord.getX() + mapData.tileWidth / 2, mapCoord.getY() + mapData.tileHeight / 2, mapData.tileWidth, mapData.tileHeight, STATIC, 0x0002, 0X003);
                            c1->ctype = ColliderType::NEXTLEVEL;
                        }
                        else if (gid == 629) {            
                            Collider* c1 = Engine::GetInstance().physics->CreateRectangleSensor(mapCoord.getX() + mapData.tileWidth / 2, mapCoord.getY() + mapData.tileHeight / 2, mapData.tileWidth, mapData.tileHeight, STATIC, 0x0002, 0X003);
                            c1->ctype = ColliderType::RESPAWNPOINT;
                            playerStartPos = new Vector2D(mapCoord);
                        }
                        else if (gid == 630) {
                            Collider* c1 = Engine::GetInstance().physics->CreateRectangleSensor(mapCoord.getX() + mapData.tileWidth / 2, mapCoord.getY() + mapData.tileHeight / 2, mapData.tileWidth, mapData.tileHeight, STATIC, 0x0002, 0X003);
                            c1->ctype = ColliderType::ITEMDESTROYER;
                        }
                        else if (gid == 631) {
                            Collider* c1 = Engine::GetInstance().physics->CreateRectangleSensor(mapCoord.getX() + mapData.tileWidth / 2, mapCoord.getY() + mapData.tileHeight / 2, mapData.tileWidth, mapData.tileHeight, STATIC, 0x0002, 0X003);
                            c1->ctype = ColliderType::BOSSSTART;
                        }
                        else if (gid == 632) {
                            Collider* c1 = Engine::GetInstance().physics->CreateRectangle(mapCoord.getX() + mapData.tileWidth / 2, mapCoord.getY() + mapData.tileHeight / 2, mapData.tileWidth, mapData.tileHeight*3, STATIC, 0x0002, 0X003);
                            c1->ctype = ColliderType::DOOR;
                        }
                    }
                }
            }

            if (mapLayer->name == "Enemy") {
                for (int i = 0; i < mapData.width; i++) {
                    for (int j = 0; j < mapData.height; j++) {
                        int gid = mapLayer->Get(i, j);
                        if (gid == 0) continue;
                        EnemyData enemy;
                        
                        Vector2D mapCoord = MapToWorld(i, j);
                        int enType = 2;
                        if (gid == 630) {
                            enType = 2;
                            enemy.position = { mapCoord.getX()+10,mapCoord.getY() - 64+10 };
                        }
                        else {     
                            if (gid == 627) enType = 0;
                            else if (gid == 628) enType = 1;
                            enemy.position = mapCoord;
                        }
                        enemy.enType = enType;
                        enemies.push_back(enemy);
                    }
                }
            }

            if (mapLayer->name == "Items") {
                for (int i = 0; i < mapData.width; i++) {
                    for (int j = 0; j < mapData.height; j++) {
                        int gid = mapLayer->Get(i, j);
                        if (gid == 0) continue;
                        ItemData item;

                        Vector2D mapCoord = MapToWorld(i, j);
                        EntityType type = EntityType::UNKNOWN;
                        if (gid == 627) type = EntityType::HEART;
                        else if (gid == 628) type = EntityType::COIN;
                        else if (gid == 629) type = EntityType::ITEM;
                        else if (gid == 630) type = EntityType::KEY;
                        else if (gid == 631) type = EntityType::RECHARGE;
                        
                        item.type = (int)type;
                        item.position = {mapCoord.getX() + 10, mapCoord.getY()+ 10 };
                        items.push_back(item);
                    }
                }
            }
           /* if (mapLayer->name == "Map") {
                for (int i = 0; i < mapData.width; i++) {
                    for (int j = 0; j < mapData.height; j++) {
                        int gid = mapLayer->Get(i, j);
                        Vector2D mapCoord = MapToWorld(i, j);
                        if (gid == 486) {
                            PhysBody* c1 = Engine::GetInstance().physics->CreateRectangle(mapCoord.getX() + mapData.tileWidth / 2, mapCoord.getY() + mapData.tileHeight / 2, mapData.tileWidth, mapData.tileHeight, STATIC);
                            c1->ctype = ColliderType::SEMIRIGID; //not implemented
                        }
                    }
                }
            }
            */
        }


        for (const auto& objectGroup : mapData.objectlayers) {
            if (objectGroup->name == "Collisions") {
                for (const auto& object : objectGroup->objects) {
                    Collider* c1 = Engine::GetInstance().physics->CreateRectangle(object->x + object->width / 2, object->y + object->height / 2, object->width, object->height, STATIC, 0x0001, 0X0007);
                    c1->ctype = ColliderType::PLATFORM;
                }
            }
        }

        ret = true;

        // L06: TODO 5: LOG all the data loaded iterate all tilesetsand LOG everything
        if (ret == true)
        {
            LOG("Successfully parsed map XML file :%s", fileName.c_str());
            LOG("width : %d height : %d", mapData.width, mapData.height);
            LOG("tile_width : %d tile_height : %d", mapData.tileWidth, mapData.tileHeight);
            LOG("Tilesets----");

            //iterate the tilesets
            for (const auto& tileset : mapData.tilesets) {
                LOG("name : %s firstgid : %d", tileset->name.c_str(), tileset->firstGid);
                LOG("tile width : %d tile height : %d", tileset->tileWidth, tileset->tileHeight);
                LOG("spacing : %d margin : %d", tileset->spacing, tileset->margin);
            }
            			
            LOG("Layers----");

            for (const auto& layer : mapData.layers) {
                LOG("id : %d name : %s", layer->id, layer->name.c_str());
				LOG("Layer width : %d Layer height : %d", layer->width, layer->height);
            }   
        }
        else {
            LOG("Error while parsing map file: %s", mapPathName.c_str());
        }

        if (mapFileXML) mapFileXML.reset();

    }

    mapLoaded = ret;
    return ret;
}

// L07: TODO 8: Create a method that translates x,y coordinates from map positions to world positions
Vector2D Map::MapToWorld(int i, int j) const
{
    Vector2D ret;

    ret.setX((float)(i * mapData.tileWidth));
    ret.setY((float)(j * mapData.tileHeight));

    return ret;
}

Vector2D Map::WorldToMap(int x, int y) {

    Vector2D ret(0, 0);
    ret.setX((float)(x / mapData.tileWidth));
    ret.setY((float)(y / mapData.tileHeight));

    return ret;
}

// L09: TODO 6: Load a group of properties from a node and fill a list with it
bool Map::LoadProperties(pugi::xml_node& node, Properties& properties)
{
    bool ret = false;

    for (pugi::xml_node propertieNode = node.child("properties").child("property"); propertieNode; propertieNode = propertieNode.next_sibling("property"))
    {
        Properties::Property* p = new Properties::Property();
        p->name = propertieNode.attribute("name").as_string();
        p->value = propertieNode.attribute("value").as_bool(); // (!!) I'm assuming that all values are bool !!

        properties.propertyList.push_back(p);
    }

    return ret;
}

Vector2D Map::GetMapSizeInPixels()
{
    Vector2D sizeInPixels;
    sizeInPixels.setX((float)(mapData.width * mapData.tileWidth));
    sizeInPixels.setY((float)(mapData.height * mapData.tileHeight));
    return sizeInPixels;
}

Vector2D Map::GetMapSizeInTiles()
{
    return Vector2D((float)mapData.width, (float)mapData.height);
}

// Method to get the navigation layer from the map
MapLayer* Map::GetNavigationLayer() {
    for (const auto& layer : mapData.layers) {
        if (layer->properties.GetProperty("Navigation") != NULL &&
            layer->properties.GetProperty("Navigation")->value) {
            return layer;
        }
    }

    return nullptr;
}

Vector2D Map::GetCameraPositionInTiles() {

    Vector2D camPos = Vector2D(Engine::GetInstance().render->camera.x * -1, Engine::GetInstance().render->camera.y * -1);
    if (camPos.getX() < 0) camPos.setX(0);
    if (camPos.getY() < 0) camPos.setY(0);

    Vector2D camPosTile = WorldToMap(camPos.getX(), camPos.getY());

    return camPosTile;
}

Vector2D Map::GetCameraLimitsInTiles(Vector2D camPosTile) {

    Vector2D camSize = Vector2D(Engine::GetInstance().render->camera.w, Engine::GetInstance().render->camera.h);
    Vector2D camSizeTile = WorldToMap(camSize.getX(), camSize.getY());

    Vector2D limits = Vector2D(camPosTile.getX() + camSizeTile.getX(), camPosTile.getY() + camSizeTile.getY());
    if (limits.getX() > mapData.width) limits.setX(mapData.width);
    if (limits.getY() > mapData.height) limits.setY(mapData.height);

    return limits;
}
