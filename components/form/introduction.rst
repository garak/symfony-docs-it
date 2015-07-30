.. index::
    single: Form
    single: Componenti; Form

Il componente Form
==================

    Il componente Form consente di creare, processare e riusare facilmente
    form HTML.

Il componente Form è uno strumento che aiuta a risolvere il problema di consentire agli utenti finali
di interagire con i dati e modificare i dati in un'applicazione. Sebbene,
tradizionalmente, ciò viene fatto tramite form HTML, il componente si focalizza sul
processamento dei dati da e verso il client e l'applicazione, sia che i dati
vengano da un classico form, sia che vengano da un'API.

Installazione
-------------

Si può installare il componente in due modi:

* :doc:`Installarlo tramite Composer </components/using_components>` (``symfony/form`` su `Packagist`_).
* Usare il repository ufficiale Git (https://github.com/symfony/Form);

.. include:: /components/require_autoload.rst.inc

Configurazione
--------------

.. tip::

    Se si lavora con il framework Symfony, il componente Form
    è già configurato. In questo caso, passare a :ref:`component-form-intro-create-simple-form`.

In Symfony, i form sono rappresentati da oggetti e tali oggetti sono costruiti
usando un *factory di form*. Costruire un factory di form è semplice::

    use Symfony\Component\Form\Forms;

    $formFactory = Forms::createFormFactory();

Questo factory può già essere usato per creare form di base, ma manca di
supporto per alcune cose importanti:

* **Gestione della richiesta:** Supporto per gestione delle richieste e caricamento di file;
* **Protezione da CSRF:** Supporto per protezione contro attacchi di tipo Cross-Site-Request-Forgery
  (CSRF);
* **Template:** Integrazione con uno strato di template, che consenta di riutilizzare
  frammenti HTML usando un form;
* **Traduzione:** Supporto per la traduzione di messaggi di errore, label dei campi e
  altre stringhe;
* **Validazione:** Integrazione con una libreria di validazione, per generare messaggi di
  errore per i dati inseriti.

Il componente Form si appoggia su altre librerie per risolvere questi problemi.
La maggior parte delle volte si useranno i componenti Twig e
:doc:`HttpFoundation </components/http_foundation/introduction>`,
Translation e Validator, ma si possono sostituire tutte queste librerie
con altre a scelta.

Le sezioni seguenti spiegano come usare queste librerie insieme al factory
di form.

.. tip::

    Per un esempio funzionante, si veda https://github.com/bschussek/standalone-forms

Gestione della richiesta
~~~~~~~~~~~~~~~~~~~~~~~~

.. versionadded:: 2.3
    Il metodo ``handleRequest()`` è stato aggiunto in Symfony 2.3.

Per processare i dati di un form, occorre richiamare il metodo :method:`Symfony\\Component\\Form\\Form::handleRequest`::


    $form->handleRequest();

Dietro le quinte, viene usato un oggetto :class:`Symfony\\Component\\Form\\NativeRequestHandler`
per leggere i dati dalle opportune variabili di PHP (``$_POST`` o
``$_GET``), in base al metodo HTTP configurato nel form (quello predefinito è POST).

.. seealso::

    Se occorre maggiore controllo su come esattamente il form viene inviato o su quali
    dati vi siano passati, si può usare :method:`Symfony\\Component\\Form\\FormInterface::submit`.
    Per saperne di più, vedere :ref:`il ricettario <cookbook-form-call-submit-directly>`.

.. sidebar:: Integrazione con il componente HttpFoundation

    Per l'integrazione con HttpFoundation, aggiungere
    :class:`Symfony\\Component\\Form\\Extension\\HttpFoundation\\HttpFoundationExtension`
    al factory di form::

        use Symfony\Component\Form\Forms;
        use Symfony\Component\Form\Extension\HttpFoundation\HttpFoundationExtension;

        $formFactory = Forms::createFormFactoryBuilder()
            ->addExtension(new HttpFoundationExtension())
            ->getFormFactory();

    Ora, quando si processa un form, si può passare l'oggetto :class:`Symfony\\Component\\HttpFoundation\\Request`
    a :method:`Symfony\\Component\\Form\\Form::handleRequest`::

        $form->handleRequest($request);

    .. note::

        Per maggiori informazioni sul componente HttpFoundation e su come
        installarlo, vedere :doc:`/components/http_foundation/introduction`.

Protezione da CSRF
~~~~~~~~~~~~~~~~~~

La protezione da attacchi CSRF è compresa nel componente Form, ma occorre
abilitarla esplicitamente o rimpiazzarla con una soluzione personalizzata. Il codice
seguente aggiunge la protezione da CSRF al factory di form::

    use Symfony\Component\Form\Forms;
    use Symfony\Component\Form\Extension\Csrf\CsrfExtension;
    use Symfony\Component\Form\Extension\Csrf\CsrfProvider\SessionCsrfProvider;
    use Symfony\Component\HttpFoundation\Session\Session;

    // generare in qualche modo una parola segreta
    $csrfSecret = '<generated token>';

    // creare un oggetto sessione da HttpFoundation
    $session = new Session();

    $csrfProvider = new SessionCsrfProvider($session, $csrfSecret);

    $formFactory = Forms::createFormFactoryBuilder()
        // ...
        ->addExtension(new CsrfExtension($csrfProvider))
        ->getFormFactory();

Per proteggere un'applicazione da attacchi CSRF, occorre definire una parola
segreta. Generare una stringa casuale con almeno 32 caratteri, inserirla nel
codice appena visto e assicurarsi che nessuno, tranne il server web, possa
accedervi.

Internamente, l'estensione aggiungerà automaticamente a ogni form un campo nascosto
(chiamato ``__token``), il cui valore è automaticamente generato e
validato.

.. tip::

    Se non si usa il componente HttpFoundation, usare
    :class:`Symfony\\Component\\Form\\Extension\\Csrf\\CsrfProvider\\DefaultCsrfProvider`,
    che si basa sulla gestione nativa di PHP delle sessioni::

        use Symfony\Component\Form\Extension\Csrf\CsrfProvider\DefaultCsrfProvider;

        $csrfProvider = new DefaultCsrfProvider($csrfSecret);

Template Twig
~~~~~~~~~~~~~

Se si usa il componente Form per processare form HTML, occorrerà un modo
per rendere facilmente i form come campi HTML (completi con valori,
errori e label). Se si usa `Twig`_ come motore di template, il componente Form
offre una ricca integrazione.

Per usare tale integrazione, occorre ``TwigBridge``, che integra
Twig con vari componenti di Symfony. Usando Composer, si può
installare la versione 2.3 più recente. aggiungendo la seguente riga
al file ``composer.json``:

.. code-block:: json

    {
        "require": {
            "symfony/twig-bridge": "2.3.*"
        }
    }

L'integrazione TwigBridge fornisce varie :doc:`funzioni Twig </reference/forms/twig_reference>`,
che aiutano a rendere ciascun widget, label ed errore per ogni campo
(insieme ad alcune altre cose). Per configurare l'integrazione, occorrerà
accedere a Twig e aggiungere  :class:`Symfony\\Bridge\\Twig\\Extension\\FormExtension`::

    use Symfony\Component\Form\Forms;
    use Symfony\Bridge\Twig\Extension\FormExtension;
    use Symfony\Bridge\Twig\Form\TwigRenderer;
    use Symfony\Bridge\Twig\Form\TwigRendererEngine;

    // il file Twig con tutti i tag per i form
    // questo file fa parte di TwigBridge
    $defaultFormTheme = 'form_div_layout.html.twig';

    $vendorDir = realpath(__DIR__.'/../vendor');
    // percorso di TwigBridge, che consente a Twig di trovare il file
    // form_div_layout.html.twig
    $vendorTwigBridgeDir =
        $vendorDir . '/symfony/twig-bridge/Symfony/Bridge/Twig';
    // percorso degli altri template
    $viewsDir = realpath(__DIR__.'/../views');

    $twig = new Twig_Environment(new Twig_Loader_Filesystem(array(
        $viewsDir,
        $vendorTwigBridgeDir.'/Resources/views/Form',
    )));
    $formEngine = new TwigRendererEngine(array($defaultFormTheme));
    $formEngine->setEnvironment($twig);
    // aggiunge FormExtension a Twig
    $twig->addExtension(
        new FormExtension(new TwigRenderer($formEngine, $csrfProvider))
    );

    // creare il factory, come al solito
    $formFactory = Forms::createFormFactoryBuilder()
        // ...
        ->getFormFactory();

I dettagli esatti della `configurazione di Twig`_ possono variare, ma lo scopo è
sempre quello di aggiungere :class:`Symfony\\Bridge\\Twig\\Extension\\FormExtension`
a Twig, che dà accesso alle funzioni Twig functions per rendere i form.
Per poterlo fare, occorre prima creare un :class:`Symfony\\Bridge\\Twig\\Form\\TwigRendererEngine`,
in cui definire i propri :ref:`form themes <cookbook-form-customization-form-themes>`
(cioè file o risorse che definiscono i tag HTML per i form).

Per dettagli sulla resa dei form, vedere :doc:`/cookbook/form/form_customization`.

.. note::

    Se si usa l'integrazione con Twig, leggere ":ref:`component-form-intro-install-translation`"
    più avanti, per i dettagli sui necessari filtri di traduzione.

.. _component-form-intro-install-translation:

Traduzione
~~~~~~~~~~

Se si usa l'integrazione con Twig con uno dei file di temi di form predefiniti
(come ``form_div_layout.html.twig``), ci sono due filtri Twig (``trans``
e ``transChoice``), usati per tradurre label, errori, opzioni
e altre stringhe.

Per aggiungere questi filtri, si può usare
:class:`Symfony\\Bridge\\Twig\\Extension\\TranslationExtension`, che si integra
con il componente Translation, oppure aggiungere i due filtri a mano,
tramite un'estensione Twig.

Per usare l'integrazione predefinita, assicurarsi che il progetto abbia i componenti
Translation e :doc:`Config </components/config/introduction>` installati.
Se si usa Composer, si possono ottenere le versioni 2.3 più recenti di
entrambi aggiungendo le seguenti righe al file ``composer.json``:

.. code-block:: json

    {
        "require": {
            "symfony/translation": "2.3.*",
            "symfony/config": "2.3.*"
        }
    }

Aggiungere quindi :class:`Symfony\\Bridge\\Twig\\Extension\\TranslationExtension`
all'istanza di ``Twig_Environment``::

    use Symfony\Component\Form\Forms;
    use Symfony\Component\Translation\Translator;
    use Symfony\Component\Translation\Loader\XliffFileLoader;
    use Symfony\Bridge\Twig\Extension\TranslationExtension;

    // creare il Translator
    $translator = new Translator('en');
    // caricare traduzioni in qualche modo
    $translator->addLoader('xlf', new XliffFileLoader());
    $translator->addResource(
        'xlf',
        __DIR__.'/percorso/delle/traduzioni/messages.en.xlf',
        'en'
    );

    // aggiungere TranslationExtension (fornisce i filtri trans e transChoice)
    $twig->addExtension(new TranslationExtension($translator));

    $formFactory = Forms::createFormFactoryBuilder()
        // ...
        ->getFormFactory();

A seconda di come sono state caricate le traduzioni, si possono ora aggiungere chiavi
stringa, come label di campi, e le loro traduzioni nei file di traduzione.

Per maggiori dettagli sulle traduzioni, vedere :doc:`/book/translation`.

Validazione
~~~~~~~~~~~

Il componente Form dispone di un'integrazione stretta (ma facoltativa) con il componente
Validator di Symfony. Si può anche usare una soluzione diversa per la validazione.
Basta prendere i dati inseriti nel form (che sono un array o
un oggetto) e passarli al proprio sistema di validazione.

Per usare l'integrazione con il componente Validator, assicurarsi innanzitutto
di installarlo nell'applicazione. Se si usa Composer e si vogliono
installare le versioni 2.3 più recenti, aggiungere a ``composer.json``:

.. code-block:: json

    {
        "require": {
            "symfony/validator": "2.3.*"
        }
    }

Chi non avesse familiarità con il componente Validator può approfondire
su :doc:`/book/validation`. Il componente Form dispone di una classe
:class:`Symfony\\Component\\Form\\Extension\\Validator\\ValidatorExtension`,
che applica automaticamente la validazione ai dati. Gli errori sono
quindi mappati sui rispettivi campi e resi.

L'integrazione con il componente Validation sarà simile a questa::

    use Symfony\Component\Form\Forms;
    use Symfony\Component\Form\Extension\Validator\ValidatorExtension;
    use Symfony\Component\Validator\Validation;

    $vendorDir = realpath(__DIR__.'/../vendor');
    $vendorFormDir = $vendorDir.'/symfony/form/Symfony/Component/Form';
    $vendorValidatorDir =
        $vendorDir.'/symfony/validator/Symfony/Component/Validator';

    // creare il validatore (i dettagli possono variare)
    $validator = Validation::createValidator();

    // ci sono traduzioni predefinite per i messaggi di errore principali
    $translator->addResource(
        'xlf',
        $vendorFormDir.'/Resources/translations/validators.en.xlf',
        'en',
        'validators'
    );
    $translator->addResource(
        'xlf',
        $vendorValidatorDir.'/Resources/translations/validators.en.xlf',
        'en',
        'validators'
    );

    $formFactory = Forms::createFormFactoryBuilder()
        // ...
        ->addExtension(new ValidatorExtension($validator))
        ->getFormFactory();

Per approfondire, vedere la sezione :ref:`component-form-intro-validation`.

Accesso al factory dei form
~~~~~~~~~~~~~~~~~~~~~~~~~~~

L'applicazione ha bisogno di un unico factory di form, quello che andrebbe
usato per creare tutti gli oggetti form nell'applicazione. Questo vuol
dire che andrebbe creato in una parte centralizzata iniziale dell'applicazione
e quindi acceduto ovunque ci sia bisogno di costruire un form.

.. note::

    In questo documento, il factory di form è sempre una variabile locale, chiamata
    ``$formFactory``. Il punto è che probabilmente si avrà la necessità di creare
    questo oggetto in un qualche modo "globale", per potervi accedere ovunque.

Il modo esatto in cui si accede al factory di form dipende dallo sviluppatore. Se si
usa un :term:`Contenitore di servizi`, si dovrebbe aggiungere il factory di form
al contenitore e recuperarlo all'occorrenza. Se l'applicazione usa
variabili globali o statiche (di solito una cattiva idea), si può memorizzare
l'oggetto in una classe statica o qualcosa del genere.

Indipendentemente dall'architettura dell'applicazione, si ricordi che si dovrebbe
avere solo un factory di form e che occorrerà accedervi in ogni parte
dell'applicazione.

.. _component-form-intro-create-simple-form:

Creazione di un semplice form
-----------------------------

.. tip::

    Se si usa il framework Symfony, il factory di form è disponibile
    automaticamente come servizio, chiamato ``form.factory``. Inoltre, la
    classe controller base ha un metodo :method:`Symfony\\Bundle\\FrameworkBundle\\Controller::createFormBuilder`,
    che è una scorciatoia per recuperare il factory di form e richiamare ``createBuilder``
    su di esso.

La creazione di un form si esegue tramite un oggetto :class:`Symfony\\Component\\Form\\FormBuilder`,
in cui si costruiscono e configurano i vari campi. Il costruttore di form
è creato dal factory di form.

.. configuration-block::

    .. code-block:: php-standalone

        $form = $formFactory->createBuilder()
            ->add('task', 'text')
            ->add('dueDate', 'date')
            ->getForm();

        echo $twig->render('new.html.twig', array(
            'form' => $form->createView(),
        ));

    .. code-block:: php-symfony

        // src/Acme/TaskBundle/Controller/DefaultController.php
        namespace Acme\TaskBundle\Controller;

        use Symfony\Bundle\FrameworkBundle\Controller\Controller;
        use Symfony\Component\HttpFoundation\Request;

        class DefaultController extends Controller
        {
            public function newAction(Request $request)
            {
                // createFormBuilder è una scorciatoia per prendere il factory di form
                // e richiamare createBuilder() su di esso
                $form = $this->createFormBuilder()
                    ->add('task', 'text')
                    ->add('dueDate', 'date')
                    ->getForm();

                return $this->render('AcmeTaskBundle:Default:new.html.twig', array(
                    'form' => $form->createView(),
                ));
            }
        }

Come si può vedere, creare un form è come scrivere una ricettta: si richiama ``add``
per ogni nuovo campo da creare. Il primo parametro di ``add`` è il
nome del campo, il secondo il "tipo" di campo. Il componente Form
dispone di molti :doc:`tipi già pronti </reference/forms/types>`.

Una volta costruito il form, si può capire come :ref:`renderlo <component-form-intro-rendering-form>`
e come :ref:`processarne l'invio <component-form-intro-handling-submission>`.

Impostazione di valori predefiniti
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Se il form deve caricare alcuni valori predefiniti (o se si sta costruendo
un form di modifica), basta passare i dati predefiniti durante la creazione del
costruttore di form:

.. configuration-block::

    .. code-block:: php-standalone

        $defaults = array(
            'dueDate' => new \DateTime('tomorrow'),
        );

        $form = $formFactory->createBuilder('form', $defaults)
            ->add('task', 'text')
            ->add('dueDate', 'date')
            ->getForm();

    .. code-block:: php-symfony

        $defaults = array(
            'dueDate' => new \DateTime('tomorrow'),
        );

        $form = $this->createFormBuilder($defaults)
            ->add('task', 'text')
            ->add('dueDate', 'date')
            ->getForm();

.. tip::

    In questo esempio, i dati predefiniti sono in un array, se invece si usa l'opzione
    :ref:`data_class <book-forms-data-class>` per legare i dati direttamente a
    oggetti, i dati predefiniti saranno un'istanza dell'oggetto specificato.

.. _component-form-intro-rendering-form:

Resa del form
~~~~~~~~~~~~~

Una volta creato il form, il passo successivo è renderlo. Lo si può fare
passando un oggetto "vista" del form a un template (si noti la chiamata a
``$form->createView()`` nel controllore visto sopra) e usando delle funzioni
aiutanti:

.. code-block:: html+jinja

    <form action="#" method="post" {{ form_enctype(form) }}>
        {{ form_widget(form) }}

        <input type="submit" />
    </form>

.. image:: /images/book/form-simple.png
    :align: center

Ecco fatto! Richiamando ``form_widget(form)``, viene reso ogni campo del form,
insieme a label ed eventuali messaggi di errore. Essendo facile,
non è ancora molto flessibile. Di solito, si vuole rendere ogni campo del form
singolarmente, in modo da poterne controllare l'aspetto. Si vedrà come farlo
nella sezione ":ref:`form-rendering-template`".

Cambiare metodo e azione del form
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

.. versionadded:: 2.3
    La possibilità di configurare metodo e azione del form è stata introdotta in
    Symfony 2.3.

Ogni form viene normalmente inviato allo stesso URI in cui è stato resto, con una
richiesta HTTP POST. Si può modificare tale comportamento, usando le opzioni :ref:`form-option-action`
e :ref:`form-option-method` (l'opzione ``method`` è usata anche
da ``handleRequest()``, per determinare se il form sia stato inviato):

.. configuration-block::

    .. code-block:: php-standalone

        $formBuilder = $formFactory->createBuilder('form', null, array(
            'action' => '/search',
            'method' => 'GET',
        ));

        // ...

    .. code-block:: php-symfony

        // ...

        public function searchAction()
        {
            $formBuilder = $this->createFormBuilder('form', null, array(
                'action' => '/search',
                'method' => 'GET',
            ));

            // ...
        }

.. _component-form-intro-handling-submission:

Gestione dell'invio di form
~~~~~~~~~~~~~~~~~~~~~~~~~~~

Per gestire l'invio del form, usare il metodo
:method:`Symfony\\Component\\Form\\Form::handleRequest`:

.. configuration-block::

    .. code-block:: php-standalone

        use Symfony\Component\HttpFoundation\Request;
        use Symfony\Component\HttpFoundation\RedirectResponse;

        $form = $formFactory->createBuilder()
            ->add('task', 'text')
            ->add('dueDate', 'date')
            ->getForm();

        $request = Request::createFromGlobals();

        $form->handleRequest($request);

        if ($form->isValid()) {
            $data = $form->getData();

            // ... fare qualcosa, come salvare i dati

            $response = new RedirectResponse('/task/success');
            $response->prepare($request);

            return $response->send();
        }

        // ...

    .. code-block:: php-symfony

        // ...

        public function newAction(Request $request)
        {
            $form = $this->createFormBuilder()
                ->add('task', 'text')
                ->add('dueDate', 'date')
                ->getForm();

            $form->handleRequest($request);

            if ($form->isValid()) {
                $data = $form->getData();

                // ... fare qualcosa, come salvare i dati

                return $this->redirect($this->generateUrl('task_success'));
            }

            // ...
        }

In questo modo  si definisce un flusso comune per i form, con tre diverse possibilità:

1) Nella richiesta GET iniziale (cioè quando l'utente apre la pagina),
   costruire e mostrare il form;

Se la richiesta è POST, processare i dati inseriti (tramite ``handleRequest()``).
Quindi:

2) se il form non è valido, rendere nuovamente il form (che ora contiene errori)
3) se il form è valido, eseguire delle azioni e redirigere.

