.. index::
   single: Bundle; Best Practice

Struttura del bundle e best practice
====================================

Un bundle è una cartella con una struttura ben definita, che può ospitare qualsiasi cosa,
dalle classi ai controllori e alle risorse web. Anche se i bundle sono molto
flessibili, si devono seguire alcune best practice per distribuirne uno.

.. index::
   pair: Bundle; Convenzioni di nomenclatura

.. _bundles-naming-conventions:

Nome del bundle
---------------

Un bundle è anche uno spazio dei nomi di PHP. Lo spazio dei nomi deve seguire gli
`standard`_ tecnici di interoperabilità di PHP 5.3 per gli spazi dei nomi e i nomi delle
classi: inizia con un segmento del venditore, seguito da zero o più segmenti di categoria
e finisce con il nome breve dello spazio dei nomi, che deve terminare col suffisso
``Bundle``.

Uno spazio dei nomi diventa un bundle non appena vi si aggiunge una classe Bundle. La
classe Bundle deve seguire queste semplici regole:

* Usare solo caratteri alfanumerici e trattini bassi;
* Usare un nome in CamelCase;
* Usare un nome breve e descrittivo (non oltre 2 parole);
* Aggiungere un prefisso con il nome del venditore (e, opzionalmente, con gli spazi dei
  nomi della categoria);
* Aggiungere il suffisso ``Bundle``.

Ecco alcuni spazi dei nomi e nomi di classi Bundle validi:

+-----------------------------------+--------------------------+
| Spazio dei nomi                   | Nome classe Bundle       |
+===================================+==========================+
| ``Acme\Bundle\BlogBundle``        | ``AcmeBlogBundle``       |
+-----------------------------------+--------------------------+
| ``Acme\Bundle\Social\BlogBundle`` | ``AcmeSocialBlogBundle`` |
+-----------------------------------+--------------------------+
| ``Acme\BlogBundle``               | ``AcmeBlogBundle``       |
+-----------------------------------+--------------------------+

Per convenzione, il metodo ``getName()`` della classe Bundle deve restituire il
nome della classe.

.. note::

    Se si condivide pubblicamente il bundle, si deve usare il nome della classe Bundle
    per il repository (``AcmeBlogBundle`` e non ``BlogBundle``, per
    esempio).

.. note::

    I bundle del nucleo di Symfony2 non hanno il prefisso ``Symfony`` e
    hanno sempre un sotto-spazio dei nomi ``Bundle``; per esempio:
    :class:`Symfony\\Bundle\\FrameworkBundle\\FrameworkBundle`.

Ogni bundle ha un alias, che è la versione breve in minuscolo del nome del bundle,
con trattini bassi (``acme_hello`` per ``AcmeHelloBundle`` o
``acme_social_blog`` per ``Acme\Social\BlogBundle``, per esempio). Questo alias
è usato per assicurare l'univocità di un bundle (vedere più avanti alcuni esempi
d'utilizzo).

Struttura della cartella
------------------------

La struttura di base delle cartella di un bundle ``HelloBundle`` deve essere come
segue:

.. code-block:: text

    XXX/...
        HelloBundle/
            HelloBundle.php
            Controller/
            Resources/
                meta/
                    LICENSE
                config/
                doc/
                    index.rst
                translations/
                views/
                public/
            Tests/

Le cartelle ``XXX`` riflettono la struttura dello spazio dei nomi del bundle.

I seguenti file sono obbligatori:

* ``HelloBundle.php``;
* ``Resources/meta/LICENSE``: La licenza completa del codice;
* ``Resources/doc/index.rst``: Il file iniziale della documentazione del bundle.

.. note::

    Queste convenzioni assicurano che strumenti automatici possano appoggiarsi a tale
    struttura predefinita nel loro funzionamento.

La profondità delle sotto-cartelle va mantenuta al minimo per le classi e i file più usati
(massimo 2 livelli). Ulteriori livelli possono essere definiti per file meno usati e
non strategici.

La cartella del bundle è in sola lettura. Se occorre scrivere file temporanei,
memorizzarli sotto le cartelle ``cache/`` o ``log/`` dell'applicazione. Degli strumenti
possono generare file nella cartella del bundle, ma solo se i file generati devono far
parte del repository.

Le seguenti classi e i seguenti file hanno postazioni specifiche:

+------------------------------+-----------------------------+
| Tipo                         | Cartella                    |
+==============================+=============================+
| Comandi                      | ``Command/``                |
+------------------------------+-----------------------------+
| Controllori                  | ``Controller/``             |
+------------------------------+-----------------------------+
| Estensioni del contenitore   | ``DependencyInjection/``    |
+------------------------------+-----------------------------+
| Ascoltatori di eventi        | ``EventListener/``          |
+------------------------------+-----------------------------+
| Configurazione               | ``Resources/config/``       |
+------------------------------+-----------------------------+
| Risorse Web                  | ``Resources/public/``       |
+------------------------------+-----------------------------+
| File di traduzione           | ``Resources/translations/`` |
+------------------------------+-----------------------------+
| Template                     | ``Resources/views/``        |
+------------------------------+-----------------------------+
| Test unitari e funzionali    | ``Tests/``                  |
+------------------------------+-----------------------------+

