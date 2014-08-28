.. index::
    single: Estensioni; ExpressionLanguage

Estendere ExpressionLanguage
============================

Si può estendere ExpressionLanguage, aggiungendo funzioni. Per
esempio, nel framework Symfony, la sicurezza ha funzioni personalizzate per
verificare il ruolo dell'utente.

.. note::

    Se si vuole imparare come usare funzioni in un'espressione, leggere
    ":ref:`component-expression-functions`".

Registrare funzioni
-------------------

Le funzioni sono registrate su una specifica istanza di ``ExpressionLanguage``.
Questo vuole dire che le funzioni possono essere usate in qualsiasi espressione eseguita
da quella istanza.

Per registrare una funzione, usare
:method:`Symfony\\Component\\ExpressionLanguage\\ExpressionLanguage::register`.
Questo metodo ha tre parametri:

* **name** - il nome della funzione in un'espressione;
* **compiler** - una funzione eseguita quando si compila un'espressione usando la
  funzione;
* **evaluator** - una funzione eseguita quando l'espressione viene valutata.

.. code-block:: php

    use Symfony\Component\ExpressionLanguage\ExpressionLanguage;

    $language = new ExpressionLanguage();
    $language->register('lowercase', function ($str) {
        return sprintf('(is_string(%1$s) ? strtolower(%1$s) : %1$s)', $str);
    }, function ($arguments, $str) {
        if (!is_string($str)) {
            return $str;
        }

        return strtolower($str);
    });

    echo $language->evaluate('lowercase("HELLO")');

Questo mostrerà ``hello``. Al **compilatore** e al **valutatore** viene passata
come primo parametro una variabile ``arguments``, che è uguale al
secondo parametro per ``evaluate()`` o ``compile()`` (p.e. i "valori" quando
si valuta o i "nomi" se si compila).

Creare una nuova classe ExpressionLanguage
------------------------------------------

Quando si usa la classe ``ExpressionLanguage`` in una libreria, si raccomanda
di creare una nuova classe ``ExpressionLanguage`` e di registrarvi all'interno le funzioni.
Sovrascrivere ``registerFunctions`` per aggiungere funzioni::

    namespace Acme\AwesomeLib\ExpressionLanguage;

    use Symfony\Component\ExpressionLanguage\ExpressionLanguage as BaseExpressionLanguage;

    class ExpressionLanguage extends BaseExpressionLanguage
    {
        protected function registerFunctions()
        {
            parent::registerFunctions(); // non dimenticare di registrare anche funzioni del nucleo

            $this->register('lowercase', function ($str) {
                return sprintf('(is_string(%1$s) ? strtolower(%1$s) : %1$s)', $str);
            }, function ($arguments, $str) {
                if (!is_string($str)) {
                    return $str;
                }

                return strtolower($str);
            });
        }
    }
