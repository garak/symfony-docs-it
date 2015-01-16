.. index::
    single: Server web; Server web interno

Usare il server web interno di PHP
==================================

A partire da PHP 5.4, la linea di comando dispone di un `server web interno`_. Lo si può usare
per eseguire applicazioni PHP locali durante lo sviluppo, per i test o per
demo applicative. In questo modo, non occorre configurare
un server web in piena regola, come
:doc:`Apache o Nginx </cookbook/configuration/web_server_configuration>`.

.. caution::

    Il server web interno è pensato per girare in ambienti controllati.
    Non è stato progettato per l'uso su reti pubbliche.

Far partire il server web
-------------------------

È facile eseguire un'applicazione Symfony nel server web interno, basta
eseguire il comando ``server:run``:

.. code-block:: bash

    $ php app/console server:run

Questo comando fa partire il server su ``localhost:8000``, che esegue l'applicazione Symfony.
Il comando resterà in attesa e risponderà alle richieste HTTP in ingresso, finché non
sarà terminato (solitamente tramite la pressione dei tasti Ctrl+C).

La porta predefinita su cui ascolta il server è 8080 su localhost. Si può
cambiare il socket, passando un indirizzo IP e una porta, come parametri del comando:

.. code-block:: bash

    $ php app/console server:run 192.168.0.1:8080

.. sidebar:: Uso del server web interno da una macchina virtuale

    Se si vuole usare il server web interno da una macchina virtuale
    e quindi caricare il sito da un browser sulla macchina locale, occorre
    ascoltare sull'indirizzo ``0.0.0.0:8000`` (cioè su tutti gli indirizzi IP
    assegnati alla macchina virtuale):

    .. code-block:: bash

        $ php app/console server:run 0.0.0.0:8000

    .. caution::

        Non si dovrebbe **MAI** ascoltare su tutte le interfacce in un computer
        direttamente accessibile da Internet. Il server web interno non è
        progettato per l'uso su reti pubbliche.

Opzioni dei comandi
-------------------

Il server interno si aspetta uno script "router" (approfondimento sullo script "router"
sono disponibili su `php.net`_) come parametro. Symfony si occupa di passare tale
script quando il comando viene eseguito in ambiente ``prod`` o ``dev``.
Si può usare l'opzione ``--router`` in altri ambienti oppure usare uno script
diverso:

.. code-block:: bash

    $ php app/console server:run --env=test --router=app/config/router_test.php

Se la document root dell'applicazione non è quella standard,
si deve passare la posizione corretta, usando l'opzione ``--docroot``:

.. code-block:: bash

    $ php app/console server:run --docroot=public_html

.. _`server web interno`: http://www.php.net/manual/it/features.commandline.webserver.php
.. _`php.net`: http://php.net/manual/it/features.commandline.webserver.php#example-401