Classi
------

La struttura delle cartelle di un bundle è usata dalla gerarchia degli spazi dei nomi.
Per esempio, un controllore ``HelloController`` è posto in
``Bundle/HelloBundle/Controller/HelloController.php`` e il nome pienamente qualificato
della classe è ``Bundle\HelloBundle\Controller\HelloController``.

Tutte le classi e i file devono seguire gli :doc:`standard di codice
</contributing/code/standards>` di Symfony2.

Alcune classi vanno viste solo come facciati e devono essere più corte possibile, come
comandi, helper, ascoltatori e controllori.

Le classi che si connettono al distributore di eventi devono avere come suffisso
``Listener``.

Le classi eccezione devono essere poste nel sotto-spazio dei nomi ``Exception``.

Venditori
---------

Un bundle non deve includere librerie PHP di terze parti. Deve invece appoggiarsi
all'auto-caricamento standard di Symfony2.

Un bundle non dovrebbe includere librerie di terze parti scritte in JavaScript, CSS o
altro linguaggio.

Test
----

Un bundle deve avere una suite di test scritta con PHPUnit e posta sotto la cartella
``Tests/``. I test devono seguire i seguenti principi:

* La suite di test deve essere eseguibile con un semplice comando ``phpunit``, eseguito da
  un'applicazione di esempio;
* I test funzionali vanno usati solo per testare la risposta e alcune informazioni di
  profilo, se se ne hanno;
* La copertura del codice deve essere almeno del 95%.

.. note::
   Una suite di test non deve contenere script come ``AllTests.php``, ma appoggiarsi
   a un file ``phpunit.xml.dist``.

Documentazione
--------------

Tutte le classi e le funzioni devono essere complete di PHPDoc.

Una documentazione estensiva andrebbe fornita in formato
:doc:`reStructuredText </contributing/documentation/format>`, sotto la cartella
``Resources/doc/``; il file ``Resources/doc/index.rst`` è l'unico file obbligatorio
e deve essere il punto di ingresso della documentazione.

Controllori
-----------

Come best practice, i controllori di un bundle inteso per essere distribuito
non devono estendere la classe base
:class:`Symfony\\Bundle\\FrameworkBundle\\Controller\\Controller`.
Possono implementare
:class:`Symfony\\Component\\DependencyInjection\\ContainerAwareInterface` oppure
estendere :class:`Symfony\\Component\\DependencyInjection\\ContainerAware`
.

.. note::

    Se si dà uno sguardo ai metodi di
    :class:`Symfony\\Bundle\\FrameworkBundle\\Controller\\Controller`,
    si vedrà che sono solo delle scorciatoie utili per facilitare l'apprendimento.

Rotte
-----

Se il bundle fornisce delle rotte, devono avere come prefisso l'alias del bundle.
Per esempio, per AcmeBlogBundle, tutte le rotte devono avere come prefisso
``acme_blog_``.

Template
--------

Se un bundle fornisce template, devono usare Twig. Un bundle non deve fornire un
layout principale, tranne se fornisce un'applicazione completa.

File di traduzione
------------------

Se un bundle fornisce messaggi di traduzione, devono essere definiti in formato
XLIFF; il dominio deve avere il nome del bundle (``bundle.hello``).

Un bundle non deve sovrascrivere messaggi esistenti in altri bundle.

Configurazione
--------------

Per fornire maggiore flessibilità, un bundle può fornire impostazioni configurabili,
usando i meccanismi di Symfony2.

Per semplici impostazioni di configurazione, appoggiarsi alla voce predefinita
``parameters`` della configurazione di Symfony2. I parametri di Symfony2 sono semplici
coppie chiave/valore; un valore può essere un qualsiasi valore valido in PHP. Ogni nome di
parametro dovrebbe iniziare con l'alias del bundle, anche se questo è solo un suggerimento.
Gli altri nomi di parametri useranno un punto (``.``) per separare le varie parti (p.e.
``acme_hello.email.from``).

L'utente finale può fornire valori in qualsiasi file di configurazione:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        parameters:
            acme_hello.email.from: fabien@example.com

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <parameters>
            <parameter key="acme_hello.email.from">fabien@example.com</parameter>
        </parameters>

    .. code-block:: php

        // app/config/config.php
        $container->setParameter('acme_hello.email.from', 'fabien@example.com');

    .. code-block:: ini

        [parameters]
        acme_hello.email.from = fabien@example.com

Recuperare i parametri di configurazione nel proprio codice dal contenitore::

    $container->getParameter('acme_hello.email.from');

Pur essendo questo meccanismo abbastanza semplice, si consiglia caldamente l'uso
della configurazione semantica, descritta nel ricettario.

.. note::

    Se si definiscono servizi, deve avere anche essi come prefisso l'alias del
    bundle.

Imparare di più dal ricettario
------------------------------

* :doc:`/cookbook/bundles/extension`

.. _standard: http://groups.google.com/group/php-standards/web/psr-0-final-proposal
