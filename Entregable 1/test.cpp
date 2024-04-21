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
    im.print_stacks();
    std::cout << std::endl;
    im.draw("image1.png");

    im.rotate();
    im.print_stacks();
    std::cout << std::endl;
    im.draw("image2.png");

    im.undorotate();
    im.print_stacks();
    std::cout << std::endl;
    im.draw("image3.png");

    im.move_right(100);
    im.print_stacks();
    std::cout << std::endl;
    im.draw("image4.png");

    im.undo();
    im.print_stacks();
    std::cout << std::endl;
    im.draw("image5.png");

    im.undo();
    im.print_stacks();
    std::cout << std::endl;
    im.draw("image6.png");

    im.redo();
    im.print_stacks();
    std::cout << std::endl;
    im.draw("image7.png");

    im.move_left(243);
    im.print_stacks();
    std::cout << std::endl;
    im.draw("image8.png");

    /* NOTA 1: Si usan el mismo nombre para las imágenes, entonces cada llamada al
    método draw() sobreescribirá a la imagen */

    /* NOTA 2: Si usan un ciclo while(1) y usan el mismo nombre de imagen,
    entonces se puede generar un efecto de desplazamiento continuo (algo así como
    un video =D ) */
    return 0;
}
