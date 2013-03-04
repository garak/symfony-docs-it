.. index::
   single: Template; Variabili globali

Iniettare variabili in tutti i template (variabili globali)
===========================================================

A volte si vuole che una variabile sia accessibile in tutti i template usati.
Lo si può fare, modificando il file ``app/config/config.yml``:

.. code-block:: yaml

    # app/config/config.yml
    twig:
        # ...
        globals:
            ga_tracking: UA-xxxxx-x

Ora, la variabile ``ga_tracking`` è disponibile in tutti i template Twig

.. code-block:: html+jinja

    <p>Il codice di tracciamento Google è: {{ ga_tracking }} </p>

È molto facile! Si può anche usare il sistema :ref:`book-service-container-parameters`,
che consente di isolare o riutilizzare il valore:

.. code-block:: ini

    # app/config/parameters.yml
    parameters:
        ga_tracking: UA-xxxxx-x

.. code-block:: yaml

    # app/config/config.yml
    twig:
        globals:
            ga_tracking: "%ga_tracking%"

La stessa variabile è disponibile esattamente come prima.

Variabili globali più complesse
-------------------------------

Se la variabile globale che si vuole impostare è più complicata, per esempio un oggetto,
non si potrà usare il metodo precedente. Invece, occorrerà creare
un':ref:`estensione Twig<reference-dic-tags-twig-extension>` e restituire la
variabile globale come una delle voci del metodo ``getGlobals``.
