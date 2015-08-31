.. index::
   single: Doctrine; Semplice form di registrazione
   single: Form; Semplice form di registrazione

Implementare un semplice form di registrazione
==============================================

Alcuni form hanno campi aggiuntivi, i cui valori non devono essere memorizzati nella
base dati. Per esempio, si potrebbe voler creare un form di registrazione con alcuni
campi aggiuntivi (come un campo checkbox "termini accettati") e includere il form che
contiene effettivamente informazioni sull'account.

Il semplice modello User
------------------------

Si ha una semplice entità ``User`` mappata sulla base dati::

    // src/Acme/AccountBundle/Entity/User.php
    namespace Acme\AccountBundle\Entity;

    use Doctrine\ORM\Mapping as ORM;
    use Symfony\Component\Validator\Constraints as Assert;
    use Symfony\Bridge\Doctrine\Validator\Constraints\UniqueEntity;

    /**
     * @ORM\Entity
     * @UniqueEntity(fields="email", message="Email non disponibile")
     */
    class User
    {
        /**
         * @ORM\Id
         * @ORM\Column(type="integer")
         * @ORM\GeneratedValue(strategy="AUTO")
         */
        protected $id;

        /**
         * @ORM\Column(type="string", length=255)
         * @Assert\NotBlank()
         * @Assert\Email()
         */
        protected $email;

        /**
         * @ORM\Column(type="string", length=255)
         * @Assert\NotBlank()
         * @Assert\Length(max = 4096)
         */
        protected $plainPassword;

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

        public function getPlainPassword()
        {
            return $this->plainPassword;
        }

        public function setPlainPassword($password)
        {
            $this->plainPassword = $password;
        }
    }

L'entità ``User`` contiene tre campi, due dei quali (``email`` e
``plainPassword``) vanno visualizzati nel form. La proprietà email deve essere univoca
nella base dati, il che viene assicurato dalla validazione all'inizio della
classe.

.. note::

    Se si vuole integrare questa classe User con il sistema di sicurezza, occorre
    implementare l'interfaccia :ref:`UserInterface<book-security-user-entity>` del
    componente della sicurezza.

.. _cookbook-registration-password-max:

.. sidebar:: Perché il limite a 4096 per la password?

    Si noti che ``plainPassword`` ha una lunghezza massima di 4096 caratteri.
    Per motivi di sicurezza (`CVE-2013-5750`_), Symfony limita la lunghezza delle
    password a 4096 caratteri, prima della codifica. L'aggiunta di questo vincolo
    assicura che il form darà un errore di validazione, se qualcuno tenta di inserire
    una password veramente molto lunga.

    Occorre aggiungere tale vincolo in qualsiasi punto dell'applicazione in cui
    l'utente può inserire una password in chiaro (p.e. in un form di cambio password).
    L'unico punto in cui non occorre preoccuparsene è il form di login,
    poiché il componente Security di Symfony lo gestisce autonomamente.

Creare un form per il modello
-----------------------------

Quindi, creare un form per il modello ``User``::

    // src/Acme/AccountBundle/Form/Type/UserType.php
    namespace Acme\AccountBundle\Form\Type;

    use Symfony\Component\Form\AbstractType;
    use Symfony\Component\Form\FormBuilderInterface;
    use Symfony\Component\OptionsResolver\OptionsResolverInterface;

    class UserType extends AbstractType
    {
        public function buildForm(FormBuilderInterface $builder, array $options)
        {
            $builder->add('email', 'email');
            $builder->add('plainPassword', 'repeated', array(
               'first_name'  => 'password',
               'second_name' => 'confirm',
               'type'        => 'password',
            ));
        }

        public function setDefaultOptions(OptionsResolverInterface $resolver)
        {
            $resolver->setDefaults(array(
                'data_class' => 'Acme\AccountBundle\Entity\User'
            ));
        }

        public function getName()
        {
            return 'user';
        }
    }

