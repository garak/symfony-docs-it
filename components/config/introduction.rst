.. index::
   single: Config
   single: Componenti; Config

Il componente Config
====================

Introduzione
------------

Il componente Config fornisce diverse classi che aiutano a trovare, caricare, combinare,
riempire e validare valori di configurazione di ogni tipo, indipendentemente dal tipo
di sorgente (file YAML, XML o INI, oppure ad esempio una base dati).

.. caution::

    ``IniFileLoader`` analizza i contenuti del file tramite la funzione
    :phpfunction:`parse_ini_file`, quindi si possono impostare solo
    stringhe come valori dei parametri. Per impostare i parametri come altri tipi di dato
    (p.e. booleano, intero, ecc.), si raccomandano gli altri caricatori.

Installazione
-------------

Si può installare il componente in molti modi diversi:

* Installarlo via :doc:`Composer </components/using_components>` (``symfony/config`` su `Packagist`_);
* Usare il repository ufficiale su Git (https://github.com/symfony/Config).

Sezioni
-------

* :doc:`/components/config/resources`
* :doc:`/components/config/caching`
* :doc:`/components/config/definition`

.. _Packagist: https://packagist.org/packages/symfony/config
