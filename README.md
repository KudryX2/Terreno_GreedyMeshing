# Terreno_GreedyMeshing
Proyecto personal antiguo, GLUT y C++

Crea un terreno de voxels, los combina en bloques mas grandes creando una malla que es la que se dibuja finalmente.
Al destruir o construir un bloque la malla a renderizar se vuelve a generar para los bloques cercanos. Con esto se 
consigue reducir notablemente la carga de la GPU , (gracias a ello puede funcionar en un portatil de hace +10 años).

Para algunas cosas se utiliza la siguiente libreria : https://www.sfml-dev.org/
