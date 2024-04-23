/* Compilación: g++ -g -o img test.cpp
 * Ejecución: ./img
 *
 * Luego de la ejecución se generarán 3 imagines: imagen.png,
 * imagen_desplazada_500.png e imagen_desplazada_600.png
 */

#include "moving_image.h"
#include "command.h"
#include <unistd.h> // para sleep (linux). Usar  #include<windows.h> para Windows

int main()
{
    moving_image im;
    im.rotate();
    im.rotate();
    im.undo();
    im.redo();
    im.move_right(500);
    im.move_down(100);
    im.repeat();
    /* NOTA 1: Si usan el mismo nombre para las imágenes, entonces cada llamada al
    método draw() sobreescribirá a la imagen */

    /* NOTA 2: Si usan un ciclo while(1) y usan el mismo nombre de imagen,
    entonces se puede generar un efecto de desplazamiento continuo (algo así como
    un video =D ) */
    return 0;
}
