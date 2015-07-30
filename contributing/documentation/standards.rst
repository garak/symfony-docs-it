Standard di documentazione
==========================

Per aiutare il più possibile il lettore e per creare esempi di codice che
sembrino familiare, si devono seguire i seguenti standard.

Sphinx
------

* Per i vari livelli di intestazione vanno usati i segeuenti caratteri: livello 1
  è ``=`` (simbolo di uguale), livello 2 ``-`` (trattino), livello 3 ``~`` (tilde), livello 4
  ``.`` (punto) e livello 5 ``"`` (apice doppio);
* Ogni riga va interrotta approssimativamente dopo la prima parola che attraversa
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

    Quando si lavora sulla documentazione, si devono seguire gli
    standard della `documentazione di Symfony`_.

    Livello 2
    ---------

    Un esempio in PHP può essere::

        echo 'Ciao mondo';

    Livello 3
    ~~~~~~~~~

    .. code-block:: php

        echo 'Qui non si può usare la scorciatoia ::';

    .. _`documentazione di Symfony`: http://symfony.com/doc/current/contributing/documentation/standards.html

Esempi di codice
----------------

* Il codice segue gli :doc:`standard di codice di Symfony </contributing/code/standards>`
  e gli `standard di codice di Twig`_;
* Gli esempi di codice devono sembrare reali per il contesto di un'applicazione web. Evitare esempi
  astratti o banali (``pippo``, ``pluto``, ``demo``, ecc.);
* Il codice deve seguire le :doc:`best practice di Symfony </best_practices/introduction>`.
  A meno che l'esempio non richieda un bundle personalizzato, assicurarsi di usare sempre
  ``AppBundle`` per il codice;
* Usare ``Acme``, quando il codice richiede un nome di venditore;
* Per evitare le barre orizzontali sui blocchi di codici, si preferisce interrompere una riga
  se va oltre l'85esimo carattere;
* Quando si omettono righe di codice, porre ``...`` in un commento nel punto
  di omissione. I commenti sono: ``// ...`` (php), ``# ...`` (yaml/bash), ``{# ... #}``
  (twig), ``<!-- ... -->`` (xml/html), ``; ...`` (ini), ``...`` (testo);
* Quando si omette una parte di riga, p.e. il valore di una variabile, porre ``...`` (senza commenti)
  nel punto di omissione;
* Descrizione del codice omesso (facoltativa):

  * Se si omettono molte righe: la descrizione dell'omissione può essere posta dopo ``...``;
  * Se si omette parte di una riga: la descrizione può essere posta prima della riga;

* Se utile per il lettore, un esempio di codice PHP dovrebbe iniziare con la dichiarazione dello
  spazio dei nomi;
* Facendo riferimento a una classe, assicurarsi di mostrare l''istruzione ``use`` in cima
  al blocco di codice. Non occorre mostrare *tutte* le istruzioni ``use``
  in ogni esempio, basta mostrare quelle usate effettivamente nel blocco di codice;
* Se utile, un ``codeblock`` dovrebbe iniziare con un commento contenente il nome del
  file nel blocco di codice. Inserire una riga vuota dopo il commento, a meno che la riga
  successiva non sia anch'essa un commento;
* Inserire il simbolo ``$`` all'inizio di ogni riga di bash.

Formati
~~~~~~~

Gli esempi di configurazione vanno mostrati in tutti i formati supportati, usando i
:ref:`blocchi di configurazione <docs-configuration-blocks>`. I formati supportati
(in ordine) sono:

* **Configurazione** (inclusi servizi e rotte): YAML, XML, PHP
* **Rotte**: Annotazioni, YAML, XML, PHP
* **Validazione**: YAML, Annotazioni, XML, PHP
* **Mappatura Doctrine**: Annotazioni, YAML, XML, PHP
* **Traduzione**: XML, YAML, PHP

Esempio
~~~~~~~

.. code-block:: php

    // src/Pippo/Pluto.php
    namespace Pippo;

    use Acme\Demo\Gatto;
    // ...

    class Pluto
    {
        // ...

        public function pippo($pluto)
        {
            // imposta pippo con il valore di pluto
            $pippo = ...;

            $gatto = new Gatto($pippo);

            // ... verifica se $pluto ha il valore corretto

            return $pippo->paperino($pluto, ...);
        }
    }

.. caution::

    In YAML va messo uno spazio dopo ``{`` e prima di ``}`` (p.e. ``{ _controller: ... }``),
    ma non va fatto in Twig (p.e. ``{'ciao' : 'valore'}``).

File e cartelle
~~~~~~~~~~~~~~~

* Quando si fa riferimento a cartelle, aggiungere sempre una barra finale, per evitare confusioni
  con file normali (p.e. "eseguire lo script ``console``, che si trova nella
  cartella ``app/``").
* Quando si fa riferimento esplicito a estensioni di file, si dovrebbe includere il punto iniziale,
  per ogni estensione (p.e. "i file XML usano l'estensione ``.xml``").
* Quando si elenca una gerarchia di file e cartelle di Symfony, usare ``progetto/`` come
  cartella iniziale. P.e.

  .. code-block:: text

      progetto/
      ├─ app/
      ├─ src/
      ├─ vendor/
      └─ ...

Standard di linguaggio
----------------------

* Per le sezioni, usare la seguente regola per le maiuscole:
  La prima lettera in maiuscolo, poi tutte le lettere in minuscolo:
  Questo è un esempio di titolo
* Non usare la virgola prima della congiunzione;
* Si dovrebbe usare la forma impersonale, non *noi* o *voi* (quindi evitare il punto
  di vista in prima persona).



















.. _`documentazione di Sphinx`: http://sphinx-doc.org/rest.html#source-code
.. _`standard di codice di Twig`: http://twig.sensiolabs.org/doc/coding_standards.html





