.. index::
    single: Form; Eventi dei form

Eventi dei form
===============

Il componente Form fornisce un processo strutturato, che consente di personalizzare
i form, facendo uso del componente 
:doc:`EventDispatcher </components/event_dispatcher/introduction>`.
Usando gli eventi dei form, si possono modificare informazioni o campi in
vari punti del flusso: dal popolamento del form all'invio
dei dati dalla richiesta.

La registrazione di ascoltatori di eventi è molto facile, usando il componente Form.

Per esempio, se si vuole registrare una funzione all'evento
``FormEvents::PRE_SUBMIT``, il codice seguente consente di aggiungere un campo,
a seconda dei valori della richiesta::

    // ...

    use Symfony\Component\Form\FormEvent;
    use Symfony\Component\Form\FormEvents;

    $listener = function (FormEvent $event) {
        // ...
    };

    $form = $formFactory->createBuilder()
        // aggiunge campi al form
        ->addEventListener(FormEvents::PRE_SUBMIT, $listener);

    // ...

Il flusso del form
------------------

Il flusso di invio del form
~~~~~~~~~~~~~~~~~~~~~~~~~~~

.. image:: /images/components/form/general_flow.png
    :align: center

1) Pre-popolare il form (``FormEvents::PRE_SET_DATA`` e ``FormEvents::POST_SET_DATA``)
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

.. image:: /images/components/form/set_data_flow.png
    :align: center

Durante il pre-popolamento di un form, sono distribuiti due eventi, al richiamo del metodo
:method:`Form::setData() <Symfony\\Component\\Form\\Form::setData>`:
``FormEvents::PRE_SET_DATA`` e ``FormEvents::POST_SET_DATA``.

A) L'evento ``FormEvents::PRE_SET_DATA``
........................................

L'evento ``FormEvents::PRE_SET_DATA`` è distribuito all'inizio del metodo
``Form::setData()``. Può essere usato per:

* Modificare i dati forniti durante il pre-popolamento;
* Modificare un form a seconda dei dati di pre-popolamento (aggiunta o rimozione dinamica di campi).

================= ========
Tipo di dati      Valore
================= ========
Dati modello      ``null``
Dati normalizzati ``null``
Dati vista        ``null``
================= ========

.. seealso::

    Si possono vedere tutti gli eventi insieme nella
    :ref:`tabella degli eventi dei form <component-form-event-table>`.

.. caution::

    Durante ``FormEvents::PRE_SET_DATA``,
    :method:`Form::setData() <Symfony\\Component\\Form\\Form::setData>`
    è bloccato e lancerà un'eccezione, se usato. Se si vogliono modificare
    i dati, usare invece
    :method:`FormEvent::setData() <Symfony\\Component\\Form\\FormEvent::setData>`.


.. sidebar:: ``FormEvents::PRE_SET_DATA`` nel componente Form

    Il tipo di form ``collection`` si appoggia al sottoscrittore
    :class:`Symfony\\Component\\Form\\Extension\\Core\\EventListener\\ResizeFormListener`,
    ascoltando l'evento ``FormEvents::PRE_SET_DATA`` per poter riordinare
    i campi del form, in base ai dati pre-popolati
    dell'oggetto, rimuovendo e aggiungendo tutte le righe del form.

B) L'evento ``FormEvents::POST_SET_DATA``
.........................................

L'evento ``FormEvents::POST_SET_DATA`` è distribuito alla fine del metodo
:method:`Form::setData() <Symfony\\Component\\Form\\Form::setData>`.
Questo evento per lo più serve a leggere dati dopo aver pre-popolato
il form.

=================  ============================================================
Tipo di dati       Valore
=================  ============================================================
Dati modello       Dati del modello iniettati in ``setData()``
Dati normalizzati  Dati del modello trasformati con un trasformatore di modello
Dati vista         Dati normalizzati trasformati con un trasformatore di vista
=================  ============================================================

.. seealso::

    Si possono vedere tutti gli eventi insieme nella
    :ref:`tabella degli eventi dei form <component-form-event-table>`.

.. sidebar:: ``FormEvents::POST_SET_DATA`` nel componente Form

    La classe :class:`Symfony\\Component\\Form\\Extension\\DataCollector\\EventListener\\DataCollectorListener`
    ascolta l'evento ``FormEvents::POST_SET_DATA``,
    per poter raccogliere informazioni sui form dal modello denormalizzato
    e dai dati della vista.

2) Inviare un form (``FormEvents::PRE_SUBMIT``, ``FormEvents::SUBMIT`` e ``FormEvents::POST_SUBMIT``)
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

.. image:: /images/components/form/submission_flow.png
    :align: center

Tre eventi sono distribuiti quando
:method:`Form::handleRequest() <Symfony\\Component\\Form\\Form::handleRequest>`
o :method:`Form::submit() <Symfony\\Component\\Form\\Form::submit>` vengono
richiamati: ``FormEvents::PRE_SUBMIT``, ``FormEvents::SUBMIT``,
``FormEvents::POST_SUBMIT``.

