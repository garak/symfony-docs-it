.. index::
   single: API stabile

.. _the-symfony2-stable-api:

L'API stabile di Symfony2
=========================

L'API stabile di Symfony2 è un sottoinsieme di tutti i metodi pubblici di Symfony2
(componenti e bundle del nucleo) che condividono le seguenti proprietà:

* Lo spazio dei nomi e il nome della classe non cambieranno;
* Il nome del metodo non cambierà;
* La firma del metodo (i tipi dei parametri e del valore restituito) non cambierà;
* La semantica di quello che fa il metodo non cambierà;

Tuttavia potrebbe cambiare l'implementazione. L'unico caso valido per una modifica
dell'API stabile è la soluzone di una questione di sicurezza.

L'API stabile è basata su una lista, con il tag `@api`. Quindi,
tutto ciò che non possiede esplicitamente il tag non fa parte dell'API stabile.

.. tip::

    Si può approfondire l'API stabile in :doc:`/contributing/code/bc`.

.. tip::

    Ogni bundle di terze parti dovrebbe a sua volta pubblicare la sua API stabile.

A partire da Symfony 2.0, i seguenti componenti hanno un tag API pubblico:

* BrowserKit
* ClassLoader
* Console
* CssSelector
* DependencyInjection
* DomCrawler
* EventDispatcher
* Filesystem (da Symfony 2.1)
* Finder
* HttpFoundation
* HttpKernel
* Process
* Routing
* Templating
* Translation
* Validator
* Yaml
