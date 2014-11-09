.. index::
   single: Espressioni nel framework

Usare le espressioni in sicurezza, rotte, servizi e validazione
===============================================================

.. versionadded:: 2.4
    La funzionalità delle espressioni è stata introdotta in Symfony 2.4.

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
