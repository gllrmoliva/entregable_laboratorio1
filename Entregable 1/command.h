#include <algorithm>
enum Command {
    MOVE_LEFT = 0, // par
    MOVE_RIGHT,
    MOVE_DOWN, // par
    MOVE_UP,
    ROTATE, // par
    UNDO_ROTATE
};

struct Movement {
    Command command;
    Command inv_command;
    int arg;
    Movement(Command command, int arg) {
        this->command = command;
        this->arg = arg;
        if (command % 2) {
            inv_command = (Command) (command - 1);
        } else {
            inv_command = (Command) (command + 1);
        }
    }
};
