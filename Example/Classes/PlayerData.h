#ifndef __PLAYER_DATA_H__
#define __PLAYER_DATA_H__


#include <map>
#include <string>
#include "GameOfWhales/GameOfWhales.h"

using namespace std;

#define SPECIALITY_WIZARD "wizard"
#define SPECIALITY_WARRIOR "warrior"
#define SPECIALITY_ROGUE "rogue"

class PlayerData
{
public:
    void load();
    void save();
    void addCoins(int coins);
    void nextLocation();
    void nextLevel() { _level ++; }
    
    int getCoins() { return _coins; }
    const string& getSpeciality() { return _speciality; }
    bool getGender() { return _gender; }
    int getLevel() { return _level; }
    char * getLocation() { return &_locationC; }
    
private:
    void updateLocation();
    
    int _level = 1;
    bool _gender = false;
    string _speciality = SPECIALITY_WIZARD;
    int _coins = 100000;
    int _location = 0;
    char _locationC = 'A';
};

#endif // __PLAYER_DATA_H__
