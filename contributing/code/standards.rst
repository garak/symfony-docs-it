Standard del codice
===================

Contribuendo al codice di Symfony, bisogna seguire i suoi standard. Per farla
breve, ecco una regola d'oro: **imitare il codice esistente di Symfony**.
La maggior parte dei bundle e delle librerie open source usati da Symfony
segue le stesse linee guida.

Ricordare che il vantaggio principale degli standard è che ogni pezzo di codice
sembra familiare, non è che questo o quello siano più o meno leggibili.

Symfony segue gli standard definiti nei documenti `PSR-0`_, `PSR-1`_ e
`PSR-2`_.

Poiché un'immagine (o un po' di codice) vale più di mille parole, ecco un
breve esempio contenente la maggior parte delle caratteristiche descritte sotto:

.. code-block:: html+php

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
     * Dimostrazione degli standard del codice.
     */
    class FooBar
    {
        const UNA_COSTANTE = 42;

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
         * @param array  $options
         *
         * @return string|null Input trasformato
         *
         * @throws \RuntimeException
         */
        private function transformText($dummy, array $options = array())
        {
            $mergedOptions = array_merge(
                array(
                    'un_valore_predefinito' => 'valori',
                    'un_altro_valore_predefinito' => 'altri valori',
                ),
                $options
            );

            if (true === $dummy) {
                return;
            }

            if ('string' === $dummy) {
                if ('values' === $mergedOptions['un_valore_predefinito']) {
                    return substr($dummy, 0, 5);
                }

                return ucwords($dummy);
            }

            throw new \RuntimeException(sprintf('Opzione "%s" non riconosciuta', $dummy));
        }

        private function reverseBoolean($value = null, $theSwitch = false)
        {
            if (!$theSwitch) {
                return;
            }

            return !$value;
        }
    }

Struttura
---------

* Aggiungere un singolo spazio dopo ogni virgola delimitatrice;

* Aggiungere un singolo spazio intorno agli operatori (``==``, ``&&``, ...),
  tranne l'operatore di concatenazione (``.``);

* Mettere gli operatori unari (``!``, ``--``, ...) accanto alla variabile affetta;

* Aggiungere una virgola dopo ogni elemento di array multi-linea, anche dopo
  l'ultimo;

* Aggiungere una riga vuota prima dei ``return``, a meno che il ``return`` non sia
  da solo in un gruppo di istruzioni (come per esempio un ``if``);

* Usare le parentesi graffe per le strutture di controllo, indipendentemente dal numero
  di istruzioni contenute;

* Definire una classe per file (non si applica a classi private di aiutanti
  che non devono essere istanziate dall'esterno e quindi esulano dallo
  standard `PSR-0`_ e `PSR-4`_);

* Dichiarare le proprietà di una classe prima dei metodi;

* Dichiarare prima i metodi pubblici, poi quelli protetti e infine quelli privati.
  Fanno eccezione a questa regola il costruttore della classe e i metodi ``setUp`` e
  ``tearDown`` delle classi dei test di PHPUnit, che vanno dichiarati all'inizio, per
  aumentare la leggibilità;

* Usare le parentesi per istanziare le classi, indipendentemente dal numero di
  parametri del costruttore.

* Le stringhe dei messaggi di eccezione vanno concatenate usando :phpfunction:`sprintf`.

Convenzioni sui nomi
--------------------

* Usare camelCase, non i trattini bassi, per nomi di variabili, di funzioni
  e di metodi;

* Usare i trattini bassi per nomi di opzioni e parametri;

* Usare gli spazi dei nomi per tutte le classi;

* Aggiungere il prefisso ``Abstract`` alle classi astratte. Si noti che alcune vecchie classi di Symfony
  non seguono questa convenzione e non sono state rinominate per questioni di retrocompatibilità.
  Tuttavia, tutte le nuove classi astratte devono seguire questa convenzione;

* Aggiungere il suffisso ``Interface`` alle interfacce;

* Aggiungere il suffisso ``Trait`` ai trait;

* Aggiungere il suffisso ``Exception`` alle eccezioni;

* Usare caratteri alfanumerici e trattini bassi per i nomi di file;

* Per i tipi di PHPDocs e per i casting, usare ``bool`` (al posto di ``boolean``
  o ``Boolean``), ``int`` (al posto di ``integer``), ``float`` (al posto di
  ``double`` o ``real``);

* Non dimenticare di dare un'occhiata al documento più prolisso sulle :doc:`conventions`,
  per considerazioni più soggettive sulla nomenclatura.

.. _service-naming-conventions:

Convenzioni sui nomi dei servizi
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

* Il nome di un servizio contiene gruppi, separati da punti;

* L'alias DI del bundle è il primo gruppo (p.e. ``fos_user``);

* Usare lettere minuscole per nomi di servizi e parametri;

* Un nome di gruppo usa la notazione con trattini bassi;

Documentazione
--------------

* Aggiungere blocchi PHPDoc per ogni classe, metodo e funzione;

* Omettere il tag ``@return``, se il metodo non restituisce nulla;

* Le annotazioni ``@package`` e ``@subpackage`` non sono usate.

Licenza
-------

* Symfony è rilasciato sotto licenza MIT e il blocco della licenza deve essere presente
  in cima a ogni file PHP, prima dello spazio dei nomi.

.. _`PSR-0`: http://www.php-fig.org/psr/psr-0/
.. _`PSR-1`: http://www.php-fig.org/psr/psr-1/
.. _`PSR-2`: http://www.php-fig.org/psr/psr-2/
.. _`PSR-4`: http://www.php-fig.org/psr/psr-4/
