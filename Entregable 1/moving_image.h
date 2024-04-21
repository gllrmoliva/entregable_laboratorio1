#ifndef MOVING_IMG_H
#define MOVING_IMG_H

#include "basics.h"
#include <iostream>

#include <stack>
#include <queue>
#include "command.h"

// Clase que representa una imagen como una colección de 3 matrices siguiendo el
// esquema de colores RGB

class moving_image
{
private:
    unsigned char **red_layer;   // Capa de tonalidades rojas
    unsigned char **green_layer; // Capa de tonalidades verdes
    unsigned char **blue_layer;  // Capa de tonalidades azules
    std::stack<Movement> history_stack;
    std::stack<Movement> undo_stack;

    std::queue<Movement> all_movements; // Todavía no se muy bien que vamos a hacer con este


    
    // Función para realizar un movimiento en base al struct de Movement.
    // IMPORTANTE: esto no es una interfaz para el usuario, por lo que llama
    // a la función privada move(), y no realizará cambios a los stacks de historial.
    void move_from_movement(Movement movement) {
        Command cmd = movement.command;
        int arg = movement.arg;
        switch (cmd)
        {
        case MOVE_LEFT:
            // Un mov a la izquierda es traslacion eje x negativa.
            move(-1*arg, 0);
            break;
        case MOVE_RIGHT:
            // Un mov a la derecha es traslacion eje x positiva.
            move(arg, 0);
            break;
        case MOVE_UP:
            // Un mov hacia arriba es traslacion eje y negativa.
            move(0, -1*arg);
            break;
        case MOVE_DOWN:
            // Un mov hacia abajo es traslacion eje y positiva.
            move(0, arg);
            break;
        case ROTATE:
            rotate();
            break;
        case UNDO_ROTATE:
            undorotate();
            break;

        default:
            break;
        }
    }

