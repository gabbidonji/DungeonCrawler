#include <iostream>
#include <map>
#include <vector>
#include <algorithm>
#include <math.h>

class Map{
  int size;
  std::vector<std::vector<char>> grid;
  std::vector<std::vector<bool>> visible;
  std::pair<int,int> playerPosition;

public:
  Map(int size, std::vector<std::vector<char>> grid, std::vector<std::vector<bool>> visible, std::pair<int,int> playerPosition):
  size(size), grid(grid), visible(visible), playerPosition(playerPosition){};
  bool movePlayer(char inp); //return 1 if goal reached else 0, w = move up, d = move right, s = move down, a = move left
  friend std::ostream &operator<<(std::ostream &os, const Map &ps);
  static Map createFloor(int size);
};

std::ostream &operator<<(std::ostream &os, const Map &m){
  for(int y = 0; y < m.grid.size(); y++){
    for(int x = 0; x < m.grid.size(); x++){
      char space = m.grid[y][x];
      if(m.visible[y][x]){
        if(space == 'X'){
          os << "XXX";
        } else {
          os << " " << space << " ";
        }
      } else {
      os << "   ";
      }
    }
    os << "\n";
  }
  return os;
}

Map Map::createFloor(int size)
{
  //Size should be odd and >= 5
  std::vector<std::vector<char>> grid;
  std::vector<std::vector<bool>> visible;
  std::pair<int,int> playerPosition;
  std::pair<int,int> goalPosition;
  //Create initial grid
  for(int y = 0; y < size; y++){
    std::vector<char> row;
    for(int x = 0; x < size; x++){
      char location = ' ';
      if(x == 0 || x == size-1 || y == 0 || y == size-1 || (x%2 == 0 && y%2 == 0)){
        location = 'X';
      }
      row.push_back(location);
    }
    grid.push_back(row);
    row.clear();
  }
  //Add player
  int middle = (int)floor((double)size/2);
  playerPosition = std::make_pair(middle, middle);
  grid[middle][middle] = 'P';

  //Place goal
  int goalX = rand()%(size - 1);
  if(goalX%2 != 1){
    goalX = goalX + 1;
  }
  if(goalX >= size - 1){
    goalX = size - 2;
  }
  int goalY = rand()%(size - 1);
  if(goalY%2 != 1){
    goalY = goalY + 1;
  }
  if(goalY >= size - 1){
    goalY = size - 2;
  }
  if(goalY == middle && goalX == middle){
    goalX = goalX + 1;
  }
  goalPosition = std::make_pair(goalY, goalX);
  grid[goalY][goalX] = 'G';
  //Add spaces
  int extraSpaces = ceil((size-1)*(size-1)/(double)20);
  for(int i = 0; i <= extraSpaces; i++){
    int x = rand()%(size - 1);
    if(x%2 != 0 || x == 0){
      x = x + 1;
    }
    if(x >= size - 1){
      x = size - 3;
    }
    int y = rand()%(size - 1);
    if(y%2 != 0 || y == 0){
      y = y + 1;
    }
    if(y >= size - 1){
      y = size - 3;
    }
    std::pair<int, int> newSpace = std::make_pair(x, y);
    if(newSpace != playerPosition){
      grid[y][x] = ' ';
    }
  }
  //remove extra spaces
  bool horizontalOrVertical = (rand()%1 == 1);
  int usableColumns = ceil(((double)size - 2)/2);
  for(int i = 0; i < usableColumns; i++){
    int coord1 = (i)*4 + 1;
    for(int i = 0; i < (int)(usableColumns*2/3); i++){
      int coord2 = (rand()%usableColumns + 1)*2 - 1;
      if(std::make_pair(coord1,coord2) != playerPosition){
        if(coord1 != middle || coord2 != middle){
          if(horizontalOrVertical){
            if(grid[coord1][coord2] != 'G'){
              grid[coord1][coord2] = 'X';
            }
          } else {
            if(grid[coord2][coord1] != 'G'){
              grid[coord2][coord1] = 'X';
            }
          }
        }
      }
    }
  }
  //fill holes
  for(int y = 1; y < size - 1; y++){
    for(int x = 1; x < size - 1; x++){
      int surrounding = 0;
      for(int deltaY = -1; deltaY < 2; deltaY++){
        for(int deltaX = -1; deltaX < 2; deltaX++){
          if((deltaX == 0 && deltaY != 0) || (deltaY == 0 && deltaX != 0)){
            if(grid[y + deltaY][x + deltaX] == 'X'){
              surrounding = surrounding + 1;
            }
          }
        }
      }
      char value = grid[y][x];
      if(surrounding >= 3 && value != 'G'){
        grid[y][x] = 'X';
      }
    }
  }
  //set initial visibility
  for(int y = 0; y < size; y++){
    std::vector<bool> rowVisible;
    for(int x = 0; x < size; x++){
      bool spaceVisible = false;
      if(((x == middle || x == middle + 1 || x == middle - 1) &&
      (y == middle || y == middle + 1 || y == middle - 1)) ||
      (x == 0 || x == size-1) ||
      (y == 0 || y == size-1)) {
        spaceVisible = true;
      }
      rowVisible.push_back(spaceVisible);
    }
    visible.push_back(rowVisible);
    rowVisible.clear();
  }
  //Seg Fault at return
  return Map(size, grid, visible, playerPosition);
}

