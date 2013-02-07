UserPassword
============

.. versionadded:: 2.1
   Questo vincolo è stato aggiunto nella versione 2.1.

Valida che un valore inserito sia uguale alla password dell'utente attualmente
autenticato. Può tornare utile in un form in cui un utente può cambiare la propria
password, ma deve inserire quella vecchia per motivi di sicurezza.

.. note::

    Questo vincolo **non** va usato per la validazione di un form di login, che viene
    fatta automaticamente dal sistema di sicurezza.

+----------------+-------------------------------------------------------------------------------------------+
| Si applica a   | :ref:`proprietà o metodo<validation-property-target>`                                     |
+----------------+-------------------------------------------------------------------------------------------+
| Opzioni        | - `message`_                                                                              |
+----------------+-------------------------------------------------------------------------------------------+
| Classe         | :class:`Symfony\\Component\\Security\\Core\\Validator\\Constraint\\UserPassword`          |
+----------------+-------------------------------------------------------------------------------------------+
| Validatore     | :class:`Symfony\\Component\\Security\\Core\\Validator\\Constraint\\UserPasswordValidator` |
+----------------+-------------------------------------------------------------------------------------------+

Uso di base
-----------

Si supponga di avere una classe `PasswordChange`, usata in un form in cui l'utente possa
cambiare la sua password, inserendo la sua vecchia password e quella nuova.
Questo vincolo valida che la vecchia password corrisponda alla password attuale
dell'utente:

.. configuration-block::

    .. code-block:: yaml

        # src/UserBundle/Resources/config/validation.yml
        Acme\UserBundle\Form\Model\ChangePassword:
            properties:
                oldPassword:
                    - Symfony\Component\Security\Core\Validator\Constraint\UserPassword:
                        message: "Password attuale sbagliata"

    .. code-block:: php-annotations

       // src/Acme/UserBundle/Form/Model/ChangePassword.php
       namespace Acme\UserBundle\Form\Model;
       
       use Symfony\Component\Security\Core\Validator\Constraint as SecurityAssert;

       class ChangePassword
       {
           /**
            * @SecurityAssert\UserPassword(
            *     message = "Password attuale sbagliata"
            * )
            */
            protected $oldPassword;
       }

    .. code-block:: xml

        <!-- src/UserBundle/Resources/config/validation.xml -->
        <class name="Acme\UserBundle\Form\Model\ChangePassword">
            <property name="Symfony\Component\Security\Core\Validator\Constraint\UserPassword">
                <option name="message">Wrong value for your current password</option>
            </property>
        </class>

    .. code-block:: php

        // src/Acme/UserBundle/Form/Model/ChangePassword.php
        namespace Acme\UserBundle\Form\Model;
        
        use Symfony\Component\Validator\Mapping\ClassMetadata;
        use Symfony\Component\Security\Core\Validator\Constraint as SecurityAssert;
  
        class ChangePassword
        {
            public static function loadValidatorData(ClassMetadata $metadata)
            {
                $metadata->addPropertyConstraint('oldPassword', new SecurityAssert\UserPassword(array(
                    'message' => 'Wrong value for your current password',
                )));
            }
        }

Opzioni
-------

message
~~~~~~~

**tipo**: ``message`` **predefinito**: ``This value should be the user current password``

Messaggio mostrato quando la stringa sottostante *non* corrisponde alla password
attuale dell'utente.
