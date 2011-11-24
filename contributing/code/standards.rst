Standard del codice
===================

Contribuendo al codice di Symfony2, bisogna seguire i suoi standard. Per farla
breve, ecco una regola d'oro: **imitare il codice esistente di Symfony2**.
La maggior parte dei bundle e delle librerie open source usati da Symfony2
segue le stesse linee guida.

Ricordare che il vantaggio principale degli standard è che ogni pezzo di codice
sembra familiare, non è che questo o quello siano più o meno leggibili.

Poiché un'immagine (o un po' di codice) vale più di mille parole, ecco un
breve esempio contenente la maggior parte delle caratteristiche descritte sotto:

.. code-block:: php

    <?php

    /*
     * This file is part of the Symfony package.
     *
     * (c) Fabien Potencier <fabien@symfony.com>
     *
     * For the full copyright and license information, please view the LICENSE
     * file that was distributed with this source code.
     */

    namespace Acme;

    class Foo
    {
        const SOME_CONST = 42;

        private $foo;

        /**
         * @param string $dummy Some argument description
         */
        public function __construct($dummy)
        {
            $this->foo = $this->transform($dummy);
        }

        /**
         * @param string $dummy Some argument description
         * @return string|null Transformed input
         */
        private function transform($dummy)
        {
            if (true === $dummy) {
                return;
            }
            if ('string' === $dummy) {
                $dummy = substr($dummy, 0, 5);
            }

            return $dummy;
        }
    }

Struttura
---------

* Non usare mai gli short tag (`<?`);

* Non terminare i file delle classi con il tag di chiusura `?>`;

* Indentare con quattro spazi (mai con le tabulazioni);

* Usare il carattere linefeed (`0x0A`) per terminare le righe;

* Aggiungere un singolo spazio dopo ogni virgola delimitatrice;

* Non mettere spazi dopo una parentesi chiusa e prima di una aperta;

* Aggiungere un singolo spazio intorno agli operatori (`==`, `&&`, ...);

* Aggiungere un singolo spazio prima di aprire le parentesi di una struttura di controllo 
  (`if`, `else`, `for`, `while`, ...);

* Aggiungere una riga vuota prima delle istruzioni `return`, a meno che non siano soli 
  dentro una struttura di controllo (come un `if`);

* Non aggiungere spazi finali in fondo alle righe;

* Usare le parentesi graffe per indicare una struttura di controllo, indipendentemente
  dal numero di istruzioni contenute;

* Inserire le parentesi graffe su una nuova riga per classi, metodi e
  funzioni;

* Separare le istruzioni condizionali (`if`, `else`, ...) e la parentesi graffa di
  apertura con un singolo spazio e senza nuove righe;

* Dichiarare esplicitamente la visibilità di classi, metodi e proprietà (non usare
  `var`);

* Usare le costanti native di PHP in minuscolo: `false`, `true` e `null`. Lo
  stesso per `array()`;

* Usare stringhe maiuscole per le costanti, con parole separate da trattini bassi;

* Definire una classe per file;

* Dichiarare le proprietà di una classe prima dei metodi;

* Dichiarare prima i metodi pubblici, poi quelli protetti e infine quelli privati;

Convenzioni sui nomi
--------------------

* Usare camelCase, non i trattini bassi, per nomi di variabili, di funzioni
  e di metodi;

* Usare i trattini bassi per nomi di opzioni e parametri;

* Usare gli spazi dei nomi per tutte le classi;

* Aggiungere il suffisso `Interface` alle interfacce;

* Usare caratteri alfanumerici e trattini bassi per i nomi di file;

* Non dimenticare di dare un'occhiata al documento più prolisso sulle :doc:`convenzioni`,
  per considerazioni più soggettive sulla nomenclatura.

Documentazione
--------------

* Aggiungere blocchi PHPDoc per ogni classe, metodo e funzione;

* Omettere il tag `@return`, se il metodo non restituisce nulla;

* Le annotazioni `@package` e `@subpackage` non sono usate.

Licenza
-------

* Symfony è rilasciato sotto licenza MIT e il blocco della licenza deve essere presente
  in cima a ogni file PHP, prima dello spazio dei nomi.
