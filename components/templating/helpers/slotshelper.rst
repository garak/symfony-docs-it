.. index::
    single: Aiutanti di Templating; Aiutante slots

Aiutante slots
==============

Molto spesso, i template in un progetto condividono alcuni elementi comuni, come
i ben noti testata e piè di pagina. Usando questo aiutante, il codice statico HTML può
essere inserito in un file di layout, insieme a degli "slot", che rappresentano le
parti dinamiche, che cambieranno in base alle varie pagine. Tali slot saranno quindi riempiti
da diversi template figli. In altre parole, il file di layout decora
il template figlio.

Mostrare gli slot
-----------------

Gli slot sono accessibili grazie all'aiutante slots (``$view['slots']``). Usare
:method:`Symfony\\Component\\Templating\\Helper\\SlotsHelper::output` per
mostrare il contenuto dello slot in quella posizione:

.. code-block:: html+php

    <!-- views/layout.php -->
    <!doctype html>
    <html>
        <head>
            <title>
                <?php $view['slots']->output('title', 'Titolo predefinito') ?>
            </title>
        </head>
        <body>
            <?php $view['slots']->output('_content') ?>
        </body>
    </html>

Il primo parametro del metodo è il nome dello slot. Il metodo ha un secondo
parametro, opzionale, che è il valore predefinito da usare nel caso in cui lo slot
non sia disponibile.

Lo slot ``_content`` è uno slot speciale, impostato da ``PhpEngine``. Contiene
il contenuto del sotto-template.

.. caution::

    Se si usa il componente da solo, assicurarsi di avere registrato
    :class:`Symfony\\Component\\Templating\\Helper\\SlotsHelper`::

        use Symfony\Component\Templating\Helper\SlotsHelper;

        // ...
        $templateEngine->set(new SlotsHelper());

Estendere i template
--------------------

Il metodo :method:`Symfony\\Component\\Templating\\PhpEngine::extend` viene richiamato nel
sotto-template, per impostare il suo template genitore. Quindi, si può usare
:method:`$view['slots']->set()<Symfony\\Component\\Translation\\Helper\\SlotsHelper::set>`
per impostare il contenuto dello slot. Tutto il contenuto non impostato esplicitamente in uno slot
è nello slot ``_content``.

.. code-block:: html+php

    <!-- views/page.php -->
    <?php $view->extend('layout.php') ?>

    <?php $view['slots']->set('title', $page->title) ?>

    <h1>
        <?php echo $page->title ?>
    </h1>
    <p>
        <?php echo $page->body ?>
    </p>

.. note::

    Si possono avere livelli multipli di ereditarietà: un layout può estendere un altro
    layout.

Per slot più grandi, c'è anche una sintassi estesa:

.. code-block:: html+php

    <?php $view['slots']->start('title') ?>
        Un grande ammontare di HTML
    <?php $view['slots']->stop() ?>
