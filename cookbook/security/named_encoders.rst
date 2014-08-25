.. index::
    single: Sicurezza; Codificatori con nome

Scegliere un algoritmo dinamico per la codifica di password
===========================================================

.. versionadded:: 2.5
    I codificatori con nome sono stati introdotti in Symfony 2.5.

Solitamente, lo stesso codificatore di password viene usato per tutti gli utenti, configurandolo
per essere applicato a tutte le istanza di una specifica classe:

.. configuration-block::

    .. code-block:: yaml

        # app/config/security.yml
        security:
            # ...
            encoders:
                Symfony\Component\Security\Core\User\User: sha512

    .. code-block:: xml

        <!-- app/config/security.xml -->
        <?xml version="1.0" encoding="UTF-8"?>
        <srv:container xmlns="http://symfony.com/schema/dic/security"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xmlns:srv="http://symfony.com/schema/dic/services"
            xsi:schemaLocation="http://symfony.com/schema/dic/services
                http://symfony.com/schema/dic/services/services-1.0.xsd"
        >
            <config>
                <!-- ... -->
                <encoder class="Symfony\Component\Security\Core\User\User"
                    algorithm="sha512"
                />
            </config>
        </srv:container>

    .. code-block:: php

        // app/config/security.php
        $container->loadFromExtension('security', array(
            // ...
            'encoders' => array(
                'Symfony\Component\Security\Core\User\User' => array(
                    'algorithm' => 'sha512',
                ),
            ),
        ));

Un'altra opzione è usare un codificatore con nome e quindi selezionare quale codificatore
si vuole usare, in modo dinamico.

Nell'esempio precedente, è stato impostato l'algoritmo ``sha512`` per ``Acme\UserBundle\Entity\User``.
Questo potrebbe essere abbastanza sicuro per un utente normale, ma si potrebbero richiedere algoritmo
più robusti per gli amministratori, come ``bcrypt``. Lo si può fare con i
codificatori con nome:

.. configuration-block::

    .. code-block:: yaml

        # app/config/security.yml
        security:
            # ...
            encoders:
                duro:
                    algorithm: bcrypt
                    cost: 15

    .. code-block:: xml

        <!-- app/config/security.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>
        <srv:container xmlns="http://symfony.com/schema/dic/security"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xmlns:srv="http://symfony.com/schema/dic/services"
            xsi:schemaLocation="http://symfony.com/schema/dic/services
                http://symfony.com/schema/dic/services/services-1.0.xsd"
        >

            <config>
                <!-- ... -->
                <encoder class="duro"
                    algorithm="bcrypt"
                    cost="15" />
            </config>
        </srv:container>

    .. code-block:: php

        // app/config/security.php
        $container->loadFromExtension('security', array(
            // ...
            'encoders' => array(
                'duro' => array(
                    'algorithm' => 'bcrypt',
                    'cost'      => '15'
                ),
            ),
        ));

È stato creato un codificatore di nome ``duro``. Per farlo usare a un'istanza di ``User``,
la classe deve implementare
:class:`Symfony\\Component\\Security\\Core\\Encoder\\EncoderAwareInterface`.
L'interfaccia richiede un solo metodo, ``getEncoderName``, che deve restituire
il nome del codificatore da usare::

    // src/Acme/UserBundle/Entity/User.php
    namespace Acme\UserBundle\Entity;

    use Symfony\Component\Security\Core\User\UserInterface;
    use Symfony\Component\Security\Core\Encoder\EncoderAwareInterface;

    class User implements UserInterface, EncoderAwareInterface
    {
        public function getEncoderName()
        {
            if ($this->isAdmin()) {
                return 'duro';
            }

            return null; // usa il codificatore predefinito
        }
    }
