.. index::
    single: Componenti; ClassLoader

Il componente ClassLoader
=========================

    Il componente ClassLoader carica le classi di un progetto automaticamente, purché
    seguano alcune convenzioni standard di PHP.

Uso
---

Ogni volta che si usa una classe non ancora richiesta o inclusa,
PHP utilizza il `meccanismo di auto-caricamento`_ per delegare il caricamento di un file che definisca
la classe. Symfony fornisce due autoloader, capaci di caricare classi:

* :doc:`/components/class_loader/class_loader`: carica classi che seguono
  lo standard dei nomi `PSR-0`;

* :doc:`/components/class_loader/map_class_loader`: carica classi che usano
  una mappa statica dal nome della classe al percorso del file.

Inoltre, il componente ClassLoader di Symfony dispone di un insieme di classi wrapper,
che si possono usare per aggiungere funzionalità agli autoloader
esistenti:

* :doc:`/components/class_loader/cache_class_loader`
* :doc:`/components/class_loader/debug_class_loader`

Installazione
-------------

Si può installare il componente in due modi:

* :doc:`Installarlo via Composer </components/using_components>` (``symfony/class-loader``
  su `Packagist`_);
* Usare il repository ufficiale su Git (https://github.com/symfony/ClassLoader).

.. include:: /components/require_autoload.rst.inc

.. _`meccanismo di auto-caricamento`: http://php.net/manual/it/language.oop5.autoload.php
.. _Packagist: https://packagist.org/packages/symfony/class-loader