Per fortuna, non serve decidere se il form sia stato inviato o meno.
Basta passare la richiesta al metodo ``handleRequest()``. Quindi, il componente Form
svolgerà tutto il lavoro necessario.

.. _component-form-intro-validation:

Validazione di form
~~~~~~~~~~~~~~~~~~~

Il modo più facile di aggiungere validazione ai form è tramite l'opzione ``constraints``,
durante la costruzione di ogni campo:

.. configuration-block::

    .. code-block:: php-standalone

        use Symfony\Component\Validator\Constraints\NotBlank;
        use Symfony\Component\Validator\Constraints\Type;

        $form = $formFactory->createBuilder()
            ->add('task', 'text', array(
                'constraints' => new NotBlank(),
            ))
            ->add('dueDate', 'date', array(
                'constraints' => array(
                    new NotBlank(),
                    new Type('\DateTime'),
                )
            ))
            ->getForm();

    .. code-block:: php-symfony

        use Symfony\Component\Validator\Constraints\NotBlank;
        use Symfony\Component\Validator\Constraints\Type;

        $form = $this->createFormBuilder()
            ->add('task', 'text', array(
                'constraints' => new NotBlank(),
            ))
            ->add('dueDate', 'date', array(
                'constraints' => array(
                    new NotBlank(),
                    new Type('\DateTime'),
                )
            ))
            ->getForm();