A) L'evento ``FormEvents::PRE_SUBMIT``
......................................

L'evento ``FormEvents::PRE_SUBMIT`` è distribuito all'inizio del metodo
:method:`Form::submit() <Symfony\\Component\\Form\\Form::submit>`.

Può essere usato per:

* Cambiare i dati dalla richiesta, prima di inviare i dati al form.
* Aggiungere i rimovere campi dal form, prima di inviare i dati al form.

=================  =======================================
Tipo di dati       Valore
=================  =======================================
Dati modello       Come in ``FormEvents::POST_SET_DATA``
Dati normalizzati  Come in ``FormEvents::POST_SET_DATA``
Dati vista         Come in ``FormEvents::POST_SET_DATA``
=================  =======================================

.. seealso::

    Si possono vedere tutti gli eventi insieme nella
    :ref:`tabella degli eventi dei form <component-form-event-table>`.

.. sidebar:: ``FormEvents::PRE_SUBMIT`` nel componente Form

    Il sottoscrittore :class:`Symfony\\Component\\Form\\Extension\\Core\\EventListener\\TrimListener`
    ascolta l'evento ``FormEvents::PRE_SUBMIT``, per poter applicare un trim
    ai dati della richiesta (per valori stringa).
    Il sottoscrittore :class:`Symfony\\Component\\Form\\Extension\\Csrf\\EventListener\\CsrfValidationListener`
    ascolta l'evento ``FormEvents::PRE_SUBMIT``, per poter
    validare il token CSRF.

B) L'evento ``FormEvents::SUBMIT``
..................................

L'evento ``FormEvents::SUBMIT`` è distribuito subito prima che il metodo
:method:`Form::submit() <Symfony\\Component\\Form\\Form::submit>`
ritrasformi i dati normalizzati in dati di modello e di vista.

Può essere usato per cambiare dati dalla rappresentazione normalizzata dei dati.

=================  ===================================================================
Tipo di dati       Valore
=================  ===================================================================
Dati modello       Come in ``FormEvents::POST_SET_DATA``
Dati normalizzati  Dati ritrasformati dalla richiesta usando un trasformatore di vista
Dati vista         Come in ``FormEvents::POST_SET_DATA``
=================  ===================================================================

.. seealso::

    Si possono vedere tutti gli eventi insieme nella
    :ref:`tabella degli eventi dei form <component-form-event-table>`.

.. caution::

    A questo punto, non si possono aggiungere o rimuovere campi dal form.

.. sidebar:: ``FormEvents::SUBMIT`` nel componente Form

    :class:`Symfony\Component\Form\Extension\Core\EventListener\ResizeFormListener`
    ascolta l'evento ``FormEvents::SUBMIT`` per poter rimuovere i
    campi che devono essere rimossi, se è stata abilitata la manipolazione di collezioni di form
    tramite ``allow_delete``.

C) L'evento ``FormEvents::POST_SUBMIT``
.......................................

L'evento ``FormEvents::POST_SUBMIT`` è distribuito dopo
:method:`Form::submit() <Symfony\\Component\\Form\\Form::submit>`, una volta che
i dati di modello e vista sono stati denormalizzati.

Può essere usato per recuperare dati dopo la denormalizzazione.

=================  ===================================================================
Tipo di dati       Valore
=================  ===================================================================
Dati modello       Dati normalizzati ritrasformati usando un trasformatore di modello
Dati normalizzati  Come in ``FormEvents::POST_SUBMIT``
Dati vista         Dati normalizzati trasformati usando un trasformatore di vista
=================  ===================================================================

.. seealso::

    Si possono vedere tutti gli eventi insieme nella
    :ref:`tabella degli eventi dei form <component-form-event-table>`.

.. caution::

    A questo punto, non si possono aggiungere o rimuovere campi dal form.

.. sidebar:: ``FormEvents::POST_SUBMIT`` nel componente Form

    :class:`Symfony\Component\Form\Extension\DataCollector\EventListener\DataCollectorListener`
    ascolta l'evento ``FormEvents::POST_SUBMIT``, per poter raccogliere
    informazioni sui form.
    :class:`Symfony\Component\Form\Extension\Validator\EventListener\ValidationListener`
    ascolta l'evento ``FormEvents::POST_SUBMIT``, per poter
    validare automaticamente l'oggetto denormalizzato e aggiornare la rappresentazione normalizzata
    e quella della vista.

Registrare ascoltatori o sottoscrittori di eventi
-------------------------------------------------

Per poter usare gli eventi dei form, occorre creare un ascoltatore di eventi
o un sottoscrittore di eventi, quindi fargli ascoltare un evento.

Il nome di ogni evento è definito come costante della classe
:class:`Symfony\\Component\\Form\\FormEvents`.
Inoltre, ciascun callback dell'evento (metodo ascoltatore o sottoscrittore) riceve un
singolo parametro, che è un'istanza di
:class:`Symfony\\Component\\Form\\FormEvent`. L'oggetto evento contiene un
riferimento allo stato corrente del form e ai dati correnti in corso
di processamento.

