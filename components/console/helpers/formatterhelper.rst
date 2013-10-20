.. index::
    single: Aiutanti della console; Aiutante Formatter

Aiutante Formatter
==================

L'aiutante Formatter fornisce funzioni per formattare l'output con colori.
Si possono fare cose più avanzate con questo aiutante rispetto a
:ref:`components-console-coloring`.

L'aiutante :class:`Symfony\\Component\\Console\\Helper\\FormatterHelper` è incluso
nell'insieme predefinito di aiutanti, ottenibile
richiamando :method:`Symfony\\Component\\Console\\Command\\Command::getHelperSet`::

    $formatter = $this->getHelperSet()->get('formatter');

I metodi restituiscono una stringa, che solitamente sarà resa nella console,
passandola al metodo
:method:`OutputInterface::writeln<Symfony\\Component\\Console\\Output\\OutputInterface::writeln>`.

Scrivere messaggi in una sezione
--------------------------------

Symfony offre uno stile definito per mostrare un messaggio che appartenga a una
"sezione". Mostra la sezione con un colore e contornata da parentesi e il
messaggio a destra di essa. A meno del colore, appare in questo modo:

.. code-block:: text

    [UnaSezione] Qui appare un messaggio correlato alla sezione

Per riprodurre questo stile, si può usare il metodo
:method:`Symfony\\Component\\Console\\Helper\\FormatterHelper::formatSection`::


    $formattedLine = $formatter->formatSection(
        'UnaSezione',
        'Qui appare un messaggio correlato alla sezione'
    );
    $output->writeln($formattedLine);
    
Scrivere messaggi in un blocco
------------------------------

A volte si vuole poter mostrare un intero blocco di testo con un colore di
sfondo. Symfony lo usa per mostrare messaggi di errore.

Se si mostrano messaggi di errore su più linee in modo manuale, si noterà
che lo sfondo ha lunghezza pari solo a ciascuna singola linea. Usare il metodo
:method:`Symfony\\Component\\Console\\Helper\\FormatterHelper::formatBlock`
per generare un blocco::

    $errorMessages = array('Errore!', 'Qualcosa è andato storto');
    $formattedBlock = $formatter->formatBlock($errorMessages, 'error');
    $output->writeln($formattedBlock);
    
Come si può vedere, passando un array di messaggi al metodo 
:method:`Symfony\\Component\\Console\\Helper\\FormatterHelper::formatBlock`,
si crea l'output desiderato. Se si passa ``true`` come terzo parametro, il
blocco sarà formattato con più padding (una linea vuota sopra e una sotto ai
messaggi e due spazi a sinistra e a destra).

Lo "stile" esatto usato nel blocco dipende dallo sviluppatore. In tal caso, 
è stato usato lo stile predefinito ``error``, ma ci sono altri stili e se ne possono
creare di propri. Vedere :ref:`components-console-coloring`.
