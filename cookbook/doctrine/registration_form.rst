.. index::
   single: Doctrine; Semplice form di registrazione
   single: Form; Semplice form di registrazione

Implementare un semplice form di registrazione
==============================================

Alcuni form hanno campi aggiuntivi, i cui valori non devono essere memorizzati nella
base dati. Per esempio, si potrebbe voler creare un form di registrazione con alcuni
campi aggiuntivi (come un campo cehckbox "termini accettati") e includere il form che
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
               'first_name' => 'password',
               'second_name' => 'confirm',
               'type' => 'password',
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
    use Symfony\Component\HttpFoundation\Response;

    use Acme\AccountBundle\Form\Type\RegistrationType;
    use Acme\AccountBundle\Form\Model\Registration;

    class AccountController extends Controller
    {
        public function registerAction()
        {
            $form = $this->createForm(
                new RegistrationType(),
                new Registration()
            );

            return $this->render(
                'AcmeAccountBundle:Account:register.html.twig',
                array('form' => $form->createView())
            );
        }
    }

e il suo template:

.. code-block:: html+jinja

    {# src/Acme/AccountBundle/Resources/views/Account/register.html.twig #}
    <form action="{{ path('create')}}" method="post" {{ form_enctype(form) }}>
        {{ form_widget(form) }}

        <input type="submit" />
    </form>

Infine, creare il controllare che gestisce l'invio del form. Questo esegue la
validazione e salva i dati nella base dati::

    public function createAction()
    {
        $em = $this->getDoctrine()->getEntityManager();

        $form = $this->createForm(new RegistrationType(), new Registration());

        $form->bind($this->getRequest());

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

Ecco fatto! Il form ora valida e consente di salvare l'oggetto ``User``
nella base dati. Il campo in più ``terms`` del modello ``Registration``
viene usato durante la registrazione, ma non viene usato successivamente, durante
il salvataggio dell'utente nella base dati.
