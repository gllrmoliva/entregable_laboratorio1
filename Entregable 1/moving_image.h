#ifndef MOVING_IMG_H
#define MOVING_IMG_H

#include "basics.h"
#include <iostream>

#include <stack>
#include <queue>
#include <algorithm>
#include <string>

// Enumerador de posibles comandos que se pueden hacer sobre la imagen.
enum Command
{
    MOVE_LEFT = 0, // par
    MOVE_RIGHT,
    MOVE_DOWN, // par
    MOVE_UP,
    ROTATE, // par
    UNDO_ROTATE
};

// Struct que define un movimiento en base al comando y su argumento.
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



// Clase que representa una imagen como una colección de 3 matrices siguiendo el
// esquema de colores RGB

class moving_image
{
private:
    unsigned char **red_layer;   // Capa de tonalidades rojas
    unsigned char **green_layer; // Capa de tonalidades verdes
    unsigned char **blue_layer;  // Capa de tonalidades azules

    /* 
     * Para el comando undo solo necesitamos ver el último movimiento ingresado, y de la misma
     * manera, para el redo solo necesitamos ver el último movimiento al que se le hizo undo
     * (antes de que se haga otro movimiento que destruya el contenedor de movimientos revertidos)
     * Entonces, considerando esos requerimientos, un stack resulta ser la estructura de datos que
     * mejor se adapta.
     */
    std::stack<Movement> history_stack;
    std::stack<Movement> undo_stack;
    /* 
     * En el caso de all_movements, queremos poder "recorrer" los movimientos realizados desde
     * más antiguo a más viejo, por lo que un queue se adapta a lo que necesitamos.
     */
    std::queue<Movement> all_movements; // Todavía no se muy bien que vamos a hacer con este

    /* 
     * Función para realizar un movimiento a la imagen en base al struct de Movement.
     * IMPORTANTE: esto no es una interfaz para el usuario, por lo que llama
     * a la función privada move() y rotation(), y no realizará cambios a los stacks que mantienen
     * un historial.
     */
    void move_from_movement(Movement movement)
    {
        Command cmd = movement.command;
        int arg = movement.arg;
        switch (cmd)
        {
        case MOVE_LEFT:
            /* Un mov a la izquierda es traslacion eje x negativa. */
            move(-1 * arg, 0);
            break;
        case MOVE_RIGHT:
            /* Un mov a la derecha es traslacion eje x positiva. */
            move(arg, 0);
            break;
        case MOVE_UP:
            /* Un mov hacia arriba es traslacion eje y negativa. */
            move(0, -1 * arg);
            break;
        case MOVE_DOWN:
            /* Un mov hacia abajo es traslacion eje y positiva. */
            move(0, arg);
            break;
        case ROTATE:
            /* Rotación de 90 grados en contra de las manecillas del reloj */
            rotation(false);
            break;
        case UNDO_ROTATE:
            /* Rotación de 90 grados en sentido de las manecillas del reloj */
            rotation(true);
            break;

        default:
            break;
        }
    }