Ci sono solo due campi: ``email`` e ``plainPassword`` (ripetuto, per confermare la
password inserita). L'opzione ``data_class`` dice al form il nome della classe dei
dati (in questo caso, l'entità ``User``).

.. tip::

    Per saperne di più sul componente form, leggere :doc:`/book/forms`.

Includere il form User in un form Registration
----------------------------------------------

Il form da usare per la pagina di registrazione non è lo stesso form usato per
modificare l'oggetto ``User`` (cioè ``UserType``). Il form di registrazione
conterrà ulteriori campi, come "accetto i termini", il cui valore non va
memorizzato nella base dati.

Iniziare creando una semplice classe, che rappresenta la registrazione::

    // src/Acme/AccountBundle/Form/Model/Registration.php
    namespace Acme\AccountBundle\Form\Model;

    use Symfony\Component\Validator\Constraints as Assert;

    use Acme\AccountBundle\Entity\User;

    class Registration
    {
        /**
         * @Assert\Type(type="Acme\AccountBundle\Entity\User")
         * @Assert\Valid()
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
            $this->termsAccepted = (Boolean) $termsAccepted;
        }
    }

Quindi, creare il form per il modello ``Registration``::

    // src/Acme/AccountBundle/Form/Type/RegistrationType.php
    namespace Acme\AccountBundle\Form\Type;

    use Symfony\Component\Form\AbstractType;
    use Symfony\Component\Form\FormBuilderInterface;

    class RegistrationType extends AbstractType
    {
        public function buildForm(FormBuilderInterface $builder, array $options)
        {
            $builder->add('user', new UserType());
            $builder->add(
                'terms',
                'checkbox',
                array('property_path' => 'termsAccepted')
            );
            $builder->add('Registrazione', 'submit');
        }

        public function getName()
        {
            return 'registration';
        }
    }

Non servono metodi speciali per includere il form ``UserType``.
Un form è anche un campo, quindi lo si può aggiungere come ogni altro campo, aspettandosi
che la proprietà ``Registration.user`` contenga un'istanza della
classe ``User``.

Gestire l'invio del form
------------------------

Ora occorre un controllore per gestire il form. Iniziare creando un semplice
controllore, per mostrare il form di registrazione::

    // src/Acme/AccountBundle/Controller/AccountController.php
    namespace Acme\AccountBundle\Controller;

    use Symfony\Bundle\FrameworkBundle\Controller\Controller;

    use Acme\AccountBundle\Form\Type\RegistrationType;
    use Acme\AccountBundle\Form\Model\Registration;

    class AccountController extends Controller
    {
        public function registerAction()
        {
            $registration = new Registration();
            $form = $this->createForm(new RegistrationType(), $registration, array(
                'action' => $this->generateUrl('account_create'),
            ));

            return $this->render(
                'AcmeAccountBundle:Account:register.html.twig',
                array('form' => $form->createView())
            );
        }
    }

e il suo template:

.. code-block:: html+jinja

    {# src/Acme/AccountBundle/Resources/views/Account/register.html.twig #}
    {{ form(form) }}

Infine, creare il controllare che gestisce l'invio del form. Questo esegue la
validazione e salva i dati nella base dati::

    use Symfony\Component\HttpFoundation\Request;
    // ...

    public function createAction(Request $request)
    {
        $em = $this->getDoctrine()->getManager();

        $form = $this->createForm(new RegistrationType(), new Registration());

        $form->handleRequest($request);

        if ($form->isValid()) {
            $registration = $form->getData();

            $em->persist($registration->getUser());
            $em->flush();

            return $this->redirect(...);
        }

        return $this->render(
            'AcmeAccountBundle:Account:register.html.twig',
            array('form' => $form->createView())
        );
    }

Aggiungere nuove rotte
----------------------

Aggiornare quindi le rotte. Se le rotte sono fuori dal bundle
(come in questo caso), non dimenticare di assicurarsi che il file delle rotte sia
:ref:`importato<routing-include-external-resources>`.

.. configuration-block::

    .. code-block:: yaml

        # src/Acme/AccountBundle/Resources/config/routing.yml
        account_register:
            path:     /register
            defaults: { _controller: AcmeAccountBundle:Account:register }

        account_create:
            path:     /register/create
            defaults: { _controller: AcmeAccountBundle:Account:create }

    .. code-block:: xml

        <!-- src/Acme/AccountBundle/Resources/config/routing.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>
        <routes xmlns="http://symfony.com/schema/routing"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/routing http://symfony.com/schema/routing/routing-1.0.xsd">

            <route id="account_register" path="/register">
                <default key="_controller">AcmeAccountBundle:Account:register</default>
            </route>

            <route id="account_create" path="/register/create">
                <default key="_controller">AcmeAccountBundle:Account:create</default>
            </route>
        </routes>

    .. code-block:: php

        // src/Acme/AccountBundle/Resources/config/routing.php
        use Symfony\Component\Routing\RouteCollection;
        use Symfony\Component\Routing\Route;

        $collection = new RouteCollection();
        $collection->add('account_register', new Route('/register', array(
            '_controller' => 'AcmeAccountBundle:Account:register',
        )));
        $collection->add('account_create', new Route('/register/create', array(
            '_controller' => 'AcmeAccountBundle:Account:create',
        )));

        return $collection;

Aggiornare lo schema della base dati
------------------------------------

Avendo aggiunto un'entità ``User``, occorre assicurarsi che lo
schema della base dati sia aggiornato di conseguenza:

.. code-block:: bash

   $ php app/console doctrine:schema:update --force

Ecco fatto! Il form ora valida e consente di salvare l'oggetto ``User``
nella base dati. Il campo in più ``terms`` del modello ``Registration``
viene usato durante la registrazione, ma non viene usato successivamente, durante
il salvataggio dell'utente nella base dati.

.. _`CVE-2013-5750`: http://symfony.com/blog/cve-2013-5750-security-issue-in-fosuserbundle-login-form
