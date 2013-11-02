.. index::
   single: Sessioni

Integrare una vecchia applicazione con le sessioni di Symfony
=============================================================

.. versionadded:: 2.3
La capacità di integrarsi con il sistema legacy di sessioni di PHP è stata aggiunt in Symfony 2.3.

Se si sta integrando il full-stack di symfony all'interno di una applicazione legacy
che inizializza la sessione con ``session_start()``, è ancora possibile
usare il sistema di gestione delle sessioni di Symfony usando le sessioni di PHP Bridge.

Se l'applicazione ha settato il proprio PHP save handler si  può specificare un valore nullo
come ``handler_id``:

.. code-block:: yaml

    framework:
        session:
            storage_id: session.storage.php_bridge
            handler_id: ~

Altrimenti, se il problema è semplicemente che non si può evitare che l'applicazione
faccia partire la sessione con un ``session_start()``, si può ancora far uso del
save handler basato su Symfony specificando il save handler come
mostrato  qui sotto:

.. code-block:: yaml

    framework:
        session:
            storage_id: session.storage.php_bridge
            handler_id: session.handler.native_file

.. note::

    Se l'applicazione legacy richiede il proprio save-handler, non si faccia override
    di quello. Invece si imposti ``handler_id: ~``. Si noti che un save-handler
    non può essere cambiato una volta che la sessione è stata inizializzata. Se l'applicazione
    inizializza la sessione prima che Symfony sia inizializzato, il save-handler sarà
    già impostato. In questo caso ci sarà bisogno di ``handler_id: ~``.
    L'override del save-handler va fatto solo quando si è certi che l'applicazione legacy
    sia in grado di usare il save-handler di Symfony senza effetti indesiderati e che la sessione
    non viene inizializzata prima dell'inizializzazione di Symfony.

Per  maggiori dettagli si veda :doc:`/components/http_foundation/session_php_bridge`.