bool Map::movePlayer(char inp){
  int x = std::get<0>(this -> playerPosition);
  int y = std::get<1>(this -> playerPosition);
  switch(inp){
    case 'w': {
      if(this -> grid[y-1][x] != 'X'){
        this -> playerPosition = std::make_pair(x, y-1);
      }
      break;
    }
    case 'a': {
      if(this -> grid[y][x-1] != 'X'){
        this -> playerPosition = std::make_pair(x-1, y);
      }
      break;
    }
    case 's': {
      if(this -> grid[y+1][x] != 'X'){
        this -> playerPosition = std::make_pair(x, y+1);
      }
      break;
    }
    case 'd': {
      if(this -> grid[y][x+1] != 'X'){
        this -> playerPosition = std::make_pair(x+1, y);
      }
      break;
    }
  }
  int newX = std::get<0>(this -> playerPosition);
  int newY = std::get<1>(this -> playerPosition);
  if(x == newX && y == newY){
    return 0;
  }
  for(int deltaY = -1; deltaY < 2; deltaY++){
    for(int deltaX = -1; deltaX < 2; deltaX++){
      this->visible[newY + deltaY][newX + deltaX] = 1;
    }
  }
  bool returnBool;
  if(this -> grid[newY][newX] == 'G'){
    returnBool = 1;
  } else {
    returnBool =  0;
  }
  this->grid[newY][newX] = 'P';
  this->grid[y][x] = ' ';
  return returnBool;
}

enum EnemyType
{//Weakness, Attribute
  ORC, //Slash, Defense
  GOBLIN, //Strike, Attack
  BAT, //Shoot, Speed
  FIRESPRITE, //Water, Speed
  WOODNYMPH, //Fire, Speed
  SLIME, //Plant, Defense
  DIREWOLF, //Shoot, Power
  GOLEM,//Strike, Defense
  OGRE,//Slash, Attack
  KOBOLD, //Strike, Speed
  UNDEAD, //Fire, Power
  SALAMANDER, //Water, Attack
  LAST
};

enum AttackType
{
  STRIKE,
  SHOOT,
  SLASH,
  WATER,
  FIRE,
  PLANT
};

enum Attribute
{
  SPEED,
  POWER,
  DEFENSE
};

