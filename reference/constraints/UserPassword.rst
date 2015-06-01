UserPassword
============

.. note::

    Da Symfony 2.2, le classi ``UserPassword*`` nello spazio dei nomi
    :namespace:`Symfony\\Component\\Security\\Core\\Validator\\Constraint <Symfony\\Component\\Security\\Core\\Validator\\Constraint>`
    sono deprecate, saranno rimosse in Symfony 2.3. Usare invece le classi
    ``UserPassword*`` nello spazio dei nomi
    :namespace:`Symfony\\Component\\Security\\Core\\Validator\\Constraints <Symfony\\Component\\Security\\Core\\Validator\\Constraints>`


Valida che un valore inserito sia uguale alla password dell'utente attualmente
autenticato. Può tornare utile in un form in cui un utente può cambiare la propria
password, ma deve inserire quella vecchia per motivi di sicurezza.

.. note::

    Questo vincolo **non** va usato per la validazione di un form di login, che viene
    fatta automaticamente dal sistema di sicurezza.

+----------------+--------------------------------------------------------------------------------------------+
| Si applica a   | :ref:`proprietà o metodo <validation-property-target>`                                     |
+----------------+--------------------------------------------------------------------------------------------+
| Opzioni        | - `message`_                                                                               |
+----------------+--------------------------------------------------------------------------------------------+
| Classe         | :class:`Symfony\\Component\\Security\\Core\\Validator\\Constraints\\UserPassword`          |
+----------------+--------------------------------------------------------------------------------------------+
| Validatore     | :class:`Symfony\\Component\\Security\\Core\\Validator\\Constraints\\UserPasswordValidator` |
+----------------+--------------------------------------------------------------------------------------------+

Uso di base
-----------

Si supponga di avere una classe `PasswordChange`, usata in un form in cui l'utente possa
cambiare la sua password, inserendo la sua vecchia password e quella nuova.
Questo vincolo valida che la vecchia password corrisponda alla password attuale
dell'utente:

.. configuration-block::

    .. code-block:: php-annotations

        // src/Acme/UserBundle/Form/Model/ChangePassword.php
        namespace Acme\UserBundle\Form\Model;

        use Symfony\Component\Security\Core\Validator\Constraints as SecurityAssert;

        class ChangePassword
        {
            /**
             * @SecurityAssert\UserPassword(
             *     message = "Password attuale sbagliata"
             * )
             */
             protected $oldPassword;
        }

    .. code-block:: yaml

        # src/Acme/UserBundle/Resources/config/validation.yml
        Acme\UserBundle\Form\Model\ChangePassword:
            properties:
                oldPassword:
                    - Symfony\Component\Security\Core\Validator\Constraints\UserPassword:
                        message: "Password attuale sbagliata"

    .. code-block:: xml

        <!-- src/Acme/UserBundle/Resources/config/validation.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>
        <constraint-mapping xmlns="http://symfony.com/schema/dic/constraint-mapping"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/dic/constraint-mapping http://symfony.com/schema/dic/constraint-mapping/constraint-mapping-1.0.xsd">

            <class name="Acme\UserBundle\Form\Model\ChangePassword">
                <property name="oldPassword">
                    <constraint
                        name="Symfony\Component\Security\Core\Validator\Constraints\UserPassword"
                    >
                        <option name="message">Password attuale sbagliata</option>
                    </constraint>
                </property>
            </class>
        </constraint-mapping>

    .. code-block:: php

        // src/Acme/UserBundle/Form/Model/ChangePassword.php
        namespace Acme\UserBundle\Form\Model;

        use Symfony\Component\Validator\Mapping\ClassMetadata;
        use Symfony\Component\Security\Core\Validator\Constraints as SecurityAssert;

        class ChangePassword
        {
            public static function loadValidatorData(ClassMetadata $metadata)
            {
                $metadata->addPropertyConstraint(
                    'oldPassword',
                    new SecurityAssert\UserPassword(array(
                        'message' => 'Password attuale sbagliata',
                    ))
                );
            }
        }

Opzioni
-------

message
~~~~~~~

**tipo**: ``message`` **predefinito**: ``This value should be the user current password``

Messaggio mostrato quando la stringa sottostante *non* corrisponde alla password
attuale dell'utente.
