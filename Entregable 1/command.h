#ifndef COMMAND_H
#define COMMAND_H

#include <algorithm>
#include <string>
enum Command
{
    MOVE_LEFT = 0, // par
    MOVE_RIGHT,
    MOVE_DOWN, // par
    MOVE_UP,
    ROTATE, // par
    UNDO_ROTATE
};

struct Movement
{
    Command command;
    Command inv_command;
    int arg;
    Movement(Command command, int arg)
    {
        this->command = command;
        this->arg = arg;
        if (command % 2)
        {
            inv_command = (Command)(command - 1);
        }
        else
        {
            inv_command = (Command)(command + 1);
        }
    }

    // Esto es solo para ver si se guardan bien las cosas en stack
    std::string to_string()
    {
        switch (this->command)
        {
        case MOVE_LEFT:
            return "IZQUIERDA - " + std::to_string(arg);
            break;
        case MOVE_RIGHT:
            return "DERECHA - " + std::to_string(arg);
            break;
        case MOVE_UP:
            return "ARRIBA - " + std::to_string(arg);
            break;
        case MOVE_DOWN:
            return "ABAJO - " + std::to_string(arg);
            break;
        case ROTATE:
            return "ROTAR 90";
            break;
        case UNDO_ROTATE:
            return "ROTAR -90";
            break;
        default:
            return "ALGO RARO OCURRIO";
            break;
        }
    }
};

#endif