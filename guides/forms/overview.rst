.. index::
   single: Form

Form
====

Symfony2 dispone di un sofisticato componente Form che permette di creare
facilmente i form.

Il primo form
-------------

Un form in Symfony2 è uno strato trasparente posizionato sopra al modello del dominio.
Legge proprietà da un oggetto, visualizza i valori nel form e permette all'utente di
modificarli. Quando il form viene inviato, i valori vengono scritti nuovamente 
nell'oggetto.

Vediamo come funziona in un esempio pratico. Creiamo una semplice
classe ``Customer``::

    class Customer
    {
        public $name;
        private $age = 20;

        public function getAge()
        {
            return $this->age;
        }

        public function setAge($age)
        {
            $this->age = $age;
        }
    }

La classe contiene due proprietà ``name`` e "age". La proprietà ``$name``
è pubblica, mentre ``$age`` può essere modificata solo attraverso setter e getter.
	
Ora creiamo un form per consentire al visitatore di inserire i dati dell'oggetto::

    // src/Application/HelloBundle/Controller/HelloController.php
    public function signupAction()
    {
        $customer = new Customer();

        $form = new Form('customer', $customer, $this['validator']);
        $form->add(new TextField('name'));
        $form->add(new IntegerField('age'));

        return $this->render('HelloBundle:Hello:signup.php', array(
            'form' => $this['templating.form']->get($form)
        ));
    }

Un form è costituito da vari campi. Ogni campo rappresenta una proprietà nella
classe. La proprietà deve avere lo stesso nome del campo e deve essere
pubblica o accessibile attraverso getter e setter pubblici.

Invece di passare l'istanza del form direttamente alla vista, la "avvolgiamo" con un
oggetto che fornisce i metodi che aiutano a visualizzare il form con maggiore flessibilità
(``$this['templating.form']->get($form)``).	

Creiamo un semplice template per visualizzare il form:

.. code-block:: html+php

    # src/Application/HelloBundle/Resources/views/Hello/signup.php
    <?php $view->extend('HelloBundle::layout.php') ?>

    <?php echo $form->form('#') ?>
        <?php echo $form->render() ?>

        <input type="submit" value="Send!" />
    </form>

.. note::
   La visualizzazione dei form nei template è trattata in due diversi capitoli: uno per i
   :doc:`template PHP </guides/forms/view>` e uno per i :doc:`template Twig
   </guides/forms/twig>`.

Quando l'utente invia il form, c'è bisogno anche di gestire i dati inviati. Tutti
i dati sono memorizzati in un parametro POST con il nome del form::

    # src/Application/HelloBundle/Controller/HelloController.php
    public function signupAction()
    {
        $customer = new Customer();
        $form = new Form('customer', $customer, $this['validator']);

        // form setup...

        if ('POST' === $this['request']->getMethod()) {
            $form->bind($this['request']->request->get('customer'));

            if ($form->isValid()) {
                // save $customer object and redirect
            }
        }

        return $this->render('HelloBundle:Hello:signup.php', array('form' => $form));
    }

Congratulazioni! E' stato appena creato il primo form pienamente funzionante con
Symfony2.

.. index::
   single: Form; Campi

Form Campi
----------

Come si è appreso, un form è costituito da uno o più campi di form. Un campo sa
come convertire i dati tra rappresentazioni normalizzate e "umane".

Si può dare un'occhiata a ``DateField``, per esempio. Mentre il programamtore preferisce
memorizzare le date come stringhe o oggetti ``DateTime``, gli utenti al contrario preferiscono
sceglierle utilizzando un menu a tendina. ``DateField``
da un elenco di elenchi a discesa. ``DateField`` gestisce la visualizzazione e il tipo
di conversione.

Campi di base
~~~~~~~~~~~~~

Symfony2 ha tutti i campi disponibili in semplice HTML:

============= ==================
Campo         Descrizione
============= ==================
TextField     Un tag input per l'immissione di testo breve
TextareaField Un tag textarea per l'immissione di testi lunghi
CheckboxField Un checkbox
ChoiceField   Un menu a tendina o pulsanti radio/caselle di conrtrollo multiple per selezionare valori
PasswordField Un tag input per le password
HiddenField   Un tag hidden nascosto
============= ==================

Campi localizzati
~~~~~~~~~~~~~~~~~

Il componente Form dispone anche di campi che vengono visualizzati in modo diverso
a seconda della localizzazione degli utenti:

============= ==================
Campo         Descrizione
============= ==================
NumberField   Un campo di testo per l'immissione di numeri
IntegerField  Un campo di testo per l'immissione di numeri interi
PercentField  Un campo di testo per immettere valori percentuali
MoneyField    Un campo di testo per immettere valori monetari
DateField     Un campo di testo o menu a tendina multipli per immettere date
BirthdayField Una estensione di DateField per selezionare compleanni
TimeField     Un campo di testo o menu a tendina multipli per immettere tempi
DateTimeField Una combinazione di DateField e TimeField
TimezoneField Una estensione di ChoiceField per selezionare un timezone
============= ==================

