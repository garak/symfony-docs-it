.. index::
   single: Sessioni

Integrare una vecchia applicazione con le sessioni di Symfony
=============================================================

.. versionadded:: 2.3
    La capacità di integrarsi con il sistema legacy di sessioni di PHP è stata aggiunta in Symfony 2.3.

Se si sta integrando il framework Symfony completo all'interno di una vecchia applicazione
che inizializza la sessione con ``session_start()``, è ancora possibile
usare il sistema di gestione delle sessioni di Symfony, usando le sessioni di PHP Bridge.

Se l'applicazione ha impostato il proprio gestore di salvataggio PHP, si può specificare un valore nullo
per ``handler_id``:

.. code-block:: yaml

    framework:
        session:
            storage_id: session.storage.php_bridge
            handler_id: ~

Altrimenti, se il problema è semplicemente che non si può evitare che l'applicazione
faccia partire la sessione con un ``session_start()``, si può ancora far uso del
gestore di salvataggio delle sessioni basato su Symfony, specificando il gestore di salvataggio come
in questo esempio:

.. code-block:: yaml

    framework:
        session:
            storage_id: session.storage.php_bridge
            handler_id: session.handler.native_file

.. note::

    Se la vecchia applicazione richiede il proprio gestore di salvataggio, non
    sovrascriverlo. Invece, impostare ``handler_id: ~``. Si noti che un gestore di salvataggio
    non può essere cambiato, una volta che la sessione è stata inizializzata. Se l'applicazione
    inizializza la sessione prima che Symfony sia inizializzato, il gestore di salvataggio sarà
    già stato impostato. In questo caso, ci sarà bisogno di ``handler_id: ~``.
    La ridefinizione del gestore di salvataggio va fatto solo quando si è certi che la vecchia applicazione
    sia in grado di usare il gestore di salvataggio di Symfony senza effetti indesiderati e che la sessione
    non sia stata inizializzata prima dell'inizializzazione di Symfony.

Per maggiori dettagli, si veda :doc:`/components/http_foundation/session_php_bridge`.
