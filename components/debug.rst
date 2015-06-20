.. index::
   single: Debug
   single: Componenti; Debug

Il componente Debug
===================

    Il componente Debug fornisce strumenti per facilitare il debug del codice PHP

.. versionadded:: 2.3
    Il componente Debug è nuovo in Symfony 2.3. In precedenza, le classi si
    trovavano nel componente HttpKernel.

Installazione
-------------

Si può installare il componente in due modi:

* Usando il  repository ufficiale su Git (https://github.com/symfony/Debug);
* :doc:`Installandolo tramite Composer </components/using_components>` (``symfony/debug`` su `Packagist`_).

.. include:: /components/require_autoload.rst.inc

Uso
---

Il componente Debug fornisce diversi strumenti che aiutano nel debug del codice PHP.
Per abilitarlo, bastano poche istruzioni::

    use Symfony\Component\Debug\Debug;

    Debug::enable();

Il metodo :method:`Symfony\\Component\\Debug\\Debug::enable` registra un
gestore di errori e un gestore di eccezioni. Se è disponibile il
:doc:`componente ClassLoader </components/class_loader/introduction>`, viene registrato
anche uno speciale caricatore di classi.

Leggere le sezioni seguenti per maggiori informazioni sui vari strumenti
a disposizione.

.. caution::

    Non si dovrebbero mai abilitare gli strumenti di debug in ambiente di produzione,
    perché potrebbero rivelare informazioni sensibili agli utenti.

Abilitare il gestore di errori
------------------------------

La classe :class:`Symfony\\Component\\Debug\\ErrorHandler` cattura gli errori PHP
e li converte in eccezioni (di classe :phpclass:`ErrorException` o
:class:`Symfony\\Component\\Debug\\Exception\\FatalErrorException` per errori
fatali PHP)::

    use Symfony\Component\Debug\ErrorHandler;

    ErrorHandler::register();

Abilitare il gestore di eccezioni
---------------------------------

La classe :class:`Symfony\\Component\\Debug\\ExceptionHandler` cattura le eccezioni
PHP non catturate e le converte in una risposta PHP. È utile in
modalità di debug, per sostituire l'output predefinito di XDebug con qualcosa di
più elegante e utile::

    use Symfony\Component\Debug\ExceptionHandler;

    ExceptionHandler::register();

.. note::

    Se è disponibile il :doc:`componente HttpFoundation </components/http_foundation/introduction>`,
    il gestore usa un oggetto ``Response`` di Symfony. Altrimenti, si accontenta
    di una semplice risposta PHP.

.. _Packagist: https://packagist.org/packages/symfony/debug
