.. index::
   single: Controller

Il controllere
==============

Un controllere è una funzione PHP che bisogna creare, che prende le informazioni dalla
richiesta HTTP e costruttori e restituisce una risposta HTTP (come oggetto
``Response`` di Symfony2). La risposta potrebbe essere una pagina HTML, un documento XML,
un array serializzato JSON, una immagine, una redirezione, un errore 404 o qualsiasi altra cosa
possa venire in mente. Il controllore contiene una qualunque logica arbitraria di cui la
*propria applicazione* necessita per rendere il contenuto di una pagina.

Per vedere come questo è semplice, diamo un'occhiata  ad un controllere di Symfony2 in azione.
Il seguente controllore renderebbe una pagina che stampa semplicemente ``Ciao mondo!``::

    use Symfony\Component\HttpFoundation\Response;

    public function helloAction()
    {
        return new Response('Ciao mondo!');
    }

L'obiettivo di un controllore è sempre lo stesso: creare e restituire un oggetto
``Response``. Lungo il percorso, potrebbe leggere le informazioni dalla richiesta, caricare una
risorsa da un database, inviare una e-mail, o impostare informazioni sulla sessione dell'utente.
Ma in ogni caso, il controllere alla fine restituirà un oggetto ``Response``
che verrà restituito al client.
	
Non c'è nessuna magia e nessun altro requisito di cui preoccuparsi! Di seguito alcuni
esempi comuni:

* Il *controllore A* prepara un oggetto ``Response`` che rappresenta il contenuto
  della homepage di un sito.

* Il *controllore B* legge il parametro ``slug`` da una richiesta per caricare un
  blog da un database e creare un oggetto ``Response`` che visualizza
  quel blog. Se lo ``slug`` non viene trovato nel database, crea e
  restituisce un oggetto ``Response`` con codice di stato 404.

* Il *controllore C* gestisce l'invio form di un form contatti. Legge le
  informazioni del form da dalla richiesta, salva le informazioni del contatto nel
  database ed invia una email con le informazioni del contatto al webmaster. Infine,
  crea un oggetto ``Response``che reindirizza il browser del client al
  alla pagina di ringraziamento del form contatti.

.. index::
   single: Controller; Request-controller-response lifecycle

Richieste, controllori, ciclo di vita della risposta
----------------------------------------------------

Ogni richiesta gestita da un progetto Symfony2 passa attraverso lo stesso semplice ciclo di vita.
Il framework si occupa dei compiti ripetitivi ed infine esegue un
controllore, che ospita il codice personalizzato dell'applicazione:

#. Ogni richiesta è gestita da un singolo file con il controllore principale (ad esempio ``app.php``
   o ``app_dev.php``) che è il bootstrap dell'applicazione;

#. Il ``Router`` legge le informazioni dalla richiesta (ad esempio l'URI), trova
   una rotta che corrisponde a tali informazioni e legge il parametro ``_controller``
   dalla rotta;

#. Viene eseguito il controllore della rotta corrispondente e il codice all'interno del
   controllore crea e restituisce un oggetto ``Response``;

#. Le intestazioni HTTP e il contenuto dell'oggetto ``Response`` vengono rispedite
   al client.

Creare una pagina è facile come creare un controllore (#3) e fare una rotta che
mappa un URL su un controllore (#2).

.. note::

    Anche se ha un nome simile, un "controllore principale" è diverso dagli altri
    "controllori" di cui si parla in questo capitolo. Un controllore principale
    è un breve file PHP che è presente nella vostra cartella web e sul quale sono
    dirette tutte le richieste. Una tipica applicazione avrà un controllore
    principale di produzione (ad esempio ``app.php``) e un controllore principale per lo sviluppo
    (ad esempio ``app_dev.php``). Probabilmente non si avrà mai bisogno di modificare, visualizzare o preoccuparsi
    del controllore principale dell'applicazione.

.. index::
   single: Controller; Simple example

Un semplice controllore
-----------------------

Mentre un controllore può essere un qualsiasi PHP callable (una funzione, un metodo di un oggetto,
o una ``Closure``), in Symfony2, un controllore di solito è un unico metodo all'interno
di un oggetto controllore. I controllori sono anche chiamati *azioni*.

.. code-block:: php
    :linenos:

    // src/Acme/HelloBundle/Controller/HelloController.php

    namespace Acme\HelloBundle\Controller;
    use Symfony\Component\HttpFoundation\Response;

    class HelloController
    {
        public function indexAction($name)
        {
          return new Response('<html><body>Ciao '.$name.'!</body></html>');
        }
    }