.. _component-form-event-table:

======================  =============================  ===============
Nome                    Costante ``FormEvents``        Dati evento
======================  =============================  ===============
``form.pre_set_data``   ``FormEvents::PRE_SET_DATA``   Dati modello
``form.post_set_data``  ``FormEvents::POST_SET_DATA``  Dati modello
``form.pre_bind``       ``FormEvents::PRE_SUBMIT``     Dati richiesta
``form.bind``           ``FormEvents::SUBMIT``         Dati normalizzati
``form.post_bind``      ``FormEvents::POST_SUBMIT``    Dati vista
======================  =============================  ===============

.. versionadded:: 2.3
    Prima di Symfony 2.3, ``FormEvents::PRE_SUBMIT``, ``FormEvents::SUBMIT``
    e ``FormEvents::POST_SUBMIT`` si chiamavano ``FormEvents::PRE_BIND``,
    ``FormEvents::BIND`` e ``FormEvents::POST_BIND``.

.. caution::

    Le costanti ``FormEvents::PRE_BIND``, ``FormEvents::BIND`` e
    ``FormEvents::POST_BIND`` saranno rimosse nella versione 3.0 di
    Symfony.
    I nomi degli eventi mantengono i valori originali, quindi assicurarsi di usare
    le costanti ``FormEvents``, per compatibilità futura.

Ascoltatori di eventi
~~~~~~~~~~~~~~~~~~~~~

Un ascoltatore di eventi può essere un qualsiasi tipo di callable valido.

Creare un ascoltatore di eventi e legarlo al form è molto facile::

    // ...

    use Symfony\Component\Form\FormEvent;
    use Symfony\Component\Form\FormEvents;

    $form = $formFactory->createBuilder()
        ->add('username', 'text')
        ->add('show_email', 'checkbox')
        ->addEventListener(FormEvents::PRE_SUBMIT, function (FormEvent $event) {
            $user = $event->getData();
            $form = $event->getForm();

            if (!$user) {
                return;
            }

            // Verifica se l'utente ha scelto di mostrare la sua email.
            // Se i dati sono stati già inviati, il valore addizionale che è incluso
            // nelle variabili della richiesta va rimosso.
            if (true === $user['show_email']) {
                $form->add('email', 'email');
            } else {
                unset($user['email']);
                $event->setData($user);
            }
        })
        ->getForm();

    // ...

dopo aver creato una classe tipo form, si può usare uno dei suoi metodi come
callback, per maggiore leggibilità::

    // ...

    class SubscriptionType extends AbstractType
    {
        public function buildForm(FormBuilderInterface $builder, array $options)
        {
            $builder->add('username', 'text');
            $builder->add('show_email', 'checkbox');
            $builder->addEventListener(
                FormEvents::PRE_SET_DATA,
                array($this, 'onPreSetData')
            );
        }

        public function onPreSetData(FormEvent $event)
        {
            // ...
        }
    }

Sottoscrittori di eventi
~~~~~~~~~~~~~~~~~~~~~~~~

I sottoscrittori di eventi hanno vari usi:

* Migliorare la leggibilità;
* Ascoltare più eventi;
* Raggruppare più ascoltatori in una singola classe.

.. code-block:: php

    use Symfony\Component\EventDispatcher\EventSubscriberInterface;
    use Symfony\Component\Form\FormEvent;
    use Symfony\Component\Form\FormEvents;

    class AddEmailFieldListener implements EventSubscriberInterface
    {
        public static function getSubscribedEvents()
        {
            return array(
                FormEvents::PRE_SET_DATA => 'onPreSetData',
                FormEvents::PRE_SUBMIT   => 'onPreSubmit',
            );
        }

        public function onPreSetData(FormEvent $event)
        {
            $user = $event->getData();
            $form = $event->getForm();

            // Verifica se l'utente dei dati iniziali ha scelto
            // di mostrare la sua email.
            if (true === $user->isShowEmail()) {
                $form->add('email', 'email');
            }
        }

        public function onPreSubmit(FormEvent $event)
        {
            $user = $event->getData();
            $form = $event->getForm();

            if (!$user) {
                return;
            }

            // Verifica se l'utente ha scelto di mostrare la sua email.
            // Se i dati sono stati già inviati, il valore addizionale che è incluso
            // nelle variabili della richiesta va rimosso.
            if (true === $user['show_email']) {
                $form->add('email', 'email');
            } else {
                unset($user['email']);
                $event->setData($user);
            }
        }
    }

Per registrare il sottoscrittore di eventi, usare il metodo addEventSubscriber()::

    // ...

    $form = $formFactory->createBuilder()
        ->add('username', 'text')
        ->add('show_email', 'checkbox')
        ->addEventSubscriber(new AddEmailFieldListener())
        ->getForm();

    // ...