Gruppi di campi
~~~~~~~~~~~~~~~

I gruppi di campi consentono di combinare più campi insieme. Mentre normalmente
i campi consentono di modificare solo i tipi di dati scalari, i gruppi di campi
possono essere utilizzati per modificare interi oggetti o array. Aggiungiamo
una nuova classe ``Address`` al nostro modello::

    class Address
    {
        public $street;
        public $zipCode;
    }

Ora si può aggiungere una proprietà ``$address`` al cliente, che memorizza
un oggetto ``Address``::

    class Customer
    {
         // other properties ...

         public $address;
    }

Si può utilizzare un gruppo di campi per visualizzare i campi del cliente
e al tempo stesso l'indirizzo nidificato::

    # src/Application/HelloBundle/Controller/HelloController.php
    public function signupAction()
    {
        $customer = new Customer();
        $customer->address = new Address();

        // form configuration ...

        $group = new FieldGroup('address');
        $group->add(new TextField('street'));
        $group->add(new TextField('zipCode'));
        $form->add($group);

        // process form ...
    }

Sono bastate queste piccole modifiche per poter modificare anche l'oggetto ``Address``!

Campi ripetuti
~~~~~~~~~~~~~~

I ``RepeatedField`` sono una estensione dei gruppi di campi che permette di visualizzare
un campo due volte. Il campo ripetuto verrà validato solo se l'utente inserisce lo stesso
valore in entrambi i campi::

    $form->add(new RepeatedField(new TextField('email')));

Questa funzionalità è molto utile per l'inserimento di indirizzi email o password!

Collezione di campi
~~~~~~~~~~~~~~~~~~~

``CollectionField`` è un gruppo speciale di campi per la gestione di array o
oggetti che implementano l'interfaccia ``Traversable``. Per dimostrare ciò,
verrà estesa la classe ``Customer`` per memorizzare tre indirizzi email::

    class Customer
    {
        // other properties ...

        public $emails = array('', '', '');
    }

Ora verrà aggiunto un ``CollectionField`` per gestire questi indirizzi::

    $form->add(new CollectionField(new TextField('emails')));

Se si imposta l'opzione "modifiable" a ``true``, è anche possibile aggiungere o rimuovere
righe della collezione tramite JavaScript! Il ``CollectionField`` se ne accorgerà
e ridimensionerà di conseguenza l'array sottostante.

.. index::
   pair: Forms; Validation

Validazione edl form
--------------------

Nell'ultima parte di questo tutorial si è appreso come configurare
vincoli di validazione per una classe PHP. La cosa bella è che questo è già sufficiente
per validare un form! Bisogna ricordare che form non è altro che un punto di passaggio per
modificare i dati in un oggetto.

Cosa succede se ci sono altri vincoli di validazione per uno specifico form, che
sono irrilevanti per la classe di base? Cosa succede se il form contiene campi che
non dovrebbero essere salvati nell'oggetto?

La risposta più frequente a questa domanda è quella di estendere il modello di dominio.
Verrà mostrato questo approccio, estendendo il form con una casella di controllo per
accettare termini e condizioni.

Per questo scopo, si può creare una semplice classe ``Registration``::

    class Registration
    {
        /** @validation:Valid */
        public $customer;

        /** @validation:AssertTrue(message="Per favore accettare termini e condizioni") */
        public $termsAccepted = false;

        public function process()
        {
            // salva l'utente, invia email ecc.
        }
    }

Ora si può adattare facilmente il form nel controller::

    # src/Application/HelloBundle/Controller/HelloController.php
    public function signupAction()
    {
        $registration = new Registration();
        $registration->customer = new Customer();

        $form = new Form('registration', $registration, $this['validator']);
        $form->add(new CheckboxField('termsAccepted'));

        $group = new FieldGroup('customer');

        // aggiungere campi cliente a questo gruppo ...

        $form->add($group);

        if ('POST' === $this['request']->getMethod()) {
            $form->bind($this['request']->request->get('registration'));

            if ($form->isValid()) {
                $registration->process();
            }
        }

        return $this->render('HelloBundle:Hello:signup.php', array('form' => $form));
    }

Il grande vantaggio di questo refactoring è che si può riutilizzare la classe
``Registration``. Estendere l'applicazione per consentire agli utenti di
registrarsi via XML non è un problema!

Considerazioni finali
---------------------

In questo capitolo viene mostrato come il componente Form di Symfony2 può aiutare a
creare rapidamente form per gli oggetti del dominio. Il componente abbraccia una rigorosa
separazione tra logica di business e la presentazione. Molti campi sono
automaticamente localizzati per mettere a proprio agio i visitatori del sito web.
E con una architettura flessibile, questo è solo l'inizio per molti potenti campi
creati dall'utente!