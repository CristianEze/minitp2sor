# minitp2sor
comencé creando un pseudocodigo que constaba de 8 funciones donde tenemos
-cortar-mezclar-salar-armarMedallon-cortarLT-cocinar-hornear-armarHamburguesas
donde "mezclar" debia esperar "cortar" referido a los ingredientes.
"salar" debia esperar a "mezclar"  par mas tarde poder ejecutar "armarMedallon"
luego viene la funcion de "cocinar" e independientemente las funciones de "cortarLT" que se refiere a cortar lechuga y tomate, "hornear" referido a la preparacion del pan.
Por ultimo la funcion "armarHamburgesas" que debe esperar que todas las funciones anterioroes se lleven a cabo.
en las funciones donde no podian ser compartidas utilice un "mutex" simulando la falta fisica de los productos para llevar a cabo dichas acciones, estas funciones fueron"salar","hornear" y "cocinar"
