.. index::
    single: Riferimento configurazione; Swiftmailer

Configurazione di SwiftmailerBundle ("swiftmailer")
===================================================

Questo riferimento è ancora provvisorio. Dovrebbe essere accurato, ma
non sono pienamente coperte tutte le opzioni. Per un elenco completo delle
opzioni predefinite di configurazione, vedere `Configurazione`_

La chiave ``swiftmailer`` configura l'integrazione di Symfony con Swiftmailer,
che si occupa di creare e spedire messaggi email.

La sezione seguente elenca tutte le opzioni disponibili per configurare un
mailer. Si possono anche configurare più mailer (vedere `Usare più mailer`_).

Configurazione
--------------

* `transport`_
* `username`_
* `password`_
* `host`_
* `port`_
* `encryption`_
* `auth_mode`_
* `spool`_
    * `type`_
    * `path`_
* `sender_address`_
* `antiflood`_
    * `threshold`_
    * `sleep`_
* `delivery_address`_
* `delivery_whitelist`_
* `disable_delivery`_
* `logging`_

transport
~~~~~~~~~

**tipo**: ``stringa`` **predefinito**: ``smtp``

Il metodo di trasporto usato per inviare le email. Valori validi:

* smtp
* gmail (vedere :doc:`/cookbook/email/gmail`)
* mail
* sendmail
* null (lo stesso che impostare `disable_delivery`_ a ``true``)

username
~~~~~~~~

**tipo**: ``stringa``

Il nome utente quando si usa ``smtp`` come trasporto.

password
~~~~~~~~

**tipo**: ``stringa``

La password quando si usa ``smtp`` come trasporto.

host
~~~~

**tipo**: ``stringa`` **predefinito**: ``localhost``

L'host a cui connettersi quando si usa ``smtp`` come trasporto.

port
~~~~

**tipo**: ``stringa`` **predefinito**: 25 o 465 (a seconda di `encryption`_)

La porta quando si usa ``smtp`` come trasporto. Predefinito 465 se ``encryption``
è ``ssl``, 25 in caso contrario.

encryption
~~~~~~~~~~

**tipo**: ``stringa``

La modalità di criptazione quando si usa ``smtp`` come trasporto. Valori validi
sono ``tls``, ``ssl`` o ``null`` (che indica nessuna criptazione).

auth_mode
~~~~~~~~~

**tipo**: ``stringa``

La modalità di autenticazione quando si usa ``smtp`` come trasporto. Valori validi
sono ``plain``, ``login``, ``cram-md5`` o ``null``.

spool
~~~~~

Per dettagli sullo spool delle email, vedere :doc:`/cookbook/email/spool`.

type
....

**tipo**: ``stringa`` **predefinito**: ``file``

Il metodo usato per memorizzare i messaggi nello spool. Attualmente è supportato
solo ``file``. Tuttavia, si dovrebbe poter creare uno spool personalizzato,
creando un servizio di nome ``swiftmailer.spool.mio_spool`` e impostando questo valore a ``mio_spool``.

path
....

**tipo**: ``stringa`` **predefinito**: ``%kernel.cache_dir%/swiftmailer/spool``

Quando si usa ``file`` come spool, questo è il percorso in cui i messaggi in spool
sono memorizzati.

sender_address
~~~~~~~~~~~~~~

**tipo**: ``stringa``

Se impostato, tutti i messaggi saranno inviati con questo indirizzo come "return path",
che è l'indirizzo a cui i messaggi in bounce vengono spediti. Viene gestito internamente
dalla classe ``Swift_Plugins_ImpersonatePlugin`` di Swiftmailer.

antiflood
~~~~~~~~~

threshold
.........

**tipo**: ``stringa`` **predefinito**: ``99``

Usato con ``Swift_Plugins_AntiFloodPlugin``. È il numero di email da inviare prima
di far ripartire il trasporto.

sleep
.....

**tipo**: ``stringa`` **predefinito**: ``0``

Usato con ``Swift_Plugins_AntiFloodPlugin``. È il numero di secondi da attendere
prima della ripartenza del trasporto.

delivery_address
~~~~~~~~~~~~~~~~

**tipo**: ``stringa``

Se impostato, tutti i messaggi email saranno inviati a questo indirizzo, invece
che ai loro reali destinatari. Molto utile durante lo sviluppo, Per esempio,
impostandolo nel file ``config_dev.yml``, ci si può assicurare che tutte le
email inviate durante la fase di sviluppo siano inviate a un singolo account.

