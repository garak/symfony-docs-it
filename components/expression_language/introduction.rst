.. index::
    single: Espressioni
    Single: Componenti; Expression Language

Il componente ExpressionLanguage
================================

    Il componente ExpressionLanguage fornisce un motore che compila e
    valuta espressioni. Un'espressione è una singola riga che restituisce un valore
    (soprattutto, ma non sempre, un booleano).

.. versionadded:: 2.4
    Il componente ExpressionLanguage è stato introdotto in Symfony 2.4.

Installazione
-------------

Si può installare il componente in due modi:

* Installarlo tramite :doc:`Composer </components/using_components>` (``symfony/expression-language`` su `Packagist`_);
* Usare il repository ufficiale Git (https://github.com/symfony/expression-language).

In che modo il linguaggio delle espressioni può essere utile?
-------------------------------------------------------------

Lo scopo del componente è consentire agli utenti di usare espressioni all'interno
della configurazione, per logiche più complesse. Per esempio, il framework Symfony2
usa espressioni nella sicurezza, per le regole di validazione e per la corrispondenza di rotte.

Oltre a usare il componente nel framework, il componente ExpressionLanguage
è un perfetto candidato per la fondazione di un *motore di regole di business*.
L'idea è quella di lasciare al webmaster di un sito la possibilità di configurare le cose in modo
dinamico, usando PHP e senza introdurre problemi di sicurezza:

.. _component-expression-language-examples:

.. code-block:: text

    # Ottieni il prezzo speciale se
    user.getGroup() in ['good_customers', 'collaborator']

    # Promuovi l'articolo in homepage quando
    article.commentCount > 100 and article.category not in ["misc"]

    # Invia un avviso quando
    product.stock < 15

Si possono vedere le espressioni come una sanbox PHP molto ristretta e immune a
intrusioni esterne, dovendo dichiarare esplicitamente quali variabili sono disponibili
in un'espressione.

Uso
---

Il componente ExpressionLanguage può compilare e valutare espressioni.
Le espressioni sono righe che spesso restituiscono un booleano, che può essere usato
nel codice che esegue l'espressione in un costrutto ``if`. Un semplice esempio
di espressione è ``1 + 2``. Si possono usare espressioni più complesse,
come ``unArray[3].unMetodo('pluto')``.

Il componente fornisce due modi di lavorare con le espressioni:

* **valuazione**: l'espressione viene valutata senza essere compilata in PHP;
* **compilazone**: l'espressione viene compilata in PHP, in modo da poter essere messa in cache e
  valutata.

La classe principale del componente è
:class:`Symfony\\Component\\ExpressionLanguage\\ExpressionLanguage`::

    use Symfony\Component\ExpressionLanguage\ExpressionLanguage;

    $language = new ExpressionLanguage();

    echo $language->evaluate('1 + 2'); // mostra 3

    echo $language->compile('1 + 2'); // mostra (1 + 2)

Sintassi delle espressioni
--------------------------

Vedere :doc:`/components/expression_language/syntax` per conoscere la sintassi del componente
ExpressionLanguage.

Passare variabili
-----------------

Si possono anche passare variabili dentro a un'espressione, le quali possono essere di qualsiasi tipo
valido in PHP (inclusi oggetti)::

    use Symfony\Component\ExpressionLanguage\ExpressionLanguage;

    $language = new ExpressionLanguage();

    class Apple
    {
        public $variety;
    }

    $apple = new Apple();
    $apple->variety = 'Honeycrisp';

    echo $language->evaluate(
        'fruit.variety',
        array(
            'fruit' => $apple,
        )
    );

Questo codice mostrerà "Honeycrisp". Per maggiori informazioni, vedere :doc:`/components/expression_language/syntax`,
in particolare :ref:`component-expression-objects` e :ref:`component-expression-arrays`.

Cache
-----

Il componente fornisce varie strategie di cache, si può approfondire
in :doc:`/components/expression_language/caching`.

.. _Packagist: https://packagist.org/packages/symfony/expression-language
