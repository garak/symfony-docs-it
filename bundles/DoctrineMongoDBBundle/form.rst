Implementare un semplice form di registrazione con MongoDB
==========================================================

Alcuni form hanno campi extra, i cui valori non hanno bisogno di essere memorizzati
nella base dati. In questo esempio, creeremo un form di registrazione con alcuni campi
extra (come un campo checkbox "accettazione termini") e inseriremo il form che
si occupa effettivamente di memorizzare le informazioni dell'account. Useremo MongoDB per memorizzare i dati.

Il semplice modello User
------------------------

Quindi, in questa guida inizieremo con il modello per un documento ``User``::

    // src/Acme/AccountBundle/Document/User.php
    namespace Acme\AccountBundle\Document;

    use Doctrine\ODM\MongoDB\Mapping\Annotations as MongoDB;
    use Symfony\Component\Validator\Constraints as Assert;
    use Doctrine\Bundle\MongoDBBundle\Validator\Constraints\Unique as MongoDBUnique;

    /**
     * @MongoDB\Document(collection="users")
     * @MongoDBUnique(path="email")
     */
    class User
    {
        /**
         * @MongoDB\Id
         */
        protected $id;

        /**
         * @MongoDB\Field(type="string")
         * @Assert\NotBlank()
         * @Assert\Email()
         */
        protected $email;

        /**
         * @MongoDB\Field(type="string")
         * @Assert\NotBlank()
         */
        protected $password;

        public function getId()
        {
            return $this->id;
        }

        public function getEmail()
        {
            return $this->email;
        }

        public function setEmail($email)
        {
            $this->email = $email;
        }

        public function getPassword()
        {
            return $this->password;
        }

        // una forma semplice e stupida di crittazione (da non copiare!)
        public function setPassword($password)
        {
            $this->password = sha1($password);
        }
    }

Il documento ``User`` contiene tre campi, di cui due (email e
password) vanno mostrati sul form. La proprietà email deve essere univoca
nella base dati, quindi abbiamo aggiunto la validazione in cima alla classe.

.. note::

    Se si vuole integrare questo User con il sistema di sicurezza, occorre
    implementare :ref:`UserInterface<book-security-user-entity>`, del componente
    della sicurezza.

Creare un form per il modello
-----------------------------

Quindi, creare il form per il modello ``User``::

    // src/Acme/AccountBundle/Form/Type/UserType.php
    namespace Acme\AccountBundle\Form\Type;

    use Symfony\Component\Form\AbstractType;
    use Symfony\Component\Form\Extension\Core\Type\RepeatedType;
    use Symfony\Component\Form\FormBuilder;

    class UserType extends AbstractType
    {
        public function buildForm(FormBuilder $builder, array $options)
        {
            $builder->add('email', 'email');
            $builder->add('password', 'repeated', array(
               'first_name' => 'password',
               'second_name' => 'confirm',
               'type' => 'password'
            ));
        }

        public function getDefaultOptions(array $options)
        {
            return array('data_class' => 'Acme\AccountBundle\Document\User');
        }

        public function getName()
        {
            return 'user';
        }
    }

Abbiamo solo aggiunto due campi: email e password (ripetuta, per conferma).
L'opzione ``data_class`` dice al form il nome della classe dei dati
(cioè il documento ``User``).

.. tip::

    Per approfondire il componente dei form, leggere la :doc:`documentazione</book/forms>`.

Inserire il form User in un form di registrazione
-------------------------------------------------

Il form che useremo per la pagina di regstrazione non è lo stesso form usato
per modificare l'utente (cioè ``UserType``). Il form di registrazione conterrà
campi in più, come "accettazione termini", i cui valori non saranno
memorizzati nella base dati.

In altre parole, creare un secondo form per la registrazione, in cui inserire il form
``User`` e aggiungere i campi extra necessari. Iniziare creando una semplice classe,
che rappresenta la "registrazione"::

    // src/Acme/AccountBundle/Form/Model/Registration.php
    namespace Acme\AccountBundle\Form\Model;

    use Symfony\Component\Validator\Constraints as Assert;

    use Acme\AccountBundle\Document\User;

    class Registration
    {
        /**
         * @Assert\Type(type="Acme\AccountBundle\Document\User")
         */
        protected $user;

        /**
         * @Assert\NotBlank()
         * @Assert\True()
         */
        protected $termsAccepted;

        public function setUser(User $user)
        {
            $this->user = $user;
        }

        public function getUser()
        {
            return $this->user;
        }

        public function getTermsAccepted()
        {
            return $this->termsAccepted;
        }

        public function setTermsAccepted($termsAccepted)
        {
            $this->termsAccepted = (boolean)$termsAccepted;
        }
    }

Quindi, creare il form per il modello ``Registration``::

    // src/Acme/AccountBundle/Form/Type/RegistrationType.php
    namespace Acme\AccountBundle\Form\Type;

    use Symfony\Component\Form\AbstractType;
    use Symfony\Component\Form\Extension\Core\Type\RepeatedType;
    use Symfony\Component\Form\FormBuilder;

    class RegistrationType extends AbstractType
    {
        public function buildForm(FormBuilder $builder, array $options)
        {
            $builder->add('user', new UserType());
            $builder->add('terms', 'checkbox', array('property_path' => 'termsAccepted'));
        }

        public function getName()
        {
            return 'registration';
        }
    }

Non occorre usare un metodo speciale per inserire il form ``UserType``.
Un form è un campo, quindi lo si può aggiungere come ogni altro campo,
aspettandosi che la corrispondente proprietà ``user`` conterrà un'istanza
della classe ``UserType``.

Gestire l'invio del form
------------------------

Quindi, occorre un controllore che gestisca il form. Iniziare creando un semplice
controllore, per mostrare il form di registrazione::

    // src/Acme/AccountBundle/Controller/AccountController.php
    namespace Acme\AccountBundle\Controller;

    use Symfony\Bundle\FrameworkBundle\Controller\Controller;
    use Symfony\Component\HttpFoundation\Response;

    use Acme\AccountBundle\Form\Type\RegistrationType;
    use Acme\AccountBundle\Form\Model\Registration;

    class AccountController extends Controller
    {
        public function registerAction()
        {
            $form = $this->createForm(new RegistrationType(), new Registration());

            return $this->render('AcmeAccountBundle:Account:register.html.twig', array('form' => $form->createView()));
        }
    }

e il suo template:

.. code-block:: html+jinja

    {# src/Acme/AccountBundle/Resources/views/Account/register.html.twig #}

    <form action="{{ path('create')}}" method="post" {{ form_enctype(form) }}>
        {{ form_widget(form) }}

        <input type="submit" />
    </form>

Infine, creare il controllore che gestisce l'invio del form. Questo eseguirà
la validazione e salverà i dati in MongoDB::

    public function createAction()
    {
        $dm = $this->get('doctrine.odm.mongodb.default_document_manager');

        $form = $this->createForm(new RegistrationType(), new Registration());

        $form->bindRequest($this->getRequest());

        if ($form->isValid()) {
            $registration = $form->getData();

            $dm->persist($registration->getUser());
            $dm->flush();

            return $this->redirect(...);
        }

        return $this->render('AcmeAccountBundle:Account:register.html.twig', array('form' => $form->createView()));
    }

Ecco fatto! Il form ora valida e consente di salvare l'oggetto ``User``
in MongoDB.
