.. index::
   single: Interno

Panoramica dell'interno
=======================

Questa sezione è una spiegazione approfondita dell'interno di Symfony2.

.. note::

    Occorre leggere questa sezione solo se si vuole comprendere come Symfony2
    lavori dietro le quinte, oppure se si vuole estendere Symfony2.

Il codice di Symfony2 è composto da diversi livelli indipendenti. Ogni livello
è costruito sulla base del precedente.

.. tip::

    L'auto-caricamento non viene gestito direttamente dal framework, ma
    indipendentemente con l'aiuto della classe
    :class:`Symfony\\Component\\HttpFoundation\\UniversalClassLoader` e
    del file ``src/autoload.php``. Leggere il :doc:`capitolo dedicato
    </guides/tools/autoloader>` per maggiori informazioni.

Il componente ``HttpFoundation``
--------------------------------

Il livello più profondo è il componente :namespace:`Symfony\\Component\\HttpFoundation`.
HttpFoundation fornisce gli oggetti principali necessari per trattare con HTTP. È
un'astrazione orientata gli oggetti di alcune funzioni e variabili native di PHP:

* La classe :class:`Symfony\\Component\\HttpFoundation\\Request` astrae le
  variabili globali principali di PHP, come ``$_GET``, ``$_POST``, ``$_COOKIE``,
  ``$_FILES`` e ``$_SERVER``;

* La classe :class:`Symfony\\Component\\HttpFoundation\\Response` astrae alcune
  funzioni PHP, come ``header()``, ``setcookie()`` e ``echo``;

* La classe :class:`Symfony\\Component\\HttpFoundation\\Session` e l'interfaccia
  :class:`Symfony\\Component\\HttpFoundation\\SessionStorage\\SessionStorageInterface`
  astraggono le funzioni di gestione della sessione ``session_*()``.

.. seealso::

    Approfondimento sul componente :doc:`HttpFoundation <http_foundation>`.

Il componente ``HttpKernel``
----------------------------

Sopra HttpFoundation c'è il componente :namespace:`Symfony\\Component\\HttpKernel`.
HttpKernel gestisce la parte dinamica di HTTP e incapsula in modo leggero
le classi della richiesta e della risposta, per standardizzare il modo in cui sono gestite
le richieste. Fornisce anche dei punti di estensione e degli strumenti che lo
rendono il punto di partenza ideale per creare un framework web senza troppo
overhead.

Opzionalmente, aggiunge anche configurabilità ed estensibilità, grazie al
componente Dependency Injection e a un potente sistema di plugin (bundle).

.. seealso::

    Approfondimento sul componente :doc:`HttpKernel <kernel>`. Approfondimento
    sul componente :doc:`Dependency Injection </guides/dependency_injection/index>`
    e sui :doc:`Bundle </guides/bundles/index>`.

Il bundle ``FrameworkBundle``
-----------------------------

Il bundle :namespace:`Symfony\\Bundle\\FrameworkBundle` è il bundle che accoppia
i componenti e le librerie principali, per creare un framework MVC leggero e
veloce. Ha una configurazione predefinita e delle convenzioni intelligenti, per
facilitare la curva di apprendimento.
