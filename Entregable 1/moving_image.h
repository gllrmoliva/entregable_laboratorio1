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
        unsigned char tmp_layer[H_IMG][W_IMG];

        // Mover la capa roja
        for (int i = 0; i < H_IMG; i++)
            for (int j = 0; j < W_IMG - d; j++)
                tmp_layer[i][j] = red_layer[i][j + d];

        for (int i = 0; i < H_IMG; i++)
            for (int j = W_IMG - d, k = 0; j < W_IMG; j++, k++)
                tmp_layer[i][j] = red_layer[i][k];

        for (int i = 0; i < H_IMG; i++)
            for (int j = 0; j < W_IMG; j++)
                red_layer[i][j] = tmp_layer[i][j];

        // Mover la capa verde
        for (int i = 0; i < H_IMG; i++)
            for (int j = 0; j < W_IMG - d; j++)
                tmp_layer[i][j] = green_layer[i][j + d];

        for (int i = 0; i < H_IMG; i++)
            for (int j = W_IMG - d, k = 0; j < W_IMG; j++, k++)
                tmp_layer[i][j] = green_layer[i][k];

        for (int i = 0; i < H_IMG; i++)
            for (int j = 0; j < W_IMG; j++)
                green_layer[i][j] = tmp_layer[i][j];

        // Mover la capa azul
        for (int i = 0; i < H_IMG; i++)
            for (int j = 0; j < W_IMG - d; j++)
                tmp_layer[i][j] = blue_layer[i][j + d];

        for (int i = 0; i < H_IMG; i++)
            for (int j = W_IMG - d, k = 0; j < W_IMG; j++, k++)
                tmp_layer[i][j] = blue_layer[i][k];

        for (int i = 0; i < H_IMG; i++)
            for (int j = 0; j < W_IMG; j++)
                blue_layer[i][j] = tmp_layer[i][j];

        history_stack.push(Movement(MOVE_LEFT, d));

        // Por ahora solo voy a eliminar con pop los elementos de stack, aunque esto no es lo mas optimo
        while (!undo_stack.empty())
        {
            undo_stack.pop();
        }
    }

    // Función que similar desplazar la imagen, de manera circular, d pixeles a la derecha
    void move_right(int d)
    {

        unsigned char tmp_layer[H_IMG][W_IMG];

        // Mover la capa roja hacia la derecha
        for (int i = 0; i < H_IMG; i++)
            for (int j = d; j < W_IMG; j++)
                tmp_layer[i][j] = red_layer[i][j - d];

        for (int i = 0; i < H_IMG; i++)
            for (int j = 0, k = W_IMG - d; j < d; j++, k++)
                tmp_layer[i][j] = red_layer[i][k];

        for (int i = 0; i < H_IMG; i++)
            for (int j = 0; j < W_IMG; j++)
                red_layer[i][j] = tmp_layer[i][j];

        // Mover la capa verde hacia la derecha
        for (int i = 0; i < H_IMG; i++)
            for (int j = d; j < W_IMG; j++)
                tmp_layer[i][j] = green_layer[i][j - d];

        for (int i = 0; i < H_IMG; i++)
            for (int j = 0, k = W_IMG - d; j < d; j++, k++)
                tmp_layer[i][j] = green_layer[i][k];

        for (int i = 0; i < H_IMG; i++)
            for (int j = 0; j < W_IMG; j++)
                green_layer[i][j] = tmp_layer[i][j];

        // Mover la capa azul hacia la derecha
        for (int i = 0; i < H_IMG; i++)
            for (int j = d; j < W_IMG; j++)
                tmp_layer[i][j] = blue_layer[i][j - d];

        for (int i = 0; i < H_IMG; i++)
            for (int j = 0, k = W_IMG - d; j < d; j++, k++)
                tmp_layer[i][j] = blue_layer[i][k];

        for (int i = 0; i < H_IMG; i++)
            for (int j = 0; j < W_IMG; j++)
                blue_layer[i][j] = tmp_layer[i][j];
        history_stack.push(Movement(MOVE_RIGHT, d));

        // Por ahora solo voy a eliminar con pop los elementos de stack, aunque esto no es lo mas optimo
        while (!undo_stack.empty())
        {
            undo_stack.pop();
        }
    }

    // Función que similar desplazar la imagen, de manera circular, d pixeles hacia abajo
    void move_down(int d)
    {
        unsigned char tmp_layer[H_IMG][W_IMG];
        for (int i = 0; i < H_IMG; i++)
            for (int j = 0; j < W_IMG; j++)
                tmp_layer[(i + d) % H_IMG][j] = red_layer[i][j];

        for (int i = 0; i < H_IMG; i++)
            for (int j = 0; j < W_IMG; j++)
                red_layer[i][j] = tmp_layer[i][j];

        for (int i = 0; i < H_IMG; i++)
            for (int j = 0; j < W_IMG; j++)
                tmp_layer[(i + d) % H_IMG][j] = green_layer[i][j];

        for (int i = 0; i < H_IMG; i++)
            for (int j = 0; j < W_IMG; j++)
                green_layer[i][j] = tmp_layer[i][j];

        for (int i = 0; i < H_IMG; i++)
            for (int j = 0; j < W_IMG; j++)
                tmp_layer[(i + d) % H_IMG][j] = blue_layer[i][j];

        for (int i = 0; i < H_IMG; i++)
            for (int j = 0; j < W_IMG; j++)
                blue_layer[i][j] = tmp_layer[i][j];
        history_stack.push(Movement(MOVE_DOWN, d));

        // Por ahora solo voy a eliminar con pop los elementos de stack, aunque esto no es lo mas optimo
        while (!undo_stack.empty())
        {
            undo_stack.pop();
        }
    }

    // Función que similar desplazar la imagen, de manera circular, d pixeles arriba
    void move_up(int d)
    {
        unsigned char tmp_layer[H_IMG][W_IMG];

        for (int i = 0; i < H_IMG; i++)
            for (int j = 0; j < W_IMG; j++)
                tmp_layer[i][j] = red_layer[(i + d) % H_IMG][j];

        for (int i = 0; i < H_IMG; i++)
            for (int j = 0; j < W_IMG; j++)
                red_layer[i][j] = tmp_layer[i][j];

        for (int i = 0; i < H_IMG; i++)
            for (int j = 0; j < W_IMG; j++)
                tmp_layer[i][j] = green_layer[(i + d) % H_IMG][j];

        for (int i = 0; i < H_IMG; i++)
            for (int j = 0; j < W_IMG; j++)
                green_layer[i][j] = tmp_layer[i][j];

        for (int i = 0; i < H_IMG; i++)
            for (int j = 0; j < W_IMG; j++)
                tmp_layer[i][j] = blue_layer[(i + d) % H_IMG][j];

        for (int i = 0; i < H_IMG; i++)
            for (int j = 0; j < W_IMG; j++)
                blue_layer[i][j] = tmp_layer[i][j];

        history_stack.push(Movement(MOVE_UP, d));

        // Por ahora solo voy a eliminar con pop los elementos de stack, aunque esto no es lo mas optimo
        while (!undo_stack.empty())
        {
            undo_stack.pop();
        }
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
        return;
    }

    void redo()
    {
        return;
    }

    // todo:  vamos a tener que pensar el caso undo() -> repeat() -> repeat()
    // imagino que en este caso se deberia hacer undo 3 veces ¿?
    // quizas añadir un bit is repeated??? en command, con eso creo que se puede solucionar pero no se
    // si seria sobreingenieria para algo mas simple
    void repeat()
    {
        Movement last_movement(history_stack.top());

        switch (last_movement.command)
        {
        case MOVE_LEFT:
            move_left(last_movement.arg);
            break;
        case MOVE_RIGHT:
            move_right(last_movement.arg);
            break;
        case MOVE_UP:
            move_up(last_movement.arg);
            break;
        case MOVE_DOWN:
            move_down(last_movement.arg);
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

        std::cout << std::endl
                  << "UNDOS" << std::endl;
        while (!historial.empty())
        {
            std::cout << historial.top().to_string() << " => ";
            historial.pop();
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
