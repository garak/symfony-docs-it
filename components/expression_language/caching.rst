.. index::
    single: Cache; ExpressionLanguage

Cache di espressioni analizzate
===============================

Il componente ExpressionLanguage fornisce già un metodo
:method:`Symfony\\Component\\ExpressionLanguage\\ExpressionLanguage::compile`,
per consentire di mettere in cache le espressioni in puro PHP. Ma, internamente, il
componente già mette in cache le espressioni analizzate, quindi le espressioni duplicate
possono essere compilate e valutate più rapidamente.

Il flusso di lavoro
-------------------

I metodi :method:`Symfony\\Component\\ExpressionLanguage\\ExpressionLanguage::evaluate`
e ``compile()`` necessitano entrambi di fare alcune cose, prima di poter restituire
valori. Per ``evaluate()``, questo overhead è più grande.

Entrambi i metodi necessitano di spezzettare e analizzare l'espressione. Lo fanno tramite il metodo
:method:`Symfony\\Component\\ExpressionLanguage\\ExpressionLanguage::parse`.
Questo metodo restituisce una :class:`Symfony\\Component\\ExpressionLanguage\\ParsedExpression`.
Ora, il metodo ``compile()`` restituisce semplicemente la conversione in stringa di tale oggetto.
Il metodo ``evaluate()`` deve ciclare tra i "nodi" (i pezzi di
un'esoressione salvata in ``ParsedExpression``) e valutarli al volo.

Per risparmiare tempo, ``ExpressionLanguage`` mette in cache ``ParsedExpression``, in modo
da saltare i passi di spezzettamento e analisi con espressioni duplicate.
La cache è eseguita da un'istanza di
:class:`Symfony\\Component\\ExpressionLanguage\\ParserCache\\ParserCacheInterface`
(che usa un
:class:`Symfony\\Component\\ExpressionLanguage\\ParserCache\\ArrayParserCache`).
Si può personalizzare il comportamento, creando una ``ParserCache`` personalizzata e iniettandola
nell'oggetto, tramite costruttore::

    use Symfony\Component\ExpressionLanguage\ExpressionLanguage;
    use Acme\ExpressionLanguage\ParserCache\MyDatabaseParserCache;

    $cache = new MyDatabaseParserCache(...);
    $language = new ExpressionLanguage($cache);

.. note::

    `DoctrineBridge`_ fornisce un'implementazione di ParserCache, che usa la
    `libreria di cache di Doctrine`_, che fornisce cache per ogni sorta di strategia,
    come APC,filesystem e Memcached.

Uso di espressioni analizzate e serializzate
--------------------------------------------

Sia ``evaluate()`` sia ``compile()`` possono gestire ``ParsedExpression`` e
``SerializedParsedExpression``::

    use Symfony\Component\ExpressionLanguage\ParsedExpression;
    // ...

    $expression = new ParsedExpression($language->parse('1 + 4'));

    echo $language->evaluate($expression); // mostra 5

.. code-block:: php

    use Symfony\Component\ExpressionLanguage\SerializedParsedExpression;
    // ...

    $expression = new SerializedParsedExpression(
        serialize($language->parse('1 + 4'))
    );

    echo $language->evaluate($expression); // mostra 5

.. _DoctrineBridge: https://github.com/symfony/DoctrineBridge
.. _`libreria di cache di Doctrine`: http://docs.doctrine-project.org/projects/doctrine-common/en/latest/reference/caching.html
