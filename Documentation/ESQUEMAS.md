# KeyToLCDEMAIL2

La idea es conectar una LCD 16x2 en un MEGA2560 y un PS/2.

Las fotos del conexionado PS/2 están en este proyecto.
Yo he usado un conversor de tipo USB a PS/2 o viceversa, sin lógica, solo el conexionado y el formato del conector.

Por ende, el actual documento de comandos aquí en el directorio de este proyecto es SIM800L y varias cositas.
Pero la idea es sumar ESP8266 y banda 433Mhz.

¿Como formateamos los datos? KEEP READING.

Por el momento no tengo solucionado el tema pues los mensajes entre muchos miembros necesitan ser marcados, organizados y estructurados para que se entienda de donde vienen.
En resumen: hace falta un <<standard>> y yo no conozco el de SOS porque me he especializado en buscar soluciones a momentos de crisis y emergencia individual, familiar o colectiva. Pero nunca lo he puesto en práctica. Como consejo decir: siempre usa el dispositivo que tengas más a mano y consideres seguro. Sencillamente. Y échale valía, qué la vida lo vale y la muerte llega y después el sufrimiento se acaba. Y eso, es la realidad. Ni más, ni menos. Pero debes abrazar la muerte como un regalo por toda una vida de sentido, dirección y rumbo... sufrir en algunos o más momentos y haber llegado aquí sin haberlo intentado.

Propongo hablar del tema. Sobre definir un standard de SOS para casos de apocalipsis, colapso o pandemia internacional, etc...
es decir, para el kaos.

En resumen, este <<sketch>> se usa básicamente enviando comandos AT al SIM800L o el que sea (comunicación <<Serial>>) o ESP8266 o el que sea, con el teclado PS/2 y pudiendo leer y escribir en la LCD.

NOTA: la LCD guarda el buffer que envías, he tenido que implementar el código para que lea el buffer de la LCD y ahorrar memoria, cómputo y tiempo. Por eso, pulsa ESC antes de cada comando o después de leer la respuesta. 
Además usa Shift+ENTER para enviar el comando.

El layout está en "ES" si quieres otro, trata de configurar o programar el tuyo.

Mucha suerte y que la fuerza te acompañe.

-Abel Romero Pérez.
