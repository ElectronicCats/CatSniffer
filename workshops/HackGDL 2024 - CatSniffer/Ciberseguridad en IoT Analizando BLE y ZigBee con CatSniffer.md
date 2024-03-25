# Content
- [Conociendo el Hardware](#conociendo-el-hardware)
  - [Hardware - CatSniffer](#hardware---catsniffer)
  - [Hardware - CatSniffer - Features](#hardware---catsniffer---features)
  - [Hardware - CatSniffer - Funcionamiento](#hardware---catsniffer---funcionamiento)
- [Bluetooth Low Energy (BLE)](#bluetooth-low-energy-ble)
  - [BLE - Conociendo el protocolo](#ble---conociendo-el-protocolo)
  - [BLE - Generic Access Profile (GAP)](#ble---generic-access-profile-gap)
  - [BLE - Generic Attribute Profile (GATT)](#ble---generic-attribute-profile-gatt)
- [PyCatSniffer](#pycatsniffer)
  - [PyCatSniffer - Demo - Sniffeando Conexion BLE](#pycatsniffer---demo---sniffeando-conexion-ble)
  - [PyCatSniffer - PCAP - BLE](#pycatsniffer---pcap---ble)
- [ZigBee](#zigbee)
  - [ZigBee - Arquitectura](#zigbee---arquitectura)
  - [ZigBee - Application Profiles](#zigbee---application-profiles)
  - [ZigBee - Device Descriptor](#zigbee---device-descriptor)
  - [ZigBee - Cluster Identifiers](#zigbee---cluster-identifiers)
  - [ZigBee - Network Security Model](#zigbee---network-security-model)
  - [PyCatSniffer - Demo - Sniffeando Conexion ZigBee Centralizada](#pycatsniffer---demo---sniffeando-conexion-zigbee-centralizada)
  - [PyCatSniffer - Demo - Sniffeando Conexion ZigBee Distribuida](#pycatsniffer---demo---sniffeando-conexion-zigbee-distribuida)
  - [PyCatSniffer - Demo - PCAP - Analizando la informacion](#pycatsniffer---demo---pcap---analizando-la-informacion)
- [HackGDL](#hackgdl)
- [Bibliografía](#bibliografía)



# Conociendo el Hardware

## Hardware - CatSniffer
CatSniffer es un dispositivo multiprotocolo y multibanda creado para sniffear, comunicar y atacar dispositivos IoT. Esta placa es como una navaja suiza de los investigadores, desarrolladores y entusiastas.

![[static/CatSniffer_Board.jpeg]]

## Hardware - CatSniffer - Features
CatSniffer puede operar en 3 diferentes tecnologías:
- Lora
- Sub 1 GHz
- 2.4 GHz
Protocolos soportados:
- Thread
- ZigBee
- Bluetooth 5 Low Energy (BLE)

>[!NOTE]
>Para mas informacion revisar el repositorio del [proyecto](https://github.com/ElectronicCats/CatSniffer.

## Hardware - CatSniffer - Funcionamiento

![[static/CatSniffer_BlockDriagram.png]]

# Bluetooth Low Energy (BLE)
Bluetooth Low Energy es diseñado para la operación en dispositivos de bajo consumo, su versatilidad permite crear múltiples topologías de comunicación:
- **Point-To-Point**: Es utilizada típicamente en la comunicación directa entre dos dispositivos
- **Broadcast**: Se utiliza para enviar información sin establecer una conexión previa
- **Mesh**: Todos los dispositivos están interconectados

![[static/BLE_Topology.png]]

## BLE - Conociendo el protocolo
![[BLE_Channels.png]]

Ble opera en la banda de los 2.4 GHz, el rango de operación es de 2400 MHz a 2483.5 MHz dividido en 40 canales, dentro del rango 3 canales son de *Advertising* (ADV).
- 37, 38, 39
- En estos canales los dispositivos mandan un "ping"
- Estos paquetes son conocidos como Beacons

![[static/BLE_Communication_Diagram.png]]

Los dispositivos BLE solo mandan paquetes de advertising cuando necesitan conectarse e intercambiar información, el dispositivo escucha tambien llamado *central device*, responde a un paquete de advertising con un **SCAN Request**, la respuesta al *scan request* y la misma estructura que el paquete de ADV.

##  BLE - Conociendo el protocolo - Paquete BLE

| Preamble | Access Address | Protocol Data Unit (PDU) | CRC     |
| -------- | -------------- | ------------------------ | ------- |
| 1 Byte   | 4 Byte         | 2-257 Bytes              | 3 Bytes |
**Preamble**: Permite al receptor sincronizar de manera precisa la frecuencia de la señal
**Access Address**: Es usado por el receptor para diferencia señal de ruido y determinar el paquete recibido
**CRC**: Este valor es calculado por el transmisor usando el valor de los demas bits del paquete, se utiliza para la deteccion de errores.

> El campo de **PDU** puede ser diferentes PDU's dependiendo en como el dispositivo BLE es usado.

##  BLE - Conociendo el protocolo - Paquete BLE
**Directed - Undirected**:
- El tipo Directed ADV acepta solicitudes de conexion de un dispositivo conocido.
- Undirected ADV acepta conexiones de cualquier dispositivo
**Connectable - No connectable**:
- Determina si el dispositivo ADV acepta conexiones o no.

**Legacy**: Son las unidades de datos que bluetooth tiene por defecto en cualquiere dispositivo.
- ADV_IND: Conectable, escaneable, no dirigido ADV
- ADV_DIRECT_IND: Conectable dirigido ADV
- ADV_NONCONN_IND: No conectable, no escaneable ADV
- ADV_SCAN_IND: Escaneable, no dirigido ADV

**Escaneado**:
Son las unidades de datos relacionados al proceso de intercomunicacion entre el dispositivo central y el dispositivo final.
- SCAN_REQ
- SCAN_RSP
- CONNECT_IND

![[static/Wireshark_BLE_Capture.png]]

![[static/BLE_Capture_frame.png]]

## BLE - Generic Access Profile (GAP)
El GAP define cómo se pueden conectar con otros dispositivos, comunicar y estar visibles para su descubrimiento, un dispositivo periférico solo se puede conectar a un dispositivo central, el dispositivo central se puede conectar a los dispositivos perifericos que este soporte.
Por cada conexion el dispositivo periferico envia pquetes de ADV en intervalos de tiempo.

> Todos los dipositivos BLE tienen *Generic Access Profile (GAP)*

![[static/BLE_GAP_Diagrama.png]]
## BLE - Generic Attribute Profile (GATT)
El GATT define el formato y como el dispositivo debe transferir información este nos muestra como la informacion es slmacenada, agrupada y modificada.
El GATT lista las **caracteristicas del dispositivo, descriptores y servicios**
> Los servicios son mostrados como un valor de 16 o 32 bits

Una *caracteristica* es un valor enviado entre el dispositivo central y el periferico, las caracteristicas contienen *descriptores*, estos proveen informacion adicional.

![[static/BLE_GATT_Diagrama.png]]

# PyCatSniffer
PyCatSniffer es una herramienta multiplataforma escrita en Python, compatible con el firmware *Packet Sniffer TI*, permite exportar la informacion en diferentes formatos y crear un pipeline con Wireshark.
Soporta diferentes protocolos:
- BLE
- ZigBee
- Thread

> Para mas informacion revisar el [repo](https://github.com/ElectronicCats/CatSniffer-Tools)

En Wireshar podemos ver la comunicacion entre los dispositivos Bluetooth, realizar el seguimiento cuando encuentra una conexion en el mismo canal que estamos sniffeando.
Al usar Wireshark nos permite realizar tratamiento de datos como:
- Filtrar informacion
- Colorizar conversaciones
- Exportar informacion

## PyCatSniffer - Demo - Sniffeando Conexion BLE
<iframe width="720" height="480" src="https://www.youtube.com/embed/oOcJloXW9dI?si=CUnorBsKGtlgDLta" title="YouTube video player" frameborder="0" allow="accelerometer; autoplay; clipboard-write; encrypted-media; gyroscope; picture-in-picture; web-share" referrerpolicy="strict-origin-when-cross-origin" allowfullscreen></iframe>


## PyCatSniffer - PCAP - BLE
En el siguiente PCAP podemos ver que encontramos un paquete de datos de **CONNECT_IND**, esto indica que una conexion ha sido solicitada y establecida entre un dispositivo BLE y un central device.
![[static/Wireshar_PCAP_BLE_Capture.png]]

Dentro del PCAP podemos encontrar un paquete con parametros de *Security Request*, esta paquete nos indica que el dispositivo solicitada una conexion, en la estructura de datos del frame podemos observar los siguientes datos:
- Secure Connection Flag: True
- MITM Flag: True

El MITM hace referencia a *Man-In- The-Middle*, esta flag se utiliza para securizar la conexion haciendo que el dispositivo que envia no comparta la key de emparejamiento.

> Si uno de los dos dispositivos que buscan realizar un emparejamiento, no tienen la bandera de MITM, se puede obtener la clave de emparejamiento.


![[static/Wireshar_PCAP_BLE_Capture_Flipper.png]]

# ZigBee
Zigbee es un protocolo de comunicación desarrollado por Zigbee Alliance, basado en el estándar IEEE 802.15.4. Se destaca por su eficiencia energética, lo que lo hace ideal para aplicaciones IoT donde se requiere un bajo consumo de energía. Su topología tipo Mesh permite que los dispositivos se comuniquen entre sí de manera redundante, lo que aumenta la confiabilidad y la cobertura de la red. Con la capacidad de admitir hasta 65535 nodos, Zigbee es escalable y puede adaptarse a una amplia gama de aplicaciones. Además, utiliza un sólido cifrado AES de 128 bits para garantizar la seguridad de las comunicaciones.

![[static/ZigBee_Layers.png]]

## ZigBee - Arquitectura
**PHY Layer**: Es la capa donde el transmisor de RF envia los bytes en base a la frecuencia de la region.
- Existen tres regiones:
	- Global: 2.4 GHz
	- America: 915 MHz
	- Europa: 868 MHz
- 2.4 GHz opera en 16 canales

**MAC Layer**: Es la capa encargada de asignar y administrar las conexiones de los dispositivos.

**Coordinador**: Es el dispositivo encargado de administrar la red, el crea las conexiones entre los dispositivos, intercambia las claves de red.
**Router**: Se encarga de repetir la senal de los end device al coordinador, este dispositivo se utiliza para extender el alcance de la red y distribuir la red.
**End Device**: Tambien conocidos como dispositvos de funcion reducida, solo envia senales, estos dispositivios pueden ser los sensores, actuares, etc.

![[static/ZigBee_Red.png]]

## ZigBee - Application Profiles
Es una coleccion de descriptores del dispositivo, permite enviar comandos, solicitar informacion y procesar comandos y solicitudes, define el tipo de mensaje que se enviara.

![[static/ZigBee_ApplicationProfile.png]]

## ZigBee - Device Descriptor
Es una descripcion del dispositivo entre una *application profile*, define la informacion basica del dispositivo, tiene un identificador unicao que es intercambiado como parte del proceso de descubrimiento.

## ZigBee - Cluster Identifiers
Es una descripcion del dispositivo entre una *application profile*, representa una coleccion de atributos y comandos que son usados para controlar aspectos del dispositivo, cada cluster identifier soporta una pila de atributos.

| ZigBee device Group    | ZigBee device ID | Description                |
| ---------------------- | ---------------- | -------------------------- |
| Generic                | 0x0000           | ON/OFF Switch              |
|                        | 0x0001           | Level Control Switch       |
|                        | 0x0002           | ON/OFF Ouput               |
| Lighting               | 0x0100           | ON/OFF Light               |
|                        | 0x0101           | Diammable light            |
| Intruder Alarm Systems | 0x0400           | IAS Control and Indication |
|                        | 0x0402           | IAS Zone                   |

## ZigBee - Network Security Model
### Centralizado
Solo el coordinador/trust center puede iniciar redes centralizadas, cuando un nodo se une, recibe la llave de red proporcionada por el trust center y establecen una llave de emparejamiento unica.

> Los nodos pueden soportar instalar codigos. Esto corresponde a instalar en el dispositivo una llave unica diferente a la utilizada al incio de la conexion.

### Distribuido
Este tipo de red no tiene nodo central/trust center, el router es el encargado de comenzar redes distribuidas, los nodos envias y reciben las llaves de red.
En esta comunicacion la llave de red es compartida por cada end device hacia el nodo en donde se realice la conexion.

## PyCatSniffer - Demo - Sniffeando Conexion ZigBee Centralizada
<iframe width="720" height="480" src="https://www.youtube.com/embed/Nl8MkhRTczk?si=z3UCTXmmP0Yl9aw1" title="YouTube video player" frameborder="0" allow="accelerometer; autoplay; clipboard-write; encrypted-media; gyroscope; picture-in-picture; web-share" referrerpolicy="strict-origin-when-cross-origin" allowfullscreen></iframe>

## PyCatSniffer - Demo - Sniffeando Conexion ZigBee Distribuida
<iframe width="720" height="480" src="https://www.youtube.com/embed/eAaMv7jrr0I?si=fLrv8p-naG7Fl1H8" title="YouTube video player" frameborder="0" allow="accelerometer; autoplay; clipboard-write; encrypted-media; gyroscope; picture-in-picture; web-share" referrerpolicy="strict-origin-when-cross-origin" allowfullscreen></iframe>

## PyCatSniffer - Demo - PCAP - Analizando la informacion

![[static/ZigBee_PCAP.png]]

Dentro de la informacion recopilada encontramos informacion que nos ayudan a saber donde encontramos los valores para escribir en los dispositivos finales. En el PCAP podemos notar la siguiente informacion:
- **Transport Key**: Clave de Red distribuida
- **Device Profile**:
	- Cluster: La ubicacion donde se almacena los valores del dispositivo
	- Profile: El tipo de application profile del dispositivo
	- Source Endpoint: Los dispositivos cuentan con un ruta donde podemos apuntar para escribir o leer un valor
	- Attribute Field: Ubicacion del valor a leer
	- Data type: Tipo de dato que podemos utilizar sobre el atributo

Con esa informacion podemos obtener los valores y posteriormente podemos escribir en ellos mandando la informacion con *ZCL* (ZigBee Cluster Library).

# HackGDL
[Slides de la conferencia](https://docs.google.com/presentation/d/1V1vNgGP26OVy4PmkgJffvl4cvWKJ1ykW/edit?usp=sharing&ouid=113651827532850924319&rtpof=true&sd=true)

# Bibliografía
- [Practical IoT Hacking ](https://nostarch.com/practical-iot-hacking)