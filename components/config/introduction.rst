.. index::
   single: Config
   single: Componenti; Config

Il componente Config
====================

    Il componente Config fornisce diverse classi che aiutano a trovare, caricare, combinare,
    riempire e validare valori di configurazione di ogni tipo, indipendentemente dal tipo
    di sorgente (file YAML, XML o INI, oppure ad esempio una base dati).

.. caution::

    ``IniFileLoader`` analizza i contenuti dei file usando la funzione
    :phpfunction:`parse_ini_file`, quindi si possono impostare solamente
    parametri stringa. Per impostare tipi di versi di parametri
    (p.e. booleani, interi, ecc), si raccomanda l'uso di altri caricatori.

Installazione
-------------

Si può installare il componente in molti modi diversi:

* :doc:`Installarlo tramite Composer </components/using_components>` (``symfony/config`` su `Packagist`_);
* Usare il repository ufficiale su Git (https://github.com/symfony/Config).

Sezioni
-------

* :doc:`/components/config/resources`
* :doc:`/components/config/caching`
* :doc:`/components/config/definition`

.. _Packagist: https://packagist.org/packages/symfony/config