class Player
{
  int maxHitPoints;
  int hitPoints;
  bool vulnerable;
  bool dodging;
  bool guarded;

public:
  bool playerAlive();
  int resetStatus();
  int running();
  int defend();
  int dodge();
  friend std::ostream &operator<<(std::ostream &os, const Player &p);
  Player(int hp): hitPoints(hp), maxHitPoints(hp), vulnerable(0), dodging(0), guarded(0){}
  bool takeDamage(int damage); //1 if alive, 0 if dead;
};

bool Player::playerAlive(){
  return this -> hitPoints > 0;
}

bool Player::takeDamage(int damage){
  int dodged = 10 - (dodging*rand()%10);
  if(dodged <= 6){
    std::cout << "You dodged and took 0 damage!\n";
    return 1;
  } else {
    if(vulnerable){
      damage = 2*damage;
      if(!dodging){
        std::cout << "You're vulnerable so you took double damage!\n";
      } else {
        std::cout << "You stumbled and took double damage!\n";
      }
    } else if (guarded){
      damage = 0.5*damage;
      std::cout << "You guarded and took half damage!\n";
    }
    std::cout << "Took " << damage << " damage!\n";
    this->hitPoints = this->hitPoints - damage;
  }
  if(this->hitPoints <= 0){
    std::cout << "You died...\n\n";
    return 0;
  }
  return 1;
}

std::ostream &operator<<(std::ostream &os, const Player &p){
  os << "HP: " << p.hitPoints << "/" << p.maxHitPoints << "\n";
  return os;
}

int Player::running(){
  this->vulnerable = 1;
  this->guarded = 0;
  this->dodging = 0;
  return 0;
}

int Player::defend(){
  this->guarded = 1;
  this->vulnerable = 0;
  this->dodging = 0;
  return 0;
}

int Player::dodge(){
  this->vulnerable = 1;
  this->dodging = 1;
  this->guarded = 0;
  return 0;
}

int Player::resetStatus(){
  this->vulnerable = 0;
  this->guarded = 0;
  this->dodging = 0;
  return 0;
}

class Enemy
{
  EnemyType type;
  std::string name;
  AttackType weakness;
  Attribute ability;
  int hitPoints;
  int defense;
  int attack;
  int speed;

public:
  Enemy(EnemyType type, AttackType weakness, Attribute ability, int attack, int defense, int speed, std::string name):
  type(type), weakness(weakness), ability(ability), hitPoints(100), attack(attack), defense(defense), speed(speed), name(name){}
  friend std::ostream &operator<<(std::ostream &os, const Enemy &en);
  int getSpeed();
  int calculateDamage();
  bool takeDamage(AttackType receivedAttack); //1 alive, 0 dead
  static Enemy generateEnemy();
  bool isType(EnemyType type);
  std::string getName();
};

bool Enemy::isType(EnemyType type){
  if (this -> type == type){
    return 1;
  }
  return 0;
}

std::string Enemy::getName(){
  return this->name;
}

bool Enemy::takeDamage(AttackType receivedAttack){
  std::map<AttackType, std::string> attackStringMap;
  attackStringMap[SLASH] = "slash";
  attackStringMap[STRIKE] = "blunt";
  attackStringMap[SHOOT] = "pierce";
  attackStringMap[FIRE] = "fire";
  attackStringMap[WATER] = "water";
  attackStringMap[PLANT] = "nature";
  int damage = 40 - (defense - 2 + rand()%5);
  if(receivedAttack == this -> weakness){
    damage = 2 * damage;
    std::cout << "It's strong against the enemy! Damage is doubled!\n";
  }
  this -> hitPoints = this -> hitPoints - damage;
  std::cout << "Did " << damage << " points of " << attackStringMap[receivedAttack] << " damage!\n";
  if(this -> hitPoints <= 0){
    return 0;
  }
  return 1;
}

int Enemy::calculateDamage(){
  return (attack*3/4) - 2 + (rand()%4);
}

int Enemy::getSpeed(){
  return this->speed;
}