    /* Función de movimiento, recordar que el eje x es positivo a la derecha, eje y hacia abajo. */
    void move(int d_x, int d_y)
    {
        unsigned char tmp_layer[H_IMG][W_IMG];

        /* 
         * Las distancias positivas son simples, ya que se trata de una operación resto sencilla,
         * para las distancias negativas simplemente las trate como si fueran distancias positivas
         * pero sobre la matriz "de cabeza", por así decirlo, que se soluciona con la misma lógica
         * anterior.
         * Habiendo dicho eso, el bloque "else" probablemente se pueda mejorar
         */
        for (int i = 0; i < H_IMG; i++)
            for (int j = 0; j < W_IMG; j++)
            {
                int new_x, new_y;
                if (d_x >= 0)
                {
                    new_x = (j + d_x) % W_IMG;
                }
                else
                {
                    new_x = W_IMG - 1 - (W_IMG - 1 - j - d_x) % W_IMG;
                }

                if (d_y >= 0)
                {
                    new_y = (i + d_y) % H_IMG;
                }
                else
                {
                    new_y = H_IMG - 1 - (H_IMG - 1 - i - d_y) % H_IMG;
                }

                // Movemos
                tmp_layer[new_y][new_x] = red_layer[i][j];
            }

        for (int i = 0; i < H_IMG; i++)
            for (int j = 0; j < W_IMG; j++)
                red_layer[i][j] = tmp_layer[i][j];

        for (int i = 0; i < H_IMG; i++)
            for (int j = 0; j < W_IMG; j++)
            {
                int new_x, new_y;
                if (d_x >= 0)
                {
                    new_x = (j + d_x) % W_IMG;
                }
                else
                {
                    new_x = W_IMG - 1 - (W_IMG - 1 - j - d_x) % W_IMG;
                }

                if (d_y >= 0)
                {
                    new_y = (i + d_y) % H_IMG;
                }
                else
                {
                    new_y = H_IMG - 1 - (H_IMG - 1 - i - d_y) % H_IMG;
                }

                // Movemos
                tmp_layer[new_y][new_x] = green_layer[i][j];
            }

        for (int i = 0; i < H_IMG; i++)
            for (int j = 0; j < W_IMG; j++)
                green_layer[i][j] = tmp_layer[i][j];

        for (int i = 0; i < H_IMG; i++)
            for (int j = 0; j < W_IMG; j++)
            {
                int new_x, new_y;
                if (d_x >= 0)
                {
                    new_x = (j + d_x) % W_IMG;
                }
                else
                {
                    new_x = W_IMG - 1 - (W_IMG - 1 - j - d_x) % W_IMG;
                }

                if (d_y >= 0)
                {
                    new_y = (i + d_y) % H_IMG;
                }
                else
                {
                    new_y = H_IMG - 1 - (H_IMG - 1 - i - d_y) % H_IMG;
                }

                // Movemos
                tmp_layer[new_y][new_x] = blue_layer[i][j];
            }

        for (int i = 0; i < H_IMG; i++)
            for (int j = 0; j < W_IMG; j++)
                blue_layer[i][j] = tmp_layer[i][j];
    }

    /* 
     * Realiza una rotación contra las agujas del reloj si se pone true y se hace una rotación
     * hacia las agujas del reloj si se pone false. El argumento *is_undo* indica si la rotación
     * se está haciendo con fines de transformar la imagen o para "eliminar" una rotación ya hecha
     * con anterioridad.
     */
    void rotation(bool is_undo)
    {
        /*
         * Los if's determinan el orden en que se transpone-refleja la matriz, ya que
         * dependiendo del orden la rotación se hace en contra o hacia las agujas del reloj
         */

        /* transponemos las matrices */
        if (is_undo == false)
        {
            for (int i = 0; i < H_IMG; i++)
                for (int j = i + 1; j < W_IMG; j++)
                    std::swap(red_layer[i][j], red_layer[j][i]);

            for (int i = 0; i < H_IMG; i++)
                for (int j = i + 1; j < W_IMG; j++)
                    std::swap(blue_layer[i][j], blue_layer[j][i]);

            for (int i = 0; i < H_IMG; i++)
                for (int j = i + 1; j < W_IMG; j++)
                    std::swap(green_layer[i][j], green_layer[j][i]);
        }

        /* reflejamos las matrices horizontalmente */
        for (int i = 0; i < H_IMG / 2; i++)
            for (int j = 0; j < W_IMG; j++)
                std::swap(red_layer[i][j], red_layer[H_IMG - i - 1][j]);

        for (int i = 0; i < H_IMG / 2; i++)
            for (int j = 0; j < W_IMG; j++)
                std::swap(blue_layer[i][j], blue_layer[H_IMG - i - 1][j]);

        for (int i = 0; i < H_IMG / 2; i++)
            for (int j = 0; j < W_IMG; j++)
                std::swap(green_layer[i][j], green_layer[H_IMG - i - 1][j]);

        /* transponemos las matrices */
        if (is_undo == true)
        {
            for (int i = 0; i < H_IMG; i++)
                for (int j = i + 1; j < W_IMG; j++)
                    std::swap(red_layer[i][j], red_layer[j][i]);

            for (int i = 0; i < H_IMG; i++)
                for (int j = i + 1; j < W_IMG; j++)
                    std::swap(blue_layer[i][j], blue_layer[j][i]);

            for (int i = 0; i < H_IMG; i++)
                for (int j = i + 1; j < W_IMG; j++)
                    std::swap(green_layer[i][j], green_layer[j][i]);
        }
    }

public:
    /* Constructor de la imagen. Se crea una imagen por defecto */
    moving_image()
    {
        /* Reserva de memoria para las 3 matrices RGB */
        red_layer = new unsigned char *[H_IMG];
        green_layer = new unsigned char *[H_IMG];
        blue_layer = new unsigned char *[H_IMG];

        for (int i = 0; i < H_IMG; i++)
        {
            red_layer[i] = new unsigned char[W_IMG];
            green_layer[i] = new unsigned char[W_IMG];
            blue_layer[i] = new unsigned char[W_IMG];
        }

        /* Llenamos la imagen con su color de fondo */
        for (int i = 0; i < H_IMG; i++)
            for (int j = 0; j < W_IMG; j++)
            {
                red_layer[i][j] = DEFAULT_R;
                green_layer[i][j] = DEFAULT_G;
                blue_layer[i][j] = DEFAULT_B;
            }

        /* Dibujamos el objeto en su posición inicial */
        for (int i = 0; i < 322; i++)
            for (int j = 0; j < 256; j++)
            {
                if (!s_R[i][j] && !s_G[i][j] && !s_B[i][j])
                {
                    red_layer[INIT_Y + i][INIT_X + j] = DEFAULT_R;
                    green_layer[INIT_Y + i][INIT_X + j] = DEFAULT_G;
                    blue_layer[INIT_Y + i][INIT_X + j] = DEFAULT_B;
                }
                else
                {
                    red_layer[INIT_Y + i][INIT_X + j] = s_R[i][j];
                    green_layer[INIT_Y + i][INIT_X + j] = s_G[i][j];
                    blue_layer[INIT_Y + i][INIT_X + j] = s_B[i][j];
                }
            }
    }