    // Función de movimiento, recordar que el eje x es positivo a la derecha, eje y hacia abajo.
    void move(int d_x, int d_y) {
        unsigned char tmp_layer[H_IMG][W_IMG];
        for (int i = 0; i < H_IMG; i++)
            for (int j = 0; j < W_IMG; j++) {
                int new_x, new_y;
                if (d_x >= 0) {
                    new_x = (j + d_x) % W_IMG;
                } else {
                    new_x = W_IMG - 1 - (W_IMG - 1 - j - d_x) % W_IMG;
                }

                if (d_y >= 0) {
                    new_y = (i + d_y) % H_IMG;
                } else {
                    new_y = H_IMG - 1 - (H_IMG - 1 - i - d_y) % H_IMG;
                }

                // Movemos
                tmp_layer[new_y][new_x] = red_layer[i][j];
            }

        for (int i = 0; i < H_IMG; i++)
            for (int j = 0; j < W_IMG; j++)
                red_layer[i][j] = tmp_layer[i][j];

        for (int i = 0; i < H_IMG; i++)
            for (int j = 0; j < W_IMG; j++) {
                int new_x, new_y;
                if (d_x >= 0) {
                    new_x = (j + d_x) % W_IMG;
                } else {
                    new_x = W_IMG - 1 - (W_IMG - 1 - j - d_x) % W_IMG;
                }

                if (d_y >= 0) {
                    new_y = (i + d_y) % H_IMG;
                } else {
                    new_y = H_IMG - 1 - (H_IMG - 1 - i - d_y) % H_IMG;
                }

                // Movemos
                tmp_layer[new_y][new_x] = green_layer[i][j];
            }

        for (int i = 0; i < H_IMG; i++)
            for (int j = 0; j < W_IMG; j++)
                green_layer[i][j] = tmp_layer[i][j];

        for (int i = 0; i < H_IMG; i++)
            for (int j = 0; j < W_IMG; j++) {
                int new_x, new_y;
                if (d_x >= 0) {
                    new_x = (j + d_x) % W_IMG;
                } else {
                    new_x = W_IMG - 1 - (W_IMG - 1 - j - d_x) % W_IMG;
                }

                if (d_y >= 0) {
                    new_y = (i + d_y) % H_IMG;
                } else {
                    new_y = H_IMG - 1 - (H_IMG - 1 - i - d_y) % H_IMG;
                }

                // Movemos
                tmp_layer[new_y][new_x] = blue_layer[i][j];
            }

        for (int i = 0; i < H_IMG; i++)
            for (int j = 0; j < W_IMG; j++)
                blue_layer[i][j] = tmp_layer[i][j];
    }

public:
    // Constructor de la imagen. Se crea una imagen por defecto
    moving_image()
    {
        // Reserva de memoria para las 3 matrices RGB
        red_layer = new unsigned char *[H_IMG];
        green_layer = new unsigned char *[H_IMG];
        blue_layer = new unsigned char *[H_IMG];

        for (int i = 0; i < H_IMG; i++)
        {
            red_layer[i] = new unsigned char[W_IMG];
            green_layer[i] = new unsigned char[W_IMG];
            blue_layer[i] = new unsigned char[W_IMG];
        }

        // Llenamos la imagen con su color de fondo
        for (int i = 0; i < H_IMG; i++)
            for (int j = 0; j < W_IMG; j++)
            {
                red_layer[i][j] = DEFAULT_R;
                green_layer[i][j] = DEFAULT_G;
                blue_layer[i][j] = DEFAULT_B;
            }

        // Dibujamos el objeto en su posición inicial
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

    // Destructor de la clase
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

    // Función utilizada para guardar la imagen en formato .png
    void draw(const char *nb)
    {
        _draw(nb);
    }

    


    // Función que similar desplazar la imagen, de manera circular, d pixeles a la izquierda
    void move_left(int d)
    {
        this->move(-1*d, 0);

        history_stack.push(Movement(MOVE_LEFT, d));

        // Por ahora solo voy a eliminar con pop los elementos de stack, aunque esto no es lo mas optimo
        while (!undo_stack.empty())
        {
            undo_stack.pop();
        }

        // unsigned char tmp_layer[H_IMG][W_IMG];

        // // Mover la capa roja
        // for (int i = 0; i < H_IMG; i++)
        //     for (int j = 0; j < W_IMG - d; j++)
        //         tmp_layer[i][j] = red_layer[i][j + d];

        // for (int i = 0; i < H_IMG; i++)
        //     for (int j = W_IMG - d, k = 0; j < W_IMG; j++, k++)
        //         tmp_layer[i][j] = red_layer[i][k];

        // for (int i = 0; i < H_IMG; i++)
        //     for (int j = 0; j < W_IMG; j++)
        //         red_layer[i][j] = tmp_layer[i][j];

        // // Mover la capa verde
        // for (int i = 0; i < H_IMG; i++)
        //     for (int j = 0; j < W_IMG - d; j++)
        //         tmp_layer[i][j] = green_layer[i][j + d];

        // for (int i = 0; i < H_IMG; i++)
        //     for (int j = W_IMG - d, k = 0; j < W_IMG; j++, k++)
        //         tmp_layer[i][j] = green_layer[i][k];

        // for (int i = 0; i < H_IMG; i++)
        //     for (int j = 0; j < W_IMG; j++)
        //         green_layer[i][j] = tmp_layer[i][j];

        // // Mover la capa azul
        // for (int i = 0; i < H_IMG; i++)
        //     for (int j = 0; j < W_IMG - d; j++)
        //         tmp_layer[i][j] = blue_layer[i][j + d];

        // for (int i = 0; i < H_IMG; i++)
        //     for (int j = W_IMG - d, k = 0; j < W_IMG; j++, k++)
        //         tmp_layer[i][j] = blue_layer[i][k];

        // for (int i = 0; i < H_IMG; i++)
        //     for (int j = 0; j < W_IMG; j++)
        //         blue_layer[i][j] = tmp_layer[i][j];

        
    }

    // Función que similar desplazar la imagen, de manera circular, d pixeles a la derecha
    void move_right(int d)
    {

        this->move(d, 0);
        history_stack.push(Movement(MOVE_RIGHT, d));

        // Por ahora solo voy a eliminar con pop los elementos de stack, aunque esto no es lo mas optimo
        while (!undo_stack.empty())
        {
            undo_stack.pop();
        }

        // unsigned char tmp_layer[H_IMG][W_IMG];

        // // Mover la capa roja hacia la derecha
        // for (int i = 0; i < H_IMG; i++)
        //     for (int j = d; j < W_IMG; j++)
        //         tmp_layer[i][j] = red_layer[i][j - d];

        // for (int i = 0; i < H_IMG; i++)
        //     for (int j = 0, k = W_IMG - d; j < d; j++, k++)
        //         tmp_layer[i][j] = red_layer[i][k];

        // for (int i = 0; i < H_IMG; i++)
        //     for (int j = 0; j < W_IMG; j++)
        //         red_layer[i][j] = tmp_layer[i][j];

        // // Mover la capa verde hacia la derecha
        // for (int i = 0; i < H_IMG; i++)
        //     for (int j = d; j < W_IMG; j++)
        //         tmp_layer[i][j] = green_layer[i][j - d];

        // for (int i = 0; i < H_IMG; i++)
        //     for (int j = 0, k = W_IMG - d; j < d; j++, k++)
        //         tmp_layer[i][j] = green_layer[i][k];

        // for (int i = 0; i < H_IMG; i++)
        //     for (int j = 0; j < W_IMG; j++)
        //         green_layer[i][j] = tmp_layer[i][j];

        // // Mover la capa azul hacia la derecha
        // for (int i = 0; i < H_IMG; i++)
        //     for (int j = d; j < W_IMG; j++)
        //         tmp_layer[i][j] = blue_layer[i][j - d];

        // for (int i = 0; i < H_IMG; i++)
        //     for (int j = 0, k = W_IMG - d; j < d; j++, k++)
        //         tmp_layer[i][j] = blue_layer[i][k];

        // for (int i = 0; i < H_IMG; i++)
        //     for (int j = 0; j < W_IMG; j++)
        //         blue_layer[i][j] = tmp_layer[i][j];
        
    }

    // Función que similar desplazar la imagen, de manera circular, d pixeles hacia abajo
    void move_down(int d)
    {

        this->move(0, d);
        history_stack.push(Movement(MOVE_DOWN, d));

        // Por ahora solo voy a eliminar con pop los elementos de stack, aunque esto no es lo mas optimo
        while (!undo_stack.empty())
        {
            undo_stack.pop();
        }

        // unsigned char tmp_layer[H_IMG][W_IMG];
        // for (int i = 0; i < H_IMG; i++)
        //     for (int j = 0; j < W_IMG; j++)
        //         tmp_layer[(i + d) % H_IMG][j] = red_layer[i][j];

        // for (int i = 0; i < H_IMG; i++)
        //     for (int j = 0; j < W_IMG; j++)
        //         red_layer[i][j] = tmp_layer[i][j];

        // for (int i = 0; i < H_IMG; i++)
        //     for (int j = 0; j < W_IMG; j++)
        //         tmp_layer[(i + d) % H_IMG][j] = green_layer[i][j];

        // for (int i = 0; i < H_IMG; i++)
        //     for (int j = 0; j < W_IMG; j++)
        //         green_layer[i][j] = tmp_layer[i][j];

        // for (int i = 0; i < H_IMG; i++)
        //     for (int j = 0; j < W_IMG; j++)
        //         tmp_layer[(i + d) % H_IMG][j] = blue_layer[i][j];

        // for (int i = 0; i < H_IMG; i++)
        //     for (int j = 0; j < W_IMG; j++)
        //         blue_layer[i][j] = tmp_layer[i][j];
        
    }

    // Función que similar desplazar la imagen, de manera circular, d pixeles arriba
    void move_up(int d)
    {
        this->move(0, -1*d);
        history_stack.push(Movement(MOVE_UP, d));

        // Por ahora solo voy a eliminar con pop los elementos de stack, aunque esto no es lo mas optimo
        while (!undo_stack.empty())
        {
            undo_stack.pop();
        }

        // for (int i = 0; i < H_IMG; i++)
        //     for (int j = 0; j < W_IMG; j++)
        //         tmp_layer[i][j] = red_layer[(i + d) % H_IMG][j];

        // for (int i = 0; i < H_IMG; i++)
        //     for (int j = 0; j < W_IMG; j++)
        //         red_layer[i][j] = tmp_layer[i][j];

        // for (int i = 0; i < H_IMG; i++)
        //     for (int j = 0; j < W_IMG; j++)
        //         tmp_layer[i][j] = green_layer[(i + d) % H_IMG][j];

        // for (int i = 0; i < H_IMG; i++)
        //     for (int j = 0; j < W_IMG; j++)
        //         green_layer[i][j] = tmp_layer[i][j];

        // for (int i = 0; i < H_IMG; i++)
        //     for (int j = 0; j < W_IMG; j++)
        //         tmp_layer[i][j] = blue_layer[(i + d) % H_IMG][j];

        // for (int i = 0; i < H_IMG; i++)
        //     for (int j = 0; j < W_IMG; j++)
        //         blue_layer[i][j] = tmp_layer[i][j];

        
    }

    // Función que similar desplazar la imagen, de manera circular, d pixeles a la derecha

    // toda +90 grados
    void rotate()
    {
        // Rotamos la capa roja
        // Transponemos la matriz
        for (int i = 0; i < H_IMG; i++)
            for (int j = i + 1; j < W_IMG; j++)
                std::swap(red_layer[i][j], red_layer[j][i]);
        // Reflejamos la matriz horizontalmente
        for (int i = 0; i < H_IMG / 2; i++)
            for (int j = 0; j < W_IMG; j++)
                std::swap(red_layer[i][j], red_layer[H_IMG - i - 1][j]);

        // Rotamos la capa azul
        for (int i = 0; i < H_IMG; i++)
            for (int j = i + 1; j < W_IMG; j++)
                std::swap(blue_layer[i][j], blue_layer[j][i]);

        for (int i = 0; i < H_IMG / 2; i++)
            for (int j = 0; j < W_IMG; j++)
                std::swap(blue_layer[i][j], blue_layer[H_IMG - i - 1][j]);

        // Rotamos la capa verde
        for (int i = 0; i < H_IMG; i++)
            for (int j = i + 1; j < W_IMG; j++)
                std::swap(green_layer[i][j], green_layer[j][i]);

        for (int i = 0; i < H_IMG / 2; i++)
            for (int j = 0; j < W_IMG; j++)
                std::swap(green_layer[i][j], green_layer[H_IMG - i - 1][j]);

        history_stack.push(Movement(ROTATE, 0));

        // Por ahora solo voy a eliminar con pop los elementos de stack, aunque esto no es lo mas optimo
        while (!undo_stack.empty())
        {
            undo_stack.pop();
        }
    }

    // todo: no me gusta mucho esto, pero es la forma mas facil de hacerlo
    // toda -90 grados
    void undorotate()
    {
        // Rotamos la capa roja
        // Reflejamos la matriz horizontalmente
        for (int i = 0; i < H_IMG / 2; i++)
            for (int j = 0; j < W_IMG; j++)
                std::swap(red_layer[i][j], red_layer[H_IMG - i - 1][j]);
        // Transponemos la matriz
        for (int i = 0; i < H_IMG; i++)
            for (int j = i + 1; j < W_IMG; j++)
                std::swap(red_layer[i][j], red_layer[j][i]);

        // Rotamos la capa azul
        for (int i = 0; i < H_IMG / 2; i++)
            for (int j = 0; j < W_IMG; j++)
                std::swap(blue_layer[i][j], blue_layer[H_IMG - i - 1][j]);

        for (int i = 0; i < H_IMG; i++)
            for (int j = i + 1; j < W_IMG; j++)
                std::swap(blue_layer[i][j], blue_layer[j][i]);

        // Rotamos la capa verde
        for (int i = 0; i < H_IMG / 2; i++)
            for (int j = 0; j < W_IMG; j++)
                std::swap(green_layer[i][j], green_layer[H_IMG - i - 1][j]);

        for (int i = 0; i < H_IMG; i++)
            for (int j = i + 1; j < W_IMG; j++)
                std::swap(green_layer[i][j], green_layer[j][i]);

        history_stack.push(Movement(UNDO_ROTATE, 0));

        // Por ahora solo voy a eliminar con pop los elementos de stack, aunque esto no es lo mas optimo
        while (!undo_stack.empty())
        {
            undo_stack.pop();
        }
    }


    void undo()
    {
        // Removemos el movimiento del historial
        Movement last_movement(history_stack.top());
        history_stack.pop();

        // Lo dejamos en el stack de undo.
        undo_stack.push(last_movement);

        // Calculamos el movimiento inverso
        Command inverse_command = last_movement.inv_command;
        Movement inverse_movement = Movement(inverse_command, last_movement.arg);
        
        /* Tener en cuenta que esto no deja el movimiento inverso en el stack historial. */
        move_from_movement(inverse_movement);
    }

    void redo()
    {
        // Removemos el movimiento del historial de stack
        Movement last_undo(undo_stack.top());
        undo_stack.pop();

        // Lo devolvemos al historial de movimientos
        history_stack.push(last_undo);

        /* Tener en cuenta que esto no deja el movimiento en el stack historial. */
        move_from_movement(last_undo); 
    }

    // todo:  vamos a tener que pensar el caso undo() -> repeat() -> repeat()
    // imagino que en este caso se deberia hacer undo 3 veces ¿?
    // quizas añadir un bit is repeated??? en command, con eso creo que se puede solucionar pero no se
    // si seria sobreingenieria para algo mas simple
    void repeat()
    {
        Movement last_movement(history_stack.top());

        move_from_movement(last_movement);
        
    }

    // Imprime en la terminal los stacks, solo para debug
    void print_stacks()
    {
        std::stack<Movement> historial(this->history_stack);
        std::stack<Movement> undos(this->undo_stack);

        std::cout << "HISTORIAL" << std::endl;
        while (!historial.empty())
        {
            std::cout << historial.top().to_string() << " => ";
            historial.pop();
        }
        std::cout << "NULL";

        std::cout << std::endl << "UNDOS" << std::endl;
        while (!undos.empty())
        {
            std::cout << undos.top().to_string() << " => ";
            undos.pop();
        }
        std::cout << "NULL" << std::endl;
    }

private:
    // Función privada que guarda la imagen en formato .png
    void _draw(const char *nb)
    {

        unsigned char rgb[H_IMG * W_IMG * 3], *p = rgb;
        unsigned x, y;

        // La imagen resultante tendrá el nombre dado por la variable 'nb'
        FILE *fp = fopen(nb, "wb");

        // Transformamos las 3 matrices en una arreglo unidimensional
        for (y = 0; y < H_IMG; y++)
            for (x = 0; x < W_IMG; x++)

            {
                *p++ = red_layer[y][x];   /* R */
                *p++ = green_layer[y][x]; /* G */
                *p++ = blue_layer[y][x];  /* B */
            }
        // La función svpng() transforma las 3 matrices RGB en una imagen PNG
        svpng(fp, W_IMG, H_IMG, rgb, 0);
        fclose(fp);
    }
};

#endif
