Standard di documentazione
==========================

Per aiutare il più possibile il lettore e per creare esempi di codice che
sembrino familiare, si devono seguire i seguenti standard.

Sphinx
------

* Per i vari livelli di intestazione vanno usati i segeuenti caratteri: livello 1
  è ``=``, livello 2 ``-``, livello 3 ``~``, livello 4 ``.`` e livello 5 ``"``;
* Ogni riga va interrotta approssimativametne dopo la prima parola che attraversa
  il 72esimo carattere (quindi quasi tutte le righe dovrebbeero essere tra i 72 e i 78 caratteri);
* La scorciatoia ``::`` va *preferita* a ``.. code-block:: php``, quando si inizia un
  blocco di codice PHP (si legga la `documentazione di Sphinx`_ per vedere quando usare
  la scorciatoia);
* I collegamenti in linea **non** vanno usati. Separare il collegamento e la definizione della
  destinazione, che andrà aggiunta a fondo pagina;
* Va preferita la forma impersonale a quella personale.

Esempio
~~~~~~~

.. code-block:: text

    Esempio
    =======

    When you are working on the docs, you should follow the `Symfony Docs`_
    standards.

    Livello 2
    ---------

    A PHP example would be::

        echo 'Hello World';

    Livello 3
    ~~~~~~~~~

    .. code-block:: php

        echo 'You cannot use the :: shortcut here';

    .. _`Symfony Docs`: http://symfony.com/doc/current/contributing/documentation/standards.html

Esempi di codice
----------------

* Il codice segue gli :doc:`standard di codice di Symfony</contributing/code/standards>`
  e gli `standard di codice di Twig`_;
* Per evitare le barre orizzontali sui blocchi di codici, si preferisce interrompere una riga
  se va oltre l'85esimo carattere;
* Quando si omettono righe di codice, porre ``...`` in un commento nel punto
  di omissione. I commenti sono: ``// ...`` (php), ``# ...`` (yaml/bash), ``{# ... #}``
  (twig), ``<!-- ... -->`` (xml/html), ``; ...`` (ini), ``...`` (testo);
* Quando si omette una parte di riga, p.e. il valore di una variabile, porre ``...`` (senza commenti)
  nel punto di omissione;
* Descrizione del codice omesso (facoltativa):
  se si omettono molte righe: la descrizione dell'omissione può essere posta dopo ``...``
  se si omette parte di una riga: la descrizione può essere posta prima della riga;
* Se utile, un ``codeblock`` dovrebbe iniziare con un commento contenente il nome del
  file nel blocco di codce. Inserire una riga vuota dopo il commento, a meno che la riga
  successiva non sia anch'essa un commento;
* Inserire il simbolo ``$`` all'inizio di ogni riga di bash;

Formati
~~~~~~~

Gli esempi di configurazione vanno mostrati in tutti i formati supportati, usando i
:ref:`blocchi di configurazione <docs-configuration-blocks>`. I formati supportati
(in ordine) sono:

* **Configurazione** (inclusi servizis e rotte): Yaml, Xml, Php
* **Validazione**: Yaml, Annotazioni, Xml, Php
* **Mappatura Doctrine**: Annotazioni, Yaml, Xml, Php

Esempio
~~~~~~~

.. code-block:: php

    // src/Foo/Bar.php

    // ...
    class Bar
    {
        // ...

        public function foo($bar)
        {
            // set foo with a value of bar
            $foo = ...;

            // ... check if $bar has the correct value

            return $foo->baz($bar, ...);
        }
    }

.. caution::

    In Yaml va messo uno spazio dopo ``{`` e prima di ``}`` (p.e. ``{ _controller: ... }``),
    ma non va fatto in Twig (p.e.  ``{'hello' : 'value'}``).

.. _`documentazione di Sphinx`: http://sphinx-doc.org/rest.html#source-code
.. _`standard di codice di Twig`: http://twig.sensiolabs.org/doc/coding_standards.html