Usa ``Swift_Plugins_RedirectingPlugin``. I destinatari originali sono disponibili
negli header ``X-Swift-To``, ``X-Swift-Cc`` e ``X-Swift-Bcc``.

delivery_whitelist
~~~~~~~~~~~~~~~~~~

**tipo**: ``array``

Usato in combinazione con ``delivery_address``. Se impostato, le email corrispondenti a uno di
questi schemi saranno consegnate normalmente, invece di essere inviate a ``delivery_address``.
Per maggiori dettagli, vedere :ref:`la ricetta. <sending-to-a-specified-address-but-with-exceptions>`

disable_delivery
~~~~~~~~~~~~~~~~

**tipo**: ``booleano`` **predefinito**: ``false``

Se ``true``, ``transport`` sarà automaticamente impostato a ``null`` e quindi
nessuna email sarà veramente inviata.

logging
~~~~~~~

**tipo**: ``booleano`` **predefinito**: ``%kernel.debug%``

Se ``true``, il raccoglitore di dati di Symfony sarà attivato per Swiftmailer e
le informazioni saranno disponibili nel profilatore.

Configurazione predefinita completa
-----------------------------------

.. configuration-block::

    .. code-block:: yaml

        swiftmailer:
            transport:            smtp
            username:             ~
            password:             ~
            host:                 localhost
            port:                 false
            encryption:           ~
            auth_mode:            ~
            spool:
                type:                 file
                path:                 "%kernel.cache_dir%/swiftmailer/spool"
            sender_address:       ~
            antiflood:
                threshold:            99
                sleep:                0
            delivery_address:     ~
            disable_delivery:     ~
            logging:              "%kernel.debug%"

    .. code-block:: xml

        <?xml version="1.0" encoding="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xmlns:swiftmailer="http://symfony.com/schema/dic/swiftmailer"
            xsi:schemaLocation="http://symfony.com/schema/dic/services http://symfony.com/schema/dic/services/services-1.0.xsd
                http://symfony.com/schema/dic/swiftmailer http://symfony.com/schema/dic/swiftmailer/swiftmailer-1.0.xsd">

            <swiftmailer:config
                transport="smtp"
                username=""
                password=""
                host="localhost"
                port="false"
                encryption=""
                auth_mode=""
                sender_address=""
                delivery_address=""
                disable_delivery=""
                logging="%kernel.debug%"
                >
                <swiftmailer:spool
                    path="%kernel.cache_dir%/swiftmailer/spool"
                    type="file" />

                <swiftmailer:antiflood
                    sleep="0"
                    threshold="99" />
            </swiftmailer:config>
        </container>

Usare più mailer
----------------

Si possono configurare più mailer, raggruppandoli sotto la voce ``mailers``
(il mailer predefinito è identificato dall'opzione ``default_mailer``):

.. configuration-block::

    .. code-block:: yaml

        swiftmailer:
            default_mailer: second_mailer
            mailers:
                first_mailer:
                    # ...
                second_mailer:
                    # ...

    .. code-block:: xml

        <?xml version="1.0" encoding="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xmlns:swiftmailer="http://symfony.com/schema/dic/swiftmailer"
            xsi:schemaLocation="http://symfony.com/schema/dic/services
                http://symfony.com/schema/dic/services/services-1.0.xsd
                http://symfony.com/schema/dic/swiftmailer
                http://symfony.com/schema/dic/swiftmailer/swiftmailer-1.0.xsd"
        >
            <swiftmailer:config default-mailer="second_mailer">
                <swiftmailer:mailer name="first_mailer"/>
                <swiftmailer:mailer name="second_mailer"/>
            </swiftmailer:config>
        </container>

    .. code-block:: php

        $container->loadFromExtension('swiftmailer', array(
            'default_mailer' => 'second_mailer',
            'mailers' => array(
                'first_mailer' => array(
                    // ...
                ),
                'second_mailer' => array(
                    // ...
                ),
            ),
        ));

Ogni mailer è registrato come servizio::

    // ...

    // restituisce il primo mailer
    $container->get('swiftmailer.mailer.first_mailer');

    // restituisce anche il secondo mailer, poiché è quello predefinito
    $container->get('swiftmailer.mailer');

    // restituisce il secondo mailer
    $container->get('swiftmailer.mailer.second_mailer');
