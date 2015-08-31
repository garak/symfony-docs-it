.. index::
    single: Console; Uso

Usare la console
================

La pagina :doc:`/components/console/usage` della documentazione dei componenti
mostra le opzioni globali della console. Quando si usa la console come parte del
framework completo, ci sono alcune opzioni globali aggiuntive.

Per impostazione predefinita, i comandi della console girano in ambiente ``dev``, ma si
potrebbe voler cambiare ambiente per alcuni comandi. Per esempio, si potrebbero voler
eseguire alcuni comandi in ambiente ``prod``, per questioni prestazionali. Inoltre, il
risultato di alcuni comandi varia a seconda dell'ambiente. Per esempio, il comando ``cache:clear``
cancella e prepara la cache sono per l'ambiente specificato. Per pulire e preparare la
cache di ``prod``, occorre eseguire:

.. code-block:: bash

    $ php app/console cache:clear --env=prod

o l'equivalente:

.. code-block:: bash

    $ php app/console cache:clear -e prod

Oltre a cambiare ambiente, si può anche scegliere di disabilitare la modalità di debug.
Ciò può tornare utile quando si vogliono eseguire comandi in ambiente ``dev``,
ma evitare aggravi prestazionali dovuti al raccoglimento dei dati di debug:

.. code-block:: bash

    $ php app/console list --no-debug

C'è una shell interattiva, che consente di inserire comandi senza dover specificare ogni
volta ``php app/console``, il che è utile quando occorre eseguire molti
comandi. Per entrare nella shell, eseguire:

.. code-block:: bash

    $ php app/console --shell
    $ php app/console -s

Si possono eseguire comandi usando semplicemente i rispettivi nomi:

.. code-block:: bash

    Symfony > list

Quando si usa la shell, si può scegliere di eseguire ogni comando in un processo separato:

.. code-block:: bash

    $ php app/console --shell --process-isolation
    $ php app/console -s --process-isolation

In questo modo, l'output non sarà colorato e l'interattività non è
supportata, quindi occorre passare tutti i parametri in modo esplicito.

.. note::

    A meno di non usare processi isolati, la pulizia della cache nella shell
    non avrà effetto sui comandi eseguiti successivamente. Questo perché
    vengono ancora usati i file originariamente in cache.
