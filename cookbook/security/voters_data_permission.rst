.. index::
   single: Sicurezza; Votanti su permessi di dati

Usare votanti per verificare i permessi dell'utente
===================================================

In Symfony, si possono verificare permessi di accesso ai dati, usando il
:doc:`modulo ACL </cookbook/security/acl>`, che è un po' troppo complesso
per molte applicazioni. Una soluzione molto più semplice consiste nell'uso di votanti personalizzati,
che sono come semplici istruzioni condizionali.

.. seealso::

    Si possono usare i votanti in altri modi, per esempio per escludere degli
    indirizzi IP dall'intera applicazione: :doc:`/cookbook/security/voters`.

.. tip::

    Dare un'occhiata al capitolo
    sull':doc:`autorizzazione </components/security/authorization>`
    per una comprensione più approfondita sui votanti.

Come Symfony usa i votanti
--------------------------

Per poter usare i votanti, occorre capire in che modo Symfony li usi.
Tutti i votanti sono richiamati ogni volta che viene usato il metodo ``isGranted()``
del contesto della sicurezza di Symfony (cioè il servizio ``security.context``). Ciascuno
di essi decide se l'utente corrente debba avere accesso a una qualche risorsa.

Infine, Symfony usa uno dei tre approcci disponibili per decidere cosa
fare con le decisioni dei votanti: affermativo, consenso o unanime.

Per maggiori informazioni, dare un'occhiata alla
:ref:`sezione sui gestori di decisione di accesso <components-security-access-decision-manager>`.

L'interfaccia VoterInterface
----------------------------

Un votante personalizzato deve implementare
:class:`Symfony\\Component\\Security\\Core\\Authorization\\Voter\\VoterInterface`,
che ha questa struttura:

.. include:: /cookbook/security/voter_interface.rst.inc

In questo esempio, il votante verificherà se l'utente abbia accesso a uno specifico
oggetto, a seconda delle condizioni personalizzate (p.e. deve possedere
l'oggetto). Se la condizione non è soddisfatta, si restituirà
``VoterInterface::ACCESS_DENIED``, altrimenti si restituirà
``VoterInterface::ACCESS_GRANTED``. Se la responsabilità di questa decisione non
è di questo votante, esso restituirà ``VoterInterface::ACCESS_ABSTAIN``.

Creare un votante personalizzato
--------------------------------

Lo scopo è creare un votante che verifichi se un utente abbia accesso alla visualizzazione o
modifica di un particolare oggetto. Ecco una possibile implementazione::

    // src/AppBundle/Security/Authorization/Voter/PostVoter.php
    namespace AppBundle\Security\Authorization\Voter;

    use Symfony\Component\Security\Core\Authorization\Voter\VoterInterface;
    use Symfony\Component\Security\Core\Authentication\Token\TokenInterface;
    use Symfony\Component\Security\Core\User\UserInterface;

    class PostVoter implements VoterInterface
    {
        const VIEW = 'view';
        const EDIT = 'edit';

        public function supportsAttribute($attribute)
        {
            return in_array($attribute, array(
                self::VIEW,
                self::EDIT,
            ));
        }

        public function supportsClass($class)
        {
            $supportedClass = 'AppBundle\Entity\Post';

            return $supportedClass === $class || is_subclass_of($class, $supportedClass);
        }

        /**
         * @var \AppBundle\Entity\Post $post
         */
        public function vote(TokenInterface $token, $post, array $attributes)
        {
            // verifica se la classe di questo oggetto sia supportata da questo votante
            if (!$this->supportsClass(get_class($post))) {
                return VoterInterface::ACCESS_ABSTAIN;
            }

            // verifica se il votante è usato correttamente, consente un singolo attributo
            // questo non è un requisito, ma solo un modo semplice per
            // progettare un votante
            if(1 !== count($attributes)) {
                throw new InvalidArgumentException(
                    'È consentito un solo attributo per VIEW o EDIT'
                );
            }

            // imposta l'attributo da verificare
            $attribute = $attributes[0];

            // verifica se l'attributo dato sia coperto da questo votante
            if (!$this->supportsAttribute($attribute)) {
                return VoterInterface::ACCESS_ABSTAIN;
            }

            // ottiene l'utente corrente
            $user = $token->getUser();

            // si assicura che ci sia un utente (che abbia fatto login)
            if (!$user instanceof UserInterface) {
                return VoterInterface::ACCESS_DENIED;
            }

            switch($attribute) {
                case self::VIEW:
                    // l'oggetto potrebbe per esempio avere un metodo isPrivate()
                    // che verifichi l'attributo booleano $private
                    if (!$post->isPrivate()) {
                        return VoterInterface::ACCESS_GRANTED;
                    }
                    break;

                case self::EDIT:
                    // ipotizziamo che l'oggetto abbia un metodo getOwner(), che
                    // restituisce l'utente proprietario dell'oggetto
                    if ($user->getId() === $post->getOwner()->getId()) {
                        return VoterInterface::ACCESS_GRANTED;
                    }
                    break;
            }
            
            return VoterInterface::ACCESS_DENIED;
        }
    }

Ecco fatto! Il votante è pronto. Il prossimo passo è iniettarlo nel
livello della sicurezza.

Dichiarare il votante come servizio
-----------------------------------

Per iniettare il votante nel livello della sicurezza, lo si deve dichiarare come servizio e
assegnarli il tag ``security.voter``:

.. configuration-block::

    .. code-block:: yaml

        # src/AppBundle/Resources/config/services.yml
        services:
            security.access.post_voter:
                class:      AppBundle\Security\Authorization\Voter\PostVoter
                public:     false
                tags:
                   - { name: security.voter }

    .. code-block:: xml

        <!-- src/AppBundle/Resources/config/services.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services"
            xsi:schemaLocation="http://symfony.com/schema/dic/services
                http://symfony.com/schema/dic/services/services-1.0.xsd">
            <services>
                <service id="security.access.post_document_voter"
                    class="AppBundle\Security\Authorization\Voter\PostVoter"
                    public="false">
                    <tag name="security.voter" />
                </service>
            </services>
        </container>

    .. code-block:: php

        // src/AppBundle/Resources/config/services.php
        $container
            ->register(
                    'security.access.post_document_voter',
                    'AppBundle\Security\Authorization\Voter\PostVoter'
            )
            ->addTag('security.voter')
        ;

Usare il votante in un controllore
----------------------------------

Il votante registrato sarà richiamato ogni volta che sarà richiamato il metodo ``isGranted()``
del contesto della sicurezza.

.. code-block:: php

    // src/AppBundle/Controller/PostController.php
    namespace AppBundle\Controller;

    use Symfony\Bundle\FrameworkBundle\Controller\Controller;
    use Symfony\Component\HttpFoundation\Response;
    use Symfony\Component\Security\Core\Exception\AccessDeniedException;

    class PostController extends Controller
    {
        public function showAction($id)
        {
            // prende un'istanza del post
            $post = ...;

            // tenere a mente che questo richiamerà tutti i votanti registrati
            if (false === $this->get('security.context')->isGranted('view', $post)) {
                throw new AccessDeniedException('Accesso non autorizzato!');
            }

            return new Response('<h1>'.$post->getName().'</h1>');
        }
    }

È così facile!
