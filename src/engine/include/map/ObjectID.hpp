#pragma once

#include <cstdint>

enum ObjectID : std::size_t
{
    FIRST_OBJECT = 2000,

    // write objects here
    Car,
    DestroyedCar,
    Chair,
    OutdoorBench,
    Table,
    SmallTree,
    LargeTree,
    SmallBush,
    LargeBush,

    LAST_OBJECT,
    OBJECT_COUNT = LAST_OBJECT - FIRST_OBJECT - 1
};

inline const char* objectIdToString(std::size_t object_id)
{
    switch(object_id)
    {
        case ObjectID::Car: return "Car"; break;
        case ObjectID::DestroyedCar: return "DestroyedCar"; break;
        case ObjectID::Chair: return "Chair"; break;
        case ObjectID::OutdoorBench: return "OutdoorBench"; break;
        case ObjectID::Table: return "Table"; break;
        case ObjectID::SmallTree: return "SmallTree"; break;
        case ObjectID::LargeTree: return "LargeTree"; break;
        case ObjectID::SmallBush: return "SmallBush"; break;
        case ObjectID::LargeBush: return "LargeBush"; break;
        default: return "Unknown item"; break;
    }
}
