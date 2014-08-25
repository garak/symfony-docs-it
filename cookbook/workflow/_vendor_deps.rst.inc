Gestire le librerie dei venditori con ``composer.json``
-------------------------------------------------------

Come funziona?
~~~~~~~~~~~~~~

Ogni progetto Symfony usa un gruppo di librerie di "venditori". In un modo o
nell'altro, lo scopo è scaricare tali file nella propria cartella ``vendor/``
e, idealmente, avere un modo tranquillo per gestire l'esatta versione necessaria
per ciascuno.

Per impostazione predefinita, tali librerie sono scaricate eseguendo uno script
"scaricatore" ``php composer.phar install``. Questo file ``composer.phar`` fa parte
di una libreria chiamata `Composer`_, di cui si può approfondire l'uso in questo
capitolo.

Il file ``composer.phar`` legge dal file ``composer.json``, posto nella radice del
progetto. Questo file, formattato in JSON, contiene una lista di ogni pacchetto
esterno necessario, la sua versione e altro ancora.
Il  file ``composer.phar`` legge anche un file ``composer.lock``, che consente
di bloccare ogni libreria a una versione **esatta**. Di fatto se esiste un file ``composer.lock``,
le versioni in esso contenute sovrascrivono quelle in ``composer.json``.
Per aggiornare le librerie alle ultime versioni, eseguire ``php composer.phar update``.

.. tip::

    Se si vuole aggiungere un nuovo pacchetto a un'applicazione, modificare il
    file ``composer.json``:

    .. code-block:: json

        {
            "require": {
                ...
                "doctrine/doctrine-fixtures-bundle": "@dev"
            }
        }

    e quindi eseguire il comando ``update`` per lo specifico pacchetto, quindi:

    .. code-block:: bash

        $ php composer.phar update doctrine/doctrine-fixtures-bundle

    Si possono anche combinare i due passi in un singolo comando:

    .. code-block:: bash

        $ php composer.phar require doctrine/doctrine-fixtures-bundle:@dev

Per saperne di più su Composer, vedere `GetComposer.org`_:

È importante capire che queste librerie di venditori *non* sono in realtà parte del
*proprio* repository. Sono invece dei semplici file non tracciati, che sono scaricati
nella cartella ``vendor/``. Ma, poiché ogni informazione
necessaria a scaricare tali file è nei file ``composer.json`` e ``composer.lock``
(che *sono* memorizzati nel proprio repository), ogni altro sviluppatore può usare il
progetto, eseguendo ``php composer.phar install`` e scaricando lo stesso preciso insieme
di librerie di venditori. Questo vuol dire che si può controllare con precisione ogni
libreria di venditore, senza dover in realtà inserirle nel *proprio* repository.

Quindi, ogni volta che uno sviluppatore usa il progetto, deve eseguire lo script
``php composer.phar install``, per assicurarsi di avere tutte le librerie necessarie.

.. sidebar:: Aggiornare Symfony

    Poiché Symfony non è altro che un gruppo di librerie di terze parti e le librerie
    di terze parti sono interamente controllate tramite ``composer.json`` e ``composer.lock``,
    aggiornare Symfony vuol dire semplicemente aggiornare questi due file, per far
    corrispondere il loro stato a quello dell'ultima Standard Edition di Symfony.

    Ovviamente, se sono state aggiunte nuove voci a ``composer.json``, assicurarsi
    di sostituire solo le parti originali (cioè assicurarsi di non cancellare alcuna
    delle proprie voci).

.. _Composer: http://getcomposer.org/
.. _GetComposer.org: http://getcomposer.org/