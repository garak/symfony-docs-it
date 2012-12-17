Standard del codice
===================

Contribuendo al codice di Symfony2, bisogna seguire i suoi standard. Per farla
breve, ecco una regola d'oro: **imitare il codice esistente di Symfony2**.
La maggior parte dei bundle e delle librerie open source usati da Symfony2
segue le stesse linee guida.

Ricordare che il vantaggio principale degli standard è che ogni pezzo di codice
sembra familiare, non è che questo o quello siano più o meno leggibili.

Symfony segue gli standard definiti nei documenti `PSR-0`_, `PSR-1`_ e
`PSR-2`_.

Poiché un'immagine (o un po' di codice) vale più di mille parole, ecco un
breve esempio contenente la maggior parte delle caratteristiche descritte sotto:

.. code-block:: php+html

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

    /**
     * Coding standards demonstration.
     */
    class FooBar
    {
        const SOME_CONST = 42;

        private $fooBar;

        /**
         * @param string $dummy Una descrizione del parametro
         */
        public function __construct($dummy)
        {
            $this->fooBar = $this->transformText($dummy);
        }

        /**
         * @param string $dummy Una descrizione del parametro
         *
         * @return string|null Input trasformato
         */
        private function transformText($dummy, $options = array())
        {
            $mergedOptions = array_merge(
                $options,
                array(
                    'some_default' => 'values',
                    'another_default' => 'more values',
                )
            );

            if (true === $dummy) {
                return;
            }
            if ('string' === $dummy) {
                if ('values' === $mergedOptions['some_default']) {
                    $dummy = substr($dummy, 0, 5);
                } else {
                    $dummy = ucwords($dummy);
                }
            }

            return $dummy;
        }
    }

Struttura
---------

* Aggiungere un singolo spazio dopo ogni virgola delimitatrice;

* Aggiungere un singolo spazio intorno agli operatori (`==`, `&&`, ...);

* Aggiungere una riga vuota prima delle istruzioni `return`, a meno che non siano soli 
  dentro una struttura di controllo (come un `if`);

* Usare le parentesi graffe per le strutture di controllo, indipendentemente dal numero
  di istruzioni contenute;

* Definire una classe per file (non si applica a classi private di helper
  che non devono essere istanziate dall'esterno e quindi esulano dallo
  standard PSR-0);

* Dichiarare le proprietà di una classe prima dei metodi;

* Dichiarare prima i metodi pubblici, poi quelli protetti e infine quelli privati;

Convenzioni sui nomi
--------------------

* Usare camelCase, non i trattini bassi, per nomi di variabili, di funzioni
  e di metodi;

* Usare i trattini bassi per nomi di opzioni e parametri;

* Usare gli spazi dei nomi per tutte le classi;

* Le classi astratte spesso hanno prefisso ``Abstract``;

* Aggiungere il suffisso ``Interface`` alle interfacce;

* Aggiungere il suffisso ``Trait`` ai trait;

* Usare caratteri alfanumerici e trattini bassi per i nomi di file;

* Non dimenticare di dare un'occhiata al documento più prolisso sulle :doc:`conventions`,
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

.. _`PSR-0`: https://github.com/php-fig/fig-standards/blob/master/accepted/PSR-0.md
.. _`PSR-1`: https://github.com/php-fig/fig-standards/blob/master/accepted/PSR-1-basic-coding-standard.md
.. _`PSR-2`: https://github.com/php-fig/fig-standards/blob/master/accepted/PSR-2-coding-style-guide.md