Enemy Enemy::generateEnemy(){
  std::map<EnemyType, std::pair<std::pair<AttackType, Attribute>, std::string>> enemyMap;
  enemyMap[ORC] = std::make_pair(std::make_pair(SLASH, DEFENSE), "Orc");
  enemyMap[GOBLIN] = std::make_pair(std::make_pair(STRIKE, POWER), "Goblin");
  enemyMap[BAT] = std::make_pair(std::make_pair(SHOOT, SPEED), "Bat");
  enemyMap[FIRESPRITE] = std::make_pair(std::make_pair(WATER, SPEED), "Fire Sprite");
  enemyMap[WOODNYMPH] = std::make_pair(std::make_pair(FIRE, SPEED), "Wood Nymph");
  enemyMap[SLIME] = std::make_pair(std::make_pair(PLANT, DEFENSE), "Slime");
  enemyMap[DIREWOLF] = std::make_pair(std::make_pair(SHOOT, POWER), "Dire Wolf");
  enemyMap[GOLEM] = std::make_pair(std::make_pair(STRIKE, DEFENSE), "Golem");
  enemyMap[OGRE] = std::make_pair(std::make_pair(SLASH, POWER), "Ogre");
  enemyMap[KOBOLD] = std::make_pair(std::make_pair(STRIKE, SPEED), "Kobold");
  enemyMap[UNDEAD] = std::make_pair(std::make_pair(FIRE, POWER), "Undead");
  enemyMap[SALAMANDER] = std::make_pair(std::make_pair(WATER, POWER), "Salamander");
  EnemyType type = static_cast<EnemyType>(rand() % LAST);
  std::pair<AttackType, Attribute> values = std::get<0>(enemyMap[type]);
  std::string name = std::get<1>(enemyMap[type]);
  AttackType weakness =  std::get<0>(values);
  Attribute attribute =  std::get<1>(values);
  int attack;
  int defense;
  int speed;
  switch(attribute){
    case POWER:{
      attack = 15 + rand()%5;
      defense = 5 + rand()%10;
      speed = 5 + rand()%10;
      break;
    }
    case DEFENSE: {
      defense = 15 + rand()%5;
      attack = 5 + rand()%10;
      speed = 5 + rand()%10;
      break;
    }
    case SPEED: {
      speed = 15 + rand()%5;
      attack = 5 + rand()%10;
      defense = 5 + rand()%10;
      break;
    }
  }
  return Enemy(type, weakness, attribute, attack, defense, speed, name);
}

std::ostream &operator<<(std::ostream &os, const Enemy &en){
  std::map<AttackType, std::string> attackMap;
  attackMap[STRIKE] = "Strike";
  attackMap[SLASH] = "Slash";
  attackMap[SHOOT] = "Shoot";
  attackMap[WATER] = "Water";
  attackMap[FIRE] = "Fire";
  attackMap[PLANT] = "Plant";
  std::map<Attribute, std::string> attributeMap;
  attributeMap[POWER] = "Power";
  attributeMap[SPEED] = "Speed";
  attributeMap[DEFENSE] = "Defense";
  os << en.name << "\n";
  os << "Weakness: " << attackMap[en.weakness] << "\n";
  os << "Attribute: " << attributeMap[en.ability] << "\n";
  os << "Att: " << en.attack << "\n";
  os << "Def: " << en.defense << "\n";
  os << "Spd: " << en.speed << "\n";
  return os;
}

class Battle
{
  Enemy enemy;
  Player player;
  bool lastTurn; //1 if player, 0 if enemy
public:
  Battle(Enemy enemy, Player player): player(player), enemy(enemy), lastTurn(0){}
  Player execute();
  static Battle createBattle(Player player);
};

Battle Battle::createBattle(Player player){
  Enemy enemy = Enemy::generateEnemy();
  return Battle(enemy, player);
}