Al bind del form, questi vincoli di validazione saranno automaticamente applicati
e gli eventuali errori mostrati accanto ai rispettivi campi.

.. note::

    Per un elenco di tutti i vincoli disponibili, vedere
    :doc:`/reference/constraints`.

Accedere agli errori
~~~~~~~~~~~~~~~~~~~~

Si può usare il metodo :method:`Symfony\\Component\\Form\\FormInterface::getErrors`
per accedere alla lista degli errori. Ogni elemento è un oggetto
:class:`Symfony\\Component\\Form\\FormError`::

    $form = ...;

    // ...

    // un array di oggetti FormError, ma solo di errori allegati a questo
    // livello del form (p.e. "errori globali")
    $errori = $form->getErrors();

    // un array di oggetti FormError, ma solo di errori allegati al campo
    // "nome"
    $errori = $form['nome']->getErrors();

    // una stringa che rappresenta tutti gli errori dell'intero form
    $errori = $form->getErrorsAsString();

.. note::

    Se si abilita l'opzione :ref:`error_bubbling <reference-form-option-error-bubbling>`
    su un campo, la chiamata a ``getErrors()`` sul form genitore includerà gli errori
    di quel campo. Tuttavia, non c'è modo di determinare a quale campo sia stato
    originariamente allegato un errore.

.. _Packagist: https://packagist.org/packages/symfony/form
.. _Twig:      http://twig.sensiolabs.org
.. _`configurazione di Twig`: http://twig.sensiolabs.org/doc/intro.html
