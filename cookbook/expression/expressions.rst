.. index::
   single: Espressioni nel framework

Usare le espressioni in sicurezza, rotte, servizi e validazione
===============================================================

In Symfony 2.4 è stato aggiunto un potente componente, :doc:`ExpressionLanguage </components/expression_language/introduction>`.
Questo componente consente di aggiungere logiche altamente personalizzate
all'interno della configurazione.

Il framework Symfony sfrutta in modo predefinito le espressioni nei modi
seguenti:

* :ref:`Configurazione dei servizi <book-services-expressions>`;
* :ref:`Condizioni di corrispondenza delle rotte <book-routing-conditions>`;
* :ref:`Verifiche di sicurezza <book-security-expressions>` e
  :ref:`Controlli di accesso con allow_if <book-security-allow-if>`;
* :doc:`Validazione </reference/constraints/Expression>`.

Per maggiori informazioni su come creare e usare le espressioni, vedere
:doc:`/components/expression_language/syntax`.

.. _book-security-expressions:

Sicurezza: controlli complessi di accesso con espressioni
---------------------------------------------------------

Oltre a un ruolo, come ``ROLE_ADMIN``, il metodo ``isGranted`` accetta anche
un oggetto :class:`Symfony\\Component\\ExpressionLanguage\\Expression`::

    use Symfony\Component\ExpressionLanguage\Expression;
    // ...

    public function indexAction()
    {
        if (!$this->get('security.authorization_checker')->isGranted(new Expression(
            '"ROLE_ADMIN" in roles or (user and user.isSuperAdmin())'
        ))) {
            throw $this->createAccessDeniedException();
        }

        // ...
    }

In questo esempio, se l'utente corrente ha ``ROLE_ADMIN`` o se il
metodo ``isSuperAdmin()`` dell'oggetto utente restituisce ``true``, sarà garantito
l'accesso (nota: l'oggetto utente potrebbe non avere un metodo ``isSuperAdmin``,
che è un metodo inventato per questo esempio).

Si sta usando un'espressione. Per saperne di più sulla sintassi del linguaggio delle
espressioni, vedere :doc:`/components/expression_language/syntax`.

.. _book-security-expression-variables:

All'interno di un'espressione, si ha accesso ad alcune variabili:

``user``
    L'oggetto utente (o la stringa ``anon``, se non si è autenticati).
``roles``
    L'array dei ruoli dell'utente, inclusi quelli dalla
    :ref:`gerarchia dei ruoli <security-role-hierarchy>`, ma esclusi gli attributi
    ``IS_AUTHENTICATED_*`` (vedere le funzioni più avanti).
``object``
     L'eventuale oggetto passato come secondo paramentro di ``isGranted``.
``token``
    L'oggetto token.
``trust_resolver``
    L'oggetto :class:`Symfony\\Component\\Security\\Core\\Authentication\\AuthenticationTrustResolverInterface`: 
    probabilmente si useranno invece le funzioni ``is_*``.

Inoltre, all'interno di un'espressione si ha access ad alcune funzioni:

``is_authenticated``
    Restituisce ``true`` se l'utente è autenticato tramite "ricordami" o
    "pienamente", in pratica dice se l'utente è loggato.
``is_anonymous``
    Equivalente a usare ``IS_AUTHENTICATED_ANONYMOUSLY`` nella funzione ``isGranted``.
``is_remember_me``
    Simile, ma non uguale a ``IS_AUTHENTICATED_REMEMBERED``, vedere sotto.
``is_fully_authenticated``
    Simile, ma non uguale a ``IS_AUTHENTICATED_FULLY``, vedere sotto.
``has_role``
    Verificare se l'utente abbia il ruolo dato, equivalente a un'espressione come
    ``'ROLE_ADMIN' in roles``.

.. sidebar:: ``is_remember_me`` è diverso da ``IS_AUTHENTICATED_REMEMBERED``

    Le funzioni ``is_remember_me`` e ``is_authenticated_fully`` sono *simili*
    all'uso di ``IS_AUTHENTICATED_REMEMBERED`` e ``IS_AUTHENTICATED_FULLY``
    nella funzione ``isGranted``, ma **non** sono uguali. Ecco
    la differenza::

        use Symfony\Component\ExpressionLanguage\Expression;
        // ...

        $ac = $this->get('security.authorization_checker');
        $access1 = $ac->isGranted('IS_AUTHENTICATED_REMEMBERED');

        $access2 = $ac->isGranted(new Expression(
            'is_remember_me() or is_fully_authenticated()'
        ));

    Qui, ``$access1`` e ``$access2`` avranno lo stesso valore. Diversamente dal
    comportamento di ``IS_AUTHENTICATED_REMEMBERED`` e ``IS_AUTHENTICATED_FULLY``, la
    funzione ``is_remember_me`` restituisce ``true`` *solo* se l'utente è autenticato
    tramite un cookie "ricordami" e ``is_fully_authenticated`` *solo* se
    l'utente si è effettivamente loggatto nella sessione attuale.