    /* Destructor de la clase */
    ~moving_image()
    {
        for (int i = 0; i < H_IMG; i++)
        {
            delete red_layer[i];
            delete green_layer[i];
            delete blue_layer[i];
        }

        delete red_layer;
        delete green_layer;
        delete blue_layer;
    }

    /* Función utilizada para guardar la imagen en formato .png */
    void draw(const char *nb)
    {
        _draw(nb);
    }

    /* 
     * Función que desplaza la imagen, de manera circular, d pixeles a la izquierda. Esta
     * función es una interfaz para el usuario y llama a la función privada move(), que es general.
     * Adicionalmente, hace los cambios necesarios en el stack.
     */
    void move_left(int d)
    {
        this->move(-1 * d, 0);

        history_stack.push(Movement(MOVE_LEFT, d));

        all_movements.push(Movement(MOVE_LEFT, d));

        /* No es optimo ajajaja */
        while (!undo_stack.empty())
        {
            undo_stack.pop();
        }

    }

    /* 
     * Función que desplaza la imagen, de manera circular, d pixeles a la derecha. Esta
     * función es una interfaz para el usuario y llama a la función privada move(), que es general.
     * Adicionalmente, hace los cambios necesarios en el stack.
     */
    void move_right(int d)
    {

        this->move(d, 0);
        history_stack.push(Movement(MOVE_RIGHT, d));

        all_movements.push(Movement(MOVE_RIGHT, d));

        while (!undo_stack.empty())
        {
            undo_stack.pop();
        }

    }

    /* 
     * Función que desplaza la imagen, de manera circular, d pixeles hacia abajo. Esta
     * función es una interfaz para el usuario y llama a la función privada move(), que es general.
     * Adicionalmente, hace los cambios necesarios en el stack.
     */
    void move_down(int d)
    {

        this->move(0, d);

        history_stack.push(Movement(MOVE_DOWN, d));

        all_movements.push(Movement(MOVE_DOWN, d));

        while (!undo_stack.empty())
        {
            undo_stack.pop();
        }
    }

    /* 
     * Función que desplaza la imagen, de manera circular, d pixeles hacia arriba. Esta
     * función es una interfaz para el usuario y llama a la función privada move(), que es general.
     * Adicionalmente, hace los cambios necesarios en el stack.
     */
    void move_up(int d)
    {
        this->move(0, -1 * d);

        history_stack.push(Movement(MOVE_UP, d));

        all_movements.push(Movement(MOVE_UP, d));

        while (!undo_stack.empty())
        {
            undo_stack.pop();
        }

    }

