.. index::
    single: Aiutanti di Templating; Aiutante per gli asset

Aiutante per gli asset
======================

Lo scopo principale dell'aiutante per gli asset è quello di rendere un'applicazione più portabile,
generando i percorsi degli asset:

.. code-block:: html+php

   <link href="<?php echo $view['assets']->getUrl('css/style.css') ?>" rel="stylesheet">

   <img src="<?php echo $view['assets']->getUrl('images/logo.png') ?>">

L'aiutante per gli asset può essere configurare per rendere percorsi in un CDN o per modificare
i percorsi nel caso in cui gli asset si trovino in una sotto-cartella (p.e. ``http://example.com/app``).

Configurare i percorsi
----------------------

Per impostazione predefinita, l'aiutante per gli asset aggiungerà a ogni percorso una barra iniziale. Si può
configurare questo comportamento, passando un percorso di base come primo parametro del
costruttore::

    use Symfony\Component\Templating\Helper\AssetsHelper;

    // ...
    $templateEngine->set(new AssetsHelper('/pippo/pluto'));

Ora, se si usa l'aiutante, il prefisso per ogni percorso sarà ``/pippo/pluto``:

.. code-block:: html+php

   <img src="<?php echo $view['assets']->getUrl('images/logo.png') ?>">
   <!-- viene reso come:
   <img src="/foo/bar/images/logo.png">
   -->

URL assoluti
------------

Si può specificare un URL da usare, nel secondo parametro del costruttore::

    // ...
    $templateEngine->set(new AssetsHelper(null, 'http://cdn.example.com/'));

Ora gli URL saranno resi come ``http://cdn.example.com/images/logo.png``.

Versionamento
-------------

Per evitare l'uso di risorse in cache, dopo l'aggiornamento di una vecchia risorsa, si possono
usare le versioni, da aggiornare ogni volta che si rilascia un nuovo progetto. La versione
può essere verificata come terzo parametro::

    // ...
    $templateEngine->set(new AssetsHelper(null, null, '328rad75'));

Ora, ogni URL avrà come suffisso ``?328rad75``. Se si vuole un formato diverso,
lo si può specificare come quarto argomento. Deve essere una stringa da
usare in :phpfunction:`sprintf`. Il primo parametro è il percorso e il
secondo è la versione. Per esempio, ``%s?v=%s`` sarà reso come
``/images/logo.png?v=328rad75``.

Pacchetti multipli
------------------

La generazione dei percorsi degli asset è gestita internamente da pacchetti. Il componente fornisce
due pacchetti predefiniti:

* :class:`Symfony\\Component\\Templating\\Asset\\PathPackage`
* :class:`Symfony\\Component\\Templating\\Asset\\UrlPackage`

Si possono anche usare più pacchetti::

    use Symfony\Component\Templating\Asset\PathPackage;
    
    // ...
    $templateEngine->set(new AssetsHelper());

    $templateEngine->get('assets')->addPackage('images', new PathPackage('/images/'));
    $templateEngine->get('assets')->addPackage('scripts', new PathPackage('/scripts/'));

In questo modo l'aiutante degli asset userà tre pacchetti: quello predefinito, che
usa come prefisso ``/`` (impostato dal costruttore), il pacchetto delle immagini, che usa
``/images/`` e il pacchetto degli scritp, che usa
``/scripts/``.

Se si vuole cambiare il pacchetto predefinito, si può usare
:method:`Symfony\\Component\\Templating\\Helper\\AssetsHelper::setDefaultPackage`.

Si può specificare quale pacchetto si vuole usare nel secondo parametro di
:method:`Symfony\\Component\\Templating\\Helper\\AssetsHelper::getUrl`:

.. code-block:: html+php

    <img src="<?php echo $view['assets']->getUrl('foo.png', 'images') ?>">
    <!-- sarà reso come:
    <img src="/images/foo.png">
    -->

Pacchetti personalizzati
------------------------

Si possono creare i propri pacchetti, estendendo
:class:`Symfony\\Component\\Templating\\Package\\Package`.
