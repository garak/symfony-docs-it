.. index::
    single: Frontend; Bower

Usare Bower con Symfony
=======================

Symfony e tutti i suoi pacchetti sono gestiti da Composer. Bower è uno
strumento di gestione per le dipendenze di frontend, come Bootstrap o
jQuery. Essendo Symfony un framework di backend, non può aiutare molto con
Bower. Per fortuna, Bower è facile da usare!

Installare Bower
----------------

Bower_ si basa su `Node.js`_. Assicurarsi di avere Node installato, quindi
eseguire:

.. code-block:: bash

    $ npm install -g bower

Al termine di questo comando, eseguire ``bower`` nel terminale, per verificare che
sia stato installato correttamente.

.. tip::

    Se non si vuole installare Node.js, si può usare in alternativa
    BowerPHP_ (un port  di Bower non ufficiale, scritto in PHP). Attenzione, perché è
    ancora in beta. Se si usa BowerPHP, utilizzare ``bowerphp`` invece di
    ``bower`` negli esempi seguenti.

Configurare Bower in un progetto
--------------------------------

Solitamente, Bower scarica tutto in una cartella ``bower_components/``. In
Symfony, solo i file nella cartella ``web/`` sono accessibili pubblicamente, quindi 
si deve configurare Bower per scaricare le cose in una posizione diversa. Per poterlo fare,
creare un file ``.bowerrc`` e specificare una cartella (come ``web/assets/vendor``):

.. code-block:: json

    {
        "directory": "web/assets/vendor/"
    }

.. tip::

    Se si usa un sistema di build per frontend, come `Gulp`_ o `Grunt`_, si può
    anche impostare una cartella diversa. Solitamente, si useranno tali
    strumenti per spostare alla fine tutte le risorse sotto la cartella ``web/``.

Un esempio: installare Bootstrap
--------------------------------

Che ci si creda o no, ora si è in grado di usare Bower in un'applicazione Symfony.
Come esempio, si vedrà ora come installare Bootstrap in un progetto e
includerlo in un layout.

Installare le dipendenze
~~~~~~~~~~~~~~~~~~~~~~~~

Per creare un file ``bower.json``, eseguire ``bower init``. Ora si è pronti
per aggiungere dipendenze al progetto. Per esempio, per aggiungere Bootstrap_ a
``bower.json`` e scaricarlo, eseguire:

.. code-block:: bash

    $ bower install --save bootstrap

Questo comando installerà Bootstrap e tutte le sue dipendenze nella cartella ``web/assets/vendor/`` (o
nella cartella configurata in ``.bowerrc``).

.. seealso::

    Per maggiori dettagli su come usare Bower, leggere la `documentazione di Bower`_.

Includere le dipendenze in un template
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Una volta installate le dipendenze, si può includere Bootstrap in un
template, come qualsiai altro CSS/JS:

.. configuration-block::

    .. code-block:: html+jinja

        {# app/Resources/views/layout.html.twig #}
        <!doctype html>
        <html>
            <head>
                {# ... #}

                <link rel="stylesheet"
                    href="{{ asset('assets/vendor/bootstrap/dist/css/bootstrap.min.css') }}">
            </head>

            {# ... #}
        </html>

    .. code-block:: html+php

        <!-- app/Resources/views/layout.html.php -->
        <!doctype html>
        <html>
            <head>
                {# ... #}

                <link rel="stylesheet" href="<?php echo $view['assets']->getUrl(
                    'assets/vendor/bootstrap/dist/css/bootstrap.min.css'
                ) ?>">
            </head>

            {# ... #}
        </html>

Ottimo! Il sito ora è pronto per usare Bootstrap. Si può facilmente aggiornare
Bootstrap alla sua versione più recente e gestire anche altre dipendenze di frontend.

Le risorse di Bower vanno ignorate o aggiunte?
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Attualmente, è probabilmente meglio *aggiungere* le risorse scaricate da Bower e non
ignorare la cartella (p.e. ``web/assets/vendor``), inserendola nel file
``.gitignore``:

.. code-block:: bash

    $ git add web/assets/vendor

Perché? Diversamente da Composer, Bower al momento non supporta il "lock", che vuol dire
che non c'è garanzia che l'esecuzione di ``bower install`` su un altro
server fornisca *esattamente* le stesse risorse installate sulla propria macchina.
Per maggiori dettagli, leggere l'aritcolo `Checking in front-end dependencies`_.

È possible che Bower aggiunga il "lock" nel prossimo futuro.
(vedere `bower/bower#1748`_).

.. _Bower: http://bower.io
.. _`Node.js`: https://nodejs.org
.. _BowerPHP: http://bowerphp.org/
.. _`documentazione di Bower`: http://bower.io/
.. _Bootstrap: http://getbootstrap.com/
.. _Gulp: http://gulpjs.com/
.. _Grunt: http://gruntjs.com/
.. _`Checking in front-end dependencies`: http://addyosmani.com/blog/checking-in-front-end-dependencies/
.. _`bower/bower#1748`: https://github.com/bower/bower/pull/1748