    /* Rota +90 grados */
    void rotate()
    {
        rotation(false);
        history_stack.push(Movement(ROTATE, 0));

        all_movements.push(Movement(ROTATE, 0));

        while (!undo_stack.empty())
        {
            undo_stack.pop();
        }

    }


    void undo()
    {

        /* Si no se puede hacer undo, simplemente no se hace nada (similar a software de dibujo) */
        if (!history_stack.empty())
        {
            /* Removemos el movimiento del historial */
            Movement last_movement(history_stack.top());
            history_stack.pop();

            /* Lo dejamos en el stack de undo. */
            undo_stack.push(last_movement);

            /* Calculamos el movimiento inverso */
            Command inverse_command = last_movement.inv_command;
            Movement inverse_movement = Movement(inverse_command, last_movement.arg);

            /* Tener en cuenta que esto no deja el movimiento inverso en el stack historial. */
            move_from_movement(inverse_movement);

            all_movements.push(inverse_movement);
        }
    }

    void redo()
    {
        /* Si no se puede hacer redo, simplemente no se hace nada (similar a software de dibujo) */
        if (!undo_stack.empty())
        {
            /* Removemos el movimiento del historial de stack */
            Movement last_undo(undo_stack.top());
            undo_stack.pop();

            /* Lo devolvemos al historial de movimientos */
            history_stack.push(last_undo);
          
            /* Tener en cuenta que esto no deja el movimiento en el stack historial. */
            move_from_movement(last_undo);

            all_movements.push(last_undo);
        }
    }

    void repeat()
    {
        /* Si no se puede hacer repeat, simplemente no se hace nada (similar a software de dibujo) */
        if (!history_stack.empty())
        {
            Movement last_movement(history_stack.top());

            move_from_movement(last_movement);

            all_movements.push(last_movement);
        }
    }

    void repeat_all()
    {
        moving_image img;
        std::queue<Movement> movements(all_movements);
        img.draw("im1.png");

        /* Dejara las imagenes en un formato im1.png, im2.png im3.png... */
        int i = 2;
        while (!movements.empty())
        {
            img.move_from_movement(movements.front());
            movements.pop();
            std::string name = "im" + std::to_string(i++) + ".png";
            img.draw(name.c_str());
        }
    }

    /* Imprime en la terminal los stacks, solo para debug */
    void print_stacks()
    {
        std::stack<Movement> historial = this->history_stack;
        std::stack<Movement> undos = this->undo_stack;
        std::queue<Movement> repeat_all = this->all_movements;

        std::cout
            << "HISTORIAL" << std::endl;
        while (!historial.empty())
        {
            std::cout << historial.top().to_string() << " => ";
            historial.pop();
        }
        std::cout << "NULL";

        std::cout << std::endl
                  << "UNDOS" << std::endl;
        while (!undos.empty())
        {
            std::cout << undos.top().to_string() << " => ";
            undos.pop();
        }
        std::cout << "NULL" << std::endl;

        std::cout << "REPEAT ALL" << std::endl;
        while (!repeat_all.empty())
        {
            std::cout << repeat_all.front().to_string() << " => ";
            repeat_all.pop();
        }
        std::cout << "NULL" << std::endl;
    }

private:
    /* Función privada que guarda la imagen en formato .png */
    void _draw(const char *nb)
    {

        unsigned char rgb[H_IMG * W_IMG * 3], *p = rgb;
        unsigned x, y;

        /* La imagen resultante tendrá el nombre dado por la variable 'nb' */
        FILE *fp = fopen(nb, "wb");

        /* Transformamos las 3 matrices en una arreglo unidimensional */
        for (y = 0; y < H_IMG; y++)
            for (x = 0; x < W_IMG; x++)

            {
                *p++ = red_layer[y][x];   /* R */
                *p++ = green_layer[y][x]; /* G */
                *p++ = blue_layer[y][x];  /* B */
            }
        /* La función svpng() transforma las 3 matrices RGB en una imagen PNG */
        svpng(fp, W_IMG, H_IMG, rgb, 0);
        fclose(fp);
    }
};

#endif
