.. index::
   single: CssSelector
   single: Componenti; CssSelector

Il componente CssSelector
=========================

    Il componente CssSelector converte selettori CSS in espressioni XPath.

Installazione
-------------

Si può installare il componente in due modi:

* Installarlo via :doc:`Composer</components/using_components>` (``symfony/css-selector`` su `Packagist`_);
* Usare il repository ufficiale su Git (https://github.com/symfony/CssSelector).

.. include:: /components/require_autoload.rst.inc

Uso
---

Perché usare selettori CSS?
~~~~~~~~~~~~~~~~~~~~~~~~~~~

Quando si analizzano documenti HTML o XML, XPath è certamente il metodo
più potente.

Le espressioni XPath sono incredibilmente flessibili, quindi c'è quasi sempre
un'espressione XPath che troverò l'elemento richiesto. Sfortunatamente, possono
essere anche molto complicate e la curva di apprendimento è ripida. Anche operazioni
comuni (come trovare un elemento con una data classe) possono richiedere
espressioni lunghe e poco maneggevoli.

Molti sviluppatori, in particolare gli sviluppatori web, si trovano più a loro agio
nel cercare elementi tramite selettori CSS. Oltre a funzionare con i fogli di stile,
i selettori CSS sono usati da Javascript con la funzione ``querySelectorAll`` e
da famose librerie Javascript, come jQuery, Prototype e MooTools.

I selettori CSS sono meno potenti di XPath, ma molto più facili da scrivere, leggere
e capire. Essendo meno potenti, quasi tutti i selettori CSS possono essere convertiti
in equivalenti XPath. Queste espressioni XPath possono quindi essere usate con
altre funzioni e classi che usano XPath per trovare elementi in un
documento.

Il componente CssSelector
~~~~~~~~~~~~~~~~~~~~~~~~~

L'unico fine del componente è la conversione di selettori CSS nei loro equivalenti
XPath::

    use Symfony\Component\CssSelector\CssSelector;

    print CssSelector::toXPath('div.item > h4 > a');

Questo fornisce il seguente output:

.. code-block:: text

    descendant-or-self::div[contains(concat(' ',normalize-space(@class), ' '), ' item ')]/h4/a

Si può usare questa espressione, per esempio, con :phpclass:`DOMXPath` o
:phpclass:`SimpleXMLElement`, per trovare elementi in un documento.

.. tip::

    Il metodo :method:`Crawler::filter()<Symfony\\Component\\DomCrawler\\Crawler::filter>`
    usa il componente CssSelector per trovare elementi in base a un selettore CSS.
    Si veda :doc:`/components/dom_crawler` per ulteriori dettagli.

Limiti del componente CssSelector
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Non tutti i selettori CSS possono essere convertiti in equivalenti XPath.

Ci sono molti selettori CSS che hanno senso solo nel contesto di un
browser.

* selettori dei collegamenti: ``:link``, ``:visited``, ``:target``
* selettori basati su azioni dell'utente: ``:hover``, ``:focus``, ``:active``
* selettori dello stato dell'interfaccia: ``:enabled``, ``:disabled``, ``:indeterminate``
  (tuttavia, ``:checked`` e ``:unchecked`` sono disponibili)

Gli pseudo-elementi (``:before``, ``:after``, ``:first-line``,
``:first-letter``) non sono supportati, perché selezionano porzioni di testo, piuttosto
che elementi.

Diverse pseudo-classi non sono ancora supportate:

* ``*:first-of-type``, ``*:last-of-type``, ``*:nth-of-type``,
  ``*:nth-last-of-type``, ``*:only-of-type``. (funzionano con il nome di un elemento
  (p.e. ``li:first-of-type``) ma non con ``*``.

.. _Packagist: https://packagist.org/packages/symfony/css-selector
