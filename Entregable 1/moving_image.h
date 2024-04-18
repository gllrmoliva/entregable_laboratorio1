#ifndef MOVING_IMG_H
#define MOVING_IMG_H

#include "basics.h"
#include <iostream>

// Clase que representa una imagen como una colección de 3 matrices siguiendo el
// esquema de colores RGB

class moving_image
{
private:
    unsigned char **red_layer;   // Capa de tonalidades rojas
    unsigned char **green_layer; // Capa de tonalidades verdes
    unsigned char **blue_layer;  // Capa de tonalidades azules

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
    }

    //--------------- DESDE AQUÍ EN ADELANTE HAY FUNCIONES HECHAS PARA LA TAREA ---------------

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