Player Battle::execute(){
  std::cout << "\nYou were discovered by ";
  if(this->enemy.isType(ORC) || this->enemy.isType(OGRE) || this->enemy.isType(UNDEAD)){
    std::cout << "an ";
  } else {
    std::cout << "a ";
  }
  std::cout << this->enemy.getName() << "!\n";
  int playerTime = 0;
  int enemyTime = 0;
  while(true){
    if(playerTime <= enemyTime){
      std::cout << "\n" << player;
      if(lastTurn == 1){
        std::cout << "\nYou moved quickly and got another turn!\n";
      }
      player.resetStatus();
      std::cout << "Pick an action:\n1. Attack\n2. Evade\n3. Defend\n4. Run\n\nEnter Action: ";
      int mainAction;
      std::cin >> mainAction;
      switch(mainAction){
        case 1: {
          int attackType;
          std::cout << "\nWhat type of attack?\n1. Use Sword\n2. Use Bow\n3. Use Mace\n4. Cast Flare\n5. Cast Torrent\n6. Cast Overgrowth\n\nEnter Action: \n";
          std::cin >> attackType;
          bool enemyAlive;
          switch(attackType){
            int attackType;
            case 1:
            enemyAlive = this->enemy.takeDamage(SLASH);
            break;
            case 2:
            enemyAlive = this->enemy.takeDamage(SHOOT);
            break;
            case 3:
            enemyAlive = this->enemy.takeDamage(STRIKE);
            break;
            case 4:
            enemyAlive = this->enemy.takeDamage(FIRE);
            break;
            case 5:
            enemyAlive = this->enemy.takeDamage(WATER);
            break;
            case 6:
            enemyAlive = this->enemy.takeDamage(PLANT);
            break;
          }
          if(!enemyAlive){
            std::cout << "You defeated the " << this->enemy.getName() << "!\n\n";
            return player;
          }
          break;
        }
        case 2:
        std::cout << "\n";
        player.dodge();
        break;
        case 3:
        std::cout << "\n";
        player.defend();
        break;
        case 4:
        std::cout << "\n";
        player.running();
        if(rand()%2){
          std::cout << "You escaped!\n\n";
          return player;
        }
        std::cout << "You let your guard down trying to escape!\n";
        break;
      }
      this -> lastTurn = 1;
      playerTime = playerTime + 15;
    } else {
      std::cout << "\nThe " << this->enemy.getName() << " attacked";
      if(lastTurn == 0){
        std::cout << " again";
      }
      std::cout << "!\n";
      bool playerAlive = player.takeDamage(enemy.calculateDamage());
      this -> lastTurn = 0;
      if(!playerAlive){
        return player;
      }
      enemyTime = enemyTime + (30 - enemy.getSpeed());
    }
  }
  return player;
}

int main(){
  srand(time(0));
  /*//Only run map
  int size;
  std::cin >> size;

  while(true){
    char move;
    bool goalReached = 0;
    std::cout << map;
    std::cin >> move;
    goalReached = map.movePlayer(move);
    if(goalReached) {
      map = Map::createFloor(size);
    }
  }
  */
  /*//Only run battle
  Player player = Player(100);
  Battle battle = Battle::createBattle(player);
  bool won = battle.execute();
  if(!won){
  return 0;
} else {
return 0;
}
return 0;
*/
  int floors = 1;
  int hitPoints = 100;
  while(floors > 0){
    int size = 17;
    Map map = Map::createFloor(size);
    Player player = Player(hitPoints);
    while(true){
      std::cout << player;
      char move;
      bool goalReached;
      std::cout << map;
      std::cin >> move;
      goalReached = map.movePlayer(move);
      int randomEncounter = ((rand()%20) + 1);
      if(randomEncounter > 17){
        Battle battle = Battle::createBattle(player);
        player = battle.execute();
        if(!player.playerAlive()){
          std::cout << "\nYou made it " << floors << " floors down.\n\nGAME OVER\n\n";
          return 0;
        }
      }
      if(goalReached) {
        floors = floors + 1;
        break;
      }
    }
  }
  return 0;
}
