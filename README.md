
# TP Bus et Réseaux Sami ASFARY
 
## Introduction

Ce projet fut réalisé durant les séances de TP Bus et Réseaux.
Ce projet consiste en une STM32 et une Raspberry Pi Zero.

La première communique avec un module BMP280 calculant la pression et la température et un moteur pas à pas indiquant là-dite température.

La Raspberry quand à elle récupère les données de la STM32 via une connexion UART. Ces données seront disponible sur une API REST qui sera hébergée sur la Raspberry Pi.

## Broche utilisées pour ce TP
Les broches utilisé sur la STM32 seront les suivants :
![Broche de la STM32](https://zupimages.net/up/22/47/lgnw.png)
Les broches utilisé sur la Raspberry seront les suivants : 
![Pin de la RPi](https://zupimages.net/up/22/47/xzwc.png)

## TP1 - Bus I2C
Cette première partie se concentre sur la connexion I2C entre la STM32 et le module BMP280 qui embarque un capteur de pression et un capteur de température.
Pour cette partie, j'ai créé une librairie composé des fichier BMP280.c et BMP280.h, cette librairie permet un fonctionnement simplifié. 
### Macros
Cette librairie a plusieurs plusieurs macro notamment définissant les adresse et certaine commande utilisé.

    #define ADD_BMP280 				0x77 << 1
    #define BMP_MODE_NORMAL 		0b11
    #define BMP_SAMPLING_SKIPPED	0b000
    #define BMP_SAMPLING_1			0b001
    #define BMP_SAMPLING_2			0b010
    #define BMP_SAMPLING_4			0b011
    #define BMP_SAMPLING_8			0b100
    #define BMP_SAMPLING_16			0b101
    #define BMP_CONFIG_T			5
    #define BMP_CONFIG_P			2
    
    #define BMP_REG_CONFIG			0xF5
    #define BMP_REG_ID				0xD0
    #define BMP_REG_T_MSB			0xFA
    #define BMP_REG_P_MSB			0xF7

### Functions
Les fonctions de cette librairie ont avant tout pour objectif de simplifié la commande du module BMP280.
**/!\\** Pour lire avec le protocole I2C, il nous faut d’abord écrire quel registre lire sur le module BMP280 avant de lire le nombre d'octet voulu.
Toutes les données transmises seront de type _uint8_t_
Les fonctions sont les suivantes :

    HAL_StatusTypeDef config_BMP280(void)
Cette fonctions permet la configuration du BMP280 en mode normal avec une résolution de 16 bit pour le capteur de température et de pression.
Elle retourne l'état de fin d’exécution de la configuration.

    uint8_t getID_BMP280(void)
Cette fonction retourne l'identifiant du module. Cela peut être utilisé pour détecter d'éventuelle problème car l'identifiant doit être _0x58_.

    uint32_t getTemp_BMP280(void)
    uint32_t getPres_BMP280(void)
Ces fonctions permettent respectivement la lecture des valeurs de température et de pression, ces deux valeur son codé sur 16 bit sur 3 octet. Ces fonctions mettent ensuite en forme les valeurs dans une variable de type _uint8_t_ qui sera ensuite renvoyé.
## TP2 - Interfaçage STM32 - Raspberry
### Initialisation de la raspberry
Le programme Raspberry Pi Imager pourra nous permettre de configurer le SSID et le mot de passe du réseaux wifi ainsi que le nom d'utilisateur et mot de passe de la raspberry. Il permettra aussi l'activation du protocole ssh. Cependant il nous faut aussi paramétrer la connexion uart. 
Pour cela il nous faut ajouter les lignes suivante dans le ficher config.txt

    enable_uart=1
    dtoverlay=disable-bt
Cela nous permet de connecter l'uart aux connecteurs GPIO.
Il nous faut ensuite supprimer la ligne suivante du fichier cmdline.txt

    console=serial0,115200
Cela permet que le noyau libère l'uart.

Ensuite pour la connexion à la Raspberry nous utiliserons un logiciel tel que PuTTY pouvant effectué une connexion SSH, pour trouver l'adresse de la raspberry sur le réseaux nous pourrons utilisé Angry IP Scanner qui permet de scanner toute les adresses visibles sur le réseaux et ainsi déterminer l'adresse de la Raspberry.

Pour tester que l'initialisation de l'uart c'est effectué sans accro nous pouvont utilisé le programme
### Programme de la Raspberry
Pour effectuer une communication avec le programme de la raspberry nous utilisons la librairie _serial_ permettant une connection série. Nous paramétrons un objet tel qui suis : 

    ser = serial.Serial(
    port='/dev/ttyAMA0',
    baudrate=115200,
    parity=serial.PARITY_NONE,
    stopbits=serial.STOPBITS_ONE,
    bytesize=serial.EIGHTBITS,
    timeout=0xFF)
Cette objet viens avec les méthodes _.write()_ et _.readline()_ qui permettent respectivement d'envoyer une chaine de caractère et de recevoir une chaine de caractère fini par le caractère _\n_.
**/!\\** Les chaines de caractères doivent être encodé dans le format utf-8 pour l'envoi et décodé de ce format après la réception.
**/!\ Lors de la communication entre la Raspberry Pi et la STM32 les masses doivent être commune**
### Programme de la STM32
La STM32 n'ayant malheureusement pas de module permettant de lire une liste de caractère jusqu'à un certains caractère à ma connaissance, nous utilisons le DMA en recevant les caractères un par un en vérifiant.
L'organigramme ressemblera donc au suivant :

![organigramme](https://zupimages.net/up/22/47/sbwr.png)

## TP3 - Interface REST
Pour permettre au utilisateur d’accéder au donnée de la Raspberry nous utiliserons une interface API REST, pour cela nous utilisons la librairie flask qui nous permet d'hébergé un site internet sur la Raspberry. 
Les pages seront définie telle qui suit : 

    @app.route('/api/request/', methods=['GET', 'POST'])
    def api_request(path=None):
	    resp = {
	            "method":   request.method,
	            "url" :  request.url,
	            "path" : path,
	            "args": request.args,
	            "headers": dict(request.headers),
	    }
	    if request.method == 'POST':
	        resp["POST"] = {
	                "data" : request.get_json(),
	                }
	        return "", 202
	    return jsonify(resp)
@app.route permet de paramétrer quel lien et quel méthode peut être utilisé pour accéder à cette fonction.

Pour lancer l'API nous exécutons cette commande :

    `FLASK_APP=hello.py FLASK_ENV=development flask run --host 0.0.0.0`

Après exécution de cette commande, nous pouvons accéder au site internet depuis un appareil connecter au réseaux. Pour cela il nous suffit d'entrer l'adresse IP de la Raspberry suivit du port utilisé (:5000) dans un navigateur.

### Gestion des erreur

Dans le cas où l'utilisateur essayerais d’accéder à une page qui n'existe pas ou fait une manipulation interdite nous avons définie la redirection suivante :

    @app.errorhandler(404)
    def page_not_found(error):
	    return render_template('page_not_found.html'), 404
Dans un Dossier templates nous avons le fichier page_not_found.html qui permet d’afficher un message d'erreur lors de l'appel de la fonction:

    abort(404)

 
## TP4 - Bus CAN
Le bus CAN sera utilisé pour commandé un moteur. Pour cela nous utiliserons les fonctions et structure fournie par la librairie HAL.
La structure que nous utiliserons, _CAN_TxHeaderTypeDef_, a les champs suivants :

 - StdId ; ce champ contient l'identifiant du message quand celui ci et standart.
 - ExtId ; ce champ contient l'identifiant du message quand celui ci et étendu.
 - IDE ; ce champ définit si l'ID est standart.
 - RTR ; ce champ définit si la trame est de type standart ou remote.
 - DLC ; ce champ représente la taille de la trame.
 - TransmitGlobalTime ; ce champ permet la gestion du temps de réponse du bus CAN

Les valeurs que nous utiliserons seront les suivantes :

    Header.StdId = 0x61;
	Header.IDE = CAN_ID_STD;
	Header.RTR = CAN_RTR_DATA;
	Header.DLC = 2;
	Header.TransmitGlobalTime = DISABLE;

Nous utiliserons la commande en angle qui correspond à l'ID de message de _0x61_ et une taille de message de 2 octet tel qu'indiqué ci-dessous. L'ID définit est standard et nous n'utiliserons ni le _Remote Trame Request_ ni la gestion du temps de réponse du bus CAN.

## TP5 - Intégration I²C - Serial - REST - CAN
L'API REST utilisé lors de l'intégration se construit comme suis : 

![API REST](https://zupimages.net/up/22/47/0z6s.jpg)Le programme de la STM32 acquerra les données du module BMP280 continuellement dans la boucle principal et si demande de la Raspberry alors les données seront envoyer par UART.

Malheureusement cette partie ne fut pas finalisé.

