.. index::
    single: Limitare le scritture di metadati; Sessione

Limitare le scritture di metadati
=================================

.. versionadded:: 2.4
    La possibilità di limitare i metadati di sessione è stata introdotta in Symfony 2.4.

Il comportamento predefinito della sessione di PHP è di persistere la sessione, indipendentemente dal fatto
che i dati di sessioni siano cambiati o no. In Symfony, ogni volta che si accede
alla sessione, vengono registrati dei metadati (sessione creata/usata l'ultima volta), che si possono
usare per determinare l'età della sessione e il tempo di inattività.

Se, per questioni di prestazioni, si vuole limitare la frequenza con cui la sessione
persiste, questa caratteristiche può aggiustare la granularità degli aggiornamenti dei metadati e
persistere la sessione meno spesso, pur mantenendo dei metadati relativamente
accurati. Se altri dati di sessione vengono modificati, la sessione sarà sempre persistita.

Si può dire a Symfony di non aggiornare i metadati sull'ultimo aggiornamento della sessione
finché non sia passato un determinato lasso di tempo, impostando
``framework.session.metadata_update_threshold`` a un valore, in secondi, maggiore
di zero:

.. configuration-block::

    .. code-block:: yaml

        framework:
            session:
                metadata_update_threshold: 120

    .. code-block:: xml

        <?xml version="1.0" encoding="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services"
            xmlns:framework="http://symfony.com/schema/dic/symfony"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/dic/services http://symfony.com/schema/dic/services/services-1.0.xsd
                                http://symfony.com/schema/dic/symfony http://symfony.com/schema/dic/symfony/symfony-1.0.xsd">

            <framework:config>
                <framework:session metadata-update-threshold="120" />
            </framework:config>

        </container>

    .. code-block:: php

        $container->loadFromExtension('framework', array(
            'session' => array(
                'metadata_update_threshold' => 120,
            ),
        ));

.. note::

    Il comportamento predefinito di PHP è di salvare la sessione, che sia cambiata o
    no. Quando si usa ``framework.session.metadata_update_threshold``, Symfony
    avvolge il gestore di sessione (configurato in
    ``framework.session.handler_id``) all'interno di WriteCheckSessionHandler. In questo
    modo preverrà ogni scrittura di sessione, in caso di mancata modifica.

.. caution::

    Fare attenzione: se la sessione non viene scritta a ogni richiesta, il garbage collector
    potrebbe scattare prima del solito. Questo vuol dire che gli utenti potrebbero essere forzati
    a uscire prima del previsto.
